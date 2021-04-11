#define BLYNK_PRINT Serial   
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <DHT.h>
#define BLYNK_PRINT Serial
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature kelembabanT(&oneWire);
BlynkTimer timer;
WidgetLCD lcd(V1);
char auth[] ="3xjC4ffjFEgbAPEoWkfeHWyDXNdoQIUB"; 
char ssid[] = "Pi"; 
char pass[] = "87654321"; 
#define DHTPIN 2    
#define DHTTYPE DHT11     
DHT dht(DHTPIN, DHTTYPE);
const int lampu = D7;
int pompa = D1;
int tPompa = 0;
int tLampu = 0;
int sensor=0;

void Widget(){  
    if (tPompa == HIGH){
      lcd.clear();
      lcd.print(0,0,"Kondisi :");
      lcd.print(0,1, "Pompa Aktif");
    }
    else if(tLampu == HIGH){
      lcd.clear();
      lcd.print(0,0,"Kondisi :");
      lcd.print(0,1,"Lampu Aktif");
    }
    else if (tLampu == HIGH && tPompa == HIGH){
      lcd.clear();
      lcd.print(0,0,"Kondisi :");
      lcd.print(0,1,"Pompa&Lampu ON");
    }
    else{
      lcd.clear();
      lcd.print(0,0,"Kondisi :");
      lcd.print(0,1,"Pompa&Lampu Mati");
    }
}

void sendSensor(){
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
    }
  Blynk.virtualWrite(V5, h);  
  Blynk.virtualWrite(V6, t);
}

void setup(){
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode (D7, INPUT);
  pinMode (D1, INPUT);
  timer.setInterval(1000L, Widget); 
  dht.begin();
  timer.setInterval(1000L, sendSensor);
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
}

void sendTemps(){
  sensor=analogRead(A0);
  kelembabanT.requestTemperatures();
  float temp = kelembabanT.getTempCByIndex(0);
  Serial.println(temp); 
  Blynk.virtualWrite(V1, temp);
  Blynk.virtualWrite(V2,sensor);
  delay(1000);
  kelembabanT.begin();}
void loop(){
   float h = dht.readHumidity();
   Serial.print("Kelembapan Udara = ");
   Serial.println(h);
   float t = dht.readTemperature();
   Serial.print("Suhu = ");
   Serial.println(t); 
   int KelembabanT;
   KelembabanT = analogRead(sensor);
   int soil = map(sensor, 1023, 0, 0, 100);
   Serial.print("Kelembapan Tanah = ");
   Serial.println(soil);
   delay(1000);
   tPompa = digitalRead(pompa);
   tLampu = digitalRead(lampu);
   Blynk.run(); 
   timer.run(); 
   sendTemps();
      if (soil <= 50 && t >= 30){
    digitalWrite(pompa,HIGH );
    }
      else {
    digitalWrite(pompa, LOW);
 }}
