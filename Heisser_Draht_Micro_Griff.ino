#include <SoftwareSerial.h>

SoftwareSerial BTSerial(3, 4);   // RX | TX

int sensor = 10;
int wire = 8;
char blueToothVal; 
char lastValue;
int lasthigh = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  //serieller Monitor wird gestartet, Baudrate auf 9600 festgelegt
  pinMode(sensor,INPUT);  //PIN 13 wird als Ausgang festgelegt
  pinMode(wire,OUTPUT);
  BTSerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(sensor)==HIGH){
    BTSerial.println(1);
    digitalWrite(wire, HIGH);
    lasthigh = 1;
    delay(10);
  }
  if (lasthigh==1){
    if(digitalRead(sensor)==LOW){
      BTSerial.println(0);
      digitalWrite(wire, LOW);
    }
    lasthigh = 0;
  }
  delay(10);
}
