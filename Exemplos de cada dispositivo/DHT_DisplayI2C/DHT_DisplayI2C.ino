
// Biblioteca Sensor de Temperatura
#include <Adafruit_Sensor.h> // biblioteca sensor
#include "DHT.h" // biblioteca sensor
#include <Wire.h> //biblioteca localizar indereço display
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); //endereço I2C, nº colunas, nº linhas





/**PINOS DIGITAIS DE ACESSO AO -- ESP32 MASTER -- ********************************************************************************************************/
const int DHTPIN   =              15;                                 // Sensor DHT11 Pode ser usado as
                                                                     // seguintes portas 3, 4, 5, 12, 13 ou 14




/***INSTANCIANDO OBJETOS***********************************************************************************************************************************/

// Define Modelo do DHT
#define DHTTYPE    DHT11       // DHT 11

//Array simbolo grau
byte grau[8] ={ B00001100,
                B00010010,
                B00010010,
                B00001100,
                B00000000,
                B00000000,
                B00000000,
                B00000000,};


// Define Variáveis do DHT
DHT dht(DHTPIN, DHTTYPE);
float humidity;     float humidity_atual;
float temperature;  float temperature_atual;




/*** SETUP DO PROGRAMA ***********************************************************************************************************************************/
void setup() {
  Serial.begin(115200);
  dht.begin(); // inicia comuinicação dht11
  lcd.begin(); // inicia comunicação lcd
  lcd.backlight(); // luz de fundo
  lcd.clear(); //Limpa o LCD
  //Cria o caractere customizado com o simbolo do grau
  lcd.createChar(0, grau);

}





void loop() {

  // Leitura do DHT ******************************************************************
  humidity =    dht.readHumidity();
  temperature = dht.readTemperature();

  // Atuaização do DHT LCD ***********************************************************
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Falha ao ler DHT!!");
  }
  else if (humidity_atual != humidity || temperature_atual != temperature) {
      humidity_atual = humidity;  temperature_atual = temperature;
      Serial.print("Temp.");  
      Serial.print(temperature);
      Serial.print("°");
      Serial.println("C");

      Serial.print("Umid.");              //Mostrar dados no painel serial
      Serial.print(humidity);
      Serial.println("%");
      Serial.println("====================");

        lcd.setCursor(0,0);
        lcd.print("Temp : ");
        lcd.print(" ");
        lcd.setCursor(7,0);
        lcd.print(temperature,1);
        lcd.setCursor(12,0);
        lcd.write((byte)0);     //Mostra o simbolo do grau formado pelo array

        lcd.setCursor(0,1);
        lcd.print("Umid : ");
        lcd.print(" ");
        lcd.setCursor(7,1);
        lcd.print(humidity,1);
        lcd.setCursor(12,1);
        lcd.print("%");
    }
    
}
