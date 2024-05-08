#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
Adafruit_MPU6050 mpu;
#define GPS_TX_PIN 2
#define GPS_RX_PIN 3
#include <RH_ASK.h>
// Include dependant SPI Library 
#include <SPI.h> 
 
// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

TinyGPSPlus gps;
SoftwareSerial mygps(GPS_TX_PIN, GPS_RX_PIN);

void setup() {
  
  mygps.begin(9600);
  rf_driver.init();
  //accl
 Serial.begin(9600);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    
    break;
  case MPU6050_BAND_184_HZ:
   
    break;
  case MPU6050_BAND_94_HZ:
   
    break;
  case MPU6050_BAND_44_HZ:
   
    break;
  case MPU6050_BAND_21_HZ:
   
    break;
  case MPU6050_BAND_10_HZ:
   
    break;
  case MPU6050_BAND_5_HZ:
    
    break;
  }

  
  delay(100);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  // Serial.print("Acceleration X: ");
  // Serial.print(a.acceleration.x);
  // Serial.print(", Y: ");
  // Serial.print(a.acceleration.y);
  // Serial.print(", Z: ");
  // Serial.print(a.acceleration.z);
  // Serial.println(" m/s^2");
  
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
      String dataString = latitudeStr + "," + longitudeStr;
    int acc = abs(a.acceleration.x) + abs(a.acceleration.y) + abs(a.acceleration.z);
    Serial.println(acc);
    String suddenBreak;
    if(acc>13){
      suddenBreak = "1";
    }else{
      suddenBreak = "0";
    }
   // String defaultData = "20.934062,85.262172";
    dataString += ",";  // Add a comma separator
    dataString += suddenBreak;
      // Transmit the data
      transmitData(dataString);

      // Print to Serial Monitor
      Serial.println(dataString);

      // Print log
      Serial.println("GPS coordinates sent successfully.");
    }
  }

  // If GPS data is not available, use default values
  if (!gps.location.isValid()) {
    int acc = abs(a.acceleration.x) + abs(a.acceleration.y) + abs(a.acceleration.z);
    Serial.println(acc);
    String suddenBreak;
    if(acc>13){
      suddenBreak = "1";
    }else{
      suddenBreak = "0";
    }
    String defaultData = "20.934062,85.262172";
    defaultData += ",";  // Add a comma separator
    defaultData += suddenBreak;  // Append the value of suddenBreak
   
    Serial.println(defaultData);
    transmitData(defaultData);
  }

  delay(100); // Delay to prevent continuous printing
}

void transmitData(String vehicleData) {
    const char *msg = vehicleData.c_str();
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
    Serial.println("Send");
    
}


