#include <avr/wdt.h>
#include <IRremote.hpp>
#define IR_RECEIVE_PIN 10
#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define DHTPIN 4 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
#define lightpin A0
#define button 13
#define button_2 2
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
unsigned long buttonPressTime;    // Moment où le bouton est pressé
unsigned long buttonReleaseTime;  // Moment où le bouton est relâché
const unsigned long longPressDuration = 2000;
bool lastatebutton = 0;
bool BP2;
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2);

float humide(){
  float h = dht.readHumidity();
  return h;
}

int lumen(){
  int h = analogRead(lightpin);;
  return h;
}

void telecommandeIR(){
  
    if (IrReceiver.decodedIRData.decodedRawData == 0xBA45FF00){
        digitalWrite(led_green, LOW); //buzzer
        for (int i; i<30; i++){
          digitalWrite(led_rgb_red, HIGH);
          digitalWrite(led_rgb_blue, LOW);
          digitalWrite(led_rgb_green, LOW);
          delay(1000);
        
          digitalWrite(led_rgb_red, LOW);
          digitalWrite(led_rgb_blue, HIGH);
          digitalWrite(led_rgb_green, LOW);
          delay(1000);
        
          digitalWrite(led_rgb_red, LOW);
          digitalWrite(led_rgb_blue, LOW);
          digitalWrite(led_rgb_green, HIGH);
          delay(1000);
        
          digitalWrite(led_rgb_red, LOW);
          digitalWrite(led_rgb_blue, HIGH);
          digitalWrite(led_rgb_green, HIGH);
          delay(1000);
        }
        
      }
  
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
  if (light_val > 500){ //>500
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
    analogWrite(led_rgb_green,100);
    
  }else if (distance<=22){
     digitalWrite(led_rgb_red, HIGH);
     digitalWrite(led_rgb_blue, LOW);
     digitalWrite(led_rgb_green, LOW);
     
  }else{
     digitalWrite(led_rgb_red, LOW);
     digitalWrite(led_rgb_blue, LOW);
     digitalWrite(led_rgb_green, LOW);
  }
}

void system_good(){
  digitalWrite(led_red, LOW);
  digitalWrite(led_green, HIGH);
  lcd.setCursor(0,0);
  lcd.print("PRET !");
  delay(500);
  long value = mesureDistance();
  rgb_led(value);
 
  lcd.setCursor(0,0);
  lcd.print("OBSTACLE a");
  lcd.setCursor(0,1);
  lcd.print(value);
  lcd.print(" CM");
  delay(1000);
  lcd.clear();
  
}

void systemRestart(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("System Restart");
  lcd.setCursor(0,1);
  lcd.print("Appuyer start");
  wdt_enable(WDTO_15MS);  // Redémarre l'Arduino après 15 ms
  while (1);
}

void systemStop(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("System Stop");
  delay(1000);
  wdt_enable(WDTO_15MS);  // Redémarre l'Arduino après 15 ms
  while (1);
}

void setup() {
  dht.begin();
  pinMode(lightpin, INPUT);
  pinMode(button, INPUT);
  pinMode(button_2, INPUT);
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
        BP2 = digitalRead(button_2);
        if ((BP2 == 1) && (lastatebutton == 0)) {
          buttonPressTime = millis();
        }
        if ((BP2 == 0) && (lastatebutton == 1)){
          buttonReleaseTime = millis();
          unsigned long Timepress = buttonReleaseTime-buttonPressTime;
          if (Timepress < longPressDuration) {
            systemRestart();
          } else {
            systemStop();
          }
        }
        lastatebutton = BP2;
        system_good();
    }
    BP1=1;

      startTime = millis();
      while(1){
        digitalWrite(led_green, LOW);
        digitalWrite(led_red, HIGH);

        BP2 = digitalRead(button_2);
        if ((BP2 == 1) && (lastatebutton == 0)) {
          buttonPressTime = millis();
        }
        if ((BP2 == 0) && (lastatebutton == 1)){
          buttonReleaseTime = millis();
          unsigned long Timepress = buttonReleaseTime-buttonPressTime;
          if (Timepress < longPressDuration) {
            systemRestart();
          } else {
            systemStop();
          }
        }
        lastatebutton = BP2;
       
        if (controlhumidity() == 0){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("ATTENTION");
          lcd.setCursor(0,1);
          lcd.print("ELECTROCUTION");
          delay(1000);
        }
        if (controlLight() == 0){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("ATTENTION");
          lcd.setCursor(0,1);
          lcd.print("ACCIDENT");
          delay(1000);
        }
        bool BP_1 = digitalRead(button);
        if (controlhumidity() && controlLight()){
          if (BP_1 == 0){
            while(controlhumidity() && controlLight()){
                system_good();
            }
          }
        }else if (millis() - startTime >=checkInterval){  // 3 minutes
          while(1){
            digitalWrite(buzer, HIGH); // buzzer
            if (buzzerStartTime == 0){
              buzzerStartTime= millis();
            }
            if (millis() - buzzerStartTime >= IRInterval){ //1 minute
                IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
                while(1){
                  if (IrReceiver.decode()){
                    if (IrReceiver.decodedIRData.decodedRawData == 0xBA45FF00){
                      telecommandeIR(); // à faire
                    }
                  }
                }
             }
          }
        }else{
          
        }
      
    }
  }
  

}
