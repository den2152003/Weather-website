/*----------Blynk--------------*/
#include <HTTPClient.h>

#define BLYNK_TEMPLATE_ID "TMPL6iwY2LzAe"
#define BLYNK_TEMPLATE_NAME "DoAn1"
#define BLYNK_AUTH_TOKEN "xUtOb2-loCfapgfClZ2vTUmXKU3wPdiM"

// #define BLYNK_TEMPLATE_ID "TMPL6AXPFrl3L"
// #define BLYNK_TEMPLATE_NAME "WeatherStation"
// #define BLYNK_AUTH_TOKEN "EUVAxKTbmyodBkrv_adLMviI2umTqb_U"

#define BLYNK_PRINT Serial
#include <BlynkRpcClient.h>
#include <BlynkSimpleEsp32.h> // thêm thư viện Blynk

/*-----------RTC--------------*/
#include "RTClib.h"
/*------library oled----------*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
/*-----------DHT--------------*/
#include <DHT.h>
/*-----------wifi--------------*/
#include <WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h> 
#include <WiFiUdp.h>
/*-----------servo--------------*/
#include <ESP32Servo.h>

/*----------SERVO----------------*/
static const int servoPin = 4;
Servo servo1;
/*----------BUZZER----------------*/
#define Buzzer 12
#define NOTE_FS5 350
#define NOTE_REST 0

/*----------DHT----------------*/
#define DHTPIN 14  
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

/*----------Blynk--------------*/
BlynkTimer timer; 

/*----------------oled-------------------*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire, -1);

/*----------------RTC-------------------*/
RTC_DS1307 rtc;
DateTime now;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

/*--------------wifi-------------------*/
char auth [] = BLYNK_AUTH_TOKEN;
const char* ssid     = "Tuhehe";                 // SSID of local network
const char* password = "78787879";                    // Password on network
// const char* ssid     = "Kha";                 // SSID of local network
// const char* password = "0123456789";   
String APIKEY = "a652c91a816d969a67f72bbfec244d4a";
String CityID = "1566083";          //Your City ID
bool id = false;
WiFiClient client;
char servername[] = "api.openweathermap.org";            // remote server we will connect to
String result;

/*----------FireBase----------------*/

const char* FIREBASE_HOST ="doan1-86e87-default-rtdb.firebaseio.com";
const char* FIREBASE_AUTH="IC4lq3X7kvgEzeeReQPHFDFYoTa2";
const char* databaseURL1="https://doan1-86e87-default-rtdb.firebaseio.com/Weather.json";
const char* databaseURL2="https://doan1-86e87-default-rtdb.firebaseio.com/Device/CheMua.json";

// const char* FIREBASE_HOST ="hw-sw2024-default-rtdb.firebaseio.com";
// const char* FIREBASE_AUTH="ZJn6SeyCfRYD38Lxi6fma4lldyh2";
// const char* databaseURL1="https://hw-sw2024-default-rtdb.firebaseio.com/Weather.json";
// const char* databaseURL2="https://hw-sw2024-default-rtdb.firebaseio.com/Device/CheMua.json";


int cnt_con = 0;

const unsigned char Process1_Sym [] PROGMEM = {
  0x00, 0x18, 0x00, 0x01, 0x99, 0x80, 0x01, 0x81, 0x80, 0x08, 0x00, 0x10, 0x18, 0x00, 0x18, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x06, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xc0, 0x00, 0x03, 0xc0, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x06, 
  0x60, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x18, 0x08, 0x00, 0x10, 0x01, 
  0x81, 0x80, 0x01, 0x99, 0x80, 0x00, 0x18, 0x00
};

// 'Process2_Sym', 8x8px
const unsigned char Process2_Sym [] PROGMEM = {
  0x00, 0x66, 0x00, 0x00, 0x66, 0x00, 0x06, 0x00, 0x60, 0x06, 0x00, 0x60, 0x00, 0x00, 0x00, 0x30, 
  0x00, 0x0c, 0x30, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x03, 0xc0, 0x00, 
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x03, 0xc0, 0x00, 0x03, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x30, 0x00, 0x0c, 0x30, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x06, 0x00, 0x60, 0x06, 
  0x00, 0x60, 0x00, 0x66, 0x00, 0x00, 0x66, 0x00
};

// 'Clear_Daylight_Sym', 24x24px
const unsigned char Clear_Daylight_Sym [] PROGMEM = {
  0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x10, 0x18, 0x08, 0x38, 0x00, 0x1c, 0x1c, 0x3c, 0x38, 0x08, 
  0xff, 0x10, 0x01, 0xc1, 0x80, 0x03, 0x00, 0xc0, 0x06, 0x00, 0x60, 0x06, 0x00, 0x60, 0x0c, 0x00, 
  0x30, 0xec, 0x00, 0x37, 0xec, 0x00, 0x37, 0x0c, 0x00, 0x30, 0x06, 0x00, 0x60, 0x06, 0x00, 0x60, 
  0x03, 0x00, 0xc0, 0x01, 0xc3, 0x80, 0x08, 0xff, 0x10, 0x1c, 0x3c, 0x38, 0x38, 0x00, 0x1c, 0x10, 
  0x18, 0x08, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00
};

// 'Clear_Night_Sym', 24x24px
const unsigned char Clear_Night_Sym [] PROGMEM = {
  0x08, 0x08, 0x80, 0x08, 0x1c, 0xc0, 0x1c, 0x08, 0xe0, 0x3e, 0x00, 0xf0, 0xff, 0x80, 0xd8, 0x3e, 
  0x00, 0xcc, 0x1c, 0x00, 0xc6, 0x08, 0x01, 0x86, 0x08, 0x01, 0x83, 0x00, 0x03, 0x03, 0x00, 0x03, 
  0x03, 0x40, 0x06, 0x03, 0xe0, 0x06, 0x03, 0x40, 0x1c, 0x03, 0x00, 0x78, 0x03, 0x01, 0xe0, 0x03, 
  0xff, 0x80, 0x06, 0x7e, 0x00, 0x06, 0x30, 0x00, 0x0c, 0x18, 0x00, 0x18, 0x0c, 0x00, 0x30, 0x07, 
  0x00, 0xe0, 0x03, 0xff, 0xc0, 0x00, 0xff, 0x00
};

// 'Rain', 24x24px
const unsigned char Rain_Sym [] PROGMEM = {
  0x01, 0xf0, 0x00, 0x03, 0xf8, 0x70, 0x06, 0x0c, 0xf8, 0x1c, 0x07, 0x8c, 0x38, 0x03, 0x06, 0x60,
  0x00, 0x03, 0xc0, 0x00, 0x03, 0xc0, 0x00, 0x03, 0xc0, 0x00, 0x03, 0xe0, 0x00, 0x07, 0x7f, 0xff, 
  0xfe, 0x3f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x84, 0x21, 0x21, 0x08, 0x42, 
  0x42, 0x10, 0x84, 0x84, 0x21, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x84, 0x21, 0x21, 
  0x08, 0x42, 0x42, 0x10, 0x84, 0x84, 0x21, 0x08
};

// 'Clouds_Daylight', 24x24px
const unsigned char Clouds_Daylight_Sym [] PROGMEM = {
  0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x10, 0x18, 0x08, 0x38, 0x00, 0x1c, 0x1c, 0x3c, 0x38, 0x08, 
  0xff, 0x10, 0x01, 0xc3, 0x80, 0x03, 0x00, 0xc0, 0x06, 0x00, 0x60, 0x06, 0x00, 0x60, 0x0c, 0x00, 
  0x30, 0xec, 0x00, 0x37, 0xed, 0xf0, 0x37, 0x0f, 0xf8, 0x70, 0x06, 0x0c, 0xf8, 0x1c, 0x07, 0x8c, 
  0x38, 0x03, 0x06, 0x60, 0x00, 0x03, 0xc0, 0x00, 0x03, 0xc0, 0x00, 0x03, 0xc0, 0x00, 0x03, 0xe0, 
  0x00, 0x07, 0x7f, 0xff, 0xfe, 0x3f, 0xff, 0xfc
};

// 'Clouds_Night_Sym', 24x24px
const unsigned char Clouds_Night_Sym [] PROGMEM = {
  0x00, 0x00, 0x02, 0x00, 0x01, 0x07, 0x04, 0x01, 0xc2, 0x04, 0x01, 0xe0, 0x0e, 0x01, 0xb0, 0x3f, 
  0x81, 0x98, 0x0e, 0x01, 0x8c, 0x04, 0x03, 0x0c, 0x04, 0x03, 0x06, 0x00, 0x03, 0x06, 0x00, 0x06, 
  0x03, 0x00, 0x06, 0x03, 0x00, 0x0c, 0x03, 0x00, 0x38, 0x03, 0x01, 0xf0, 0x06, 0x7f, 0xc0, 0x06, 
  0x3e, 0x00, 0x0c, 0x30, 0x00, 0x0c, 0x18, 0x00, 0x18, 0x0c, 0x00, 0x30, 0x07, 0x00, 0xe0, 0x43, 
  0xc3, 0xc2, 0xe0, 0xff, 0x07, 0x40, 0x3c, 0x02
};

/*-------------PIN--------------*/
#define mq135_pin 32
#define LDR 34
int rainPin = 35;
int thresholdValue = 2700;
#define ON_Board_LED 2 


int airqlty = 0;
int analogValue;
int mode = 1;
const float GAMMA = 0.7;
const float RL10 = 50;

/*----------khaibao_oled------------*/
void khaibao_oled(){
  delay(250);
  display.begin(i2c_Address, true);
  display.display();
  vTaskDelay(pdMS_TO_TICKS(2000));
  display.clearDisplay();
}

/*----------khaibao_rtc------------*/
void khaibao_rtc(){
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) 
    vTaskDelay(pdMS_TO_TICKS(10));
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0)); cau hinh thu cong
  }
}

/*-----------connect----------------*/
void Connect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Displays the text and icon of the connection process on OLED
    digitalWrite(ON_Board_LED, LOW); //--> Make the On Board Flashing LED on the process of connecting to the wifi router.

    display.clearDisplay(); //--> for Clearing the display
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(20, 15);
    display.println("Connect to WiFi");
    display.drawBitmap(52, 25, Process1_Sym, 24, 24, SH110X_WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color);
    display.drawRect(0, 0, 128, 64, SH110X_WHITE); //--> Display frame
    display.display();
    delay(250);
    
    digitalWrite(ON_Board_LED, HIGH); //--> Make the Off Board Flashing LED on the process of connecting to the wifi router.
    
    display.clearDisplay(); //--> for Clearing the display
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(20, 15);
    display.println("Connect to WiFi");
    display.drawBitmap(52, 25, Process2_Sym, 24, 24, SH110X_WHITE); //--> display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color);
    display.drawRect(0, 0, 128, 64, SH110X_WHITE); //--> Display frame
    display.display();
    delay(250);

    cnt_con++;
    if(cnt_con > 59) {
      cnt_con = 0;
      Serial.println("Failed to connect !");
      display.clearDisplay(); //--> for Clearing the display
      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(0, 28);
      display.println(" Failed to connect !");
      display.drawRect(0, 0, 128, 64, SH110X_WHITE); //--> Display frame
      display.display();
      vTaskDelay(pdMS_TO_TICKS(5000));
      Waiting_to_be_connected_again();
    }
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  //----------------------------------------If successfully connected to the wifi router, the IP Address that will be visited is displayed in the serial monitor
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  display.clearDisplay(); //--> for Clearing the display
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 24);
  display.println("     Connection");
  display.setCursor(0, 33);
  display.println("     Successful");
  display.drawRect(0, 0, 128, 64, SH110X_WHITE); //--> Display frame
  display.display();
  vTaskDelay(pdMS_TO_TICKS(2000));
  //----------------------------------------
}

/*---------connect again----------------*/
void Waiting_to_be_connected_again() {
  for(int i = 5; i > -1; i--) {
    Serial.print("Will be connected again:");
    Serial.println(i);
    display.clearDisplay(); //--> for Clearing the display
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 24);
    display.println("       Will be");
    display.setCursor(0, 33);
    display.print("  connected again:");
    display.println(i);
    display.drawRect(0, 0, 128, 64, SH110X_WHITE); //--> Display frame
    display.display();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  Connect();
}

void setup() {
  Serial.begin(9600);
  servo1.attach(servoPin);
  khaibao_oled();
  khaibao_rtc();
  pinMode(mq135_pin, INPUT);
  pinMode(LDR, INPUT);
  pinMode(rainPin, INPUT);
  pinMode(Buzzer, OUTPUT);
  
  Connect();
  //timer.setInterval(1000, sendSensor);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  display.clearDisplay(); //--> for Clearing the display
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(3, 19);
  display.println("  Get weather data");
  display.setCursor(3, 28);
  display.println("        from");
  display.setCursor(3, 37);
  display.println("   openweathermap");
  display.drawRect(0, 0, 128, 64, SH110X_WHITE); //--> Display frame
  display.display();

  xTaskCreate(displayOled,"1",20000,NULL,2,NULL);
  xTaskCreate(displayLocation,"2",20000,NULL,2,NULL);
  xTaskCreate(sendSensor,"3",20000,NULL,2,NULL);
  

  vTaskDelay(pdMS_TO_TICKS(1000));
  
}

void loop()                    
{ 
}

void displayOled(void*)
{
  while (1)
  {
    DateTime now = rtc.now();
    int sensorValue = map(analogRead(rainPin), 0, 4095, 100, 0);
    airqlty = map(analogRead(mq135_pin), 0, 4095, 0, 100);
    int analogValue = map(analogRead(LDR), 0, 4095, 100, 0);
    display.clearDisplay(); //--> for Clearing the display
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.drawRect(0, 0, 128, 64, SH110X_WHITE); //--> Display frame
    display.drawRect(0, 50, 128, 64, SH110X_WHITE); //--> Display frame
    display.setCursor(4, 36);

    display.print("T:" + String(dht.readTemperature(), 0));
    display.print((char)247);
    display.print("C");

    display.setCursor(50, 16);
    display.print("H:" + String(dht.readHumidity(), 0));
    display.print("%");

    display.setCursor(50, 26);
    display.print("Air:");
    if (airqlty <= 25)
      display.print("GOOD!");
    else  
      display.print("POOR");


    display.setCursor(6, 53);
    display.print(now.year(), DEC);
    display.print('/');
    display.print(now.month(), DEC);
    display.print('/');
    display.print(now.day(), DEC);

    display.setCursor(72, 53);
    display.print(now.hour(), DEC);
    display.print(':');
    display.print(now.minute(), DEC);
    display.print(':');
    display.println(now.second(), DEC);

    display.setCursor(50, 36);
    display.print(daysOfTheWeek[now.dayOfTheWeek()]);

    if (sensorValue > 30){
      display.drawBitmap(10, 5, Rain_Sym, 24, 24, SH110X_WHITE);
      display.setCursor(50, 6);
      display.print("Rain");

      display.display();
      delay(250);

    } else if ((now.hour() >= 18 && now.hour() < 24) || (now.hour() >=0 && now.hour() <= 3))
    {
      display.drawBitmap(10, 5, Clear_Night_Sym, 24, 24, SH110X_WHITE);
      display.setCursor(50, 6);
      display.print("Tonight");
      display.display();
      delay(250);
  } else if (now.hour() >3 && now.hour() <= 10 && analogValue > 10){
      display.drawBitmap(10, 5, Clear_Daylight_Sym, 24, 24, SH110X_WHITE);
      display.setCursor(50, 6);
      display.print("Morning");
      display.display();
      delay(250);
  } else if (now.hour() >10 && now.hour() <= 13 && analogValue > 10){
      display.drawBitmap(10, 5, Clear_Daylight_Sym, 24, 24, SH110X_WHITE);
      display.setCursor(50, 6);
      display.print("Noon");
      display.display();
      delay(250);
  } else if (now.hour() >13 && now.hour() <= 18 && analogValue > 10){
      display.drawBitmap(10, 5, Clear_Daylight_Sym, 24, 24, SH110X_WHITE);
      display.setCursor(50, 6);
      display.print("Afternoon");
      display.display();
      delay(250);
    } else if (now.hour() >3 && now.hour() <= 10 && analogValue < 10){
      display.drawBitmap(10, 5, Clouds_Daylight_Sym, 24, 24, SH110X_WHITE);
      display.setCursor(50, 6);
      display.print("Morning");
      display.display();
      delay(250);
    } else if (now.hour() >10 && now.hour() <= 13 && analogValue < 10 ){
      display.drawBitmap(10, 5, Clouds_Daylight_Sym, 24, 24, SH110X_WHITE);
      display.setCursor(50, 6);
      display.print("Noon");
      display.display();
      delay(250);
  } else if (now.hour() >13 && now.hour() <= 18 && analogValue < 10){
      display.drawBitmap(10, 5, Clouds_Daylight_Sym, 24, 24, SH110X_WHITE);
      display.setCursor(50, 6);
      display.print("Afternoon");
      display.display();
      delay(250);
    } 
  
    vTaskDelay(pdMS_TO_TICKS(5000));
    }
  
}

void displayLocation(void*)
{
  while (1)
  {
  if (client.connect(servername, 80))
  { //starts client connection, checks for connection
    client.println("GET /data/2.5/weather?id=" + CityID + "&units=metric&APPID=" + APIKEY);
    client.println("Host: api.openweathermap.org");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("connection failed");        //error message if no client connect
    Serial.println();
  }

  while (client.connected() && !client.available())
    delay(1);                                          //waits for data
  while (client.connected() || client.available())
  { //connected or data available
    char c = client.read();                     //gets byte from ethernet buffer
    result = result + c;
  }
  client.stop();                                      //stop client
  result.replace('[', ' ');
  result.replace(']', ' ');
  //Serial.println(result);
  char jsonArray [result.length() + 1];
  result.toCharArray(jsonArray, sizeof(jsonArray));
  jsonArray[result.length() + 1] = '\0';
  StaticJsonDocument<1024> doc;
  DeserializationError  error = deserializeJson(doc, jsonArray);

  if (error) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.c_str());
    return;
  }
  String location = doc["name"];
  String country = doc["sys"]["country"];
  int temperature = doc["main"]["temp"];
  int humidity = doc["main"]["humidity"];
  float pressure = doc["main"]["pressure"];
  int id = doc["id"];
  float Speed = doc["wind"]["speed"];
  int degree = doc["wind"]["deg"];
  float longitude = doc["coord"]["lon"];
  float latitude = doc["coord"]["lat"];
  
  Serial.println();
  Serial.print("Country: ");
  Serial.println(country);
  Serial.print("Location: ");
  Serial.println(location);
  Serial.print("Location ID: ");
  Serial.println(id);
  Serial.printf("Temperature: %d°C\r\n", temperature);
  Serial.printf("Humidity: %d %%\r\n", humidity);
  Serial.printf("Pressure: %.2f hpa\r\n", pressure);
  Serial.printf("Wind speed: %.1f m/s\r\n", Speed);
  Serial.printf("Wind degree: %d°\r\n", degree);
  Serial.printf("Longitude: %.2f\r\n", longitude);
  Serial.printf("Latitude: %.2f\r\n", latitude);
 
  display.clearDisplay();
  display.setCursor(5, 4);            // Start at top-left corner
  display.drawRect(0, 0, 128, 64, SH110X_WHITE); //--> Display frame
  display.setTextColor( SH110X_BLACK,SH110X_WHITE);
  display.print(country);
  display.print(" ");
  display.println(location);
  display.setTextColor(SH110X_WHITE, SH110X_BLACK);
  display.setCursor(4, 15);
  display.print("Temperature: ");
  display.print(temperature);
  display.print((char)247);
  display.print("C");

  display.setCursor(4, 25);
  display.print("Humidity: ");
  display.print(humidity);
  display.println("% ");
  
  display.setCursor(4, 35);
  display.print("Pressure:");
  display.print(pressure);
  display.println("hpa");

  display.setCursor(4, 45);
  display.print("Wind Speed:");
  display.print(Speed);
  display.print("m/s  ");
  
  display.setCursor(4, 55);
  display.print("Wind degree: ");
  display.print(degree);
  display.println((char)247);
  
  display.display();
  delay(10000);
  }
  
  
}

void sendSensor(void*){ // Tạo hàm gửi dữ liệu lên Blynk
  while (1)
  {
    Serial.println("-------------------------");
    float h =dht.readHumidity(); // Đọc giá trị độ ẩm
    float t =dht.readTemperature(); // ordht.readTemperature(true) for Fahrenheit
    int sensorValue = map(analogRead(rainPin), 0, 4095, 100, 0);
   

    airqlty = map(analogRead(mq135_pin), 0, 4095, 0, 100);
    if (airqlty > 70){      
      Serial.println("Canh bao");
      tone(Buzzer, 1000); 
      delay(500);
      noTone(Buzzer); 
    } 
    int analogValue = map(analogRead(LDR), 0, 4095, 100, 0);
    // int analogValue = analogRead(LDR);
    
    // float voltage = analogValue / 1024. * 5;
    // float resistance = 2000 * voltage / (1 - voltage / 5);
    // float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));

    String data1= String("{\"Light\":") + String(analogValue) + String(",\"Humidity\":") + String(h) + String(",\"Gas\":") + String(airqlty) + String(",\"Rain\":") + String(sensorValue) + String(",\"Temperature\":") + String(t)  + String("}");
    HTTPClient http;
    http.begin(databaseURL1);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", FIREBASE_AUTH);
    int httpResponseCode1 = http.PUT(data1);

    if(httpResponseCode1 > 0){
    Serial.println("----------------------------------");
    Serial.print("Data sent successfully, respon code: ");
    
    Serial.println(httpResponseCode1);
    Serial.println("----------------------------------");

    }
    else{
      Serial.print("error sending data, response code: ");
      Serial.println(httpResponseCode1);
    }
    http.end();

    http.begin(databaseURL2);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", FIREBASE_AUTH);
    int httpResponseCode2 = http.GET();

    if(httpResponseCode2 > 0){
      String payload = http.getString();
      // Check if "Led1Status":"1" exists in the payload
      if ((sensorValue < 30) && (payload == "false")) {
        servo1.write(180); // Đặt servo ở góc quay được nhập       
        Serial.println("Mo mai che");
      } else 
      {
        servo1.write(0); // Đặt servo ở góc quay được nhập       
        Serial.println("Dong mai che");
      } 

      
    }
    
    http.end();

    Blynk.virtualWrite(V1,dht.readTemperature());
    Blynk.virtualWrite(V2,dht.readHumidity()); 
    Blynk.virtualWrite(V0,sensorValue); // V0 la do an 1
    Blynk.virtualWrite(V4,analogValue); 
    //timer.run();
    Blynk.run();
    vTaskDelay(pdMS_TO_TICKS(1000));
  
  }
}




