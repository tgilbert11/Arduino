#include <Time.h> 


int speed = 30;

int led = 13;
int input = 12;
int minutesSerial = 11;
int standbySwitch = 10;
int standbyLight = 6;
int acquiringLight = 5;
int failedLight = 4;
int bulbsOnSwitch = 3;
int dataOutput = 7;
int latchOutput = 8;
int clockOutput = 9;
int brightnessPin = A0;

int zeroTime = 200;
int oneTime = 500;
int markerTime = 800;

boolean wasJustHigh = false;
unsigned long lastSwitch = 0;

int lastBit = 0;
boolean isCurrentlyReading = false;
boolean isStillValid = false;
boolean DUTSignValid = false;
int readPosition = 0;

int minutes = 0;
int hours = 0;
int days = 0;
int DUTSign = 1;
double DUT = 0.0;
int thisYear = 0;
int leapYear = 0;
int leapSecond = 0;
int DST = 0;
TimeElements tm;

boolean tubesOn = false;
boolean PWMHigh = false;
boolean failedLastTime = false;
boolean currentlyAcquiringTime = true;
boolean standbyMode = false;

unsigned long millisLastSecond = 0;
int secondsSinceLastSyncAttempt = 0;
// 4 hours is 14400 seconds 
int syncInterval = 60;
unsigned long lastSuccessfulSync = 0;
// 24 hours is 86400000
int unsuccessfulSyncTolerance = 86400000;

unsigned long acquisitionAttemptStarted = 0;
int secondsOfSyncAttempt = 4;

unsigned long numberOfLoopsSinceLastSecond = 0;
double fakeBrightness = 0.9;
int PWMPeriod = 18;
unsigned long millisOfLastPWMStart = 0;

int checkNumber = 0;

// the setup routine runs once when you press reset:
void setup() {                
  pinMode(led, OUTPUT);     
  pinMode(input, INPUT);  
  pinMode(standbySwitch, INPUT);

  pinMode(standbyLight, OUTPUT);     
  pinMode(minutesSerial, OUTPUT);
  pinMode(acquiringLight, OUTPUT);     
  pinMode(failedLight, OUTPUT);     
  pinMode(bulbsOnSwitch, OUTPUT);     
  pinMode(dataOutput, OUTPUT);     
  pinMode(latchOutput, OUTPUT);     
  pinMode(clockOutput, OUTPUT);     
  pinMode(brightnessPin, INPUT);

  digitalWrite(standbyLight, LOW);
  digitalWrite(acquiringLight, HIGH);
  digitalWrite(failedLight, LOW);
  digitalWrite(bulbsOnSwitch, LOW);

  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  writeToTubes(0, checkNumber/100, checkNumber%100);
  checkNumber += 1;
  if (checkNumber > 9999) { 
    checkNumber = 0; 
  }
  delay(7);
}

void turnTubesOn() {
  tubesOn = true;
  //writeToTubes(minute());
  //  digitalWrite(bulbsOnSwitch, LOW*!PWMHigh*currentlyAcquiringTime);
}
void turnTubesOff() {
  tubesOn = false;
  //  digitalWrite(bulbsOnSwitch, HIGH);
}

void invalidate() {
  isCurrentlyReading = false;
  isStillValid = false;
  Serial.println(" invalid");
}

void timeAcquired() {
  if (standbyMode) { 
    digitalWrite(standbyLight, HIGH); 
  }
  millisLastSecond = millis();
  secondsSinceLastSyncAttempt = 0;
  currentlyAcquiringTime = false;
  digitalWrite(acquiringLight, LOW);
  turnTubesOn();
}

void reacquireTime() {
  digitalWrite(standbyLight, LOW);
  currentlyAcquiringTime = true;
  digitalWrite(acquiringLight, HIGH);
  //Serial.print("reacquireTime");
  acquisitionAttemptStarted = millis();
  turnTubesOff();
}

void syncSucceeded() {
  lastSuccessfulSync = millis();
  failedLastTime = false;
  digitalWrite(failedLight, LOW);
  timeAcquired();
}


void attemptFailed() {
  Serial.println("failed...");
  failedLastTime = true;
  digitalWrite(failedLight, HIGH);
  timeAcquired();
}

double brightness() {
  //Serial.println(analogRead(brightnessPin));
  double thisBrightness = 1.0*analogRead(brightnessPin)/1024;
  //if (thisBrightness > 1.0) { thisBrightness = 1.0; }
  //if (thisBrightness < 0.0) { thisBrightness = 0.0; }

  //int steps = 4;
  //thisBrightness = 1.0*((int)(thisBrightness*steps))/(steps-1);
  return thisBrightness;
}

void ensurePoweredProperly() {
  boolean  on = currentlyAcquiringTime || standbyMode;
  if (currentlyAcquiringTime || standbyMode) {
    digitalWrite(bulbsOnSwitch, HIGH);
    digitalWrite(standbyLight, HIGH);
  }
  else {
    analogWrite(bulbsOnSwitch, 255*(1-brightness()) );
    digitalWrite(standbyLight, LOW);
  }
}

void checkStandby() {
  standbyMode = digitalRead(standbySwitch);
}

void writeToTubes(int hours, int minutes, int seconds) {
  int leftSeconds = seconds / 10;
  int rightSeconds = seconds % 10;
  int leftMinutes = minutes / 10;
  int rightMinutes = minutes % 10;
  int leftHours = hours / 10;
  int rightHours = hours % 10;

  digitalWrite(latchOutput, LOW);

  writeDigit(rightHours%2/1 > 0, rightMinutes%2/1 > 0, rightSeconds%2/1 > 0);
  writeDigit(rightHours%4/2 > 0, rightMinutes%4/2 > 0, rightSeconds%4/2 > 0);
  writeDigit(rightHours%8/4 > 0, rightMinutes%8/4 > 0, rightSeconds%8/4 > 0);
  writeDigit(rightHours/8 > 0, rightMinutes/8 > 0, rightSeconds/8 > 0);
  writeDigit(leftHours%2/1 > 0, leftMinutes%2/1 > 0, leftSeconds%2/1 > 0);
  writeDigit(leftHours%4/2 > 0, leftMinutes%4/2 > 0, leftSeconds%4/2 > 0);
  writeDigit(leftHours%8/4 > 0, leftMinutes%8/4 > 0, leftSeconds%8/4 > 0);
  writeDigit(leftHours/8 > 0, leftMinutes/8 > 0, leftSeconds/8 > 0);

  digitalWrite(latchOutput, HIGH);

}

void writeDigit(boolean hours, boolean minutes, boolean seconds) {
  digitalWrite(clockOutput, LOW);
  digitalWrite(dataOutput, seconds);
  digitalWrite(minutesSerial, minutes);
  //digitalWrite(hoursSerial, hours);
  digitalWrite(clockOutput, HIGH);
}











