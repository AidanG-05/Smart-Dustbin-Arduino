#include <LiquidCrystal_I2C.h>

#define TrigPin1 11   // U/S1 Trig connected to pin 11
#define EchoPin1 10   // U/S1 Echo connected to pin 10


#define Hit_aPin 4;
#define Hit_bPin 2;

LiquidCrystal_I2C lcd(0x27, 16, 2);


int reset = 0;
int startStop = 0;

// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(9600);
    pinMode(2, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    pinMode(TrigPin1, OUTPUT);  //Arduino's output, ranger's input
    pinMode(EchoPin1, INPUT);   //Arduino's input, ranger's output

    PCICR |= B00000100; // Enable interrupts on PD port
    PCMSK2 |= B00010100; // Trigger interrupts on pins D2 and D4
    lcd.init();
    lcd.begin(16, 2);
    lcd.backlight();
    lcd.clear();
}

// the loop function runs over and over again until power down or reset
void loop()
{

    ultrasound1();
}

ISR(PCINT2_vect) {
    if (digitalRead(reset) == LOW)
    {
        Serial.print("Free to fill");
    }
    else if (digitalRead(startStop) == LOW) {

    }
}



void ultrasound1() {
    long pulseDuration; //variable needed by the ultrasound sensor code
    int Distance;       // Ultrasound distance in cm

    // this main code runs repeatedly:
    // 1. Produce a 15us (micro-second) HIGH pulse in Trig to trigger the sensor...
    digitalWrite(TrigPin1, LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin1, HIGH);
    delayMicroseconds(5);
    digitalWrite(TrigPin1, LOW);

    // 2. Use pulseIn() function to measure the duration of the HIGH pulse in Echo 
    // every 58 us is an obstacle distance of 1 cm

    pulseDuration = pulseIn(EchoPin1, HIGH);
    Distance = pulseDuration / 58;  //  Convert to cm

    // 3. display the obstacle distance in serial monitor 
    Serial.print("Distance = ");
    Serial.print(Distance);
    Serial.println(" cm");
    delay(500);

    if (Distance < 40) {
        lcd.setCursor(0, 0);
        lcd.print("Bin is full");
        lcd.setCursor(0, 1);
        delay(1000);
    }
    else {
        lcd.setCursor(0, 0);
        lcd.print("Free to fill");
        lcd.setCursor(0, 1);
        delay(1000);
    }
}

