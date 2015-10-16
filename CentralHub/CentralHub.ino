#include <SPI.h>
#include <WiFi.h>

char ssid[] = "TermsOfEnrampagement";      //  your network SSID (name) 
char pass[] = "workingtitle";   // your network password

int status = WL_IDLE_STATUS;
WiFiServer server(50000);

int numberOfRequests = 0;

int RENT = 3;
int AMEX = 5;
int EVENT = 6;
int STATUS = 9;

void setup() {
  Serial.begin(9600);
  pinMode(RENT, OUTPUT);
  pinMode(AMEX, OUTPUT);
  pinMode(EVENT, OUTPUT);
  pinMode(STATUS, OUTPUT);
  
  digitalWrite(RENT, LOW);
digitalWrite(AMEX, LOW);
digitalWrite(EVENT, LOW);
digitalWrite(STATUS, LOW);

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
  digitalWrite(STATUS, HIGH);
}


void loop() {
  
  WiFiClient client = server.available();   // listen for incoming clients

String clStr = "Client";
Serial.print(clStr + numberOfRequests + "\n");

if (client) {                             // if you get a client,
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        if (c == '\n'){
          if (currentLine.length() == 0){
            Serial.println("length is 0");
            break;
          }
          else {
            Serial.println("that other thing");
            currentLine = "";
          }
        }
        else if (c != '\r') {
          currentLine += c;      // add it to the end of the currentLine
        }

        Serial.print("data");

        // Single LED hooked to pin 6
        if (currentLine.endsWith("REN")) {
          Serial.print("woohoo");
          digitalWrite(RENT, HIGH);
          client.flush();
          client.print("<!DOCTYPE html PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n");
          String nextText = "<html>\n<body>\nhello world. <br />";
          client.print(nextText + numberOfRequests + "\n</body>\n</html>\n");
          numberOfRequests = numberOfRequests + 1;
          //client.flush();
          break;
        }
        
        if (currentLine.endsWith("REN0")) {
          digitalWrite(RENT, LOW);
          client.flush();
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
          
          client.print("<!DOCTYPE html PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n");
          String nextText = "<html>\n<body>\nhello world. <br />";
          client.print(nextText + numberOfRequests + "\n</body>\n</html>\n");
         numberOfRequests = numberOfRequests + 1;
          
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
        
        Serial.println("down here");
      }
      
      Serial.println("client not available");
    }
    delay(1);
    client.stop();
  }
  
  Serial.println("client = nil");
}




