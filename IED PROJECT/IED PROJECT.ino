/*
 Name:		IED_PROJECT.ino
 Created:	1/12/2023 10:17:34 PM
 Author:	Aidan
*/

// the setup function runs once when you press reset or power the board
#define Hit_aPin 4
#define Hit_bPin 2
int counterA = 0;
int counterB = 0;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	pinMode(counterA, INPUT_PULLUP);
	pinMode(counterB, INPUT_PULLUP);
	PCICR = B00000100;
	PCMSK2 = B00010100;


}

// the loop function runs over and over again until power down or reset
void loop() {
	Serial.print("hit A score is: ");
	Serial.println(counterA);
	Serial.print("hit B score is: ");
	Serial.println(counterB);
	Serial.println("ready for next hit");
	delay(1000);
}

ISR(PCINT2_vect) {
	if (digitalRead(counterA) == LOW) {
		counterA += 1;
	}
	else if (digitalRead(counterB) == LOW) {
		counterB += 1;
	}



}
