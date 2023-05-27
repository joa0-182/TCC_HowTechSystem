/***INSTANCIANDO OBJETOS***********************************************************************************************************************************/

// Configuraçoes e Endereço de Internet WIFI
WiFiServer server                (80);
//Variável para armazenar a solicitação HTTP
String header;
// Decode HTTP GET value
String valueString = String(0);
int pos1 = 0;
int pos2 = 0;
// Hora Atual
unsigned long currentTime = millis();
// Vez anterior
unsigned long previousTime = 0; 
//Defina o tempo limite em milissegundos 
const long timeoutTime = 2000;




// Reset Wifi Manager
WiFiManager wifiManager;




 


// Status para controle de Led
int Status_LedVaranda = LOW;
int Status_LedBanheiro = LOW; 
int Status_LedQuarto = LOW; 
int Status_BombaAgua = LOW; 
int Status_LedGaragem = LOW;



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



//Define Variáveirs do PIR
int StatePirPrevious = LOW; //estados anterior do sensor pir
int StatePir = LOW; // estado atual do sensor
