// Conditional compilation of driver
#ifdef USE_AS608

// Define driver ID
#define XSNS_80 80

#include <TasmotaSerial.h>
#include <Adafruit_Fingerprint.h>


bool as608selected = false;
static uint8_t enrollstep = 0;
static uint8_t ModellNumber = 0;

Adafruit_Fingerprint *finger = NULL;
TasmotaSerial *serial = NULL;

uint8_t myFingerprint[sizeof(Adafruit_Fingerprint)];
uint8_t myTasmotaSerial[sizeof(TasmotaSerial)];

void as608init(){

    if (PinUsed(GPIO_AS608_RX) && PinUsed(GPIO_AS608_TX)){

        serial = new (myTasmotaSerial) TasmotaSerial(Pin(GPIO_AS608_RX), Pin(GPIO_AS608_TX), 0);
        finger = new (myFingerprint) Adafruit_Fingerprint(serial, 0);

        finger->begin(57600);

        if (finger->verifyPassword()){
          finger->getTemplateCount();
          as608selected = true;
          snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "AS60x found. %i fingerprints stored."), finger->templateCount);
        }else{
          snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "AS60x not found! Got data: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X"), finger->mydata[0], finger->mydata[1], finger->mydata[2], finger->mydata[3], finger->mydata[4], finger->mydata[5], finger->mydata[6], finger->mydata[7], finger->mydata[8], finger->mydata[9], finger->mydata[10], finger->mydata[11]);
        }
        AddLog(LOG_LEVEL_INFO);
    }

}

uint8_t as608Enroll(uint8_t nr){
     // set Variable to 1rst step
    if (!enrollstep){
        enrollstep = 1;
        ModellNumber = nr;
    }
    return true;
}

int deleteFingerprint(uint8_t id) {
  uint8_t p = -1;

  p = finger->deleteModel(id);

  if (p == FINGERPRINT_OK) {
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Delete ok"));
    return true;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Comm error"));
    return false;
  } else if (p == FINGERPRINT_BADLOCATION) {
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Could not delete in that location"));
    return false;
  } else if (p == FINGERPRINT_FLASHERR) {
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Error writing to flash"));
    return false;
  } else {
    AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "any error"));
    return false;
  }
}

int getFingerImage(){
  int p = finger->getImage();
  switch (p) {
    case FINGERPRINT_OK:
      AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Image taken"));
      break;
    case FINGERPRINT_NOFINGER:
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Comm error"));
      break;
    case FINGERPRINT_IMAGEFAIL:
      AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Imaging error"));
      break;
    default:
      AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "any error"));
      break;
    }

  return p;
}

int convertFingerImage(uint8_t slot){
    int p;
    p = finger->image2Tz(slot);
    switch (p) {
      case FINGERPRINT_OK:
        AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Image converted"));
        break;
      case FINGERPRINT_IMAGEMESS:
        AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Image too messy"));
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Comm error"));
        break;
      case FINGERPRINT_FEATUREFAIL:
        AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Could not find fingerprint features"));
        break;
      case FINGERPRINT_INVALIDIMAGE:
        AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Image invalid"));
        break;
      default:
        AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "any error"));
        break;
    }

    return p;
}

void as608Main(){
    if(!as608selected){
      return;
    }

    int p;

    if (!enrollstep){
        // search for Finger
        uint8_t p = finger->getImage();
        if (p != FINGERPRINT_OK)  return;

        p = finger->image2Tz();
        if (p != FINGERPRINT_OK)  return;

        p = finger->fingerFastSearch();
        if (p != FINGERPRINT_OK){
            AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "no matching finger!"));
            return;
        }

        // found a match!
        snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "Finger #%i found with confidence of %i"), finger->fingerID, finger->confidence);
        AddLog(LOG_LEVEL_INFO);
        snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("{\"AS680_Finger\":%i, AS608_Confidence\":%i}"), finger->fingerID, finger->confidence);
        MqttPublishPrefixTopic_P(RESULT_OR_STAT, mqtt_data);
        return;
    }else{
        // enroll is active
        switch (enrollstep){
        case 1:
            AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "place finger and wait"));
            enrollstep++;
            break;
        case 2:
        // get first image
            if (getFingerImage() == FINGERPRINT_OK){
              enrollstep++;
            }
            break;
        case 3:
        // convert image
            if(convertFingerImage(1) == FINGERPRINT_OK){
                enrollstep++;
                break;
            }else{
                enrollstep--;
                break;
            }
            break;
        case 4:
            AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "remove finger and wait"));
            enrollstep++;
            break;
        case 5:
        // Remove finger
            p = finger->getImage();
            if (p == FINGERPRINT_NOFINGER){
                enrollstep++;
            }
            break;
        case 6:
            AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "place same finger again and wait"));
            enrollstep++;
            break;
        case 7:
        // get second image of finger
            if (getFingerImage() == FINGERPRINT_OK){
              enrollstep++;
            }
            break;
        case 8:
        // convert second image
            if(convertFingerImage(2) != FINGERPRINT_OK)  {
                AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Not Ok. Try again."));
                enrollstep -= 2;
            }

        // create modell

            AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "creating model"));

            p = finger->createModel();
            if (p == FINGERPRINT_OK) {
              AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Prints matched"));
            } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
              AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Communication error"));
              enrollstep = 99;
            } else if (p == FINGERPRINT_ENROLLMISMATCH) {
              AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Fingerprints did not match"));
              enrollstep = 99;
            } else {
              AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "any error"));
              enrollstep = 99;
            }

        // store modell

            p = finger->storeModel(ModellNumber);
            if (p == FINGERPRINT_OK) {
              AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Stored!"));
              enrollstep = 0;
              ModellNumber = 0;
            } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
              AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Communication error"));
              enrollstep = 99;
            } else if (p == FINGERPRINT_BADLOCATION) {
              AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Could not store in that location"));
              enrollstep = 99;
            } else if (p == FINGERPRINT_FLASHERR) {
              AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Error writing to flash"));
              enrollstep = 99;
            } else {
              AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "any error"));
              enrollstep = 99;
            }
            break;
        case 99:
            enrollstep = 1;
            AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "enroll starts again!"));
            break;
        default:
            enrollstep = 0;
            ModellNumber = 0;
            AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "oops sth went wrong"));
            break;
        }
    }
}


bool as608Command(void){
    if (XSNS_80 == XdrvMailbox.index) {

        // convert space or = to , to get mor compatibility
        for (uint8_t ca = 0; ca < XdrvMailbox.data_len; ca++) {
          if ((' ' == XdrvMailbox.data[ca]) || ('=' == XdrvMailbox.data[ca])) { XdrvMailbox.data[ca] = ','; }
        }

        char sub_string[XdrvMailbox.data_len];
        // Test if enroll is active
        // look for command enroll and get number
        if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1),"enroll")) {
          if (!enrollstep){
                uint8_t ModellNumber = atoi(subStr(sub_string, XdrvMailbox.data, ",", 2));  // Note 2 used for param number
                snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "AS60x Enroll called #%i"), ModellNumber);
                AddLog(LOG_LEVEL_INFO);
                as608Enroll(ModellNumber);
                return true;
          }else{
                AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "AS60x Enroll active! Cancel with: enrollReset"));
                return false;
          }
        }

        // reset running enroll
        if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1),"enrollReset")) { // Note 1 used for param number
          enrollstep = 0;
          return true;
        }

        // delete one Fingerprint
        if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1),"delete")) { // Note 1 used for param number
          return deleteFingerprint(atoi(subStr(sub_string, XdrvMailbox.data, ",", 2)));  // Note 2 used for param number
        }

        // delete all Fingerprints
        if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1),"deleteAll")) { // Note 1 used for param number
          finger->emptyDatabase();
          finger->getTemplateCount();
          if(!finger->templateCount){
              AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Empty database Ok!"));
              return true;
          }else{
              AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Empty database Error!"));
              return true;
          }
        }

        // get number of Fingerprints
        if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1),"getNumber")) { // Note 1 used for param number
          finger->getTemplateCount();
          snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_LOG "%i fingerprints stored."), finger->templateCount);
          AddLog(LOG_LEVEL_INFO);
          return true;
        }

        if (!strcmp(subStr(sub_string, XdrvMailbox.data, ",", 1),"help")) { // Note 1 used for param number
          AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_LOG "Commands: enroll x, enrollReset, delete x, deleteAll, getNumber"));
          return true;
        }
    }

}

 /*********************************************************************************************\
  * Interface
 \*********************************************************************************************/

 bool Xsns80(uint8_t function)
 {
   bool result = false;

   switch (function) {
      case FUNC_INIT:
          as608init();
          break;
      case FUNC_EVERY_250_MSECOND:
          as608Main();
          break;
      case FUNC_EVERY_SECOND:
          break;
      case FUNC_JSON_APPEND:
          break;
      case FUNC_SERIAL:
          break;
      case FUNC_COMMAND_SENSOR:
          result = as608Command();
          break;
   }
   //return result;
   return result;
 }

 #endif
