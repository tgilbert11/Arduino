/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

#include <Time.h> 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int speed = 30;

// supposed to be 13
int led = 9;
int output = 12;
int offset1 = 6*3600 + 0*60 + 0;
int offset2 = 6*3600 + 0*60 + 0;
int offset3 = 6*3600 + 0*60 + 0;
int offset4 = 1*3600 + 30*60 + 0;
//TimeElements tm;

time_t thisTime = now();

//int minutes = second(t);
//int hours = 23;
//int days = 354;
double DUT = -0.7;
//int thisYear = 97;
int leapYear = 1;
int leapSecond = 1;
int DST = 3;

// the setup routine runs once when you press reset:
void setup() {                
  pinMode(led, OUTPUT);     
  pinMode(output, OUTPUT);  
  digitalWrite(led, LOW);
  Serial.begin(9600);
  //tm.Minute = 32;
  //setTime(makeTime(tm));
  blinkPattern();
}

// the loop routine runs over and over again forever:
void loop() {
  sendMessage();
}

void sendMessage() {
  thisTime = now()+offset1 + offset2 + offset3 + offset4;
  firstFrame();
  secondFrame();
  thirdFrame();
  fourthFrame();
  fifthFrame();
  sixthFrame();
//  delay(1000);
}

// minutes
void firstFrame() {
  Serial.println("-- First  --");
  int minutes = minute(thisTime);
  //minutes = 28;
  
  writeMarker();
  if (minutes >= 40) { writeOne(); } else { writeZero(); }
  if (minutes%40 >= 20) { writeOne(); } else { writeZero(); }
  if (minutes%20 >= 10) { writeOne(); } else { writeZero(); }
  writeZero();
  if (minutes%10 >= 8) { writeOne(); } else { writeZero(); }
  if (minutes%10%8 >= 4) { writeOne(); } else { writeZero(); }
  if (minutes%10%4 >= 2) { writeOne(); } else { writeZero(); }
  if (minutes%10%2 >= 1) { writeOne(); } else { writeZero(); }
  writeMarker();
}

// hours
void secondFrame() {
  Serial.println("-- Second --");
  int hours = hour(thisTime);
  
  writeZero();
  writeZero();
  if (hours >= 20) { writeOne(); } else { writeZero(); }
  if (hours%20 >= 10) { writeOne(); } else { writeZero(); }
  writeZero();
  if (hours%10 >= 8) { writeOne(); } else { writeZero(); }
  if (hours%10%8 >= 4) { writeOne(); } else { writeZero(); }
  if (hours%10%4 >= 2) { writeOne(); } else { writeZero(); }
  if (hours%10%2 >= 1) { writeOne(); } else { writeZero(); }
  writeMarker();
}

// days
void thirdFrame() {
  Serial.println("-- Third  --");
  int days = day(thisTime);
  
  writeZero();
  writeZero();
  if (days >= 200) { writeOne(); } else { writeZero(); }
  if (days%200 >= 100) { writeOne(); } else { writeZero(); }
  writeZero();
  if (days%100 >= 80) { writeOne(); } else { writeZero(); }
  if (days%100%80 >= 40) { writeOne(); } else { writeZero(); }
  if (days%100%40 >= 20) { writeOne(); } else { writeZero(); }
  if (days%100%20 >= 10) { writeOne(); } else { writeZero(); }
  writeMarker();
}

// days, DUT sign
void fourthFrame() {
  Serial.println("-- Fourth  --");
  int days = day(thisTime);
  
  if (days%10 >= 8) { writeOne(); } else { writeZero(); }
  if (days%10%8 >= 4) { writeOne(); } else { writeZero(); }
  if (days%10%4 >= 2) { writeOne(); } else { writeZero(); }
  if (days%10%2 >= 1) { writeOne(); } else { writeZero(); }
  writeZero();
  writeZero();
  if (DUT >= 0) { writeOne(); } else { writeZero(); }
  if (DUT < 0) { writeOne(); } else { writeZero(); }
  if (DUT >= 0) { writeOne(); } else { writeZero(); }
  writeMarker();
}

// DUT value, year
void fifthFrame() {
  Serial.println("-- Fifth  --");
  int thisYear = year(thisTime)-2000;
  
  int scratchDUT = DUT*10;
  if (scratchDUT < 0) { scratchDUT = scratchDUT*-1; }
  
  if (scratchDUT >= 8) { writeOne(); } else { writeZero(); }
  if (scratchDUT%8 >= 4) { writeOne(); } else { writeZero(); }
  if (scratchDUT%4 >= 2) { writeOne(); } else { writeZero(); }
  if (scratchDUT%2 >= 1) { writeOne(); } else { writeZero(); }
  writeZero();
  if (thisYear >= 80) { writeOne(); } else { writeZero(); }
  if (thisYear%80 >= 40) { writeOne(); } else { writeZero(); }
  if (thisYear%40 >= 20) { writeOne(); } else { writeZero(); }
  if (thisYear%20 >= 10) { writeOne(); } else { writeZero(); }
  writeMarker();
}

// year, modifiers
void sixthFrame() {
  Serial.println("-- Sixth  --");
  int thisYear = year(thisTime)-2000;
  
  if (thisYear%10 >= 8) { writeOne(); } else { writeZero(); }
  if (thisYear%10%8 >= 4) { writeOne(); } else { writeZero(); }
  if (thisYear%10%4 >= 2) { writeOne(); } else { writeZero(); }
  if (thisYear%10%2 >= 1) { writeOne(); } else { writeZero(); }
  writeZero();
  if (leapYear == 1) { writeOne(); } else { writeZero(); }
  if (leapSecond == 1) { writeOne(); } else { writeZero(); }
  if (DST >= 2) { writeOne(); } else { writeZero(); }
  if (DST%2 >= 1) { writeOne(); } else { writeZero(); }  
  writeMarker();
}



void writeOne() {
  Serial.println("One");
  digitalWrite(output, LOW);
  digitalWrite(led, LOW);
  delay(500/speed);
  digitalWrite(output, HIGH);
  digitalWrite(led, HIGH);
  delay(500/speed);
}

void writeZero() {
  Serial.println("Zero");
  digitalWrite(output, LOW);
  digitalWrite(led, LOW);
  delay(200/speed);
  digitalWrite(output, HIGH);
  digitalWrite(led, HIGH);
  delay(800/speed);
}

void writeMarker() {
  Serial.println("Marker");
  digitalWrite(output, LOW);
  digitalWrite(led, LOW);
  delay(800/speed);
  digitalWrite(output, HIGH);
  digitalWrite(led, HIGH);
  delay(200/speed);
}

void blinkPattern() {
  int DELAY = 100;
  digitalWrite(led, HIGH);
  delay(DELAY);
  digitalWrite(led, LOW);
  delay(DELAY);
  digitalWrite(led, HIGH);
  delay(DELAY);
  digitalWrite(led, LOW);
  delay(DELAY);
  digitalWrite(led, HIGH);
  delay(DELAY);
  digitalWrite(led, LOW);
  delay(DELAY);
}
