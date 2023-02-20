#include "OneWire.h"
#include <DallasTemperature.h>
#include <Arduino.h>
#include <Wire.h>
#include <SCD30.h>
#include <BMx280I2C.h>

#define I2C_ADDRESS 0x76
#define ONE_WIRE_BUS 2

#if defined(ARDUINO_ARCH_AVR)
    #pragma message("Defined architecture for ARDUINO_ARCH_AVR.")
    #define SERIAL Serial
#elif defined(ARDUINO_ARCH_SAM)
    #pragma message("Defined architecture for ARDUINO_ARCH_SAM.")
    #define SERIAL SerialUSB
#elif defined(ARDUINO_ARCH_SAMD)
    #pragma message("Defined architecture for ARDUINO_ARCH_SAMD.")
    #define SERIAL SerialUSB
#elif defined(ARDUINO_ARCH_STM32F4)
    #pragma message("Defined architecture for ARDUINO_ARCH_STM32F4.")
    #define SERIAL SerialUSB
#else
    #pragma message("Not found any architecture.")
    #define SERIAL Serial
#endif

//Create bmp and temperature sensor instances.
BMx280I2C bmx280(I2C_ADDRESS);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//Variables
String co2, co2_temp, co2_humid, BMP_Temp, BMP_Humid, BMP_Press, DSB_Temp, BMP_Press_Norm;
String comma = ",";
String buffer = "0,0,0,0,0,0";
int i = 1000;

void setup() {
  // put your setup code here, to run once:
  sensors.begin();
	SERIAL.begin(9600);
  Serial1.begin(9600);
	//wait for serial connection to open (only necessary on some boards)
	while (!Serial);
  Wire.begin();
  scd30.initialize();
	//begin() checks the Interface, reads the sensor ID (to differentiate between BMP280 and BME280)
	//and reads compensation parameters.
	if (!bmx280.begin())
	{
		Serial.println("begin() failed. check your BMx280 Interface and I2C Address.");
		while (1);
	}
	if (bmx280.isBME280())
		//Serial.println("sensor is a BME280");
    i ++;
	else
		//Serial.println("sensor is a BMP280");
    i --;
	//reset sensor to default parameters.
	bmx280.resetToDefaults();
	//by default sensing is disabled and must be enabled by setting a non-zero
	//oversampling setting.
	//set an oversampling setting for pressure and temperature measurements. 
	bmx280.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
	bmx280.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);
	//if sensor is a BME280, set an oversampling setting for humidity measurements.
	if (bmx280.isBME280())
		bmx280.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);
}

void loop() {
  float result[3] = {0};
	delay(1000);
  sensors.requestTemperatures();
	//start a measurement
	if (!bmx280.measure())
	{
		Serial.println("could not start BMP measurement, is a measurement already running?");
		return;
	}
	//wait for the measurement to finish
	do
	{
		delay(100);
	} while (!bmx280.hasValue());
	//Serial.print("BMP Pressure: "); Serial.println(bmx280.getPressure());
	BMP_Temp = String(bmx280.getPressure64());
  BMP_Press = String(bmx280.getPressure64());
  DSB_Temp = String(sensors.getTempCByIndex(0));
  BMP_Press_Norm = bmx280.getPressure();  
  //Serial.print("BMP Pressure (64 bit): "); Serial.println(BMP_Press);
	//Serial.print("BMP Temperature: "); Serial.println(BMP_Temp);
  //Serial.print("DSB Temperature: "); Serial.println(DSB_Temp);
  //important: measurement data is read from the sensor in function hasValue() only. 
	//make sure to call get*() functions only after hasValue() has returned true. 
	if (bmx280.isBME280())
	{
		//Serial.print("BMP Humidity: "); 
    BMP_Humid = String(bmx280.getHumidity());
		//Serial.println(BMP_Humid);
	}
  if (scd30.isAvailable()) {
      scd30.getCarbonDioxideConcentration(result);
      //SERIAL.print("Carbon Dioxide Concentration is: ");
      co2 = String(result[0]);
      //SERIAL.print(co2);
      //SERIAL.println(" ppm");
      //SERIAL.print("SCD Temperature = ");
      co2_temp = String(result[1]);
      //SERIAL.print(co2_temp);
      //SERIAL.println(" ℃");
      //SERIAL.print("SCD Humidity = ");
      co2_humid = String(result[2]);
      //SERIAL.print(co2_humid);
      //SERIAL.println(" %");
      //SERIAL.println("");
  }
  //SERIAL.println("------------------------");
  String msg = "<" + co2 + comma+ co2 + comma + co2_temp + comma + co2_humid + comma + BMP_Temp + comma + BMP_Humid + comma + BMP_Press_Norm + comma +BMP_Press+ comma + BMP_Press_Norm + comma +DSB_Temp+ comma +DSB_Temp+ comma +DSB_Temp+ comma +DSB_Temp +">";
  String msgPy = co2 + comma+ co2 + comma + co2_temp + comma + co2_humid + comma + BMP_Temp + comma + BMP_Humid + comma + BMP_Press_Norm + comma +BMP_Press+ comma + BMP_Press_Norm + comma +DSB_Temp+ comma +DSB_Temp+ comma +DSB_Temp+ comma +DSB_Temp;
  //sprintf(buffer, "<%3d,%3d,%3d,%3d,%3d,%3d,%3d>" , co2, co2_temp, co2_humid, BMP_Temp, BMP_Humid, BMP_Press, DSB_Temp);
  Serial.println(msgPy);
  Serial1.println(msg);
  //Serial1.println("<29.38,201.45,301.55,401.32,501.34,601.56,701.55,801.88,901.98,0101.76,1111.54,1256.3434,1334.43>");
  //Serial1.println("<29.38,3.69,0.87,876.6,3.11,876.6,29.1,9,9,9,9,9,9>");
  //Serial1.println("<29.38,3.69,0.87,876.6,3.11,876.6,29.1>");
  //SERIAL.println("------------------------");

}
