/*
  Calculating a custom UV index
  By: Jim Lindblom
  SparkFun Electronics
  Date: May 23, 2018
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14748
  
  This example demonstrates how to use the raw UVA, UVB and calibration values to calculate a UV index.

  The library does this for you, but, to custom-tailor the UV index, it may help to manually calculate these values.
  This calculation method is based on the "Designing the VEML6075 Into an Application" app note.
  
  Quick-start:
  - Use a SparkFun BlackBoard -or- attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
  - Check through comments to configure your sensor calibration
  - Plug the Qwiic UV Sensor onto the BlackBoard/shield
  - Open the serial monitor and set the baud rate to 9600
  - Sensor readings will stream every ~250ms.
*/
// Include the SparkFun VEML6075 library.
// Click here to get the library: http://librarymanager/All#SparkFun_VEML6075
#include <SparkFun_VEML6075_Arduino_Library.h>

VEML6075 uv; // Create a VEML6075 object

// Calibration constants:
// Four gain calibration constants -- alpha, beta, gamma, delta -- can be used to correct the output in
// reference to a GOLDEN sample. The golden sample should be calibrated under a solar simulator.
// Setting these to 1.0 essentialy eliminates the "golden"-sample calibration
const float CALIBRATION_ALPHA_VIS = 1.0; // UVA / UVAgolden
const float CALIBRATION_BETA_VIS  = 1.0; // UVB / UVBgolden
const float CALIBRATION_GAMMA_IR  = 1.0; // UVcomp1 / UVcomp1golden
const float CALIBRATION_DELTA_IR  = 1.0; // UVcomp2 / UVcomp2golden

// Responsivity:
// Responsivity converts a raw 16-bit UVA/UVB reading to a relative irradiance (W/m^2).
// These values will need to be adjusted as either integration time or dynamic settings are modififed.
// These values are recommended by the "Designing the VEML6075 into an application" app note for 100ms IT
const float UVA_RESPONSIVITY = 0.00110; // UVAresponsivity
const float UVB_RESPONSIVITY = 0.00125; // UVBresponsivity

// UV coefficients:
// These coefficients
// These values are recommended by the "Designing the VEML6075 into an application" app note
const float UVA_VIS_COEF_A = 2.22; // a
const float UVA_IR_COEF_B  = 1.33; // b
const float UVB_VIS_COEF_C = 2.95; // c
const float UVB_IR_COEF_D  = 1.75; // d

void setup() {
  Serial.begin(9600);
  if (uv.begin() == false) {
    Serial.println("Unable to communicate with VEML6075.");
    while (1) ;
  }
  // Integration time and high-dynamic values will change the UVA/UVB sensitivity. That means
  // new responsivity values will need to be measured for every combination of these settings.
  uv.setIntegrationTime(VEML6075::IT_100MS);
  uv.setHighDynamic(VEML6075::DYNAMIC_NORMAL);
}

void loop() {
  uint16_t rawA, rawB, visibleComp, irComp;
  float uviaCalc, uvibCalc, uvia, uvib, uvi;

  // Read raw and compensation data from the sensor
  rawA = uv.rawUva();
  rawB = uv.rawUvb();
  visibleComp = uv.visibleCompensation();
  irComp = uv.irCompensation();

  // Calculate the simple UVIA and UVIB. These are used to calculate the UVI signal.
  uviaCalc = (float)rawA - ((UVA_VIS_COEF_A * CALIBRATION_ALPHA_VIS * visibleComp) / CALIBRATION_GAMMA_IR)
                  - ((UVA_IR_COEF_B  * CALIBRATION_ALPHA_VIS * irComp) /  CALIBRATION_DELTA_IR);
  uvibCalc = (float)rawB - ((UVB_VIS_COEF_C * CALIBRATION_BETA_VIS * visibleComp) / CALIBRATION_GAMMA_IR)
                  - ((UVB_IR_COEF_D  * CALIBRATION_BETA_VIS * irComp) /  CALIBRATION_DELTA_IR);

  // Convert raw UVIA and UVIB to values scaled by the sensor responsivity
  uvia = uviaCalc * (1.0 / CALIBRATION_ALPHA_VIS) * UVA_RESPONSIVITY;
  uvib = uvibCalc * (1.0 / CALIBRATION_BETA_VIS) * UVB_RESPONSIVITY;

  // Use UVIA and UVIB to calculate the average UVI:
  uvi = (uvia + uvib) / 2.0;

  Serial.println(String(uviaCalc) + ", " + String(uvibCalc) + ", " + String(uvi));
  delay(250);
}