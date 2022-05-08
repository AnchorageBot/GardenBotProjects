/* This sketch will use a Adafruit 32u4 board to gather, save, and broadcast temperature, humidity, & battery data
 *  
 *  Engineering
 *    Adafruit 
 *      https://learn.adafruit.com/adafruit-feather-32u4-bluefruit-le/power-management
 *      https://learn.adafruit.com/adafruit-adalogger-featherwing
 *    ATS, May 2022
 *      https://github.com/AnchorageBot
 *
 *  Software
 *    Arduino IDE, version 1.8.19
 *
 *  32u4 BLE (Bluefruit) Feather MCU                                
 *    https://www.adafruit.com/product/2829  
 *    32KB of flash = 28672 bytes
 *    Sketch uses 26272 bytes (91%) of program storage space
 *
 *  RTC/SD Card Shield/Adalogger Featherwing
 *    https://www.adafruit.com/product/2922
 *    
 *  CR1220 12mm Diameter - 3V Lithium Coin Cell Battery - CR1220    
 *    https://www.adafruit.com/product/380
 *   
 *  Terminal Block Breakout
 *    https://www.adafruit.com/product/2926
 *     
 *  DHT22 Air Temp & Humidity
 *    https://www.adafruit.com/product/393
 *    A5 to (OUT)
 *    VCC to (+)
 *    GND to (-)
 *
 *  500mAh Lipo Battery
 *    https://www.adafruit.com/product/1578
 *    
 *  3.5mm / 1.1mm to 5.5mm / 2.1mm DC Jack Adapter    
 *    https://www.adafruit.com/product/4287
 *
 *  Medium 6V 2W Solar panel - 2.0 Watt
 *    https://www.adafruit.com/product/200
 *
 *  JST-PH 2-pin Jumper Cable - 100mm long
 *    https://www.adafruit.com/product/4714
 *
 *  Adafruit Universal USB / DC / Solar Lithium Ion/Polymer charger - bq24074
 *    https://www.adafruit.com/product/4755
 *
 *  Large Plastic Project Enclosure - Weatherproof with Clear Top
 *    https://www.adafruit.com/product/905
 *
*/

// === Libraries ================================

#include <SPI.h>                     // Load Serial Peripheral Interface (SPI) library 
//#include <Wire.h>                    // Load I2/TWC library

#include <Adafruit_Sensor.h>         // Load Adafruit sensors library

#include <DHT.h>;                    // Load DHT22 libraries
#include <DHT_U.h> 

#include "RTClib.h"                  // Load PCF 8523 RTC library
#include <SD.h>                      // Load SD card library

#include <Adafruit_BLE.h>            // Load Adafruit bluetooth libraries
#include <Adafruit_BluefruitLE_SPI.h>
#include "Adafruit_BluefruitLE_UART.h"

// === Gobal constants and variables ============

String Plant = "AKBerry_Outside";     

#define VBATPIN A9                // Read this pin's voltage, and double it, to get battery level
float measuredvbat;               // Store battery measurement in volts            

int sensorTime = 10000;           // Calibration & output delay for all sensors, 60000 millisec = 60 sec

// Air temperature & humidity sensor DHT22 (AM2302)
#define DHTPIN A5                // DHT-22 sensor uses analog pin A5 to communicate data for 32u4, pin 
#define DHTTYPE DHT22            // DHT 22 sensor type (AM2302)
float hum;                       // Store humidity in percent
float tempC;                     // Store temperature in Celcius
float tempF;                     // Store temperature in Fahrenheit
DHT dht(DHTPIN, DHTTYPE);        // Initialize DHT sensor

// RTC/SD Card Shield / Adalogger
RTC_PCF8523 rtc;
char Days[7][12] = {"Sun", "Mon", "Tues", "Wed", "Thurs", "Fri", "Sat"};

#define cardSelect 10            // Adalogger uses pin 10 to communicate data with 32u4, pin 33 for ESP32      
File logfile;                    // Data object sensor data is written to

// BlueTooth (BLE) broadcasting/commo
String BROADCAST_NAME = Plant;  
String BROADCAST_CMD = String("AT+GAPDEVNAME=" + BROADCAST_NAME);
#define BLUEFRUIT_SPI_CS 8
#define BLUEFRUIT_SPI_IRQ 7
#define BLUEFRUIT_SPI_RST 4 
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);
extern uint8_t packetbuffer[];
char buf[60];


//=== Setup code here, runs once ================

void setup() 
{
  Serial.begin (9600);                                  // Setup the serial monitor for data analysis & debugging
  
  if (! rtc.begin()) {                                  // Initialize the RTC 
    Serial.println("RTC MIA");
    while (1);
  }

 if (! rtc.initialized()) {
    Serial.println("RTC KIA");
     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));    // line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(2021, 7, 6, 13, 40, 0));      // line sets the RTC with an explicit date & time, for example Jul 6 2021, 13:40pm
  }  

pinMode(10, OUTPUT);                                    // Adalogger reserves/uses 10 as an output  
  if (! SD.begin()) {                                   // Initialize the SD card (secure digital card)
    Serial.println("SD MIA");
    while (1);
  }                               

  pinMode(LED_BUILTIN, OUTPUT);                         // Setup onboard LED     

  dht.begin();                                          // Start DHT22 sensor

  ble.begin();                                          //  Set up bluetooth
  ble.echo(false);                                      //  Turn off echo
  ble.verbose(false);
  BROADCAST_CMD.toCharArray(buf, 60);
  ble.sendCommandCheckOK(buf);
  delay(500);
  ble.setMode(BLUEFRUIT_MODE_DATA);                     //  Set to data mode
  delay(500);  
}

//=== Main code, runs/loops repeatedly==========

void loop()
{  
  Battery();
  dht22();   
  SDcard();
  BlueTooth();
  SerialMonitor();
}

//===Sensor functions ===========================

void Blink() 
{
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                       // wait for a second
}

void Battery()  // battery voltage function
{
  measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;                // we divided by 2, so multiply back
  measuredvbat *= 3.3;              // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024;             // convert to voltage
}  

void dht22()                    // Air temp & humidity sensor function
{
    delay(sensorTime);  // Delay to stabilize sensor, optimize data output, minimize energy use 
   
    hum = dht.readHumidity();                         // Get Humidity value
    tempC= dht.readTemperature();                     // Get Temperature value in C
    tempF= (tempC*(1.8))+32;                    
}


void SDcard()          // SD card function 
{
  logfile = SD.open("BerryOut.txt", FILE_WRITE);        // open and name file for sensor data to be saved to the SD card
  
  logfile.print(Plant);
  logfile.print(',');
    
  DateTime now = rtc.now();                       // connect to RTC 
  logfile.print(now.year(), DEC);                 // write RTC data to SD card
  logfile.print(',');
  logfile.print(now.month(), DEC);
  logfile.print(',');
  logfile.print(now.day(), DEC);
  logfile.print(',');
  logfile.print(Days[now.dayOfTheWeek()]);
  logfile.print(',');
  logfile.print(now.hour(), DEC);
  logfile.print(':');
  logfile.print(now.minute(), DEC);
  logfile.print(':');
  logfile.print(now.second(), DEC);
  logfile.print(',');  

  dht22();                                    // connect to air temp and humidity sensor                   
  logfile.print("Hum %");
  logfile.print(',');                        
  logfile.print(hum);                          // write air humidity value to SD card
  logfile.print(',');   
  logfile.print("Tmp C");
  logfile.print(','); 
  logfile.print(tempC);                        // write air temp value in C to SD card
  logfile.print(',');
  logfile.print("Tmp F");
  logfile.print(',');    
  logfile.print(tempF);                       // write air temp value in F to SD card
  logfile.print("");               
  
  Battery();
  logfile.println("");  
  logfile.print("VBat: " );                  // write battery voltage to SD card
  logfile.println(measuredvbat);              
  logfile.println("");
  
  logfile.close();                           //  close the sensor data file
  
  //Blink();
}

void BlueTooth()
{
    ble.println(Plant); 
    ble.println();         
    ble.println(hum);                            
    ble.println("% humidity");
    ble.println(tempF);
    ble.println("temp F");
    ble.println(); 
    ble.print("Battery Voltage: " ); 
    ble.println(measuredvbat);
    ble.println();                      
}    

void SerialMonitor()
{

  Serial.print(" ");
  Serial.print(Plant);
  Serial.print(" , ");
  
  DateTime now = rtc.now();         // Send RTC (real time clock) data to serial monitor             
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(Days[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  
  dht22();                         // Send dht22 data to the serial monitor
  Serial.print("Humidity % ");
  Serial.println(hum);       
  Serial.print("  ");
  Serial.print("Temp °C ");
  Serial.println(tempC);   
  Serial.print("  ");
  Serial.print("Temp °F ");
  Serial.print(tempF); 
  Serial.print(" ");    
  
  Battery();                     // Send Battery voltage to the serial monitor
  Serial.print(" , ");
  Serial.print("Battery Voltage: " ); 
  Serial.println(measuredvbat);
  Serial.print(" ");

  //Blink();
}
