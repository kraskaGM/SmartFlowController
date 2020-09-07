/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = {0x4C, 0x11, 0xAE, 0xEA, 0xEE, 0xE8};

// Define variables to store User Input to be sent
int flow=60;
int settime=60;
bool onoff=0;

// Define variables to store incoming readings
String incomingID;
int incomingflow;
int incomingsettime; 
bool incomingonoff; 

// Variable to store if sending data was successful
String success;
String MacAddress;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    int ID;
    int flow;
    int settime;
    bool onoff;
} struct_message;



// Create a struct_message called ESPSlave to hold the sending info
struct_message ESPSlave;

// Create a struct_message called incomingReadings
struct_message incomingReadings;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}
// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  Serial.print("Bytes received: ");
  Serial.println(len);
  incomingID = incomingReadings.ID; //1= flow, 2=settime 3=onoff
  incomingflow = incomingReadings.flow;
  incomingsettime = incomingReadings.settime;
  incomingonoff = incomingReadings.onoff;
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  else {
    Serial.println("ESP-NOW initialized");    
  }
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
    // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  esp_now_register_recv_cb(OnDataRecv);
    
  MacAddress = WiFi.macAddress();
  Serial.println("macAddress is: ");
  Serial.println(MacAddress);
  
}
 
void loop() {
  /*
  // Set values to send
  ESPSlave.flow = flow;
  ESPSlave.settime = settime;
  ESPSlave.onoff = onoff;
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &ESPSlave, sizeof(ESPSlave));
  
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }*/
  delay(5000);
  Serial.print("ID: ");
  Serial.println(incomingID);
  Serial.print("flow: ");
  Serial.println(incomingflow);
  Serial.print("Settime: ");
  Serial.println(incomingsettime);
  Serial.print("Onoff: ");
  Serial.println(incomingonoff);
  Serial.println();
}
