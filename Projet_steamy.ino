#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define DHTPIN 4 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
#define lightpin A0
#define button 13
#define led_green 12
#define led_red 11
#define led_rgb_red 3 
#define led_rgb_green 5
#define led_rgb_blue 6
#define buzzer 9 
const int trigPin = 7;  // Pin connecté au trig du capteur
const int echoPin = 8;
unsigned long startTime;
unsigned long buzzerStartTime;
const unsigned long checkInterval = 180000;
const unsigned long IRInterval = 60000;
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2);

float humide(){
  float h = dht.readHumidity();
  return h;
}

bool controlhumidity(){
  float h = dht.readHumidity();
  if (h<80){
    return 1;
  }else{
    return 0;
  }
}

bool controlLight() {
  int light_val = analogRead(lightpin);
  if (light_val > 800){
    return 1;
  }else{
    return 0;
  }
}

long mesureDistance(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);// Envoyer une impulsion de 10µs sur la pin trig pour démarrer la mesure
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Lire le temps d'impulsion sur la pin echo
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration*0.034/2;
  return distance;
}

void rgb_led(long distance){
  if (32< distance && distance <=40){
    digitalWrite(led_rgb_red, LOW);
    digitalWrite(led_rgb_blue, LOW);
    digitalWrite(led_rgb_green, HIGH);
    
  }else if (22 < distance && distance<=32){
    analogWrite(led_rgb_red,255);
    analogWrite(led_rgb_blue,0);
    analogWrite(led_rgb_green,165);
    
  }else if (distance<=22){
     digitalWrite(led_rgb_red, HIGH);
     digitalWrite(led_rgb_blue, LOW);
     digitalWrite(led_rgb_green, LOW);
     
  }
}

void system_good(){
  digitalWrite(led_red, LOW);
  digitalWrite(led_green, HIGH);
  lcd.setCursor(0,0);
  lcd.print("PRET !");
  delay (1000);
  long value = mesureDistance();
  rgb_led(value);
  delay(1000);
  lcd.setCursor(0,0);
  lcd.print("OBSTACLE a");
  lcd.setCursor(0,1);
  lcd.print(value);
  lcd.print(" CM");
  delay(1000);
  lcd.clear();
  
}

void telecommandeIR(){
  Serial.println("jeu de lumière");
}


void setup() {
  dht.begin();
  pinMode(lightpin, INPUT);
  pinMode(button, INPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_red, OUTPUT);
  lcd.init();
  lcd.backlight();
  pinMode(led_rgb_red, OUTPUT);
  pinMode(led_rgb_blue, OUTPUT);
  pinMode(led_rgb_green, OUTPUT);
  pinMode(buzzer, OUTPUT);
  buzzerStartTime=0;
  Serial.begin(9600);
  
  

}

void loop() {
  bool BP1 =digitalRead(button);
 // Serial.println(controlhumidity());
 // Serial.println(controlLight());
  //delay(1000);
  if (BP1 == 0){
   
    while(controlhumidity() && controlLight()){
        system_good();
    }
    if ((!controlhumidity() || !controlLight())){
      digitalWrite(led_green, LOW);
      digitalWrite(led_red, HIGH);
      bool ah= controlhumidity();
      bool al= controlLight();
      /*lcd.setCursor(0,0);
      lcd.print("light ");
      lcd.print(al);
      lcd.setCursor(0,1);
      lcd.print("hum ");
      lcd.print(ah);*/
      lcd.clear();
      float hum = humide();
      lcd.setCursor(0,0);
      lcd.print(hum);
      startTime = millis();
      while(1){
        if (controlhumidity() && controlLight()){
          if (BP1 == 0){
            digitalWrite(buzzer, LOW);
            system_good();
          }
        }else if (millis() - startTime >=checkInterval){  // 3 minutes
          digitalWrite(buzzer, HIGH);
          if (buzzerStartTime == 0){
            buzzerStartTime= millis();
          }
          if (millis() - buzzerStartTime >= IRInterval){ //1 minute
            telecommandeIR(); // à faire
          }
        }else{
          
        }
      }
    }
  }
  

}
