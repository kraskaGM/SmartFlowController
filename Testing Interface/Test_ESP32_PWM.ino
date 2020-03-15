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
  #include <ArduinoJson.h>
  //#include <WiFi.h>
  #include <SPIFFS.h>
  #include <ESPAsyncWebServer.h>
  /*
  //#include <WebSocketsServer.h>
#else
  #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <Hash.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include <FS.h>*/
#endif

#define DEBUG true

/*Communication specific variable*/
/* Set these to your desired credentials. */
const char *ssid = "SmartFlowController"; // You can change it according to your ease
const char *password = "Taltech2020"; // You can change it according to your ease
/* Put IP Address details */
//IPAddress local_ip(192,168,4,1);
//IPAddress gateway(192,168,4,1);
//IPAddress subnet(255,255,255,0);

const int dns_port = 53;
const int http_port = 80;
//const int ws_port = 1337;//websocket port


char msg_buf[15];
int pumpState = 0;

/*Webserver*/
AsyncWebServer server(http_port); // establishing server at port 80 (HTTP protocol's default port)
//WebSocketsServer webSocket = WebSocketsServer(1337);
AsyncWebSocket webSocket("/ws");

int flow=50;
int settime=50;
const int resolution = 12;
const int ledChannel = 1;
const int freq = 5000;
unsigned long previousMillis = 0;
bool notrunning = true;
bool mrunning = true;

/*Hardware specific variables*/
/* The flow sensor analog inputs*/
/*const int P1 = 26; 
const int P2 = 27;
const int P3 = 14;
const int P4 = 12;

float measuredFlowRate=50;
*/
/*Function Variables*/
/*bool liguidP1=false;
bool liguidP2=false;
bool liguidP3=false;
bool liguidP4=false;

int long previousTimeP1=0; //time stamp
int long previousTimeP2=0; //time stamp
int long previousTimeP3=0; //time stamp
int long previousTimeP4=0; //time stam
*/
/* Peristaltic pump analog output*/
//const int K1=23;
const int PWMPin=4;

//int maxAValue=4096;
/* Satus LED digital outputs*/
//const int GNLED = 19;
//const int RDLED = 5;

//float tubeDiameter=3.14; //mm
//int Distance1=32; //mm
//int Distance2=40; //mm
//int Distance3=32; //mm

/*PID variables*/
//int propGain=1;
//int IntegralGain=0;
//int DerGain=0;

int setpoint=2000;
//int error=0;
//int Prev_error=0;

//int integral=0;
//int prev_error=0;
//int derivative=0;
//int PID;

// Callback: receiving any WebSocket message
void onWebSocketEvent(AsyncWebSocket * server, 
                      AsyncWebSocketClient * client, 
                      AwsEventType type, void * arg, 
                      uint8_t *data, size_t length) {
  // Figure out the type of WebSocket event
  switch(type) 
  {
    // Client has disconnected
    case WS_EVT_DISCONNECT:
      Serial.printf("[%u] Disconnected!\n", client);
      break;
    // New client has connected
    case WS_EVT_CONNECT:
      {
        //IPAddress ip = webSocket.remoteIP(client);
        Serial.printf("[%u] Connection from ", client);
        //Serial.println(ip.toString());
      }
      break;
    // Handle text messages from client
    
    //For everything else: do nothing
    case WS_EVT_PONG:
    {
      Serial.println("[%u] Message type: WS_EVT_PONG");
      break;
    }   
    case WS_EVT_DATA:
    {
      AwsFrameInfo * info = (AwsFrameInfo*)arg;
      if (DEBUG)
      {
        Serial.printf("[%u] Received text: %s\n", client, data);
      }
      //StaticJsonDocument<200> doc;
      DynamicJsonDocument doc(200);
      uint8_t* input = data;
      DeserializationError error=deserializeJson(doc, input);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
      //JsonObject obj = doc.as<JsonObject>();
      const char* ID = doc["ObjectID"];
      if (DEBUG)
      {      
        Serial.println(doc["ObjectID"].as<char*>());
        Serial.printf("[%u] Received ObjectID: %s\n", ID);
      }
      // Toggle LED
      if (strcmp(ID,"togglePump")==0) 
      {  
        pumpState = pumpState ? 0 : 1;
        if (DEBUG)
        {           
         Serial.printf("Toggling Pump to %u\n", pumpState);
        }
      }
      // Report the state of the Pump
      else if (strcmp(ID,"getPumpState")==0) 
      {
        if (DEBUG)
        {    
        sprintf(msg_buf, "%d", pumpState);
        Serial.printf("Sending to [%u]: %s\n", client, msg_buf);
        }
        //webSocket.sendTXT(client, msg_buf);
        client->text(msg_buf);
        mrunning = true;
      }
      else if (strcmp(ID,"sendPumpTime")==0) 
      {
        int Recieved_message = doc["message"];
        settime=Recieved_message;
        if (DEBUG)
        {         
        sprintf(msg_buf, "%d", settime);
        Serial.printf("Sending to [%u]: %s\n", client, msg_buf);
        }
        //webSocket.sendTXT(client, msg_buf);
        client->text(msg_buf);
      }
      else if (strcmp(ID,"sendPumpFlow")==0) 
      {
        int Recieved_message = doc["message"];
        setpoint=Recieved_message;
        if (DEBUG)
        {   
        sprintf(msg_buf, "%d", setpoint);
        Serial.printf("Sending to [%u]: %s\n", client, msg_buf);
        }
        client->text(msg_buf);
        //webSocket.sendTXT(client, msg_buf);
           
      }
      // Message not recognized
      else 
      {
        Serial.println("[%u] Message not recognized");
        
      }
      break;
    }
  
    default:
    break;
  }
} 
       
/*The files which are sent from webservers memory*/

// Callback: send homepage
void onIndexRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();              
  request->send(SPIFFS, "/Index.html", "text/html");
  if (DEBUG)
  {
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  }
}
 // Callback: send style sheet
void onCSSRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  request->send(SPIFFS, "/main.css", "text/css");
  if (DEBUG)
  {  
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  } 
}
// Callback: send jquery
void onjqueyRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  request->send(SPIFFS, "/jquery.js", "application/js");
  if (DEBUG)
  {
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  }
}
// Callback: send myJsFnctions
void onmyFunctionsRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();             
  request->send(SPIFFS, "/myJsFunctions.js", "application/js");
  if (DEBUG)
  {  
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  } 
}
// Callback: send 404 if requested file does not exist
void onPageNotFound(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  request->send(404, "text/plain", "Not found");
  if (DEBUG)
  {   
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  } 
}
/*
int analogSacale(int flowRate,int maxAValue)
{
 int analogSacale = flowRate*maxAValue/100;
 return analogSacale;
}*/
/*
float flowRate(int distance, int time)
{
 float volume = float(distance)*tubeDiameter/1000;//ml/min
 float flowRate =volume*float(time);
 return flowRate;
}*/
/*
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
}*/
/*
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
}*/

void setup() {
  /* The flow sensor analog inputs*/
//  analogReadResolution(12); // 12 bit resolution for ADC reading
  //analogSetWidth(12); //12 bit resolution for ADC writing 4096
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(PWMPin, ledChannel);
  //analogSetCycles(8);// 1 analog value is read 8 times;
  //analogSetSamples(1); // analog samples - immpact in sensitivity
  //analogSetClockDiv(1); // analog clock divider 1-255, fastest 1
  //analogSetAttenuation(ADC_11db); //for all pins 3.6 volt attenuation (1V input = ADC reading of 3959)
  //analogSetPinAttenuation(pin, attenuation) //sets attenuation for specific pin
//  adcAttachPin(P1);
  //adcAttachPin(P2);
  //adcAttachPin(P3);
  //adcAttachPin(P4);
/* Peristaltic pump analog output*/
  //pinMode(K1,OUTPUT);
/* Satus LED digital outputs*/
 // pinMode(GNLED,OUTPUT);
  //pinMode(RDLED,OUTPUT);
  //digitalWrite(GNLED,HIGH);
  //digitalWrite(RDLED,LOW);   
  Serial.begin(115200);

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    while(1);
  }

  Serial.println();
  Serial.print("Configuring access point...");
    /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);// --> This line will create a WiFi hotspot.
  //WiFi.softAPConfig(local_ip, gateway, subnet);
  //delay(100);

  //IPAddress myIP = WiFi.softAPIP();
  
  Serial.print("AP IP address: ");
  //Serial.println(myIP);
  Serial.println(WiFi.softAPIP());
 
   // On HTTP request for root, provide index.html file
  server.on("/", HTTP_GET, onIndexRequest);
 
  // On HTTP request for style sheet, provide style.css
  server.on("/main.css", HTTP_GET, onCSSRequest);

  server.on("/jquery.js", HTTP_GET, onjqueyRequest);

  server.on("/myJsFunctions.js", HTTP_GET, onmyFunctionsRequest);

  // Handle requests for pages that do not exist
  server.onNotFound(onPageNotFound);
  // Start web server
  
  // Start WebSocket server and assign callback
  //webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
  server.addHandler(&webSocket);
  server.begin();
  Serial.println("HTTP server started");

  //setpoint = analogSacale(flow,maxAValue);
}
/*
void pumpControl(){
         if (pumpState==1)
         { 
            for(int dutyCycle = 0; dutyCycle <= setpoint; dutyCycle++){
              ledcWrite(ledChannel, setpoint);
              delay(1);
            }

            int delayTime=settime*1000;
            Serial.println("Pump runs on the max speed");
            delay(delayTime);
         }
          else if (pumpState==0)
         {
            for(int dutyCycle = setpoint; setpoint >= 0; dutyCycle--){
            // changing the LED brightness with PWM
              ledcWrite(ledChannel, setpoint);
              delay(1);
              }
              Serial.println("Pump is off");
              delay(1000);
         }
}*/
void loop() {

  unsigned long currentMillis = millis();
  int delayTime=settime*1000;
  if (pumpState == 1)
  {
    if (!notrunning)
    {
      notrunning = true;
    }
    if (mrunning)
    {
      for(int dutyCycle = 0; dutyCycle <= setpoint; dutyCycle++){
        ledcWrite(ledChannel, dutyCycle);
        delay(1);
        Serial.print("dutyCycle: ");
        Serial.println(dutyCycle);
      }
      mrunning = false;
      Serial.println("Pump runs on the max speed");
    }
    //Serial.println("Pump runs on the max speed");
    if (currentMillis - previousMillis >= delayTime) {
        // save the last time you blinked the LED
        previousMillis = currentMillis;
        for(int dutyCycle = setpoint; setpoint >= 0; dutyCycle--){
           // changing the LED brightness with PWM
           ledcWrite(ledChannel, dutyCycle);
           delay(1);
           Serial.println("waiting ...");
         }
     pumpState = 0;
     mrunning = true;
     Serial.println("Job completed");
    } 
  }
  else if (notrunning  && pumpState == 0)
  {
    Serial.println("Pump runs on the zero speed");
    Serial.print("setpoint: ");
    Serial.println(setpoint);
    //Serial.print("dutyCycle: ");
    //Serial.println(dutyCycle);
    
    ledcWrite(ledChannel, 0);
    notrunning = false;
    mrunning = true;
    }/*
    for(int dutyCycle = setpoint; dutyCycle >= 0; dutyCycle--)
    {
    // changing the LED brightness with PWM
      ledcWrite(ledChannel, dutyCycle);
      delay(1);
      Serial.print("dutyCycle: ");
      Serial.println(dutyCycle);
     }
     notrunning = false;
     mrunning = true;
     Serial.println("Pump runs on the zero speed");
  }*/

  //webSocket.loop();
  //ledcWrite(ledChannel, setpoint);
  //delay(10000);
  //pumpControl();
  //delay(1000);
  //pumpControl();
  //ledcWrite(PWMPin, setpoint);
  //Serial.print("was checking ledcwrite, PWMPin value is:");
  //Serial.println(setpoint);
}
