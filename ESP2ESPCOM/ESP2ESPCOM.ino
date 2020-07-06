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
#define DEBUG2 false
#include <esp_now.h>
/*Communication specific variable*/
/* Set these to your desired credentials. */
const char *ssid = "SmartFlowController"; // You can change it according to your ease
const char *password = "Taltech2020"; // You can change it according to your ease
/* Put IP Address details */
//IPAddress local_ip(192,168,4,1);
//IPAddress gateway(192,168,4,1);
//IPAddress subnet(255,255,255,0);
const char* PARAM_MESSAGE = "message";
const int dns_port = 53;
const int http_port = 80;
//const int ws_port = 1337;//websocket port

char msg_buf[15];
int pumpState = 0;

/*Webserver*/
AsyncWebServer server(http_port); // establishing server at port 80 (HTTP protocol's default port)
//WebSocketsServer webSocket = WebSocketsServer(1337);
AsyncWebSocket webSocket("/ws");

String MACAdress3;
String MACAdress3;

int flow=50;
int settime=50;
const int resolution = 12;
const int ledChannel = 1;
const int freq = 5000;

bool mrunning = true;

const int PWMPin=26;
int setpoint=2000;
void function extractMAC(String Address,uint8_t broadcastAddress[])
{
  for(int i;i);
}

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress1[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t broadcastAddress2[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} struct_message;

// Create a struct_message called myData
struct_message myData;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

PairWithDevice(uint8_t Address)
{
  // Init ESP-NOW
 if (esp_now_init() != ESP_OK) 
 {
   Serial.println("Error initializing ESP-NOW");
   return;
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, Address, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
   // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
    // Set values to send
  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1,20);
  myData.c = 1.2;
  myData.d = "Hello";
  myData.e = false;
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);
 }
}


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
      if (strcmp(ID,"togglePump1")==0) 
      {  
        pumpState = pumpState ? 0 : 1;
        if (DEBUG)
        {           
         Serial.printf("Toggling Pump to %u\n", pumpState);
        }
      }
      // Report the state of the Pump
      else if (strcmp(ID,"getPumpState1")==0) 
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
      else if (strcmp(ID,"sendPumpTime1")==0) 
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
      else if (strcmp(ID,"sendPumpFlow1")==0) 
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
      else if (strcmp(ID,"sendMACAdress2")==0) 
      {
        uint8_t Recieved_message[] = doc["message"];
        broadcastAddress1=Recieved_message;
        if (DEBUG)
        {   
        sprintf(msg_buf, "%d", broadcastAddress1);
        Serial.printf("Sending to [%u]: %s\n", client, msg_buf);
        }
        client->text(msg_buf);
        //webSocket.sendTXT(client, msg_buf);
        PairWithDevice(broadcastAddress1);   
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

void setup() {

  // PWM for Motor
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(PWMPin, ledChannel);
  
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

}
void loop() {
  int delayTime=settime*1000;
  
  while(pumpState == 1)
  {

    if (mrunning)
    {
      /*
      for(int dutyCycle = 0; dutyCycle <= setpoint; dutyCycle++){
        ledcWrite(ledChannel, dutyCycle);
        delay(1);
        Serial.print("dutyCycle: ");
        Serial.println(dutyCycle);
      }*/
      ledcWrite(ledChannel, setpoint);
      mrunning = false;
      //Serial.println("Pump runs on the max speed");
      Serial.println("Pump running time is: ");
      Serial.println(delayTime);
    }
     ledcWrite(ledChannel, setpoint);
     if (previousMillis-currentMillis >=delayTime)
     {
     mrunning = true;
     Serial.println("Job completed");
     pumpState=0;     
     break;
     }

     delay(1);

     //Waiting until time is over
     //Serial.print("Countine timer: ");
     //unsigned int long timer= (previousMillis-currentMillis)/1000;
     //Serial.println(timer);
     previousMillis = millis();
   }   
  //Serial.println("Pump runs on the zero speed");
  delay(1);
 
  ledcWrite(ledChannel, 0);
  mrunning = true;
  delay(10);



}
