unsigned long lastSweepStarted = 0;
int frequency = 41m `;


int output = 12;

void setup() {
    pinMode(output, OUTPUT);
    //Serial.begin(9600);
}

void loop() {
    //delay(1);
    unsigned long currentTime = millis();
    //Serial.println(500/frequency);
    
    if (currentTime > lastSweepStarted + 1000/frequency) {
        digitalWrite(output, HIGH);
        lastSweepStarted = currentTime;
    }
    
    if (currentTime > lastSweepStarted + 500/frequency) {
        digitalWrite(output, LOW);
    }
    
}
