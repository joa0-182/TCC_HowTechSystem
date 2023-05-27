
void setup() {

  Serial.begin(115200);

  
  
  Serial.println("Configurar Rede Wifi");




/////////////////////////////////////////////////////////////////////////////////////////////////////////

  //  SETUP -- wifi -- ////////////////////////////
  wifiManager.autoConnect("HOWTECH", "12345678");

  //wifiManager.resetSettings ();   // Apaga todas redes Salvas do Wifi

  delay(150);
  WiFi.begin();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("------------------------------------------------------------");
    Serial.println("Conectado IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("------------------------------------------------------------");

    server.begin();
    if (!MDNS.begin("howtech")) {
        Serial.println("Error setting up MDNS responder!");
        while(1) {
            delay(1000);
        }
    }
    Serial.println("mDNS responder started");

    // Start TCP (HTTP) server
    server.begin();
    Serial.println("TCP server started");

    // Add service to MDNS-SD
    MDNS.addService("http", "tcp", 80);
  }



/////////////////////////////////////////////////////////////////////////////////////////////////////////



  //*** SETUP -- Definição dos Pinos -- //////////
  delay(50);

  dht.begin(); // inicia comuinicação dht11
  lcd.begin(); // inicia comunicação lcd
  lcd.backlight(); // luz de fundo
  lcd.clear(); //Limpa o LCD
  lcd.createChar(0, grau); //Cria o caractere customizado com o simbolo do grau

 
  myservo.attach(servoPin);
  pinMode(LedVaranda,   OUTPUT);
  pinMode(LedBanheiro,   OUTPUT);
  pinMode(LedQuarto,   OUTPUT);
  pinMode(BombaAgua,   OUTPUT);
  pinMode(LedGaragem,   OUTPUT);

  // Iniciam em LOW -> Desligados
  digitalWrite(LedVaranda,   LOW);
  digitalWrite(LedBanheiro,  LOW);
  digitalWrite(LedQuarto,   LOW);
  digitalWrite(BombaAgua,   HIGH);
  digitalWrite(LedGaragem,   LOW);
  








/////////////////////////////////////////////////////////////////////////////////////////////////////////

  Serial.println("INICIOU **************************");

}
