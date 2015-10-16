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

int STATE = 0;
// 0 waiting for request
// 1 request received, preparing dispatch
// 2 waiting for return message

void setup()
{
  Serial.begin(9600);	  // Debugging only
  Serial.println("setup");

  // Initialise the IO and ISR
  vw_setup(2000);	 // Bits per sec
  vw_rx_start();       // Start the receiver PLL running

}

void loop()
{
  float temperature = getTemperature(0);
  Serial.println(temperature);
  delay(3000);
}

float getTemperature(int whichSensor) {
    
  float analogIn = analogRead(0);
  float voltage = analogIn * 0.004882814;
  float tempC = (voltage-0.5)/0.01;
  float tempF = (tempC*9/5)+32;
  int tenXTemp = (int)(tempF*10);



  String outgoingString = String(tenXTemp);
  char msg[10];
  outgoingString.toCharArray(msg, 10);
  //Serial.print("sent: ");
  //Serial.println(String(tenXTemp));
  digitalWrite(13, true); // Flash a light to show transmitting
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(13, false);
  //Serial.println("ran");



  byte buf[VW_MAX_MESSAGE_LEN];
  byte buflen = VW_MAX_MESSAGE_LEN;
  int answerInt = -1001;
  for (int listenAttempt = 0 ; listenAttempt < 30 ; listenAttempt ++){
    //Serial.println("listening...");
    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      //Serial.print("Got: ");
      String incoming = "";
      for (int i = 0; i < buflen; i++)
      {
        incoming += buf[i]-'0';
        //Serial.print(buf[i]-'0');
      }
      String answerStr = incoming;
      answerInt = answerStr.toInt();
      //Serial.println(incoming);
      break;
    }
    delay(10);
  }

  float answer = 0.1 * answerInt;
  return answer;
  
}




