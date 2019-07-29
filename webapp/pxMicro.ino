#include <WiFi.h>
#include <ESPmDNS.h>
#include <WebServer.h>

/********************************************************/
// Wifi Setup, Change between "Brackets"
/********************************************************/
//char* WifiMode = "CL"; //AP or CL //Run as Axcesspoint or Client
/********************************************************/
const char* ssid      = "xxx"; //Insert SSID
const char* password  = "xxx"; // Insert PW

// Setup your Static IP address
IPAddress local_IP(192, 168, 2, 250); // Remember to uppdate Last line of code
// Setup Gateway IP address
IPAddress gateway(192, 168, 2, 254);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional
/********************************************************/
/*char* ApSSID      = "TargetCtr";   // Broadcast this SSID Between "", 1-32 Charecters
  char* ApPassword  = "Timer1234";   // Cange Between "", 8-64 Charecters*/
WebServer server(80);
/********************************************************/
// Config of Pins, Board: NodE MCU ESP-32S v1.1 101010
/********************************************************/
#define BTN_Face     25  //Input pinnumber for triggering Face
#define BTN_Edge     26  //Input pinnumber for triggering Edge
#define BTN_Start    27  //Input pinnumber for triggering Start         
#define BTN_Stop     14  //Input pinnumber for triggering Stop
#define stat_wifi_led     13  //Wifi Led Pin number
#define stat_mdns_led     12  //MDNS Led Pin number
#define Pressed_delay 200  // Button press delay
/********************************************************/

void setup() {

  Serial.begin(115200);
  Serial.println ("Starting");

  pinMode(BTN_Face,   OUTPUT);
  pinMode(BTN_Edge,   OUTPUT);
  pinMode(BTN_Start,  OUTPUT);
  pinMode(BTN_Stop,   OUTPUT);
  digitalWrite(BTN_Stop, LOW);
  digitalWrite(BTN_Edge, LOW);
  digitalWrite(BTN_Start, LOW);
  digitalWrite(BTN_Stop, LOW);
  pinMode(stat_wifi_led,  OUTPUT);
  pinMode(stat_mdns_led,   OUTPUT);
  digitalWrite(stat_wifi_led, LOW);
  digitalWrite(stat_mdns_led, LOW);

  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  digitalWrite(stat_wifi_led, HIGH);



  if (!MDNS.begin("pxMicro")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  digitalWrite(stat_mdns_led, HIGH);

  server.on("/", handle_OnConnect);
  server.on("/Edge",  Edge);
  server.on("/Face",  Face);
  server.on("/Start", Start);
  server.on("/Stop",  Stop);

  server.begin();
  Serial.println("HTTP server started");
  MDNS.addService("http", "tcp", 80);

  // Print local IP address and start web server
  //IP_adress = WiFi.localIP();
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(local_IP);

}

void loop() {
  // Serial.println ("Loop");

  server.handleClient();
}

/*
 * * * * * * * * * * * * *
   - - Setup Of functions
 * * * * * * * * * * * * *
*/


void sendhtml() {
  server.send(200, "text/html", SendHTML());
}

void Face() {
  digitalWrite(BTN_Face, HIGH);
  Serial.println ("Face Pressed");
  delay(Pressed_delay);
  digitalWrite(BTN_Face, LOW);
  Serial.println ("Face Reset");
  sendhtml();
}
void  Edge() {
  digitalWrite(BTN_Edge, HIGH);
  Serial.println ("Edge Pressed");
  delay(Pressed_delay);
  digitalWrite(BTN_Edge, LOW);
  Serial.println ("Edge Reset");
  sendhtml();
}

void  Start() {
  digitalWrite(BTN_Start, HIGH);
  Serial.println ("Start Pressed");
  delay(Pressed_delay);
  digitalWrite(BTN_Start, LOW);
  Serial.println ("Start Reset");
  sendhtml();
}
void  Stop() {
  digitalWrite(BTN_Stop, HIGH);
  Serial.println ("Stop Pressed");
  delay(Pressed_delay);
  digitalWrite(BTN_Stop, LOW);
  Serial.println ("Stop Reset");
  sendhtml();
}

void handle_OnConnect() {
  ;
  sendhtml();
}

String SendHTML() {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Target Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #00ee00;}\n";
  ptr += ".button-off {background-color:  #8b8b7a;}\n";
  ptr += ".button-stop {background-color: #ee0000;}\n";
  ptr += ".button-stop:active {background-color: #ee0000;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Target Control</h1>";
  ptr += "<h3>For PX-MICRO</h3>";
  ptr += "<center><table><tr>";
  ptr += "<th><a class=\"button button-on\" href=\"/Start\">Start</a>\n</th>";
  ptr += "<th>&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp</th>";
  ptr += "<th><a class=\"button button-off\" href=\"/Face\">Face</a>\n</th>";
  ptr += "</tr>&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp<tr>";
  ptr += "<th><a class=\"button button-stop\" href=\"/Stop\">Stop</a>\n</th>";
  ptr += "<th></th>";
  ptr += "<th><a class=\"button button-off\" href=\"/Edge\">Edge</a>\n</th>";
  ptr += "</center></tr></table>";
  ptr += "<p>192.168.2.250 / pxMicro.local</p>"; // Update IP adress
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
