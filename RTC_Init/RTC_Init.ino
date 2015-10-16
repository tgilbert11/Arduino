#include <Wire.h>
#include <Time.h>

int LEDPin = 13;

void setup() {
    pinMode(LEDPin, OUTPUT);
    
    Serial.begin(9600);
    
    Wire.begin();
    
    Wire.beginTransmission(0x6f);
    Wire.write(0x08);
    Wire.write(B01001010);
    Wire.endTransmission();
    
    time_t tm = now();
    
}

void RTCSetTime(time_t tm) {
    Wire.beginTransmission(0x6f);
    Wire.write(0x00);
    Wire.write();
    Wire.write(0x06);
    Wire.write(0x21);
    Wire.write(0x27);
    Wire.write(0x06);
    Wire.write(0x15);
    Wire.write(0x06);
    Wire.endTransmission();

}

void loop() {
    digitalWrite(LEDPin, HIGH);
    delay(500);
    digitalWrite(LEDPin, LOW);
    delay(500);
    
    Wire.beginTransmission(0x6f);
    Wire.write(0x00);
    Wire.endTransmission();
    
    Wire.requestFrom(0x6f,7);
    int num = Wire.read();
    Serial.print(num/16*10+num%16);
    num = Wire.read();
    Serial.print(num/16*10+num%16);
    num = Wire.read();
    Serial.print(num/16*10+num%16);
    num = Wire.read();
    Serial.print(num/16*10+num%16);
    num = Wire.read();
    Serial.print(num/16*10+num%16);
    num = Wire.read();
    Serial.print(num/16*10+num%16);
    num = Wire.read();
    Serial.println(num/16*10+num%16);
    
}

