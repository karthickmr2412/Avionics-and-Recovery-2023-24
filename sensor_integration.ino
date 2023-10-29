//SDA pins are set to 21
//SCL pins are set to 22

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

  

//Moving average variables for bme
#define SEALEVELPRESSURE_HPA (1013.25)
#define WINDOW_SIZE 15
Adafruit_BME280 bme;
float alt;
int INDEX = 0;
float VALUE = 0;
float SUM = 0;
float READINGS[WINDOW_SIZE];
float Altitude;


// For bno 
Adafruit_BNO055 bno = Adafruit_BNO055(55);


void setup() {
	Serial.begin(9600);

	if (!bme.begin(0x76)) {
		Serial.println("Could not find a valid BME280 sensor, check wiring!");
		while (1);
	}
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  delay(1000);
    
  bno.setExtCrystalUse(true);

}
void loop() {
  // Altitde measurement 
  alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
  SUM = SUM - READINGS[INDEX];       
  VALUE = alt;     
  READINGS[INDEX] = VALUE;           
  SUM = SUM + VALUE;                 
  INDEX = (INDEX+1) % WINDOW_SIZE; 

  Altitude = SUM / WINDOW_SIZE;      

  Serial.print(VALUE);
  Serial.print(",");
  Serial.println(Altitude);

  // Bno measurements 
  imu::Vector<3> angular_vel = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
  Serial.print(" X: ");
  Serial.print(angular_vel.x(), 4);
  Serial.print(" Y: ");
  Serial.print(angular_vel.y(), 4);
  Serial.print(" Z: ");
  Serial.print(angular_vel.z(), 4);
  Serial.println("");

	delay(10);
}
