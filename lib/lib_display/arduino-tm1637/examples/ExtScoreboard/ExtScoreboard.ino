/*
	Dual counter example (Extended class example)

  You could do many things with a dual counter. This example demoonstrates a scoreboard. Two players rol a dice every round, the player with the highest score wins the round. The number of rounds won are displayed on the scoreboard. Once one of the players reaches a maximum score the game is over. At the end of the game a player won message or draw will be displayed.

	The circuit:
  * connect TM1637 pin CLK to Arduino pin D4
  * connect TM1637 pin DIO to Arduino pin D5
  * connect TM1637 pin Vcc to Arduino pin 5V
  * connect TM1637 pin GND to Arduino pin GND

	Created 25 September 2015
	By Bram Harmsen

	https://github.com/bremme/arduino-tm1637

*/

// include the SevenSegmentTM1637 library
#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"

/* initialize global TM1637 Display object
*  The constructor takes two arguments, the number of the clock pin and the digital output pin:
* SevenSegmentTM1637(byte pinCLK, byte pinDIO);
*/
const byte PIN_CLK = 4;   // define CLK pin (any digital pin)
const byte PIN_DIO = 5;   // define DIO pin (any digital pin)
SevenSegmentExtended      display(PIN_CLK, PIN_DIO);

// sets the maximum score for the game
const byte maxScore = 10;

// run setup code
void setup() {
  Serial.begin(9600);         // initializes the Serial connection @ 9600 baud
  display.begin();            // initializes the display
  display.setBacklight(100);  // set the brightness to 100 %
  delay(1000);                // wait 1000 ms
  randomSeed(analogRead(0));  // get a random seed for throwing dices
};

// run loop (forever)
void loop() {

  byte playerOneScore = 0;              // initialize player one score
  byte playerTwoScore = 0;              // initialize player two score
  bool gameEnd = false;                 // initialize gameEnd?

  display.print("START NEW GAME    ");  // Start a new round
  delay(500);                           // delay 1000 ms
  display.clear();                      // clear the display

  while( !gameEnd ) {                   // play till the end of the game

    byte playerOneTurn = rollDice();    // player one rolls a dice
    byte playerTwoTurn = rollDice();    // player two rolls a dice

    if ( playerOneTurn > playerTwoTurn ) {  // player one had a higher dice
      playerOneScore++;                     // increase player one's score
    } else if ( playerOneTurn < playerTwoTurn ){
      playerTwoScore++;                     // increase player two's score
    } else {                                // player one and two had same score
      display.clear();
      display.print("SAME");
      display.blink();
      display.clear();
    }
    // update scoreboard
    display.printDualCounter(playerOneScore, playerTwoScore);
    delay(500);

    // check if game has ended (player one or two's score >= maxScore)
    if ( (playerOneScore >= maxScore || playerTwoScore >= maxScore) ) {
      gameEnd = true;   // game has ended
      printGameEnd();   // print end game message
      break;            // break from while loop (jump out)
    };

  };

  // check who won
  if ( playerOneScore > playerTwoScore ) {
    printPlayerWon(1);                              // player one won
  } else if ( playerOneScore < playerTwoScore ){
    printPlayerWon(2);                              // player two won
  } else {
    display.clear(); display.print("DRAW");         // draw
    display.blink(); display.clear();
  };
};

// roll a dice: random rumber fom 0 to 5 + 1 = 0 to 6
byte rollDice() {
  return (random(6) + 1);
}

// print winning player message
void printPlayerWon(byte player) {
  display.clear();
  char winText[] = "PLAYER x WON    ";
  winText[7] = (char)player + '0';
  display.print(winText);
  delay(500);
  display.clear();
};

// print end game message
void printGameEnd(void) {
  display.clear();
  display.print("GAME END    ");
  display.blink();
  delay(500);
};
