//
//    FILE: AGS02MA_calibrate.ino
//  AUTHOR: Rob Tillaart, Beanow
// PURPOSE: test application
//     URL: https://github.com/RobTillaart/AGS02MA


#include "AGS02MA.h"

//  You can decrease/disable warmup when you're certain the chip already warmed up.
#define WARMUP_MINUTES 6
#define READ_INTERVAL 3000


uint32_t start, stop;
uint8_t version;

AGS02MA AGS(26);


void setup()
{
  //  ESP devices typically miss the first serial log lines after flashing.
  //  Delay somewhat to include all output.
  delay(1000);

  Serial.begin(115200);
  Serial.println();
  Serial.println(__FILE__);
  Serial.print("AGS02MA_LIB_VERSION: ");
  Serial.println(AGS02MA_LIB_VERSION);
  Serial.println();

  Serial.print("WARMUP:\t\t");
  Serial.println(WARMUP_MINUTES);
  Serial.print("INTERVAL:\t");
  Serial.println(READ_INTERVAL);

  Wire.begin();

  bool b = AGS.begin();
  Serial.print("BEGIN:\t\t");
  Serial.println(b);

  Serial.print("VERSION:\t");
  version = AGS.getSensorVersion();
  Serial.println(version);
  int err = AGS.lastError();

  //  Reading version correctly matters, as we display additional comments based on it.
  if(err != AGS02MA_OK)
  {
    Serial.print("Error reading version:\t");
    Serial.println(err);
    Serial.println("Won't attempt to calibrate. Reset when connection with the sensor is stable.");
    Serial.println();
    return;
  }

  b = AGS.setPPBMode();
  uint8_t m = AGS.getMode();
  Serial.print("MODE:\t\t");
  Serial.print(b);
  Serial.print("\t");
  Serial.println(m);

  Serial.println();
  Serial.print("Place the device outside in open air for ");
  Serial.print(WARMUP_MINUTES);
  Serial.println(" minute(s).");
  Serial.println("Make sure your device has warmed up sufficiently for the best results.");
  Serial.println("The PPB values should be stable (may include noise) not constantly decreasing.");
  Serial.println();

  start = millis();
  stop = WARMUP_MINUTES * 60000UL;
  while(millis() - start < stop)
  {
    Serial.print("[PRE ]\t");
    printPPB();
    delay(READ_INTERVAL);
  }

  Serial.println();
  Serial.println("About to perform calibration now.");

  AGS02MA::ZeroCalibrationData initialValue;
  if (!AGS.getZeroCalibrationData(initialValue))
  {
    Serial.print("Error reading zero calibration data:\t");
    Serial.println(AGS.lastError());
    Serial.println("Won't attempt to calibrate. Reset when connection with the sensor is stable.");
    Serial.println();
    return;
  }

  Serial.println("Your previous calibration data was:");
  printZeroCalibrationData(initialValue);

  delay(1000);

  //  returns 1 if successful written
  b = AGS.zeroCalibration();
  Serial.println();
  Serial.print("CALIB:\t");
  Serial.println(b);
  Serial.println();
  Serial.println("Calibration done.");

  AGS02MA::ZeroCalibrationData zc;
  while (!AGS.getZeroCalibrationData(zc))
  {
    Serial.print("Error:\t");
    Serial.print(AGS.lastError());
    Serial.println("\tretrying...");
    delay(READ_INTERVAL);
  }

  Serial.println("Your new calibration data is:");
  printZeroCalibrationData(zc);

  Serial.println();
  Serial.println("Showing what PPB values look like post calibration.");
  //  A 125 status is typically shown on v118's after they've been powered off.
  //  Either having this version at all, or seeing this status, we'll display a notice.
  if (version == 118 || initialValue.status == 125)
  {
    Serial.println("NOTICE: v118 sensors are known to give different results after powering off!");
    Serial.println("You may need to manually set your calibration value every time power was lost.");
  }
  Serial.println();
}


void loop()
{
  Serial.print("[POST]\t");
  printPPB();
  delay(READ_INTERVAL);
}


void printZeroCalibrationData(AGS02MA::ZeroCalibrationData &zc) {
  Serial.print("Status:\t");
  Serial.println(zc.status);
  Serial.print("Value:\t");
  Serial.println(zc.value);
}


void printPPB()
{
  uint32_t value = AGS.readPPB();
  Serial.print("PPB:\t");
  Serial.print(value);
  Serial.print("\t");
  Serial.print(AGS.lastStatus(), HEX);
  Serial.print("\t");
  Serial.print(AGS.lastError(), HEX);
  Serial.println();
}


//  -- END OF FILE --
