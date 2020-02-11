/*
 Create a WiFi access point and provide a web server on it. 
For tutorial video of this code, watch out this video
https://youtu.be/fcmb_3aBfH4
  visit 
  http://www.techiesms.com
  for IoT project tutorials.
  
        #techiesms
  explore | learn | share
*/

// Import required libraries
#ifdef ESP32
  #include <WiFi.h>
  #include <SPIFFS.h>
  #include <ESPAsyncWebServer.h>
  #include <WebSocketsServer.h>
#else
  #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <Hash.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include <FS.h>
#endif

/*Communication specific variable*/
/* Set these to your desired credentials. */
const char *ssid = "SmartFlowController"; // You can change it according to your ease
const char *password = "Taltech2020"; // You can change it according to your ease
/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

const int dns_port = 53;
const int http_port = 80;
const int ws_port = 1337;//websocket port
WebSocketsServer webSocket = WebSocketsServer(1337);
char msg_buf[10];
int pumpState = 0;

/*Webserver*/
AsyncWebServer server(80); // establishing server at port 80 (HTTP protocol's default port)

int flow=50;
int settime=50;

/*Hardware specific variables*/
/* The flow sensor analog inputs*/
const int P1 = 26; 
const int P2 = 27;
const int P3 = 14;
const int P4 = 12;

float measuredFlowRate=50;

/*Function Variables*/
bool liguidP1=false;
bool liguidP2=false;
bool liguidP3=false;
bool liguidP4=false;

int long previousTimeP1=0; //time stamp
int long previousTimeP2=0; //time stamp
int long previousTimeP3=0; //time stamp
int long previousTimeP4=0; //time stam

/* Peristaltic pump analog output*/
const int K1=23;
const int PWMPin=23;

int maxAValue=4096;
/* Satus LED digital outputs*/
const int GNLED = 19;
const int RDLED = 5;

float tubeDiameter=3.14; //mm
int Distance1=32; //mm
int Distance2=40; //mm
int Distance3=32; //mm

/*PID variables*/
int propGain=1;
int IntegralGain=0;
int DerGain=0;

int setpoint=2000;
int error=0;
int Prev_error=0;

int integral=0;
int prev_error=0;
int derivative=0;
int PID;

// Callback: receiving any WebSocket message
void onWebSocketEvent(uint8_t client_num,
                      WStype_t type,
                      uint8_t * payload,
                      size_t length) {
  // Figure out the type of WebSocket event
  switch(type) {
 
    // Client has disconnected
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", client_num);
      break;
 
    // New client has connected
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(client_num);
        Serial.printf("[%u] Connection from ", client_num);
        Serial.println(ip.toString());
      }
      break;
 
    // Handle text messages from client
    case WStype_TEXT:
      

      // Print out raw message
      Serial.printf("[%u] Received text: %s\n", client_num, payload);
      
      // Toggle LED
      if ( strcmp((char *)payload, "togglePump") == 0 ) 
      {
         String response="";   
         digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));     
         //response+=!digitalRead(BUILTIN_LED);  
         pumpState = pumpState ? 0 : 1;
         Serial.printf("Toggling Pump to %u\n", pumpState);
         if (pumpState==1)
         {
            ledcWrite(PWMPin, setpoint);
         }
            else
         {
            ledcWrite(PWMPin, 0);
         }
      }
      // Report the state of the Pump
      else if ( strcmp((char *)payload, "getPumpState") == 0 ) 
      {
      sprintf(msg_buf, "%d", pumpState);
      Serial.printf("Sending to [%u]: %s\n", client_num, msg_buf);
      webSocket.sendTXT(client_num, msg_buf);
      }
      // Message not recognized
      else 
      {
        Serial.println("[%u] Message not recognized");
      }
      break;
 
    // For everything else: do nothing
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    default:
      break;
  }
} 

/*The files which are sent from webservers memory*/

// Callback: send homepage
void onIndexRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/Index.html", "text/html");
}
 // Callback: send style sheet
void onCSSRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/main.css", "text/css");
}
// Callback: send jquery
void onjqueyRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/jquery.js", "application/js");
}
// Callback: send myJsFnctions
void onmyFunctionsRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/myJsFunctions.js", "application/js");
}
// Callback: send 404 if requested file does not exist
void onPageNotFound(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(404, "text/plain", "Not found");
}

int analogSacale(int flowRate,int maxAValue)
{
 int analogSacale = flowRate*maxAValue/100;
 return analogSacale;
}

void handlePostFlow(AsyncWebServerRequest *request)
{ 
  String response=String(flow);
  request->send(200,"text/plain",response);
  Serial.print("Flow rate was set to: ");
  Serial.println(flow);
}

void handlePostFlowUpdate(AsyncWebServerRequest *request)
{ 
  //flow= server.arg(0).toInt();
  String response=String(flow);
  request->send(200,"text/plain",response);
  setpoint = analogSacale(flow,maxAValue);
  Serial.print("Flow rate was updated: ");
  Serial.println(flow);
  Serial.print("Analog PWM setpoint was set to: ");
  Serial.println(setpoint);
}

void handlePostTime(AsyncWebServerRequest *request)
{
  String response=String(settime); 
  request->send(200,"text/plain",response);
  Serial.print("Pumping time was set to: ");
  Serial.println(settime);
}

void handlePostTimeUpdate(AsyncWebServerRequest *request)
{ 
  //settime=server.arg(0).toInt();
  String response=String(settime);
  request->send(200,"text/plain",response);
  Serial.print("Pumping time was Updated: ");
  Serial.println(settime);
}

float flowRate(int distance, int time)
{
 
 float volume = float(distance)*tubeDiameter/1000;//ml/min
 float flowRate =volume*float(time);
 
 return flowRate;
}
 
bool triggeThreshold(int analogSensoValue, int threshold)
 
{
 if (analogSensoValue > threshold)
 {
   return true;
 }
  else
  {
   return false;
  }
}

int checkSetpoint(int setpoint)
{
 if (setpoint>maxAValue)
 {
  setpoint =maxAValue;
  return setpoint;
 }
 else if (setpoint<0)
 {
  setpoint =0; //Should be found out what is the minimum flow rate required for 1ml/min
  return setpoint;
 }
 else
 {
  return setpoint;
 }
}

void setup() {
  
  pinMode(BUILTIN_LED,OUTPUT);
  /* The flow sensor analog inputs*/
  analogReadResolution(12); // 12 bit resolution for ADC reading
  analogSetWidth(12); //12 bit resolution for ADC writing
  analogSetCycles(8);// 1 analog value is read 8 times;
  analogSetSamples(1); // analog samples - immpact in sensitivity
  analogSetClockDiv(1); // analog clock divider 1-255, fastest 1
  analogSetAttenuation(ADC_11db); //for all pins 3.6 volt attenuation (1V input = ADC reading of 3959)
  //analogSetPinAttenuation(pin, attenuation) //sets attenuation for specific pin
  adcAttachPin(P1);
  adcAttachPin(P2);
  adcAttachPin(P3);
  adcAttachPin(P4);
/* Peristaltic pump analog output*/
  pinMode(K1,OUTPUT);
/* Satus LED digital outputs*/
  pinMode(GNLED,OUTPUT);
  pinMode(RDLED,OUTPUT);
  digitalWrite(GNLED,HIGH);
  digitalWrite(RDLED,LOW);   
  Serial.begin(115200);

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  Serial.println();
  Serial.print("Configuring access point...");
    /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);// --> This line will create a WiFi hotspot.
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 
   // On HTTP request for root, provide index.html file
  server.on("/", HTTP_GET, onIndexRequest);
 
  // On HTTP request for style sheet, provide style.css
  server.on("/style.css", HTTP_GET, onCSSRequest);

  server.on("/jquery.js", HTTP_GET, onjqueyRequest);

  server.on("/myJsFunctions.js", HTTP_GET, onmyFunctionsRequest);

  server.on("/postflow",HTTP_GET,handlePostFlow);    
  server.on("/postflow/update",HTTP_GET,handlePostFlowUpdate);    
  server.on("/postime",HTTP_GET,handlePostTime);
  server.on("/postime/update",HTTP_GET,handlePostTimeUpdate);  
 
  // Handle requests for pages that do not exist
  server.onNotFound(onPageNotFound);
  
  /* Old version
   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", HTML);
  });
  server.on("/main.css" , HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/css", mainCSS);
  });
  server.on("/jquery.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200,"application/js",jquery_js);
  });
  server.on("/myJsFunctions.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200,"application/js",myJsFunctions_js);
  });*/
  

 
  // Start web server
  server.begin();
 
  // Start WebSocket server and assign callback
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);

  Serial.println("HTTP server started");
  digitalWrite(BUILTIN_LED,1);
  setpoint = analogSacale(flow,maxAValue);
}

void PIDControl()
{
if (!liguidP1)
{
  int P1=analogRead(P1);
  liguidP1=triggeThreshold(P1,2500);
  previousTimeP1=millis();
}
else if (!liguidP2)
{
 int P2=analogRead(P2);
 previousTimeP2=millis(); 
 liguidP2=triggeThreshold(P2,2500);
 if (liguidP2)
 {
   int deltatime=previousTimeP2-previousTimeP1;
   measuredFlowRate=flowRate(Distance1,deltatime);
   error=measuredFlowRate-flow;
   integral =integral+error*deltatime; //Errors over time added
   derivative=(error-prev_error)/deltatime; //comparision error over time 
   PID = propGain*error+IntegralGain*integral+DerGain*derivative;//PID algorithm
   Prev_error=error;
   setpoint=setpoint+PID;
   setpoint = checkSetpoint(setpoint);
   ledcWrite(PWMPin, setpoint);
 }
 }
 else if (!liguidP3)
 {
  int P3=analogRead(P3);
  previousTimeP3=millis();
  liguidP3=triggeThreshold(P3,2500);
  if (liguidP3)
  {
   int deltatime=previousTimeP3-previousTimeP2;
   measuredFlowRate=flowRate(Distance2,deltatime);
   error=measuredFlowRate-flow;
   integral =integral+error*deltatime; //Errors over time added
   derivative=(error-prev_error)/deltatime; //comparision error over time 
   PID = propGain*error+IntegralGain*integral+DerGain*derivative;//PID algorithm
   Prev_error=error;
   setpoint=setpoint+PID;
   setpoint = checkSetpoint(setpoint);
   ledcWrite(PWMPin, setpoint);
  }  
 }
 else if (!liguidP4)
 { 
 int P4=analogRead(P4);
 previousTimeP4=millis();
 liguidP4=triggeThreshold(P4,2500);
 if (liguidP4)
 {
   int deltatime=previousTimeP4-previousTimeP3;
   measuredFlowRate=flowRate(Distance3,deltatime);
   error=measuredFlowRate-flow;
   integral =integral+error*deltatime; //Errors over time added
   derivative=(error-prev_error)/deltatime;//comparision error over time 
   PID = propGain*error+IntegralGain*integral+DerGain*derivative;//PID algorithm
   Prev_error=error;
   setpoint=setpoint+PID;
   setpoint = checkSetpoint(setpoint);
   ledcWrite(PWMPin, setpoint);
  }   
 } 
}
 
void loop() {
  //server.handleClient();
  webSocket.loop();
  PIDControl();  
}
