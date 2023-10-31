//SDA pins are set to 21
//SCL pins are set to 22

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
volatile float minutes, seconds;
volatile int degree, secs, mins;
  
SoftwareSerial GPS_SoftSerial(16, 17); //16 and 17 are GPS pins
//Moving average variables for bme
TinyGPSPlus gps;		
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
  GPS_SoftSerial.begin(9600);

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
void loop() 
{ smartDelay(100);
  unsigned long start;
  double lat_val, lng_val, alt_m_val;
  bool loc_valid, alt_valid, time_valid;
  lat_val = gps.location.lat();	/* Get latitude data */
  loc_valid = gps.location.isValid();	/* Check if valid location data is available */
  lng_val = gps.location.lng();	/* Get longtitude data */
  
  DegMinSec(lat_val);
  Serial.print("Latitude in Decimal Degrees : ");
  Serial.println(lat_val, 6);
  Serial.print("Latitude in Degrees Minutes Seconds : ");
  Serial.print(degree);
  Serial.print("\t");
  Serial.print(mins);
  Serial.print("\t");
  Serial.println(secs);
  DegMinSec(lng_val);	/* Convert the decimal degree value into degrees minutes seconds form */
  Serial.print("Longitude in Decimal Degrees : ");
  Serial.println(lng_val, 6);
  Serial.print("Longitude in Degrees Minutes Seconds : ");
  Serial.print(degree);
  Serial.print("\t");
  Serial.print(mins);
  Serial.print("\t");
  Serial.println(secs);
  
        

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

}
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (GPS_SoftSerial.available())	/* Encode data read from GPS while data is available on serial port */
      gps.encode(GPS_SoftSerial.read());
/* Encode basically is used to parse the string received by the GPS and to store it in a buffer so that information can be extracted from it */
  } while (millis() - start < ms);
}

void DegMinSec(double tot_val)		/* Convert data in decimal degrees into degrees minutes seconds form */
{  
  degree = (int)tot_val;
  minutes = tot_val - degree;
  seconds = 60 * minutes;
  minutes = (int)seconds;
  mins = (int)minutes;
  seconds = seconds - minutes;
  seconds = 60 * seconds;
  secs = (int)seconds;
  // Altitde measurement 
}
