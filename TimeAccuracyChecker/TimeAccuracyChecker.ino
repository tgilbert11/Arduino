int clockPin = 10;

int masterClock = 0;
int seconds = 0;
int minutes = 0;
int hours = 0;
int days = 0;

int timeSeconds = 10;
int timeMinutes = 5;
int timeHours = 23;

void setup() {
  attachInterrupt(0, incrementClock, RISING);
  analogReference(DEFAULT);
  pinMode(13, OUTPUT);
  pinMode(clockPin, OUTPUT);
  //pinMode(0, INPUT);
  Serial.begin(9600);
  analogWrite(clockPin, 127);
}

void incrementClock() {
  if (masterClock  == 490) {
    addSecond();
    masterClock = 0;
  }
  masterClock++;
  //Serial.println(masterClock);
  return;
}

void loop() {
  
}

void addSecond() {
  seconds++;
  timeSeconds++;
  if (seconds == 60) { seconds = 0; minutes++; }
  if (timeSeconds == 60) { timeSeconds = 0; timeMinutes++; }
  if (minutes == 60) { minutes = 0; hours++; }
  if (timeMinutes == 60) { timeMinutes = 0; timeHours++; }
  if (hours == 24) { hours = 0; days++; }
  if (timeHours == 24) { timeHours = 0; }
  //tone(13, 100, 500);
  
  Serial.print("After ");
   Serial.print(days);
 Serial.print(" days, ");
 Serial.print(hours);
 Serial.print(" hrs, ");
 Serial.print(minutes);
 Serial.print(" mins, ");
 Serial.print(seconds);
 Serial.print(" secs, the time is ");
 Serial.print(timeHours);
 Serial.print(":");
 Serial.print(timeMinutes);
 Serial.print(":");
 Serial.println(timeSeconds);

}












