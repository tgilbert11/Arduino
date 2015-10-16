// receiver.pde
//
// Simple example of how to use VirtualWire to receive messages
// Implements a simplex (one-way) receiver with an Rx-B1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: receiver.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $

#include <VirtualWire.h>

int CLK = 8;
int dig100 = 4;
int dig10 = 5;
int dig1 = 6;
float prevTemp = 72.0;

void setup()
{
  Serial.begin(9600);	// Debugging only
  Serial.println("setup");

  pinMode(CLK, OUTPUT);
  pinMode(dig100, OUTPUT);
  pinMode(dig10, OUTPUT);
  pinMode(dig1, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A1, INPUT);

  analogReference(EXTERNAL);

  // Initialise the IO and ISR
  vw_setup(2000);	 // Bits per sec
  vw_rx_start();       // Start the receiver PLL running
}

void loop()
{
  //Serial.println("loop");
  float analogIn = analogRead(A1);
  delay(10);
  analogIn = analogRead(A1);
  delay(10);
  float voltage = analogIn * 3.21 / 1024;
  //Serial.println(voltage);
  float tempC = (voltage-0.5)/0.01;
  float tempF = (tempC*9/5)+32;
  float tempFactor = 0.99;
  Serial.print("prevTemp: ");
  Serial.print(prevTemp);
  Serial.print(", voltage: ");
  Serial.print(voltage);
  float prevPrevTemp = prevTemp;
  prevTemp = tempFactor*prevTemp + tempF*(1.0-tempFactor);
  Serial.print(", curr temp: ");
  Serial.print(tempF);
  Serial.print(", prev temp: ");
  Serial.print(prevTemp);
  Serial.print(", tempFact: ");
  Serial.println(tempFactor);
  int tenXTemp = (int)(tempF*10);

  int numbers [] = {
    252, 96, 218, 242, 102, 182, 190, 224, 254, 246      };

  int displayFactor = 1;
  int dp = 1;
  if (tenXTemp > 999) {
    displayFactor = 10;
    dp = 0;
  }
  for (int i=0 ; i<8 ; i++) {
    digitalWrite(CLK, LOW);
    digitalWrite(dig100, 0);//bitRead(numbers[tenXTemp/(100*displayFactor)], i));
    digitalWrite(dig10, 0);//bitRead(numbers[(tenXTemp/(10*displayFactor))%10]+dp, i));
    digitalWrite(dig1, 0);//bitRead(numbers[(tenXTemp/(1*displayFactor))%10], i));
    digitalWrite(CLK, HIGH);
  }    


  byte buf[VW_MAX_MESSAGE_LEN];
  byte buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    digitalWrite(13, HIGH);
    //Serial.print("received: ");

    String incoming = "";
    for (int i = 0; i < buflen; i++)
    {
      incoming += buf[i]-'0';
      //eSerial.print(buf[i]-'0');
    }

    //Serial.println(incoming);

    int tenXTemp = (int)(prevTemp*10);
    String outgoingString = String(tenXTemp);
    char msg[10];
    outgoingString.toCharArray(msg, 10);
    //Serial.print("sent:     ");
    Serial.println(outgoingString);
    digitalWrite(13, LOW);
    for (int i=0 ; i<3 ; i++) {
      vw_send((uint8_t *)msg, strlen(msg));
      vw_wait_tx(); // Wait until the whole message is gone
      delay(10);
    }
    
    // Serial.println("done");


  }

  delay(10);
}









