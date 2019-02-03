/****************************************************
Measure & report soil moisture, air humidty, air temperature, and light intensity, using a LCD shield, serial port, & RPi
Use sensor data to water & light a single plant

  Materials Schedule
 
   Arduino Uno - 1 each
    https://store.arduino.cc/usa/arduino-uno-rev3
   DF Robot Gravity 1602 LCD Shield - 1 each 
    https://www.dfrobot.com/product-1009.html
   DF Robot SEN0193 Capacitive Soil Moisture Sensor - 1 each
    https://www.dfrobot.com/product-1385.html
   OSEPP Photoelectric/Photocell Sensor - 1 each
    https://www.osepp.com/electronic-modules/sensor-modules/69-light-sensor-module 
   DHT 22/AM2302 Temperature and Humidity Sensor - 1 each
    https://www.adafruit.com/product/393
   Power Supply for Arduino 9V/1000mA - 1 each
    https://www.adafruit.com/product/63
   A-B Cable - 1 each
    https://www.staples.com/usb+ab+cable/directory_usb+ab+cable 
   Wire Bundles - 6 female/female wires 
    https://www.adafruit.com/product/266
   Wire Bundle - 4 male/female wires
    https://www.adafruit.com/product/1954
   DLI IoT Relay - 2 each
    http://www.digital-loggers.com/iot2faqs.html

  Assembly
   1.Connect A-B cable to arduino and laptop and upload sketch
   2.Disconnect A-B cable   
   3.Connect aurduino and LCD shield (shield uses D4, D5, D6, D7, D8, D9, D10, and A0 pins)
   4.Connect soil moisture sensor to LCD shield analog pin 1 with wire (jst/female)
   5.Connect photocell sensor to LCD shield analog pin 2 with (3) female/female wires

      (D) Digital D4 White 
      (+) Voltage VCC Red
      (-) Ground  GND Black
   
   6.Connect temp & humidty sensor (DHT) to LCD shield digital pin 3 with (A3) (3) female/female wires

      (D) Digital D3  Yellow  Out
      (+) Voltage VCC Blue    VCC
      (-) Ground  GND Green   GND  

   7.Connect IoT (pump) relay connector (+) to LCD shield digital pin 12 and (-) GND with (2) male/female wires
   
      (D) Digital    D12     Red Wire     to IoT relay (+)
      (+) Voltage    VCC                 
      (-)  Ground    GND     Black Wire   to IoT relay (-)

   8.Connect IoT (light) relay connector (+) to LCD shield digital pin 13 and (-) GND with (2) male/female wires
   
      (D) Digital    D13     Red Wire     to IoT relay (+)
      (+) Voltage    VCC                 
      (-)  Ground    GND     Black Wire   to IoT relay (-)      
      
    9.Plug 9V power supply for arduino into relay outlet labeled "always on" and plug into arduino 9V 
   10.Plug submersible pump into IoT relay outlet labeled "normally off"
   11.Plug plant light into IoT relay outlet labeled "normally on"
   12.Plug IoT relays into mains outlets

  Engineering References 
  
    Paul McWhorter, DF Robot, DroneBotWorkshop, HowToMechatronics, & DLI 

*****************************************************/
 
// === Libraries ================================

// Include Adafruit_Sensors (DHT 22) Library
#include "DHT.h";

// Include LCD 1602 shield library
#include <LiquidCrystal.h>
 
// Initialize the LCD library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 

// === Gobal constants and variables ============

// Soil moisture sensor variables and data pin
  const int AirValue = 520;   // Adjust max AirValue as neccessary
  const int WaterValue = 260;  // Adjust min WaterValue as neccessary
  int intervals = (AirValue - WaterValue)/3;   
  int soilMoistureValue = 1; // Analog pin 1

// Soil moisture sensor string indicators
  String thirsty = "ThirstyPlant ";
  String happy = "HappyPlant ";
  String soaked = "SoakedPlant ";

// Relay control pins for turning on/off the submersible pump and the plant lights
  int relayPump = 12; // Digital pin 12
  int relayLight = 13; // Digital pin 13

// Temp & Humidity Sensor Constants
  #define DHTPIN (A3)      // DHT-22 sensor & I/O shield analog pin 3 treated as a digital pin
  #define DHTTYPE DHT22   // DHT 22 sensor type is AM2302

// Temp & Humidity Sensor Variables
  float hum;    // Stores humidity value in percent
  float temp;   // Stores temperature value in Celcius
  int iterations = 5;

// Initialize Temp (DHT) sensor for normal 16mhz Arduino
  DHT dht(DHTPIN, DHTTYPE); 

// Photocell Sensor is connected to analog pin 2 for data collection
  const int photoCell = 2;

// Chronometer variables
  // Arduino Uno (ATmega328P) max int size is 32,767
  // byte = range of 0 to 255
  // int = 16 bit = 2 byte value = range of -32,768 to 32,767
  // unsigned int = 32 bit = 4 bytes = 4,294,967,295
  // 1,000 miliseconds = 1 sec = 0.017 min
  // 30,000 miliseconds = 30 sec = 0.5 min
  // 43,200,000 miliseconds = 12 hrs, X milliseconds = (Y hours)(3.6e+6)

  int timerLED = 500;
  int timerPrint = 1000;
  int timerSensor = 2000;

  unsigned long startMillis;
  unsigned long currentMillis;
  const unsigned long period = 1000; 
  const unsigned long alaskaDay = 1000;  

//=== Setup code here, runs once =================

void setup() 
{

  // Setup onboard LED (Arduino Uno digital pin 13)
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED on 

  // Set up the relay for powering the submersible pump
  pinMode(relayPump, OUTPUT);
  digitalWrite(relayPump, HIGH); // turn the relay on 

  // Set up the relay for powering the plant light
  pinMode(relayLight, OUTPUT);
  digitalWrite(relayLight, HIGH); // turn the relay on   

  // Setup photocell sensor as an input
  pinMode(photoCell, INPUT);

  // Start the Temperature & Humidity (DHT22) sensor
  dht.begin();  

  // Timer start time
  startMillis = millis();    
  
  // Setup the LCD screen's number of columns and rows:
  lcd.begin(16, 2);

  // Setup the serial monitor for data analysis & debugging
  Serial.begin (9600);
  
}

//=== Main code here, runs/loops repeatedly=======

void loop() 
{

  soilMoisture();
  
}

//===Breakout of sensor functions==================

void blink()  // Blink the onboard LED
{
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on 
  delay(timerLED);                       
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off 
  delay(timerLED);                       
}

void multipleBlink() // Blink the onboard LED multiple times
{
  int startBlink = 0;
  int finishBlink = 4;
  while (startBlink < finishBlink)
  {
    blink();
  } 
}

void photocellReading() // Photocell Sensor Reading
{
  int readPhotocell = analogRead(photoCell);

    if (readPhotocell < 78) // (10,000 lux)(0.00775) = 78 = Full Daylight Reading
    {
      // Show data on LCD shield screen
      lcd.setCursor(0, 0);
      lcd.print("ShadyPlant ");
      lcd.println(readPhotocell);
      lcd.println("");
      delay(timerPrint);             

      // Show data on serial port (laptop screen)
      Serial.print("Shady Plant: ");
      Serial.print(readPhotocell);
      Serial.print("\n\n");
      delay(timerPrint);       
    }

    else
    {
      // Show data on LCD shield screen
      lcd.setCursor(0, 0);
      lcd.print("SunnyPlant ");   
      lcd.println(readPhotocell);
      lcd.println("");
      delay(timerPrint);
       
      // Show data on serial port (laptop screen)        
      Serial.print("Sunny Plant: ");
      Serial.print(readPhotocell);
      Serial.print("\n\n");    
      delay(timerPrint);         
    }     
}

void tempHumidity()  // Air Temperature & Humidity Sensor
{
  delay(timerSensor);  // Delay so DHT-22 sensor can stabilize
   
  hum = dht.readHumidity();  // Get Air Humidity value
  temp= dht.readTemperature();  // Get Air Temperature value

  // Show data on LCD shield screen 
  lcd.setCursor(0, 0);
  lcd.print("% Humid ");
  lcd.println(hum);
  lcd.println("");
  delay(timerPrint);   
    
  // Show data on serial port (laptop screen)   
  Serial.print("% Humidity ");
  Serial.println(hum);
  delay(timerPrint); 

  // Show data on LCD shield screen     
  lcd.setCursor(0, 0);
  lcd.print("Temp C ");
  lcd.println(temp);

  // Show data on serial port (laptop screen)       
  Serial.print("Temp in C ");
  Serial.println(temp);
  delay(timerPrint); 
}

void soilMoisture() // Soil moisture sensor and water pump relay function 
{
  // operate soil moisture sensor 
  soilMoistureValue = analogRead(1);
 
  // report soil moisture sensor values 
  String thirstySensor = thirsty + soilMoistureValue;
  String happySensor = happy + soilMoistureValue;
  String soakedSensor = soaked + soilMoistureValue;
 
  // plant is thirsty and needs more water
  if(soilMoistureValue < AirValue && soilMoistureValue > (AirValue - intervals))
  {
   
    // Turn water pump on and off
    digitalWrite(relayPump, LOW);
    delay(timerPrint);
    digitalWrite(relayPump, HIGH);
    delay(timerPrint);
    digitalWrite(relayPump, LOW);
    lcd.setCursor(0, 0);
    lcd.println("PumpingWater");
    lcd.println("");      
    Serial.println("Pumping Water");
    Serial.println("");
    delay(timerPrint);

    //Blink the onboard LED
    blink();
    blink();
    blink();
   //multipleBlink();

    // Show data on the LCD shield screen   
    lcd.setCursor(0, 0);
    lcd.println(thirstySensor);
    lcd.println("");
    delay(timerPrint);   
   
    // Show data on the serial port (laptop screen)
    Serial.print(thirstySensor);
    Serial.println("");
    delay(timerPrint);      
  }

  // plant is happy and has enough water
  else if(soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (AirValue - intervals))
  {
    // Show data on LCD shield screen
    lcd.setCursor(0, 0);
    lcd.println(happySensor);
    lcd.println("");
    delay(timerPrint);
   
    // Show data on the serial port (laptop screen)
    Serial.print(happySensor);
    Serial.println("");
    delay(timerPrint);    
   }

  // plant is soaked
  else if(soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals))
  {
    // Show data on LCD shield screen   
    lcd.setCursor(0, 0);
    lcd.println(soakedSensor);
    lcd.println("");
    delay(timerPrint);   
   
    // Show data on the serial port (laptop screen)
    Serial.print(soakedSensor);
    Serial.println("");
    delay(timerPrint);    
  }

  // Check Photocell sensor
  photocellReading();
   
  // Check Temp & Humidity Sensor
  tempHumidity(); 

  delay(timerSensor);
}

void chronometer() // Time and turn on/off light relay
{ 
  currentMillis = millis();  // the number of milliseconds since the start of the program

  // Show timer data on LCD shield screen
  lcd.setCursor(0, 0);
  lcd.print("LightTimer ");   
  lcd.println(currentMillis);
  delay(timerPrint);

  // Show timer data on serial port (laptop screen)  
  Serial.print("LightTimer ");
  Serial.println(currentMillis);
  delay(timerPrint);  
  
  if (currentMillis - startMillis >= alaskaDay)  // test whether an Alaskan Day (12 hrs) has elapsed
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  // if so, change the state of the LED
    digitalWrite(relayLight, !digitalRead(relayLight));  // if so, change the state of the relay
    startMillis = currentMillis;  // save the start time of the current state
  }
}  

void shareDataWithRPi ()
{

}
