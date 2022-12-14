/*
* Microcontroladores e aplicações - 2022.1
* Anderson Clemente
* Ester de Lima Pontes Andrade
* Igor Rocha Pereira
* Matheus Alves Fireman
*/

#include <LiquidCrystal_I2C.h>
 
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const int pwm = 9;
const int minSensor = -550;
const int maxSensor = 1500;

int sensor = 0;
float voltage = 0;
int rpm = 0;
double temp = 0;

void setup()
{
  pinMode(A0, INPUT);
  pinMode(pwm, OUTPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop()
{
  getTemperature();
  controlRPM();

  lcd.clear();
  printOnLCD();

  delay(800);
}

void getTemperature() {
  sensor = analogRead(A0);
  voltage = sensor * (5000 / 1024.0);
  Serial.println(voltage);
  temp = voltage / 10;
}

void controlRPM() {
  rpm = 0;
  if(temp >= 20) {
    rpm = map(voltage, minSensor, maxSensor, 0, 255);
  } 
  if (temp >= 60) {
    rpm = 255;
  }
  analogWrite(pwm, rpm);
}

void printOnLCD() {
  int realRPM = map(rpm, 0, 255, 0, 8505);
  lcd.print("TEMP: " + String(temp) + " C");
  lcd.setCursor(0, 1);
  lcd.print("RPM: " + String(realRPM));
}
