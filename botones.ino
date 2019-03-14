#include <ArduinoHttpClient.h>

#include <ArduinoJson.h>


//#include "Servo.h"

#include <ESP8266HTTPClient.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WiFi.h>
//#include <WiFiClient.h>

//Ultrasonicos de salida
#define boton1 D0//2//D9
#define boton2  D1//0//D8
#define BUTTON D7 //pulsador en GPIO12 (D6)
#define BUTTON_BUILTIN 0 

volatile int estado = HIGH;
volatile int estado2 = HIGH;
const char* ssid = "nerdstudio";
const char* password = "vvcdk1dN";
const char* server = "http://10.42.0.107:8000/boton";
int band=0;
int oldband=0;
int band2=0;
int oldband2=0;

void setup() {
  Serial.begin(9600);
   //Inicializando WiFi
  Serial.print("Conectando ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");}
  Serial.println("WiFi conectado");
  pinMode(boton1, INPUT);
  pinMode(boton2, INPUT);
  pinMode(BUTTON_BUILTIN, INPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  attachInterrupt(BUTTON_BUILTIN,parpadeo,RISING);
  attachInterrupt(BUTTON,parpadeo2,RISING);
  
}


void loop() {
  // SE OPRIMIO EL BOTON DE ENCENDIDO?
  if (oldband!=band) {
    // ENTONCES ENCENDEMOS EL LED
    Serial.println(band);
    post(band,"Boton Nodemcu","");
  }

  if (oldband2!=band2) {
    // ENTONCES ENCENDEMOS EL LED
    Serial.println(band2);
    post(band2,"Boton Normal","");
  }
  oldband2=band2;
  oldband=band;
}


void post (int id, const char* Nombre, const char* hora ){
  HTTPClient http;
  String json;
  StaticJsonBuffer<800> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["ID"] = id;
  root["Nombre"] = Nombre;
  root["Hora"] = hora;
  root.printTo(json); 
  http.begin(server);
  http.addHeader("Content-Type", "application/json");
  http.POST(json);
  http.writeToStream(&Serial);
  delay(100);
  http.end();
  }

  void parpadeo() {
   if(estado=!estado)
      band=1;
   else
      band=0;}

  void parpadeo2() {
   if(estado2=!estado2)
      band2=1;
   else
      band2=0;}
