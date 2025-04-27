#include <SD.h>

#include <SPI.h>
#include <IPAddress.h>
#include <IniFile.h>

// The select pin used for the SD card
#define SD_SELECT 10
//#define ETHERNET_SELECT 10

void printErrorMessage(uint8_t e, bool eol = true)
{
  switch (e) {
  case IniFile::errorNoError:
    Serial.print("no error");
    break;
  case IniFile::errorFileNotFound:
    Serial.print("file not found");
    break;
  case IniFile::errorFileNotOpen:
    Serial.print("file not open");
    break;
  case IniFile::errorBufferTooSmall:
    Serial.print("buffer too small");
    break;
  case IniFile::errorSeekError:
    Serial.print("seek error");
    break;
  case IniFile::errorSectionNotFound:
    Serial.print("section not found");
    break;
  case IniFile::errorKeyNotFound:
    Serial.print("key not found");
    break;
  case IniFile::errorEndOfFile:
    Serial.print("end of file");
    break;
  case IniFile::errorUnknownError:
    Serial.print("unknown error");
    break;
  default:
    Serial.print("unknown error value");
    break;
  }
  if (eol)
    Serial.println();
}

void setup()
{
  // Configure all of the SPI select pins as outputs and make SPI
  // devices inactive, otherwise the earlier init routines may fail
  // for devices which have not yet been configured.
  pinMode(SD_SELECT, OUTPUT);
  digitalWrite(SD_SELECT, HIGH); // disable SD card
  
//  pinMode(ETHERNET_SELECT, OUTPUT);
//  digitalWrite(ETHERNET_SELECT, HIGH); // disable Ethernet

  const size_t bufferLen = 80;
  char buffer[bufferLen];

  const char *filename = "/lunch.ini";
  Serial.begin(9600);
  SPI.begin();
  if (!SD.begin(SD_SELECT))
    while (1)
      Serial.println("SD.begin() failed");
  
  IniFile ini(filename);
  if (!ini.open()) {
    Serial.print("Ini file ");
    Serial.print(filename);
    Serial.println(" does not exist");
    // Cannot do anything else
    while (1)
      ;
  }
  Serial.println("Ini file exists");

  // Check the file is valid. This can be used to warn if any lines
  // are longer than the buffer.
  if (!ini.validate(buffer, bufferLen)) {
    Serial.print("ini file ");
    Serial.print(ini.getFilename());
    Serial.print(" not valid: ");
    printErrorMessage(ini.getError());
    // Cannot do anything else
    while (1)
      ;
  }
  
  // Browse through all sections and print contents:
  IniFileState state;
  char sectName[bufferLen];
  Serial.println();

  while (ini.browseSections(sectName, bufferLen, state)) {
    Serial.print("> ");
    Serial.print(sectName);

    if (ini.getValue(sectName, "meal", buffer, bufferLen)) { 
      Serial.print(" eats ");
      Serial.print(buffer);
    } else
      Serial.print(" eats nothing");

    if (ini.getValue(sectName, "drinks", buffer, bufferLen)) { 
      Serial.print(", drinks ");
      Serial.print(buffer);
    } else
      Serial.print(", drinks nothing");

    if (ini.getValue(sectName, "dessert", buffer, bufferLen)) { 
      Serial.print(" and has ");
      Serial.print(buffer);
      Serial.println(" for dessert.");
    } else
      Serial.println(" and has no dessert.");
  }

  // finished!
  Serial.println();
  printErrorMessage(ini.getError());
  // Cannot do anything else
  while (1)
     ;
}


void loop()
{


}
