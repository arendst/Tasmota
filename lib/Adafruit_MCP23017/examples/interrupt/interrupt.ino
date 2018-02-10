// Install the LowPower library for optional sleeping support.
// See loop() function comments for details on usage.
//#include <LowPower.h>

#include <Wire.h>
#include <Adafruit_MCP23017.h>

Adafruit_MCP23017 mcp;

byte ledPin=13;

// Interrupts from the MCP will be handled by this PIN
byte arduinoIntPin=3;

// ... and this interrupt vector
byte arduinoInterrupt=1;

volatile boolean awakenByInterrupt = false;

// Two pins at the MCP (Ports A/B where some buttons have been setup.)
// Buttons connect the pin to grond, and pins are pulled up.
byte mcpPinA=7;
byte mcpPinB=15;

void setup(){

  Serial.begin(9600);
  Serial.println("MCP23007 Interrupt Test");

  pinMode(arduinoIntPin,INPUT);

  mcp.begin();      // use default address 0
  
  // We mirror INTA and INTB, so that only one line is required between MCP and Arduino for int reporting
  // The INTA/B will not be Floating 
  // INTs will be signaled with a LOW
  mcp.setupInterrupts(true,false,LOW);

  // configuration for a button on port A
  // interrupt will triger when the pin is taken to ground by a pushbutton
  mcp.pinMode(mcpPinA, INPUT);
  mcp.pullUp(mcpPinA, HIGH);  // turn on a 100K pullup internally
  mcp.setupInterruptPin(mcpPinA,FALLING); 

  // similar, but on port B.
  mcp.pinMode(mcpPinB, INPUT);
  mcp.pullUp(mcpPinB, HIGH);  // turn on a 100K pullup internall
  mcp.setupInterruptPin(mcpPinB,FALLING);

  // We will setup a pin for flashing from the int routine
  pinMode(ledPin, OUTPUT);  // use the p13 LED as debugging
  
}

// The int handler will just signal that the int has happen
// we will do the work from the main loop.
void intCallBack(){
  awakenByInterrupt=true;
}

void handleInterrupt(){
  
  // Get more information from the MCP from the INT
  uint8_t pin=mcp.getLastInterruptPin();
  uint8_t val=mcp.getLastInterruptPinValue();
  
  // We will flash the led 1 or 2 times depending on the PIN that triggered the Interrupt
  // 3 and 4 flases are supposed to be impossible conditions... just for debugging.
  uint8_t flashes=4; 
  if(pin==mcpPinA) flashes=1;
  if(pin==mcpPinB) flashes=2;
  if(val!=LOW) flashes=3;

  // simulate some output associated to this
  for(int i=0;i<flashes;i++){  
    delay(100);
    digitalWrite(ledPin,HIGH);
    delay(100);
    digitalWrite(ledPin,LOW);
  }

  // we have to wait for the interrupt condition to finish
  // otherwise we might go to sleep with an ongoing condition and never wake up again.
  // as, an action is required to clear the INT flag, and allow it to trigger again.
  // see datasheet for datails.
  while( ! (mcp.digitalRead(mcpPinB) && mcp.digitalRead(mcpPinA) ));
  // and clean queued INT signal
  cleanInterrupts();
}

// handy for interrupts triggered by buttons
// normally signal a few due to bouncing issues
void cleanInterrupts(){
  EIFR=0x01;
  awakenByInterrupt=false;
}  

/**
 * main routine: sleep the arduino, and wake up on Interrups.
 * the LowPower library, or similar is required for sleeping, but sleep is simulated here.
 * It is actually posible to get the MCP to draw only 1uA while in standby as the datasheet claims,
 * however there is no stadndby mode. Its all down to seting up each pin in a way that current does not flow.
 * and you can wait for interrupts while waiting.
 */
void loop(){
  
  // enable interrupts before going to sleep/wait
  // And we setup a callback for the arduino INT handler.
  attachInterrupt(arduinoInterrupt,intCallBack,FALLING);
  
  // Simulate a deep sleep
  while(!awakenByInterrupt);
  // Or sleep the arduino, this lib is great, if you have it.
  //LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
  
  // disable interrupts while handling them.
  detachInterrupt(arduinoInterrupt);
  
  if(awakenByInterrupt) handleInterrupt();
}



