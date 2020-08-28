#include <Arduino.h>
#include <WiFi.h>

//SSID & Password
const char* ssid = "noShar3";
const char* password = "holacomoestas";

//Web server PORT
WiFiServer server(80);

//variable to store the HTTP request
String header;

//Auxiliar variables to store the current output state
String output26State = "off";
String output27State = "off";

//Assign output variables to GPIOs
const int output26 = 26;
const int output27 = 27;

//Current Time
unsigned long currentTime = millis();

//Previous time
unsigned long previousTime = 0;

//Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeOutTime = 2000;



void setup() {

  Serial.begin(115200);

  //initialize the output variables as outputs
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);

  //connect to wi-fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED) {
    
    delay(500);
    Serial.print(".");

  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  
}

void loop() {
  
  WiFiClient client = server.available() ;   //Listen for incoming clients

  if (client) {                             //If a new client connects
    
    currentTime = millis();
    previousTime = currentTime;

    Serial.println("New Client.");          //Print a message out in the serial port
    String currentLine = "";                //make a string to hold incoming data from the client

    while (client.connected() && currentTime - previousTime <= timeOutTime) {

      currentTime = millis();               //loop while the client's connected

      if (client.available()) {             //if there's bytes to read from the client

      char c = client.read();               //read a byte, then
        Serial.write(c);                    //print it out the serial monitor
        header += c;

        if (c == '\n') {
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0)
            {
              Serial.println("GPIO 26 on");
              output26State = "on";
              digitalWrite(output26, HIGH);
            }
            else if (header.indexOf("GET /26/off") >= 0)
            {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output26, LOW);
            }
            else if (header.indexOf("GET /27/on") >= 0)
            {
              Serial.println("GPIO 27 on");
              output27State = "on";
              digitalWrite(output27, HIGH);
            }
            else if (header.indexOf("GET /27/off") >= 0)
            {
              Serial.println("GPIO 27 off");
              output27State = "off";
              digitalWrite(output27, LOW);
            }

            //Display the HTML Web Page
            client.println("<!DOCTYPE html><html lang='en'>");
            client.println("<head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>");
            client.println("<link href='https://fonts.googleapis.com/css2?family=Montserrat:wght@100;200;300&display=swap' rel='stylesheet'>");
            client.println("<style>body{margin: 0; text-align: center; padding: .8em 1.2em; font-family: 'Montserrat', sans-serif;}p{font-family: Montserrat; font-style: normal; font-weight: 300; font-size: 24px; line-height: 29px; color: #000000;}hr{width: 162px; height: 0px; left: 125px; top: 326px; border: 1px solid #000000;}.title{/* ESP32 WEB Server */ font-family: Montserrat; font-style: normal; font-weight: 600; font-size: 35px; line-height: 44px; color: #080E2F;}.container{text-align: center; padding: .8em 1.2em;}.interaction{padding: 0em 1.2em;}.button{/* Button */ background: #111D5E; width: 162px; display: block; color: #fff; border-radius: 10px; padding: .5em; text-decoration: none; font-size: 1.5em; margin: 4% auto 1%; position: relative;}.button-active{background: #268acc; width: 162px; display: block; color: #fff; border-radius: 10px; padding: .5em; text-decoration: none; font-size: 1.5em; margin: 4% auto 1%; position: relative;}.footer{font-family: Montserrat; font-style: normal; font-weight: 100; font-size: 24px; line-height: 29px; position: sticky;}</style>");
            client.println("<title>WEB Server - ESP32</title></head>");

            

            client.println("<div class='container'><header><h1 class='title'>ESP32 WEB Server</h1></header><section class='interaction'>");

            //display the current state, and ON/OFF button for GPIO 26

            if (output26State == "off") {
              client.println("<a href='/26/on' class='button'>Turn ON</a>");
              client.println("<p>GPIO 26</p><hr><p>State " + output26State + "</p>");
            } else {
              client.println("<a href='/26/off' class='button-active'>Turn OFF</a>");
              client.println("<p>GPIO 26</p><hr><p>State " + output26State + "</p>");
            }

            //display the current state, and ON/OFF button for GPIO 27

            if (output27State == "off") {
              client.println("<a href='/27/on' class='button'>Turn ON</a>");
              client.println("<p>GPIO 27</p><hr><p>State " + output27State + "</p>");
            } else {
              client.println("<a href='/27/off' class='button-active'>Turn OFF</a>");
              client.println("<p>GPIO 27</p><hr><p>State " + output27State + "</p>");
            }
            
            client.println("</section></div>");
            client.println("<footer><p class='footer'>Made By Simon Clemente</p></footer>");
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          }
          else {
            currentLine = "";               // if you got a newline, then clear currentLine
          }
        }
        else if (c != '\r') {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
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