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
  
#define LED_RED 6  
#define Hit_aPin 4  
#define Hit_bPin 2  
#define motionPin 9  
  
  
  
LiquidCrystal_I2C lcd(0x27, 16, 2);  
int Distance2;  
int Distance1;  
int reset = 0;  
int startStop = 0;  
int counterA =0; 
int help; 
  
  
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
    pinMode(LED_RED,OUTPUT);  
      
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
    
    delay(100);  
  
}  
  
void loop()  
{  
 
 motor(Distance2);  
   if (!digitalRead(2)) 
    counterA++; 
     
  else if (!digitalRead(4))  
    counterA=0; 
      
lcd.setCursor(0, 1);  
  lcd.print("Times Opened:"); 
  lcd.print(counterA); 
 
 
  motor(Distance2);  
    ultrasound2();  
     motor(Distance2);  
    ultrasound1();  
  
     
      
    wifi();  
     
     motor(Distance2);  
      
}  
//wifi related 
String espData(String command, const int timeout, boolean debug)  
{  
   motor(Distance2); 
    Serial.print("AT Command ==> ");  
    Serial.print(command);  
    Serial.println("     ");  
  
    String response = "";  
    espSerial.println(command);  
    long int time = millis();  
    motor(Distance2);  
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
motor(Distance2);  
}  
//inside ultrasound  
void ultrasound1() {  
    long pulseDuration; //variable needed by the ultrasound sensor code  
    int Distance1;       // Ultrasound distance in cm  
 motor(Distance2);  
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
    Distance1 = pulseDuration / 58;  //  Convert to cm  
  
    // 3. display the obstacle distance in serial monitor   
    Serial.print("Distance INSIDE = ");  
    Serial.print(Distance1);  
    Serial.println(" cm");  
    motor(Distance2);  
    delay(100);  
  
    if (Distance1 < 5) {  
        lcd.setCursor(0, 0);  
        lcd.clear();  
        lcd.print("Bin is full");  
        sendVal=1;  
        lcd.setCursor(0, 1);
       digitalWrite(LED_RED , LOW);  
          delay(100);  
    }  
    else 
    {  
        lcd.setCursor(0, 0);  
        lcd.print("Free to fill");  
        sendVal=0;  
        lcd.setCursor(0, 1);  
        digitalWrite(LED_RED , HIGH);  
    
        delay(100);  
    }  
    motor(Distance2);  
}  
//outside ultrasound  
void ultrasound2() {  
    long pulseDuration;  
    //variable needed by the ultrasound sensor code  
         // Ultrasound distance in cm  
// this main code runs repeatedly:  
    // 1. Produce a 15us (micro-second) HIGH pulse in Trig to trigger the sensor...  
     motor(Distance2); 
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
    motor(Distance2);  
    return (Distance2); 
    delay(100);  
}  
  
  
void motor(int help)  
{  
   if (help > 10)  
{  
 digitalWrite(motionPin, LOW);  
 delay(550);  
 digitalWrite(motionPin, HIGH);  
 delay(500);  
   
}  
else   
{  
  digitalWrite(motionPin, LOW);  
delay(4700);  
 digitalWrite(motionPin, HIGH);  
delay(4300);  
 
  }  
}  
  
void wifi()  
{  
  ultrasound2(); 
    motor(Distance2);  
    String sendData = "GET /update?api_key=" + myAPI + "&" + myFIELD + "=" + String(sendVal);  
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections  
    espData("AT+CIPSTART=0,\"TCP\",\"" + myHOST + "\"," + myPORT, 1000, DEBUG);  
    motor(Distance2);  
    espData("AT+CIPSEND=0," + String(sendData.length() + 4), 1000, DEBUG);  
    espSerial.find('>');  
    espSerial.println(sendData);  
    Serial.print("Value to be sent: ");  
    Serial.println(sendVal);  
      
    espData("AT+CIPCLOSE=0", 1000, DEBUG);  
    ultrasound2(); 
    motor(Distance2);  
    delay(500); 
}
