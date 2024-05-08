
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define GPS_TX_PIN 2
#define GPS_RX_PIN 3
// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library 
#include <SPI.h> 
 
// Create Amplitude Shift Keying Object
RH_ASK rf_driver;
TinyGPSPlus gps;
SoftwareSerial mygps(GPS_TX_PIN, GPS_RX_PIN);
#include <Arduino.h>
#include <math.h>

#define RADIUS_OF_EARTH 6371000 
 
void setup()
{
    // Initialize ASK Object
    rf_driver.init();
    // Setup Serial Monitor
   Serial.begin(9600);
   mygps.begin(9600);
   pinMode(6,OUTPUT);
  
}
 
void loop()
{
  
   while (mygps.available() > 0) {
    gps.encode(mygps.read());
    if (gps.location.isUpdated()) {
      // Extract latitude and longitude
      float latitude = gps.location.lat();
      float longitude = gps.location.lng();

      // Convert latitude and longitude to strings
      String latitudeStr = String(latitude, 6);
      String longitudeStr = String(longitude, 6);

      // Construct the data string
      String dataString = "" + latitudeStr + "," + longitudeStr;
    double own_lat = latitude;
    double own_lon = longitude;
    uint8_t buf[21];
    uint8_t buflen = sizeof(buf);
    // Check if received packet is correct size
    if (rf_driver.recv(buf, &buflen)){
      double near_lat ;
      double near_lon ;
      int acc ;
      Serial.print("Recieved:");
      Serial.println((char*)buf);
      char temp[strlen(buf) + 1];
      strcpy(temp, buf);
      char* token = strtok(temp, ",");
      near_lat = atof(token);
      token = strtok(NULL, ",");
      near_lon = atof(token);
      token = strtok(NULL, ",");
      if (token != NULL) {
      acc = atoi(token);
      Serial.println(acc); // Assuming lastValue is an integer variable
      } else {
      acc = -1; // Indicate that there was no last value
      }
      
      if(acc == 1){
        digitalWrite(6,HIGH);
      }else{
        digitalWrite(6,LOW);
      }
      double distance = calculateDistanceBetweenPoints(own_lat, own_lon, near_lat, near_lon);
      Serial.println(distance);    
      if(distance<10){
        digitalWrite(7,HIGH);
      }else{
        digitalWrite(7,LOW);
      }    
    }
    }
  }
  if (!gps.location.isValid()) {
    double own_lat = 20.934101;
    double own_lon = 85.262180;
    String defaultData = "20.934101,85.262180";
   // Serial.println("Latitude=20.934101, Longitude=85.262180");

    uint8_t buf[21];
    uint8_t buflen = sizeof(buf);
    // Check if received packet is correct size
    if (rf_driver.recv(buf, &buflen)){
      double near_lat ;
      double near_lon ;
      int acc ;
      Serial.print("Recieved:");
      Serial.println((char*)buf);
      char temp[strlen(buf) + 1];
      strcpy(temp, buf);
      char* token = strtok(temp, ",");
      near_lat = atof(token);
      token = strtok(NULL, ",");
      near_lon = atof(token);
      token = strtok(NULL, ",");
      if (token != NULL) {
        
      acc = atoi(token); // Assuming lastValue is an integer variable
      } else {
      acc = -1; // Indicate that there was no last value
      }
      Serial.println(acc);
      if(acc == 1){
        digitalWrite(6,HIGH);
      }else{
        digitalWrite(6,LOW);
      }
      double distance = calculateDistanceBetweenPoints(own_lat, own_lon, near_lat, near_lon);
      Serial.println(distance);    
      if(distance<10){
        digitalWrite(7,HIGH);
      }else{
        digitalWrite(7,LOW);
      }    
    }
    
  }
    
}
double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    // Convert latitude and longitude from degrees to radians
    lat1 = toRadians(lat1);
    lon1 = toRadians(lon1);
    lat2 = toRadians(lat2);
    lon2 = toRadians(lon2);

    // Haversine formula
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;
    double a = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double distance = RADIUS_OF_EARTH * c;

    return distance;
}
double toRadians(double degrees) {
    return degrees * M_PI / 180.0;
}
double calculateDistanceBetweenPoints(double lat1, double lon1, double lat2, double lon2) {
    return calculateDistance(lat1, lon1, lat2, lon2);
}
