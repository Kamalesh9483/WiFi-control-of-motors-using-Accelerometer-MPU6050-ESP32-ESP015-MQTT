#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "ESP32-Access-Point";
const char* password = "123456789";

/* acceleration variables*/
String ax;
String ay;

/* ESP 015 (client end) to Arduino variables*/
const int RX = 0;
const int TX = 1;

/* Arduino to Driver module*/
/* Motor A*/
const int ENA = 11;
const int IN1 = 10;
const int IN2 = 9;

/* Motor B*/
const int IN3 = 6;
const int IN4 = 5;
const int ENB = 3;

/* IP address or domain name with URL path*/
const char* serverAccelerationX = "http://192.168.4.1/acceleration_x";
const char* serverAccelerationy = "http://192.168.4.1/acceleration_y";

/* Function to send request to server and get response in string format*/
String httpGETRequest(const char* serverName) {
  HTTPClient http;
    
  // IP address with path or Domain name with URL path 
  http.begin(serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "--"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Disconnect
  http.end();

  return payload;
}


void setup() {
  /* Wifi codes*/
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  Serial.println("Connecting");             /*Checking if wifi is connected, if not "." is printed every 500 ms*/
  while(WiFi.status() != WL_CONNECTED) { 
  delay(500);
  Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  /* Arduino input pins*/
  pinMode(RX,INPUT);
  pinMode(TX,OUTPUT);

/* Driver Module*/
  pinMode(ENA,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ENB,OUTPUT);
}


void loop() {

  int ax_int, ay_int
  /* Getting values of string information from server*/
 if(WiFi.status()== WL_CONNECTED ){ 
  ax = httpGETRequest(serverAccelerationX);
  ay = httpGETRequest(serverAccelerationy);
  ax_int = ax.toInt() /* to convert String to int*/
  ay_int = ay.toInt()
 }

 /*motor actuation*/
 /* mapping values of acceleration data to pulse width*/
   ax_int = map(ax_int, -17000, 17000, -125, 125);
   ay_int = map(ay_int, -17000, 17000, -125, 125); 

 /* Switching off the motor A and motor B if the pulse width values lie between -15 to +15 */
 if((ax_int>-15 && ax_int<15) && (ay_int>-15 && ay_int<15){
    digitalWrite(IN1,LOW);  
    digitalWrite(IN2,LOW);   
    digitalWrite(IN3,LOW);   
    digitalWrite(IN4,LOW);
 }

 /* Forward motion*/
 else if((ax_int>-15 && ax_int<15) && (ay_int>-125 && ay_int<-16){
    digitalWrite(IN1,HIGH);  
    digitalWrite(IN2,LOW);   
    digitalWrite(IN3,HIGH);   
    digitalWrite(IN4,LOW);
 }

 /* Backward motion*/
 else if((ax_int>-15 && ax_int<15) && (ay_int>16 && ay_int<125){
    digitalWrite(IN1,LOW);  
    digitalWrite(IN2,HIGH);   
    digitalWrite(IN3,LOW);   
    digitalWrite(IN4,HIGH);
 }

 /* left motion*/
 else if((ax_int>+16 && ax_int<125) && (ay_int>-15 && ay_int<15){
    digitalWrite(IN1,HIGH);  
    digitalWrite(IN2,LOW);   
    digitalWrite(IN3,LOW);   
    digitalWrite(IN4,LOW);
 }

 /* right motion*/
 else if((ax_int>-125 && ax_int<-16) && (ay_int>-15 && ay_int<15){
    digitalWrite(IN1,LOW);  
    digitalWrite(IN2,LOW);   
    digitalWrite(IN3,HIGH);   
    digitalWrite(IN4,LOW);
 }

}
