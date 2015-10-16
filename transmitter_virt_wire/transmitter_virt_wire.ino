// transmitter.pde
//
// Simple example of how to use VirtualWire to transmit messages
// Implements a simplex (one-way) transmitter with an TX-C1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: transmitter.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $

#include <VirtualWire.h>

void setup()
{
  Serial.begin(9600);	  // Debugging only
  Serial.println("setup");

  // Initialise the IO and ISR
  vw_setup(2000);	 // Bits per sec
}

void loop()
{
  float analogIn = analogRead(0);
  float voltage = analogIn * 0.004882814;
  float tempC = (voltage-0.5)/0.01;
  float tempF = (tempC*9/5)+32;
  int tenXTemp = (int)(tempF*10);
  String outgoingString = String(tenXTemp);
  char msg[10];
  outgoingString.toCharArray(msg, 10);

  digitalWrite(13, true); // Flash a light to show transmitting
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(13, false);
  Serial.println("ran");
  delay(1000);
}

