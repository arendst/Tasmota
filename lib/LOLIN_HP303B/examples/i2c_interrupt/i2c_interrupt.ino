#include <LOLIN_HP303B.h>

// HP303B Opject
LOLIN_HP303B HP303BPressureSensor = LOLIN_HP303B();

void onFifoFull();

const unsigned char pressureLength = 50;
unsigned char pressureCount = 0;
int32_t pressure[pressureLength];
unsigned char temperatureCount = 0;
const unsigned char temperatureLength = 50;
int32_t temperature[temperatureLength];



void setup()
{
  Serial.begin(115200);
  while (!Serial);

  //Call begin to initialize HP303BPressureSensor
  //The parameter 0x76 is the bus address. The default address is 0x77 and does not need to be given.
  //HP303BPressureSensor.begin(Wire, 0x76);
  //Use the commented line below instead to use the default I2C address.
  HP303BPressureSensor.begin();
  
  int16_t ret = HP303BPressureSensor.setInterruptPolarity(1);
  ret = HP303BPressureSensor.setInterruptSources(1, 0, 0);
  //clear interrupt flag by reading
  HP303BPressureSensor.getIntStatusFifoFull();

  //initialization of Interrupt for Controller unit
  //SDO pin of HP303B has to be connected with interrupt pin
  int16_t interruptPin = 3;
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), onFifoFull, RISING);

  //start of a continuous measurement just like before
  int16_t temp_mr = 3;
  int16_t temp_osr = 2;
  int16_t prs_mr = 1;
  int16_t prs_osr = 3;
  ret = HP303BPressureSensor.startMeasureBothCont(temp_mr, temp_osr, prs_mr, prs_osr);
  if (ret != 0)
  {
    Serial.print("Init FAILED! ret = ");
    Serial.println(ret);
  }
  else
  {
    Serial.println("Init complete!");
  }
}


void loop()
{
  //do other stuff
  Serial.println("loop running");
  delay(500);


  //if result arrays are full
  //This could also be in the interrupt handler, but it would take too much time for a proper ISR
  if (pressureCount == pressureLength && temperatureCount == temperatureLength)
  {
    //print results
    Serial.println();
    Serial.println();
    Serial.print(temperatureCount);
    Serial.println(" temperature values found: ");
    for (int16_t i = 0; i < temperatureCount; i++)
    {
      Serial.print(temperature[i]);
      Serial.println(" degrees of Celsius");
    }
    Serial.println();
    Serial.print(pressureCount);
    Serial.println(" pressure values found: ");
    for (int16_t i = 0; i < pressureCount; i++)
    {
      Serial.print(pressure[i]);
      Serial.println(" Pascal");
    }
    Serial.println();
    Serial.println();
    //reset result counters
    pressureCount = 0;
    temperatureCount = 0;
  }
}


//interrupt handler
void onFifoFull()
{
  //message for debugging
  Serial.println("Interrupt handler called");

  //clear interrupt flag by reading
  HP303BPressureSensor.getIntStatusFifoFull();

  //calculate the number of free indexes in the result arrays
  unsigned char prs_freespace = pressureLength - pressureCount;
  unsigned char temp_freespace = temperatureLength - temperatureCount;
  //read the results from HP303B, new results will be added at the end of the arrays
  HP303BPressureSensor.getContResults(&temperature[temperatureCount], temp_freespace, &pressure[pressureCount], prs_freespace);
  //after reading the result counters are increased by the amount of new results
  pressureCount += prs_freespace;
  temperatureCount += temp_freespace;
}
