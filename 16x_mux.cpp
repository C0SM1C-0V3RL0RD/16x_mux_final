#include <DS3231.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT11.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  
DHT11 dht11(3);
DS3231 myRTC;

const int S0 = 6;  
const int S1 = 7;  
const int S2 = 8;  
const int S3 = 9;  

const int muxSignalPin = A0;  
const int signal = 11;  
const int buttonPin1 = 2; 
const int buttonPin2 = 12; 

bool century = false;
bool h12Flag;
bool pmFlag;

int activePins = 0;  

int muxPins[16][4] = {
  {0, 0, 0, 0},                // 0000 -  0
  {1, 0, 0, 0},               // 1000 - 1
  {0, 1, 0, 0},              // 0100 - 2
  {1, 1, 0, 0},             // 1100 - 3
  {0, 0, 1, 0},            // 0010 - 4
  {1, 0, 1, 0},           // 1010 - 5
  {0, 1, 1, 0},          // 0110 - 6
  {1, 1, 1, 0},         // 1110 - 7
  {0, 0, 0, 1},        // 0001 - 8
  {1, 0, 0, 1},       // 1001 - 9
  {0, 1, 0, 1},      // 0101 - 10
  {1, 1, 0, 1},     // 1101 - 11
  {0, 0, 1, 1},    // 0011 - 12
  {1, 0, 1, 1},   // 1011 - 13
  {0, 1, 1, 1},  // 0111 - 14
  {1, 1, 1, 1}  // 1111 - 15
};

void setup() {

  lcd.init();  
  lcd.backlight();                   
  
  lcd.setCursor(0, 0);
  lcd.print("Kirikkale");
  lcd.setCursor(0, 1);
  lcd.print("Universitesi");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fizik");
  lcd.setCursor(0, 1);
  lcd.print("Laboratuvari");
  delay(1000);
  lcd.clear();
  
  Wire.begin();
  Serial.begin(57600);
  
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  pinMode(signal, OUTPUT);
  digitalWrite(signal, HIGH); 
  
  pinMode(buttonPin1, INPUT_PULLUP);  
  pinMode(buttonPin2, INPUT_PULLUP);  
}

void loop() {

  lcd.clear();
  
  for (int i = 0; i < 16; i++) {

    digitalWrite(S0, muxPins[i][0]);
    digitalWrite(S1, muxPins[i][1]);
    digitalWrite(S2, muxPins[i][2]);
    digitalWrite(S3, muxPins[i][3]);
    
    lcd.setCursor(0, 0);
    lcd.print("Aktif");
    lcd.setCursor(0, 1);
    lcd.print("Sensor:");
    lcd.print(i + 1);  
    
    lcd.setCursor(12, 1);
    lcd.print(myRTC.getHour(h12Flag, pmFlag), DEC);
    lcd.print(':');
    lcd.print(myRTC.getMinute(), DEC);

    int temperature = 0;
    int humidity = 0;
    int result = dht11.readTemperatureHumidity(temperature, humidity);
    if (result == 0) {
      lcd.setCursor(9, 0);
      lcd.print(temperature);
      lcd.print("C");
      lcd.setCursor(12, 0);
      lcd.print("/");
      lcd.setCursor(13, 0);
      lcd.print(humidity);
      lcd.print("%");
    }

    delay(1000); 
  }
}
