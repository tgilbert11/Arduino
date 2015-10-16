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

void setup()
{
  Serial.begin(9600);	// Debugging only
  Serial.println("setup");

  pinMode(CLK, OUTPUT);
  pinMode(dig100, OUTPUT);
  pinMode(dig10, OUTPUT);
  pinMode(dig1, OUTPUT);


  // Initialise the IO and ISR
  vw_setup(2000);	 // Bits per sec

  vw_rx_start();       // Start the receiver PLL running
}

void loop()
{
  byte buf[VW_MAX_MESSAGE_LEN];
  byte buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) // Non-blocking
  {

    digitalWrite(13, true); // Flash a light to show received good message
    // Message with a good checksum received, dump it.
    Serial.print("Got: ");

    String incoming = "";

    for (int i = 0; i < buflen; i++)
    {
      incoming += buf[i]-'0';
      Serial.print(buf[i]-'0');
    }
    Serial.println(incoming);

    int incomingInt = incoming.toInt();


    int numbers [] = {
      252, 96, 218, 242, 102, 182, 190, 224, 254, 246                            };

    for (int i=0 ; i<8 ; i++) {
      digitalWrite(CLK, LOW);
      digitalWrite(dig100, bitRead(numbers[incomingInt/100], i));
      digitalWrite(dig10, bitRead(numbers[(incomingInt/10)%10]+1, i));
      digitalWrite(dig1, bitRead(numbers[incomingInt%10], i));004882814
      digitalWrite(CLK, HIGH);
    }    



    digitalWrite(13, false);
  }
}


