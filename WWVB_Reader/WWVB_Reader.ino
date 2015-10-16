#include <Time.h> 


int speed = 1;

int led = 13;
int input = 12;
int minutesSerial = 11;
int standbySwitch = 10;
int standbyLight = 6;
int acquiringLight = 5;
int failedLight = 4;
int bulbsOnSwitch = 3;
int dataOutput = 7;
int secondsSerial = 2;
int latchOutput = 8;
int clockOutput = 9;
int brightnessPin = A0;

int zeroTime = 200;
int oneTime = 500;
int markerTime = 800;

boolean wasJustHigh = false;
boolean didJustHitMarkerOn59 = false;
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
int syncInterval = 14400;
unsigned long lastSuccessfulSync = 0;
// 24 hours is 86400000
int unsuccessfulSyncTolerance = 86400000;
int noiseCount = 0;
boolean currentlyHigh;

unsigned long acquisitionAttemptStarted = 0;
unsigned long secondsOfSyncAttempt = 400;

unsigned long numberOfLoopsSinceLastSecond = 0;
double fakeBrightness = 0.9;
int PWMPeriod = 18;
unsigned long millisOfLastPWMStart = 0;

// the setup routine runs once when you press reset:
void setup() {                
  pinMode(led, OUTPUT);     
  pinMode(input, INPUT);  
  pinMode(standbySwitch, INPUT);

  pinMode(standbyLight, OUTPUT);     
  pinMode(acquiringLight, OUTPUT);     
  pinMode(failedLight, OUTPUT);     
  pinMode(bulbsOnSwitch, OUTPUT);     
  pinMode(dataOutput, OUTPUT);     
  pinMode(minutesSerial, OUTPUT);
  pinMode(secondsSerial, OUTPUT);
  pinMode(latchOutput, OUTPUT);     
  pinMode(clockOutput, OUTPUT);     
  pinMode(brightnessPin, INPUT);

  digitalWrite(standbyLight, LOW);
  digitalWrite(acquiringLight, HIGH);
  digitalWrite(failedLight, LOW);
  digitalWrite(bulbsOnSwitch, HIGH);

  //Serial.println("in setup");
  acquisitionAttemptStarted = millis();
  checkStandby();
  ensurePoweredProperly();

  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {

  //Serial.println(millis());
  checkStandby();

  unsigned long currentTime = millis();
  //if (currentTime > millisOfLastPWMStart + PWMPeriod) {
  //millisOfLastPWMStart = currentTime;
  //PWMHigh = true;
  //ensurePoweredProperly();
  //    digitalWrite(bulbsOnSwitch, LOW + currentlyAcquiringTime);
  //}
  //else if (currentTime > millisOfLastPWMStart + brightness()*PWMPeriod) {
  //PWMHigh = false;
  //ensurePoweredProperly();
  //    digitalWrite(bulbsOnSwitch, HIGH);
  //}

//analogWrite(bulbsOnSwitch, fakeBrightness*255);
  ensurePoweredProperly();

  numberOfLoopsSinceLastSecond++;
  //Serial.println(acquisitionAttemptStarted);
  //Serial.println(millis());
  // Acquiring New Time
  if (currentlyAcquiringTime) {
    delay(25);

    if (millis() < acquisitionAttemptStarted + 1000*secondsOfSyncAttempt) {
      //Serial.println("millis < acquisition + time");

      boolean readValue = digitalRead(input);
      

      if (readValue) {
        if (noiseCount == 3) {
          currentlyHigh = true;
          noiseCount++;
        }
        if (noiseCount < 3) {
          noiseCount++;
        }

      }
      else {
        if (noiseCount == 1) {
          noiseCount--;
          currentlyHigh = false;
        }
        if (noiseCount > 1) {
          noiseCount--;
        }
      }

      // boolean currentlyHigh = digitalRead(input);
      
      //currentlyHigh = !currentlyHigh;
      
      if (!currentlyHigh && wasJustHigh) {
        lastSwitch = millis();
        wasJustHigh = false;
      }
      if (currentlyHigh && !wasJustHigh) {
        //Serial.println(" millis and low-high transition ");
        unsigned long thisSwitch = millis();
        unsigned long duration = thisSwitch - lastSwitch;
        //Serial.println(duration);
        
        
        int thisBit = -1;
        if (duration > 10/speed && duration < 374/speed) { 
          //Serial.println("     Marker"); 
          thisBit = 2; 
        }
        if (duration > 375/speed && duration < 625/speed) { 
          //Serial.println("     One"); 
          thisBit = 1; 
        }
        if (duration > 624/speed && duration < 990/speed) { 
          //Serial.println("     Zero"); 
          thisBit = 0; 
        }

        if (thisBit == 2 && lastBit == 2 && !isCurrentlyReading) {
          //Serial.println("started...");
          isCurrentlyReading = true;
          isStillValid = true;
          readPosition = 0;
          minutes = 0;
          hours = 0;
          days = 0;
          DUTSign = 1;
          DUT = 0.0;
          thisYear = 0;
          leapYear = 0;
          leapSecond = 0;
          DST = 0;
          Serial.print("Reading; ");
          Serial.print(hour());
          Serial.print(":");
          Serial.print(minute());
          Serial.print(":");
          Serial.print(second());
          Serial.print(" ; ");
        }
        if (isCurrentlyReading) {
          if (thisBit == 2) { 
            Serial.print("|"); 
          } 
          else if (thisBit == 1) { 
            Serial.print("+"); 
          }
          else {
            Serial.print("-");
          }
        }

        if (isCurrentlyReading && isStillValid) {


          switch (readPosition) {
          case 1:
            if (thisBit == 1) { 
              minutes += 40; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 2:
            if (thisBit == 1) { 
              minutes += 20; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 3:
            if (thisBit == 1) { 
              minutes += 10; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 4:
            if (thisBit != 0) { 
              invalidate(); 
            }
            break;
          case 5:
            if (thisBit == 1) { 
              minutes += 8; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 6:
            if (thisBit == 1) { 
              minutes += 4; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 7:
            if (thisBit == 1) { 
              minutes += 2; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 8:
            if (thisBit == 1) { 
              minutes += 1; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            if (minutes > 59 || minutes < 0) { 
              invalidate(); 
            }
            break;
          case 9:
            if (thisBit != 2) { 
              invalidate(); 
            }
            break;
          case 10:
            if (thisBit != 0) { 
              invalidate(); 
            }
            break;
          case 11:
            if (thisBit != 0) { 
              invalidate(); 
            }
            break;
          case 12:
            if (thisBit == 1) { 
              hours += 20; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 13:
            if (thisBit == 1) { 
              hours += 10; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 14:
            if (thisBit != 0) { 
              invalidate(); 
            }
            break;
          case 15:
            if (thisBit == 1) { 
              hours += 8; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 16:
            if (thisBit == 1) { 
              hours += 4; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 17:
            if (thisBit == 1) { 
              hours += 2; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 18:
            if (thisBit == 1) { 
              hours += 1; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            if (hours > 23 || hours < 0) { 
              invalidate(); 
            }
            break;
          case 19:
            if (thisBit != 2) { 
              invalidate(); 
            }
            break;
          case 20:
            if (thisBit != 0) { 
              invalidate(); 
            }
            break;
          case 21:
            if (thisBit != 0) { 
              invalidate(); 
            }
            break;
          case 22:
            if (thisBit == 1) { 
              days += 200; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 23:
            if (thisBit == 1) { 
              days += 100; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 24:
            if (thisBit != 0) { 
              invalidate(); 
            }
            break;
          case 25:
            if (thisBit == 1) { 
              days += 80; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 26:
            if (thisBit == 1) { 
              days += 40; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 27:
            if (thisBit == 1) { 
              days += 20; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 28:
            if (thisBit == 1) { 
              days += 10; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 29:
            if (thisBit != 2) { 
              invalidate(); 
            }
            break;
          case 30:
            if (thisBit == 1) { 
              days += 8; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 31:
            if (thisBit == 1) { 
              days += 4; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 32:
            if (thisBit == 1) { 
              days += 2; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 33:
            if (thisBit == 1) { 
              days += 1; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            if (days > 366 || days < 0) { 
              invalidate(); 
            }
            break;
          case 34:
            if (thisBit != 0) { 
              invalidate(); 
            }
            break;
          case 35:
            if (thisBit != 0) { 
              invalidate(); 
            }
            break;
          case 36:
            if (thisBit == 1) { 
              DUTSign = 1; 
              DUTSignValid = true; 
            } 
            else { 
              DUTSign = -1; 
              DUTSignValid = true; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 37:
            if (thisBit == 1) { 
              if (DUTSign == 1) {
                invalidate();
              } 
            } 
            else { 
              if (DUTSign == -1) {
                invalidate();
              } 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 38:
            if (thisBit == 1) { 
              if (DUTSign == -1) {
                invalidate();
              } 
            } 
            else { 
              if (DUTSign == 1) {
                invalidate();
              } 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 39:
            if (thisBit != 2) { 
              invalidate(); 
            }
            break;
          case 40:
            if (thisBit == 1) { 
              DUT += 0.8; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 41:
            if (thisBit == 1) { 
              DUT += 0.4; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 42:
            if (thisBit == 1) { 
              DUT += 0.2; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 43:
            if (thisBit == 1) { 
              DUT += 0.1; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            if (DUT > 0.9 || DUT < 0) { 
              invalidate(); 
            }
            break;
          case 44:
            if (thisBit != 0) { 
              invalidate(); 
            }
            break;
          case 45:
            if (thisBit == 1) { 
              thisYear += 80; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 46:
            if (thisBit == 1) { 
              thisYear += 40; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 47:
            if (thisBit == 1) { 
              thisYear += 20; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 48:
            if (thisBit == 1) { 
              thisYear += 10; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 49:
            if (thisBit != 2) { 
              invalidate(); 
            }
            break;
          case 50:
            if (thisBit == 1) { 
              thisYear += 8; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 51:
            if (thisBit == 1) { 
              thisYear += 4; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 52:
            if (thisBit == 1) { 
              thisYear += 2; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 53:
            if (thisBit == 1) { 
              thisYear += 1; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            if (thisYear > 99 || thisYear < 0) { 
              invalidate(); 
            }
            break;
          case 54:
            if (thisBit != 0) { 
              invalidate(); 
            }
            break;
          case 55:
            if (thisBit == 1) { 
              leapYear = true; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 56:
            if (thisBit == 1) { 
              leapSecond = true; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 57:
            if (thisBit == 1) { 
              DST += 2; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            break;
          case 58:
            if (thisBit == 1) { 
              DST += 1; 
            }
            if (thisBit == 2) { 
              invalidate(); 
            }
            if (DST > 3 || DST < 0) { 
              invalidate(); 
            }
            break;
          case 59:
            if (thisBit != 2) { 
              invalidate(); 
            }
            if (isStillValid) {
              
              didJustHitMarkerOn59 = true;

              int adjustedSecond = 1;
              int adjustedMinute = minutes + 1;
              int DSTAdjustment = 0;
              if (DST) {
                DSTAdjustment = 1;
              }
              int adjustedHour = hours - 8 + DSTAdjustment;
              if (adjustedMinute > 59) {
                adjustedHour++;
                adjustedMinute = 0;
              }
              int adjustedDay = days;
              if (adjustedHour < 0) {
                adjustedHour = adjustedHour + 24;
                adjustedDay--;
              }
              int adjustedYear = thisYear+30;
              if (adjustedDay < 0) {
                int leapYearAdder = 0;
                if (leapYear) {
                  leapYearAdder = 1;
                }
                adjustedDay = 365 + leapYearAdder;
                adjustedYear--;
              }

              Serial.print(" ; Success! It's ");
              Serial.print(adjustedHour);
              Serial.print(":");
              Serial.print(adjustedMinute);
              Serial.print(":00 on day ");
              Serial.print(adjustedDay);
              Serial.print(" of year ");
              Serial.print(thisYear);

              Serial.print(", DUT is ");
              Serial.print(DUT*DUTSign);
              Serial.print(", leap year: ");
              Serial.print(leapYear);
              Serial.print(", leap second: ");
              Serial.print(leapSecond);
              Serial.print(", DST: ");
              Serial.println(DST);


              tm.Second = adjustedSecond;
              tm.Minute = adjustedMinute;
              tm.Hour = adjustedHour;
              tm.Day = adjustedDay;
              tm.Month = 1;
              tm.Year = adjustedYear;

              delay(930);
              setTime(makeTime(tm));
              syncSucceeded();
              

              isStillValid = false;
              isCurrentlyReading = false;
            }
            break;
          default:
            break;
          }
        }
        lastBit = thisBit;
        readPosition++;
      }
      if (currentlyHigh) { 
        wasJustHigh = true; 
      }
    } 
    else {
      //Serial.println("failed here");
      //attemptFailed();
    }

  }
  // Displaying Time
  else {

    // not in standbyMode
    unsigned long currentTime = millis();
    if (currentTime > millisLastSecond + 1000) {

      standbyMode = digitalRead(standbySwitch);
      if (standbyMode) { 
        digitalWrite(standbyLight, HIGH); 
      }
      else { 
        digitalWrite(standbyLight, LOW); 
      }

      millisLastSecond = millisLastSecond + 1000;
      secondsSinceLastSyncAttempt++;

//Serial.print("It's ");
//Serial.print(hour());
//Serial.print(":");
//Serial.print(minute());
//Serial.print(":");
//Serial.print(second());
//Serial.print(" on ");
//Serial.print(month());
//Serial.print("/");
//Serial.print(day());
//Serial.print("/");
//Serial.println(year());

      //Serial.print("number of loops: ");
      //Serial.println(numberOfLoopsSinceLastSecond);
      numberOfLoopsSinceLastSecond = 0;

      writeToTubes(hour(), minute(), second());

      if (secondsSinceLastSyncAttempt > syncInterval-1) {
        reacquireTime();
      }

    }
    // standby mode?
    else {

    }
  }
}

void turnTubesOn() {
  tubesOn = true;
  writeToTubes(hour(), minute(), second());
  //  digitalWrite(bulbsOnSwitch, LOW*!PWMHigh*currentlyAcquiringTime);
}
void turnTubesOff() {
  tubesOn = false;
  //  digitalWrite(bulbsOnSwitch, HIGH);
}

void invalidate() {
  isCurrentlyReading = false;
  isStillValid = false;
  Serial.println(" ; invalid ;");
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
  // Serial.println("failed...");
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
  digitalWrite(dataOutput, minutes);
  digitalWrite(minutesSerial, hours);
  digitalWrite(secondsSerial, seconds);
  digitalWrite(clockOutput, HIGH);
}







