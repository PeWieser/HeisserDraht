/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <LiquidCrystal_I2C.h>  //Bibliothek für Display wird eingebunden
#include <Adafruit_NeoPixel.h>  //Bibliothek für LED Streifen wird eingebunden
#include <SoftwareSerial.h>     //Bibliothek für Software Serial wird eingebunden, damit über alle Pins eine serielle Verbindung hergestellt werden kann

SoftwareSerial BTSerial(9, 10);   // RX | TX  PINs werden als RX und TX konfiguriert

int LEDpin = 7; //Pin wird für Kommunkikation mit LED Streifen festgelegt
int NUMPIXELS = 11; //Anzahl LEDs auf Streifen wird festgelegt
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LEDpin, NEO_GRB + NEO_KHZ800);  //LED Streifen wirdin pixels konfiguriert

// LCD Konfiguration
int lcdColumns = 16;  //Anzahl der Stelle
int lcdRows = 2;      //Anzahl der Zeilen

// set LCD address
// if you don't know your display address, run an I2C scanner sketch
// Display Adresse = 0x3F
LiquidCrystal_I2C lcd(0x3F, lcdColumns, lcdRows);

int i;   //i für Hochzählen

//ifm Logo als Byte Code
byte ibottom[8] = {
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10111
};

byte itop[8] = {
  0b00000,
  0b01111,
  0b10001,
  0b10001,
  0b10001,
  0b01110,
  0b00000,
  0b11110
};

byte fbottom[8] = {
  0b00010,
  0b00010,
  0b00010,
  0b00010,
  0b00010,
  0b00010,
  0b00010,
  0b11111
};

byte ftop[8] = {
  0b11111,
  0b00000,
  0b00000,
  0b00011,
  0b00010,
  0b00011,
  0b00000,
  0b00011
};

byte mlbottom[8] = {
  0b00000,
  0b00000,
  0b00010,
  0b00101,
  0b00101,
  0b00101,
  0b00101,
  0b11101
};

byte mltop[8] = {
  0b11100,
  0b00100,
  0b00100,
  0b11100,
  0b00000,
  0b11100,
  0b00100,
  0b11101
};

byte mmbottom[8] = {
  0b00000,
  0b00000,
  0b00001,
  0b00010,
  0b00010,
  0b00010,
  0b00010,
  0b11110
};

byte mmtop[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111
};

byte mrbottom[8] = {
  0b00001,
  0b00001,
  0b00001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b11111
};

byte mrtop[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11110
};


int Start = 11;   //Pin, an dem der Startpin(draht) angeschlossen ist
int Mid = 12;     //Pin, an dem der Mittepin(draht) angeschlossen ist
int End = 13;     //Pin, an dem der Endpin(draht) angeschlossen ist
int Motor = 2;    //Pin, an dem der Motor angeschlossen ist

boolean timer_change = true;    //legt fest, ob der Timer geändert wir, oder nicht

//LED
int mistakes = 0;   //Anzahl der gemachten Fehler
boolean newmistake = false;   //legt fest, ob ein neuer Fehler gemacht wurde
boolean ledeffectchange = true;   //legt fest, ob LEDs geändert werden, oder nicht
int currentrot;   //jetziger rot Wert
int currentgelb;  //jetziger gelb Wert
int currentblau;  //jetziger blau Wert


int rot;  //gewünschter rot Wert
int gelb; //gewünschter gelb Wert
int blau; //gewünschter blau Wert

//int rot=31;
//int gelb=19;
//int blau=242;


int rotc1=50;   //rot Wert für ersten Stand des Pulsierens
int gelbc1=60;  //gelb Wert für ersten Stand des Pulsierens
int blauc1=230; //blau Wert für ersten Stand des Pulsierens

int rotc2=20;   //rot Wert für zweite Stand des Pulsierens
int gelbc2=5;   //gelb Wert für zweite Stand des Pulsierens
int blauc2=200; //blau Wert für zweite Stand des Pulsierens

int mistakerot = 180; //rot Wert wenn ein Fehler gemacht wurde
int mistakegelb = 30; //gelb Wert wenn ein Fehler gemacht wurde
int mistakeblau = 20; //blau Wert wenn ein Fehler gemacht wurde

boolean mistakecolour = false;  //legt fest, ob die Fehlerfarbe angezeigt werden soll

boolean releasemistake = true;  //legt fest, ob nach einem Fehler der Draht nicht mehr berührt wird

long startmillis;   //Zeit mit der gestartet wurde
long millisnow;     //Zeit, die seit Spielstrt abgelaufen ist

int gamehasended = 0;   //Legtt fest, ob das Spiel beendet ist

int timefac = 20;   //Pause zwischen den einzelnen Aufrufen, bei game
boolean changebright =false;  //legt fest, ob das Pulsieren anfangen soll
int minute, sekunde;  
int currentseconds;
int endminute, endsekunde;    //Zeit, mit der das Spiel beendet wurde

int ingame = 0;

//soll bereit getriggert werden
int displayon = 1;
int displayoff = 0;

long timebetween=0;

//wenn Spiel beendet wurde
void endgame(){
  lcd.clear();
  delay(50);
  lcd.setCursor(3, 1);
  lcd.print("Fehler: ");
  lcd.print(mistakes);
  lcd.setCursor(3,0);
  lcd.print("Zeit: ");    //benötigte Zeit wird angezeigt
  if (endminute<10) lcd.print("0");
  lcd.print(endminute);
  lcd.print(":");
  if (endsekunde<10) lcd.print("0");
  lcd.print(endsekunde);
  gamehasended = 1;
  delay(10000);
  displayon=1;
}


void gamestart(){
  releasemistake = true;
  mistakes = 0;
  startmillis = millis();
  pinMode(Motor, OUTPUT);
  currentrot = 38;
  currentgelb = 31;
  currentblau = 181;
  for(i=0;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(currentrot,currentgelb,currentblau)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
  }
  while(ingame==1){
    millisnow = (millis()-startmillis);
    sekunde = (millisnow/1000) % 60; // Das Zeichen: "%" ist der Modulo-Operator
    minute  = (millisnow/60000) % 60;
    //currentTime = millisnow;
    timercount();
    mistake();
    endfunction();
    ledeffect();
    //long nextMinWakeUp = (nexttimecount < nextchangeleds) ? nexttimecount : nextchangeleds;
    //int delaytime = (nextMinWakeUp-currentTime);
    delay(timefac);       
  }
}

long timercount(){
   if(timer_change==true) {
      timer_change = false;
      //Serial.print(millisnow);
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("Zeit: ");
       if (minute<10) lcd.print("0");
      lcd.print(minute);
      lcd.print(":");
      if (sekunde<10) lcd.print("0");
      lcd.print(sekunde);
      currentseconds=sekunde;
      }
    else {
      if(currentseconds!=sekunde) {
      timer_change = true;
      }
    }
  }
long mistake(){
   if(digitalRead(Mid)==HIGH && releasemistake==true && timebetween>=410){ 
    newmistake = true;
    mistakes+=1;
    releasemistake = false;
    timebetween=0;
   }
   if(digitalRead(Mid)==LOW){
    releasemistake = true;
    int zeit = (420/timefac);
    timebetween+=zeit;
   }
}

long endfunction(){
    if(digitalRead(End)==HIGH){
      ingame = 0;
      endminute = minute;
      endsekunde = sekunde;
      for(i=0;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(20,255,20)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
      }
      endgame();
    }
}

//pulsieren der LED
long ledeffect(){
    if(ledeffectchange) {
      if (newmistake==true){
        mistakecolour = true;
        rot=mistakerot;
        gelb=mistakegelb;
        blau=mistakeblau;
        newmistake=false;
        timefac=3;
      }
      if (currentrot==mistakerot && currentgelb==mistakegelb && currentblau==mistakeblau){
        mistakecolour = false;        
      }
      if(changebright==true && mistakecolour==false){
        rot=rotc2;
        gelb=gelbc2;
        blau=blauc2;
      }
      if(changebright==false && mistakecolour==false){
        rot=rotc1;
        gelb=gelbc1;
        blau=blauc1;
      }
      if(rot!=currentrot){
        if(rot<=currentrot){
          currentrot-=1;
        }
        else{
          currentrot+=1;
        }
      }
      if(gelb!=currentgelb){
        if(gelb<=currentgelb){
          currentgelb-=1;
        }
        else{
          currentgelb+=1;
        }
      }
      if(blau!=currentblau){
        if(blau<=currentblau){
          currentblau-=1;
        }
        else{
          currentblau+=1;
        }
      }
      for(i=0;i<NUMPIXELS;i++){
        pixels.setPixelColor(i, pixels.Color(currentrot,currentgelb,currentblau)); // Moderately bright green color.
      }
      pixels.show(); // This sends the updated pixel color to the hardware.
     
      if(currentrot==rotc2 && currentgelb==gelbc2 && currentblau==blauc2) {
        changebright = false;
        mistakecolour = false;
        timefac=70;
      }
      else {
       if(currentrot==rotc1 && currentgelb==gelbc1 && currentblau==blauc1) {
          changebright = true;
          timefac=70;
        }
      }
    }
}


void lookforhigh(){
  int Startval = digitalRead(Start);
  int Midval = digitalRead(Mid);
  int Endval = digitalRead(End);
  
  if(Startval==HIGH){
    ingame = 1;
    gamestart();
  }
  if(Midval==HIGH){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hier kann nicht");
    lcd.setCursor(0, 1);
    lcd.print("gestartet werden");
    delay(1500);
    lcd.clear();
    int displayon = 1;
  }

  if(Endval==HIGH){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hier kann nicht");
    lcd.setCursor(0, 1);
    lcd.print("gestartet werden");
    delay(1500);
    lcd.clear();
    int displayon = 1;
  }
}

void bereit(){
  if(displayon==1){
    lcd.clear();
    delay(50);
    lcd.setCursor(5, 0);
    lcd.print("Bereit");
    displayon=0;
  }
}

void setup(){
  //Serial.begin(9600);
  BTSerial.begin(9600);
  pixels.begin();
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  pinMode(Start, INPUT);  // sets the digital pin 13 as output
  pinMode(Mid, INPUT);
  pinMode(End, INPUT);
  pinMode(Motor, OUTPUT);


  for(i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(255,50,0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
  //ifm Logo
  lcd.createChar(0, ibottom);
  lcd.createChar(1, itop);
  lcd.createChar(2, fbottom);
  lcd.createChar(3, ftop);
  lcd.createChar(4, mlbottom);
  lcd.createChar(5, mltop);
  lcd.createChar(6, mmbottom);
  lcd.createChar(7, mmtop);
  lcd.createChar(8, mrbottom);
  lcd.createChar(9, mrtop);

  lcd.setCursor(5, 0);
  // print message
  lcd.write(1);
  lcd.setCursor(6, 0);
  lcd.write(3);
  lcd.setCursor(7, 0);
  lcd.write(5);
  lcd.setCursor(8, 0);
  lcd.write(7);
  lcd.setCursor(9, 0);
  lcd.write(9);
  // set cursor to fifth column, first row
  lcd.setCursor(5, 1);
  // print message
  lcd.write(0);
  lcd.setCursor(6, 1);
  lcd.write(2);
  lcd.setCursor(7, 1);
  lcd.write(4);
  lcd.setCursor(8, 1);
  lcd.write(6);
  lcd.setCursor(9, 1);
  lcd.write(8);
  delay(3000);
  // clears the display to print new message
  lcd.clear();
  delay(50);
  lcd.setCursor(1, 0);
  lcd.print("Heisser Draht");
  delay(2000);
}

void loop(){
  if(gamehasended==1){
    for(i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(255,50,0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    }
    gamehasended=0;
  }
    bereit();
    lookforhigh();
    //long nextMinWakeUp = (nexttimecount < nextchangeleds) ? nexttimecount : nextchangeleds;
    //int delaytime = (nextMinWakeUp-currentTime);
    delay(20);
}
