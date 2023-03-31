/*** INCLUSÃO DE BIBLIOTECAS DO SISTEMA********************************************************************************************************************/

// Biblioteca comunicação WIFI e WIFI MANAGER
#include <FS.h>          
#include <WiFiManager.h> 
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <Servo.h>


// Biblioteca Sensor de Temperatura
#include <Adafruit_Sensor.h> // biblioteca sensor
#include "DHT.h" // biblioteca sensor
#include <Wire.h> //biblioteca localizar indereço display
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); //endereço I2C, nº colunas, nº linhas 






  
