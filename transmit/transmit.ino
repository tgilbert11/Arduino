#include <VirtualWire.h>

int counter = 0;

void setup(){
  //2400 baud for the 434 model
  pinMode(13, OUTPUT);
  pinMode(02, INPUT);
  pinMode(03, OUTPUT);
  Serial.begin(2400);
  counter = 0;

  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);      // Bits per sec

}
void loop(){
  //send out to transmitter
  rfSerial.write(counter);
  Serial.println(counter);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  if (counter > 300) {
    counter = 0;
  }
  counter = counter + 1;
  delay(1000);
}



