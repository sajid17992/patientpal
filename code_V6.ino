/* connections
  DHT 11 data pin ---  27
  gas(mq2)  ---  13
*/


#define BLYNK_TEMPLATE_ID "TMPL6Z0MxW2Rz"
#define BLYNK_TEMPLATE_NAME "IOT HealthCare DashBoard"
#define BLYNK_AUTH_TOKEN "c58wFBOhAxlw7Cwps0O9OsIiYY8tgvI3"
#define BLYNK_PRINT Serial

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Blynk.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd2(0x27,16,2);
LiquidCrystal_I2C lcd1(0x26,16,2);

#define REPORTING_PERIOD_MS 1000
PulseOximeter pox;

char auth[] = "c58wFBOhAxlw7Cwps0O9OsIiYY8tgvI3";
char ssid[] = "Home";  // Home  ----  Oxygen Robotics
char pass[] = "6DCPpQYt";//  6DCPpQYt  --- DRMC2024

#define DHTTYPE DHT11
#define DHTPIN 27
DHT dht(DHTPIN, DHTTYPE);

#define mq2pin 13             // defining the gas sensor pin. mq2 gas sensor

float t;
float h;
float BPM, SpO2;
uint32_t tsLastReport = 0;
float gasvalue;



void onBeatDetected() {
   Serial.println("Beat Detected!");
}

void setup() {
  Serial.begin(115200);
  pinMode(19, OUTPUT);
  pinMode(DHTPIN, INPUT);
  dht.begin();
  Blynk.begin(auth, ssid, pass);
  Serial.print("Initializing Pulse Oximeter..");
  Serial.println("mq2 warming up!");

  lcd1.init();
  lcd1.backlight();
  lcd2.init();
  lcd2.backlight();

  lcd2.clear();

  lcd1.setCursor(0, 0);
  lcd1.print("  Initializing  ");
  for (int a = 5; a <= 10; a++) {
    lcd1.setCursor(a, 1);
    lcd1.print(".");
    delay(500);
  }
  lcd1.clear();

  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;)
      ;
  } else {
    Serial.println("SUCCESS");
    pox.setOnBeatDetectedCallback(onBeatDetected);
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
}

void loop() {
  pox.update();
  Blynk.run();
  t = dht.readTemperature(); 
  h = dht.readHumidity();       
  BPM = pox.getHeartRate();
  SpO2 = pox.getSpO2();
  gasvalue = analogRead(mq2pin);

  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Heart rate:");
    Serial.print(BPM);
    Serial.print(" bpm / SpO2:");
    Serial.print(SpO2);
    Serial.println(" %");

    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print("    Humidity : ");
    Serial.println(h);

    lcd1.setCursor(0, 0);
    lcd1.print("T:");
    lcd1.print(t);
    lcd1.setCursor(8, 0);
    lcd1.print("H:");
    lcd1.print(h);

    lcd1.setCursor(0, 1);
    lcd1.print("gas");
    lcd1.print(BPM);

    lcd2.setCursor(0, 1);
    lcd2.print("Hr:");
    lcd2.print(BPM);
    lcd2.setCursor(8, 1);
    lcd2.print("Ox:");
    lcd2.print(SpO2);

    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, h);
    Blynk.virtualWrite(V2, gasvalue);
    Blynk.virtualWrite(V3, SpO2);
    Blynk.virtualWrite(V4, BPM);;

    tsLastReport = millis();
  }
}
