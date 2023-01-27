#include <SoftwareSerial.h> //Software Serial library
#include <LiquidCrystal_I2C.h>
SoftwareSerial espSerial(12, 13);   //Pin 12 and 13 act as RX and TX. Connect them to TX and RX of ESP8266      
#define DEBUG true
String mySSID = "Galaxy S22 4429";       // WiFi SSID
String myPWD = "Ninja???"; // WiFi Password
String myAPI = "I8RPAYBYPSFHVBSP";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD = "field1";
int sendVal;
#define TrigPin1 11   // U/S1 Trig connected to pin 11
#define EchoPin1 10   // U/S1 Echo connected to pin 10
#define TrigPin2 8
#define EchoPin2 7

#define LED_RED 6;
#define Hit_aPin 4;
#define Hit_bPin 2;
#define motionPin 9



LiquidCrystal_I2C lcd(0x27, 16, 2);
int Distance2;
int reset = 0;
int startStop = 0;

void setup()
{
    //wifi
    Serial.begin(9600);
    espSerial.begin(9600);

    espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
    espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
    espData("AT+CWJAP=\"" + mySSID + "\",\"" + myPWD + "\"", 1000, DEBUG);   //Connect to WiFi network
    pinMode(2, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    
    //ultrasound
    pinMode(TrigPin1, OUTPUT);  //Arduino's output, ranger's input
    pinMode(EchoPin1, INPUT);  //Arduino's input, ranger's output
    pinMode(EchoPin2, INPUT);
    pinMode(TrigPin2, OUTPUT);

    pinMode(motionPin, OUTPUT); //digital output for motor direction control


    //targetboard
    PCICR |= B00000100; // Enable interrupts on PD port
    PCMSK2 |= B00010100; // Trigger interrupts on pins D2 and D4
    
    //lcd
    lcd.init();
    lcd.begin(16, 2);
    lcd.backlight();
    lcd.clear();
  
    delay(1000);

}

void loop()
{
  
    ultrasound2();
    ultrasound1();

    motor();
    
    wifi();
}
//wifi related
String espData(String command, const int timeout, boolean debug)
{
    Serial.print("AT Command ==> ");
    Serial.print(command);
    Serial.println("     ");

    String response = "";
    espSerial.println(command);
    long int time = millis();
    while ((time + timeout) > millis())
    {
        while (espSerial.available())
        {
            char c = espSerial.read();
            response += c;
        }
    }
    if (debug)
    {
        Serial.print(response);
    }
    return response;

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
    Serial.print("Distance INSIDE = ");
    Serial.print(Distance);
    Serial.println(" cm");
    delay(500);

    if (Distance < 40) {
        lcd.setCursor(0, 0);
        lcd.print("Bin is full");
        lcd.setCursor(0, 1);
        digitalWrite(6, HIGH);
        delay(2000);

        delay(1000);
    }
    else {
        lcd.setCursor(0, 0);
        lcd.print("Free to fill");
        lcd.setCursor(0, 1);
        digitalWrite(6, LOW);
        delay(2000);
        delay(1000);
    }
}
void ultrasound2() {
    long pulseDuration;
    //variable needed by the ultrasound sensor code
         // Ultrasound distance in cm

    // this main code runs repeatedly:
    // 1. Produce a 15us (micro-second) HIGH pulse in Trig to trigger the sensor...
    digitalWrite(TrigPin2, LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin2, HIGH);
    delayMicroseconds(5);
    digitalWrite(TrigPin2, LOW);

    // 2. Use pulseIn() function to measure the duration of the HIGH pulse in Echo 
    // every 58 us is an obstacle distance of 1 cm

    pulseDuration = pulseIn(EchoPin2, HIGH);
    Distance2 = pulseDuration / 58;  //  Convert to cm

    // 3. display the obstacle distance in serial monitor 
    Serial.print("Distance OUTSIDE = ");
    Serial.print(Distance2);
    Serial.println(" cm");
    delay(500);
}


void motor() {
    if (Distance2 < 40)
    {
        digitalWrite(motionPin, LOW);
        delay(3000);

    }
    else
    {
        digitalWrite(motionPin, HIGH);
        delay(2000);

    }



void wifi() {
    sendVal = random(100); // Send a random number between 1 and 1000
    String sendData = "GET /update?api_key=" + myAPI + "&" + myFIELD + "=" + String(sendVal);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\"" + myHOST + "\"," + myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," + String(sendData.length() + 4), 1000, DEBUG);
    espSerial.find('>');
    espSerial.println(sendData);
    Serial.print("Value to be sent: ");
    Serial.println(sendVal);

    espData("AT+CIPCLOSE=0", 1000, DEBUG);
    delay(15000);

}