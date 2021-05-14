#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Wire.h>
#include "MPU6050.h"

MPU6050 gyro;

// Accelerometer
// gyro and acclerometer has both positive and negative values so signed int 16 bit is used
  int16_t ax, ay, az; 

// Gyro
  int16_t gx, gy, gz; 

// Setting access point network credentials
  const char* ssid = "ESP32-Access-Point";
  const char* password = "123456789";

// Create AsyncWebServer object on port 80
  AsyncWebServer server(80);                /* Using asynchronous network means that you can handle more than one connection at
                                          the same time*/
void setup(){
  Serial.begin(115200);
  Wire.begin();  // Wire.begin(address) --- address: the 7-bit slave address (optional); 
              // if not specified, join the bus as a master.

  gyro.initialize();

  if(gyro.testConnection() != 0){
    Serial.println("Successfully connected");
  }

  else{
     Serial.println("Connection failure");
  }


   
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  
  WiFi.softAP(ssid, password,3);          /* ESP to run in soft access point mode so that Wi-Fi stations(Clients) can connect to it
//                                        WiFi.softAP(ssid, password, channel, hidden, max_connection)*/
// 
  IPAddress IP = WiFi.softAPIP();       /*Return IP address of the soft access point’s network interface.*/
  Serial.print("AP IP address: ");
//  Serial.println(IP);

/* server.on function now takes three parameters: the URI, the request method (GET or POST) and the function to execute.
* A Uniform Resource Identifier (URI) is a unique sequence of characters that identifies a logical or physical resource used by web technologies
* it sends the acceleration returned by the analogRead() function as a char that’s why we use the c_str() method.
 */

  server.on("/acceleration_x", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/plain", String(ax).c_str());});               

   server.on("/acceleration_y", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/plain", String(ay).c_str());});     

   server.begin();                                      
}

void loop() 
{ 
  /* This is an asynchronous web server,so there’s nothing in the loop()
   * In the Asynchronous Web it is possible to deliver spontaneous changes as the state of a dynamic system changes*/
  gyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  Serial.println(ay);
  delay(500);
}
