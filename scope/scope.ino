 #include <Process.h>

int clock = 2;
boolean clockWasJustLow = false;

unsigned long lastSweepBeginTime = 0;
int frameNumber = 0;

int frequency = 40;
boolean lastReadings[40];

String upperRow;
String lowerRow;

boolean printTwo = false;

void setup() {
    pinMode(12,INPUT);
    pinMode(clock,INPUT);
    Serial.begin(9600);
}

void loop() {
    delay(1);
    
    if (digitalRead(clock) == HIGH && clockWasJustLow) {
        readBitFromAntenna();
        redrawView();

        clockWasJustLow = false;
    }
    else if (digitalRead(clock) == HIGH && !clockWasJustLow) {
        
    }
    else {
        clockWasJustLow = true;
    }
    //unsigned long currentTime = millis();
    //if (currentTime > lastSweepBeginTime + 1000/frequency) {

     //   readBitFromAntenna();
     //   redrawView();

     //   lastSweepBeginTime = currentTime;

    //}

}

void readBitFromAntenna() {
    frameNumber++;  
    if (frameNumber >= frequency) { 
        frameNumber = 0; 
    }
    lastReadings[frameNumber] = digitalRead(12);
}

void redrawView() {

    //String test = "-----";
    //test[2] = '+';
    //Serial.println(test);

    //Serial.println(frameNumber);
    
    String command = "updateFiles";
    //for (int i=0 ; i<frequency ; i++) {
        
        //upperLine += ( lastReadings[i] == true  ? "+" : " " );
        
    //}
    
    Serial.print(command + " " + frequency + " " + frameNumber + " " + lastReadings[frameNumber] + "\n");
    //Serial.print("clear\n");
    //Serial.print(command + "\n");
    

    //Serial.print(thisBit == 1 ? "+" : " ");
    //Serial.print(thisBit == 0 ? " " : "+aoehunsatoheunstahoensutahoesntuahoenstuahoetnuhaoenstuahoeuhosnetuhaonsetuhanotehuansoehunsotehuntashoeutnsahoensuhaontseuhanostehuatnsoehuansoeuh");
    //Serial.println();
    //Serial.println();

}



