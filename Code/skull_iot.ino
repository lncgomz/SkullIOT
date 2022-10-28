/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com
*********/

// Load Wi-Fi library
#include <Arduino.h>
#include <analogWrite.h>
#include <WiFi.h>

// Red, green, and blue pins for PWM control
#define PIN_RED    23 // GIOP23
#define PIN_GREEN  22 // GIOP22
#define PIN_BLUE   21 // GIOP21

// Replace with your network credentials
const char* ssid     = "RAVENCLAW";
const char* password = "0251#2660253";

// Set web server port number to 80
WiFiServer server(80);

// Decode HTTP GET value
String redString = "0";
String greenString = "0";
String blueString = "0";
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;

int gR = 0;
int gG = 0;
int gB = 0;

bool isFixed = true;

// Variable to store the HTTP req  uest
String header;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {            // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\">");
            client.println("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js\"></script>");
            
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".red { background-color: #FF0000; border: solid; color: white; padding: 5px 5px;}");
            client.println(".yellow { background-color: #FFFF00; border: solid; color: white; padding: 5px 5px;}");
            client.println(".green { background-color: #00FF00; border: solid; color: white; padding: 5px 5px;}");
            client.println(".blue { background-color: #0000FF; border: solid; color: white; padding: 5px 5px;}");
            client.println(".purple { background-color: #FF00FF; border: solid; color: white; padding: 5px 5px;}");
            client.println(".off { background-color: #000000; border: solid; color: white; padding: 5px 5px;}");
            client.println(".square { width: 100px; height: 100px;}");
            client.println(".image { width: 350px; height: 350px; margin-top:50px;}");
            client.println(".footer { display: flex; position: fixed; bottom: 0; width: 100%;}");
            
            
            client.println(".button2 {background-color: #555555;}</style>");

            client.println("</head><body><div class=\"container\"><div class=\"row justify-content-center\"><h1>CALAVERA IOT</h1></div>");
            
            client.println("<div class=\"row justify-content-center\"><img class=\"image\" referrerpolicy=\"no-referrer\" src=\"https://vectorportal.com/storage/punk-bonehead-vector_8979.jpg\"></div>");
            client.println("<div class=\"row justify-content-center\"><div class=\"col-12 footer\">");
            client.println("<a class=\"btn col-2 square red\" href=\"/r255g0b0\"></a>");
            client.println("<a class=\"btn col-2 square yellow\" href=\"/r255g255b0\"></a>");
            client.println("<a class=\"btn col-2 square green\" href=\"/r0g255b0\"></<a>");
            client.println("<a class=\"btn col-2 square blue\" href=\"/r0g0b255\"></a>");
            client.println("<a class=\"btn col-2 square purple\" href=\"/r255g0b255\"></a>");
            client.println("<a class=\"btn col-2 square off\" href=\"/r0g0b0\"></a>");
            client.println("</div></div>");
            client.println("</body></html>");
            // The HTTP response ends with another blank line
            client.println();

            // Request sample: /?r201g32b255&
            // Red = 201 | Green = 32 | Blue = 255
            if (header.indexOf("GET /r") >= 0) {
              pos1 = header.indexOf('r');
              pos2 = header.indexOf('g');
              pos3 = header.indexOf('b');
              pos4 = header.indexOf('&');
              redString = header.substring(pos1 + 1, pos2);
              greenString = header.substring(pos2 + 1, pos3);
              blueString = header.substring(pos3 + 1, pos4);
              Serial.println("R" + redString);
              Serial.println("G" + greenString);
              Serial.println("B" + blueString);
              setColor(redString.toInt(), greenString.toInt(), blueString.toInt());
            }
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void setColor(int R, int G, int B) {
  analogWrite(PIN_RED, R - 255);
  analogWrite(PIN_GREEN, G - 255);
  analogWrite(PIN_BLUE, B - 255);
}
