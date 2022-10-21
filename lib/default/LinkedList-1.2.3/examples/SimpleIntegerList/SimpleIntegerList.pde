/*
	LinkedList Example
	Link: http://github.com/ivanseidel/LinkedList

	Example Created by
		Tom Stewart, github.com/tastewar

	Edited by:
		Ivan Seidel, github.com/ivanseidel
*/
#include <LinkedList.h>

LinkedList<int> myList = LinkedList<int>();

void setup()
{
	
	Serial.begin(9600);
	Serial.println("Hello!");

	// Add some stuff to the list
	int k = -240,
		l = 123,
		m = -2,
		n = 222;
	myList.add(n);
	myList.add(0);
	myList.add(l);
	myList.add(17);
	myList.add(k);
	myList.add(m);
}

void loop() {

	int listSize = myList.size();

	Serial.print("There are ");
	Serial.print(listSize);
	Serial.print(" integers in the list. The negative ones are: ");

	// Print Negative numbers
	for (int h = 0; h < listSize; h++) {

		// Get value from list
		int val = myList.get(h);

		// If the value is negative, print it
		if (val < 0) {
			Serial.print(" ");
			Serial.print(val);
		}
	}

	while (true); // nothing else to do, loop forever
}


