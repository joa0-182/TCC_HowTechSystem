const int sensorPir = 2;
int led = 23;

int StatePirPrevious = LOW; //estados anterior do sensor pir
int StatePir = LOW; // estado atual do sensor

void setup() {
 Serial.begin(115200);
 pinMode(sensorPir, INPUT); //define se a porta é entrada ou saida
 pinMode(led, OUTPUT);
 
}

void loop() {
  StatePirPrevious = StatePir; // passa o estado de leitura atual para o passado
  StatePir = digitalRead(sensorPir); // realiza a leitura do sensor
  delay(500);

  if (StatePirPrevious == LOW && StatePir == HIGH) { //se estado do pir anterior for low e o novo for high tem movimento
    Serial.println ("MOVIMENTO DETECTADO");
    digitalWrite(led, LOW);
    }
  else if (StatePirPrevious == HIGH && StatePir == LOW){//Se o estado novo do pir for LOW Nao tem mais movimento
    Serial.println("SEM MOVIMENTACAO");
    digitalWrite(led, HIGH);
   }
    
}
