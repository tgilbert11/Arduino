#include <SPI.h>
#include <WiFi.h>

char ssid[] = "TermsOfEnrampagement";      //  your network SSID (name) 
char pass[] = "workingtitle";   // your network password

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  //Serial.begin(9600);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(9, OUTPUT);

  digitalWrite(9, LOW);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    while(true);        // don't continue
  } 

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);
  } 
  server.begin();                           // start the web server on port 80
  digitalWrite(9, HIGH);
}


void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        if (c == '\n'){
          if (currentLine.length() == 0){
            break;
          }
          else {
            currentLine = "";
          }
        }
        else if (c != '\r') {
          currentLine += c;      // add it to the end of the currentLine
        }

        // Single LED hooked to pin 6
        if (currentLine.endsWith("EOM")) {
          int value = currentLine.substring(5,8).toInt();
          analogWrite(6, value);               // GET /H turns the LED on
          client.flush();
 
          client.print("<html>");
          String label = "Value: ";
          String footer = "</H1>\n</html>\n";
          client.print(label + value + footer);
          break;

        }
        
        // Tricolor LED hooked to R:6, G:5, B:3
        if (currentLine.endsWith("RGB")) {
          Serial.println(currentLine);
          int red = currentLine.substring(5,8).toInt();
          int green = currentLine.substring(8,11).toInt();
          int blue = currentLine.substring(11,14).toInt();
          
          Serial.print(red);
          Serial.print(", ");
          Serial.print(green);
          Serial.print(", ");
          Serial.println(blue);
          
          analogWrite(6, red);               // GET /H turns the LED on
          analogWrite(5, green);               // GET /H turns the LED on
          analogWrite(3, blue);               // GET /H turns the LED on
          client.flush();
 
          //client.print("HTTP/1.1 200 OK\nContent-Type:text/html\nConnection: close\n");
          //String starter = "\n<body bgcolor=\"rgb(";
          //String comma = ",";
          //client.print("\n\n<!DOCTYPE HTML>\n<html>");
          //client.print(starter + red + comma + green + comma + blue);
          //client.print(")\">\n<H1 align=\"center\">");
          //String label = "Value: ";
          //String footer = "</H1>\n</html>\n";
          //client.print(label + footer);
          break;

        }
      }
    }
    delay(1);
    client.stop();
  }
}




