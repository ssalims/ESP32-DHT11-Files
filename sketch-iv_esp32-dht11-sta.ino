/*
  * ESP32 Web Server - STATION Mode
*/
//===============================================================
// Load Libraries
//===============================================================

#include <WiFi.h> 
#include <WebServer.h>
#include <ESPmDNS.h>

#include <dht11.h>
dht11 DHT;
#define DHT11_PIN 4

//===============================================================
// Initialization
//===============================================================

// enter your network credentials
const char* ssid = "air24";
const char* password = "polis12345";

// port 80 is default website port
WebServer server(80);  

//===============================================================
// Setup Function
//===============================================================

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to: ");
  Serial.println(ssid);

  // set ESP32 mode
  // WiFi.mode(mode);   
  
  // ESP32 will connect to your WiFi modem with SSID & PASSWORD
  WiFi.begin(ssid, password);  

  // wait until ESP32 connect to WiFi
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Your ESP32 IP: ");
  
  // display "ESP32 IP Address" at Serial Monitor
  Serial.println(WiFi.localIP());  

  // set up mDNS responder:
  // "http://esp32.local" instead of "http://IPaddress"
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
 
  // attach handles
  server.on("/", handle_root); 
  
  // "http://ipaddress/inline" OR "htpp://esp32.local/inline"
  server.on("/inline", []() { 
    server.send(200, "text/plain", "this works as well");
  });
 
  // start web server
  server.begin();
  Serial.println("HTTP server started");
  delay(100); 

  // print key WiFi diagnostic information
  WiFi.printDiag(Serial);
 
 // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("RSSI:");
  Serial.println(rssi);
    
} // end of void setup( ) function

//===============================================================
// Loop Function
//===============================================================

void loop() {
  // monitors the presence of a client 
  // and delivers the requested HTML page
  server.handleClient();
} //end of loop function

//===============================================================
// Other Functions
//===============================================================

// Handle root url (/)
void handle_root() {
  server.send(200, "text/html", SendHTML()); 
}

// HTML & CSS contents which display on web server
// park your GPIO activities in "SendHtml( )" function
String SendHTML( ){
  // returns the number of milliseconds that 
  // your ESP32 board since powered up, 
  // & become zero when reset or unplugged
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  int chk = DHT.read(DHT11_PIN);    // READ DATA
  
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP32 Web Page</title>\n";
  ptr +="<meta http-equiv=refresh content=5>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr += "Server Up Time: <b>";
  ptr += hr;
  ptr += "h:";
  ptr += min % 60;
  ptr += "m:";
  ptr +=  sec % 60;
  ptr += "s </b>\n";
  ptr +="<h1>My Portable Data Center</h1>\n";
  ptr +="<h1>Temperature & Humidity Monitoring System</h1>\n";
  ptr +="<h2>ID: dc#1a</h2>\n";
  ptr +="<p>Temperature: ";
  ptr +=DHT.temperature,1;
  ptr +=" ";
  ptr +=char(176);
  ptr +="C</p>";
  ptr +="<p>Humidity: ";
  ptr +=DHT.humidity,1;
  ptr +="%</p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
