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
const int minSensor = -55;
const int maxSensor = 150;
const int variationCutoff = 100;
const int runningAverageCount = 50;
int nextRunningAverage;

int sensor = 0;
float voltage = 0;
float lastVoltage;
bool firstIteration = true;
int rpm = 0;
double temp = 0;
float runningAverageBuffer[runningAverageCount];
int filteredTemp = 0;

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
  filterTemperature();
  controlRPM();

  lcd.clear();
  printOnLCD();

  firstIteration = false;

  delay(300);
}

void getTemperature() {
  sensor = analogRead(A0);
  voltage = sensor * (5000 / 1024.0);
  if(!firstIteration) {
    if(abs(voltage - lastVoltage) >= variationCutoff || voltage == 0) {
      voltage = lastVoltage;
    }
  }
  lastVoltage = voltage;
  temp = voltage / 10;
}

void filterTemperature() {
  runningAverageBuffer[nextRunningAverage++] = temp;

  if (nextRunningAverage >= runningAverageCount)
  {
    nextRunningAverage = 0; 
  }

  for(int i = 0; i < runningAverageCount; ++i)
  {
    filteredTemp += runningAverageBuffer[i];
  }

  filteredTemp = filteredTemp/runningAverageCount;
}

void controlRPM() {
  rpm = 0;
  if(filteredTemp >= 20) {
    rpm = map(filteredTemp, minSensor, maxSensor, 0, 255);
  } 
  if (filteredTemp >= 60) {
    rpm = 255;
  }
  analogWrite(pwm, rpm);
}

void printOnLCD() {
  int realRPM = map(rpm, 0, 255, 0, 8505);
  lcd.print("TEMP: " + String(filteredTemp) + " C");
  lcd.setCursor(0, 1);
  lcd.print("RPM: " + String(realRPM));
}
