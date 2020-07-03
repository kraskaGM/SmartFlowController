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

int flow=50;
int settime=50;
const int resolution = 12;
const int ledChannel = 1;
const int freq = 5000;
unsigned long previousMillis = 0;
unsigned long currentMillis=0;
bool mrunning = true;

const int PWMPin=26;
int setpoint=2000;

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
const int P1 = 34; 
const int P2 = 35;

// variable for storing the potentiometer value
int AnalogValue = 0;
int LastAnalogValue=0;
int AnalogValue2 = 0;
int LastAnalogValue2=0;

const int l=60;
const int d=1;
const float pi=3.14;
float S = pi*d*d/4;
float volume = l*S/1000.0;
float flowrate =0;

unsigned long StateTime1[]= {0,0,0,0}; //StateTime1[0] = UP trigger time, Down trigger time, counter1 counter2
unsigned long StateTime2[]= {0,0,0,0};

void setup() {
  analogReadResolution(12); // 12 bit resolution for ADC reading
  analogSetWidth(12); //12 bit resolution for ADC writing
  analogSetCycles(8);// 1 analog value is read 8 times;
  analogSetSamples(1); // analog samples - immpact in sensitivity
  analogSetClockDiv(1); // analog clock divider 1-255, fastest 1
  analogSetAttenuation(ADC_11db);//ADC_0db no voltage reduction, 1/3,6 reduction (ADC_11db provides an attenuation so that IN/OUT = 1 / 3.6 an input of 3 volts is reduced to 0.833 volts before ADC measurement)
  //analogSetPinAttenuation(P1, ADC_0db); //sets attenuation for specific pin
  //analogSetPinAttenuation(P2, ADC_0db);
  adcAttachPin(P1);
  adcAttachPin(P2);
  
  

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
unsigned long EdgeDetection(int previousValue, int currentValue, unsigned long *StateTime)
{

  
  if (previousValue<1500 && currentValue>1500)
  {
    Serial.print("Prevous Analog Value is: ");
    Serial.println(previousValue);
    Serial.print("Current Analog Value is: ");
    Serial.println(currentValue);
    unsigned long currentMillis = millis();
    StateTime[0] = currentMillis;  // Remember the time
    StateTime[2] = StateTime[2]+1; //edge up counter
    Serial.print("Edge up was detected. Timestamp: ");
    Serial.println(currentMillis);
  }/*
  else if ((previousValue-currentValue)<-1500)
  {
    Serial.print("Prevous Analog Value is: ");
    Serial.println(previousValue);
    Serial.print("Current Analog Value is: ");
    Serial.println(currentValue);
    unsigned long currentMillis = millis();
    StateTime[1] = currentMillis;  // Remember the time
    StateTime[3] = StateTime[3]+1; //edge down counter
    Serial.print("Edge down was detected. Timestamp: ");
    Serial.println(currentMillis);
  }*/

  return *StateTime;
}

unsigned long calcflowrate(unsigned long *StateTime1, unsigned long *StateTime2)
{
  
    if (StateTime1[3]>0)
    {
      if (StateTime1[3] == StateTime2[3]); // checking up counters if they are equal
      {
        unsigned long int t=StateTime1[0]-StateTime2[0];
        float newflowrate = volume/t*60*1000;
        if (flowrate!=newflowrate)
        {
          flowrate=newflowrate;
          Serial.print("2nd up edge triggered, flowrate is: ");
          Serial.println(flowrate);
        }
      }
    }
    if (StateTime1[2]>0)
    {
    if (StateTime1[2] == StateTime2[2]); // checking down counters if they are equal
    {
        unsigned long int t=StateTime1[0]-StateTime2[0];
        float newflowrate = volume/t*60*1000;
        if (flowrate!=newflowrate)
        {
          flowrate=newflowrate;
          Serial.print("2nd down edge triggered, flowrate is: ");
          Serial.println(flowrate);
        }
    }
    }

  // bubble detection
  if (StateTime1[2]>0)
  {
    if ((StateTime1[2]-StateTime2[2])>1) // if up counters have more difference that 1then there was an bubble in the flow and the edge detected was overwritten, not getting accurate flow
    {
      Serial.println("Bubbles detected in flow (up edge), setting counter to 0");
      StateTime1[2]=0;
      StateTime2[2]=0;
    }
  }
   if (StateTime1[3]>0)
    {
      if ((StateTime1[3]-StateTime2[3])>1) //edgde down bubbles
      {
        Serial.println("Bubbles detected in flow (down edge), setting counter to 0");
        StateTime1[3]=0;
        StateTime2[3]=0;
      }
    }
  return *StateTime1, *StateTime2;
}
void loop() {
  int delayTime=settime*1000;
  
  while(pumpState == 1)
  {

    if (mrunning)
    {
      currentMillis = millis();/*
      for(int dutyCycle = 0; dutyCycle <= setpoint; dutyCycle++){
        ledcWrite(ledChannel, dutyCycle);
        delay(1);
        Serial.print("dutyCycle: ");
        Serial.println(dutyCycle);
      }*/
      ledcWrite(ledChannel, setpoint);
      mrunning = false;
      Serial.println("Pump runs on the max speed");
      Serial.println("Pump running time is: ");
      Serial.println(delayTime);
      previousMillis= millis();
    }
     ledcWrite(ledChannel, setpoint);
     if (previousMillis-currentMillis >=delayTime)
     {
     mrunning = true;
     Serial.println("Job completed");
     pumpState=0;     
     break;
     }
     LastAnalogValue = analogRead(P1);
     LastAnalogValue2 = analogRead(P2);
          
     EdgeDetection(LastAnalogValue,AnalogValue,StateTime1);
     EdgeDetection(LastAnalogValue2,AnalogValue2,StateTime2);
     calcflowrate(StateTime1,StateTime2);

     delay(1);

     AnalogValue = analogRead(P1);
     AnalogValue2 = analogRead(P2);
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
  LastAnalogValue = analogRead(P1);
  LastAnalogValue2 = analogRead(P2);

  delay(10);
  AnalogValue = analogRead(P1);
  AnalogValue2 = analogRead(P2);
  if (DEBUG2)
  {
  Serial.print("Prevous Analog Value 1 is: ");
  Serial.println(LastAnalogValue);
  Serial.print("Current Analog Value 1 is: ");
  Serial.println(AnalogValue);
  Serial.print("Previous Analog Value 2 is: ");
  Serial.println(LastAnalogValue2);

  Serial.print("Current Analog Value 2 is: ");
  Serial.println(AnalogValue2);
  }
  EdgeDetection(LastAnalogValue,AnalogValue,StateTime1);
  EdgeDetection(LastAnalogValue2,AnalogValue2,StateTime2);
  calcflowrate(StateTime1,StateTime2);
}
