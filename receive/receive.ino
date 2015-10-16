#include <SoftwareSerial.h>

int CLK = 8;
int dig100 = 4;
int dig10 = 5;
int dig1 = 6;
SoftwareSerial rfSerial = SoftwareSerial(02, 03);
int prevChar = 0;

void setup(){
  //2400 baud for the 434 model
  rfSerial.begin(2400);
  Serial.begin(2400);
  pinMode(CLK, OUTPUT);
  pinMode(dig100, OUTPUT);
  pinMode(dig10, OUTPUT);
  pinMode(dig1, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(2, INPUT);
  pinMode(13, OUTPUT);

  digitalWrite(9, HIGH);

}
void loop() {
  // read in values, debug to computer
  int someChar = 0;
  if (rfSerial.available()){
    someChar = rfSerial.read();
    if (someChar != prevChar) {
      Serial.print(someChar);
      prevChar = someChar;

      int numbers [] = {
        252, 96, 218, 242, 102, 182, 190, 224, 254, 246                    };

      for (int i=0 ; i<8 ; i++) {
        digitalWrite(CLK, LOW);
        digitalWrite(dig100, bitRead(numbers[someChar/100], i));
        digitalWrite(dig10, bitRead(numbers[(someChar/10)%10], i));
        digitalWrite(dig1, bitRead(numbers[someChar%10], i));
        digitalWrite(CLK, HIGH);
      }    

    }
  }

  digitalWrite(13, HIGH);




  //Serial.println("got one");
  digitalWrite(13, LOW);

  //}
}









