int sensorSolo = 35;
int leituraSolo;
 
void setup() {
 Serial.begin(115200);
  pinMode(sensorSolo, INPUT);
 
}

void loop() {
  leituraSolo = analogRead(sensorSolo);
  delay(500);
  Serial.println(leituraSolo);

}
