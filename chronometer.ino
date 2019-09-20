#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//Variables
int status = 0; //0 - V1 and v2 stopped 
                //1 - Running v1 and v2
                //2 - Running v2 and v1 stopped
                //3 - Running v1 and v2 stopped

//time
unsigned long begin = 0;
unsigned long actual = 0;
unsigned long v1Time = 0;
unsigned long v2Time = 0;

//Buttons 
//arduino pins don't change
const int v1Pin = 2;
const int v2Pin = 4;
const int startPin = 6;
// value of the button (pressed or not)
int v1State;
int v2State;
int startState;
int startLastState;


void start(){
  begin = actual;
  v1Time = actual;
  v2Time = actual;
  status = 1;
}

void finishV1(){
  switch(status){
    case 0: break;
    case 1: {
      status = 2;
      v1Time = actual;
      break;
    }
    case 2: break;
    case 3: {
      status = 0;
      v1Time = actual;
      break;
    }
  }
}

void finishV2(){
  switch(status){
    case 0: break;
    case 1: {
      status = 3;
      v2Time = actual;
      break;
    }
    case 2: {
      status = 0;
      v2Time = actual;
      break;
    } 
    case 3: break;
  }
}

void pressStart(){
    if(startLastState) return;
    start();
    status = 0;
}

void setup()
{
  lcd.init();                      // initialize the lcd 
  status = 0;
  lcd.backlight();
  lcd.setCursor(4,0);
  lcd.print("Bom Dia!");
  lcd.setCursor(0,1);
  lcd.print("Made by Marco <3");
  delay(3000);
  lcd.clear();
  actual = 0;
  start();
  status = 0;
}

char * TimeToString(unsigned long t){
 long o = t;
 t = t/1000;
 static char str[12];
 long h = t / 3600;

 t = t % 3600;
 int m = t / 60;
 int s = t % 60;
 int ms = o % 1000;
 sprintf(str, "%02d:%02d:%02d", m, s, ms);
 return str;
}

void readButtons(){
  v1State = digitalRead(v1Pin);
  v2State = digitalRead(v2Pin);
  startLastState = startState;
  startState = digitalRead(startPin);
  if (v1State) finishV1();
  if (v2State) finishV2();
  if (startLastState && !startState) start();
  if (startState) pressStart();
}

void loop(){
  actual = millis();
  readButtons();
  switch(status){

    case 0:{
      lcd.setCursor(0,0);
      lcd.print("Via 1");
      lcd.setCursor(8,0);
      lcd.print("Via 2");
      lcd.setCursor(0,1);
      lcd.print(TimeToString(v1Time-begin));
      lcd.setCursor(8,1);
      lcd.print(TimeToString(v2Time-begin));
      break;
    }
    case 1:{
      lcd.setCursor(0,0);
      lcd.print("Via 1");
      lcd.setCursor(8,0);
      lcd.print("Via 2");
      lcd.setCursor(0,1);
      lcd.print(TimeToString(actual-begin));
      lcd.setCursor(8,1);
      lcd.print(TimeToString(actual-begin));
      break;
    }
    case 2:{
      lcd.setCursor(0,0);
      lcd.print("Via 1");
      lcd.setCursor(8,0);
      lcd.print("Via 2");
      lcd.setCursor(0,1);
      lcd.print(TimeToString(v1Time-begin));
      lcd.setCursor(8,1);
      lcd.print(TimeToString(actual-begin));
      break;
    }
    case 3:{
      lcd.setCursor(0,0);
      lcd.print("Via 1");
      lcd.setCursor(8,0);
      lcd.print("Via 2");
      lcd.setCursor(0,1);
      lcd.print(TimeToString(actual-begin));
      lcd.setCursor(8,1);
      lcd.print(TimeToString(v2Time-begin));
      break;
    }
  }
}
