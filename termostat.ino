#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <math.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define ThermistorPIN 0                 // Analog Pin 0
float vcc = 4.91;                       // only used for display purposes, if used
                                         // set to the measured Vcc.
float pad = 9850;                       // balance/pad resistor value, set this to
                                         // the measured resistance of your pad resistor
float thermr = 10000;                   // thermistor nominal resistance

float Thermistor(int RawADC) {
   long Resistance;  
   float Temp;  // Dual-Purpose variable to save space.

   Resistance=pad*((1024.0 / RawADC) - 1); 
   Temp = log(Resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
   Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
   Temp = Temp - 273.15;  // Convert Kelvin to Celsius                      

   // BEGIN- Remove these lines for the function not to display anything
   //Serial.print("ADC: "); 
   //Serial.print(RawADC); 
   //Serial.print("/1024");                           // Print out RAW ADC Number
   //Serial.print(", vcc: ");
   //Serial.print(vcc,2);
   //Serial.print(", pad: ");
   //Serial.print(pad/1000,3);
   //Serial.print(" Kohms, Volts: "); 
   //Serial.print(((RawADC*vcc)/1024.0),3);   
   //Serial.print(", Resistance: "); 
   //Serial.print(Resistance);
   //Serial.print(" ohms, ");
   // END- Remove these lines for the function not to display anything

   // Uncomment this line for the function to return Fahrenheit instead.
   //temp = (Temp * 9.0)/ 5.0 + 32.0;                  // Convert to Fahrenheit
   return Temp;                                      // Return the Temperature
}

#define btn1 A1
#define btn2 A2
#define btn3 A3
#define przekaznik1 6
#define przekaznik2 7
#define przekaznik3 8

   int pos = 0;
   bool conf = false;
   int tzad;
   float temp;

void setup() {
   Serial.begin(115200);
   lcd.begin(16, 2);
   lcd.setCursor(0,1);
   lcd.cursor();
   pinMode(btn1, INPUT);
   pinMode(btn2, INPUT);
   pinMode(btn3, INPUT);
   pinMode(przekaznik1, OUTPUT);
   pinMode(przekaznik2, OUTPUT);
   pinMode(przekaznik3, OUTPUT);
   EEPROM[0] = tzad;
}

void loop() {
  lcd.clear();
   temp=Thermistor(analogRead(ThermistorPIN));       // read ADC and  convert it to Celsius
   lcd.setCursor(0, 1);
   lcd.print("Temp: ");
   lcd.print(temp, 1);
   lcd.setCursor(1,1);
   lcd.print("Temp. zad: ");
   lcd.print(tzad);
   
   if (btn3 == HIGH) {conf = true;}
   if (conf == true) {
    menu();
   }
      
   Serial.print("Celsius: "); 
   Serial.print(temp,1);                             // display Celsius
   Serial.println("");

   if (temp >= tzad+1) {
    digitalWrite(przekaznik1, HIGH);
    digitalWrite(przekaznik2, HIGH);
    digitalWrite(przekaznik3, HIGH);
  } else if (temp < tzad) {
    digitalWrite(przekaznik1, LOW);
    digitalWrite(przekaznik1, LOW);
    digitalWrite(przekaznik1, LOW);
  }
}

void menu() {
  lcd.clear();
    if (btn1 == HIGH) {pos++;}
    if (btn2 == HIGH) {pos--;}

    if (pos == 0 && btn3 == HIGH) {
      tzad--;
      if (tzad<-15) {tzad=7;}
      lcd.setCursor(0,1);
      lcd.print("Temp:");
      lcd.print(temp, 1);
      lcd.setCursor(1,1);
      lcd.print("Temp. zad: ");
      lcd.print(tzad);
      lcd.cursor();
      delay(300);
      lcd.noCursor();
      delay(300);
    }

    if (pos == 1) {
      lcd.setCursor(0,1);
      lcd.print("Zapisz");
      lcd.cursor();
      delay(300);
      lcd.noCursor();
      delay(300);
      if (btn3 == HIGH) {
        tzad = EEPROM[0];
        conf = false;
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Zapisano");
        delay(2000);
      }
    }
    if (pos > 1) {pos = 0;}
    if (pos < 0) {pos = 1;} 
  }

