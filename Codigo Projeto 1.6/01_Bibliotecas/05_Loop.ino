void loop () {



  
             // Leitura e programação do sensor PIR
                  StatePirPrevious = StatePir; // passa o estado de leitura atual para o passado
                  StatePir = digitalRead(sensorPir); // realiza a leitura do sensor
                  delay(500);
                
                  if (StatePirPrevious == LOW && StatePir == HIGH) { //se estado do pir anterior for low e o novo for high tem movimento
                    Serial.println ("MOVIMENTO DETECTADO");
                    Status_LedVaranda =! Status_LedVaranda;
                    digitalWrite(LedVaranda, LOW);
                    }
                  else if (StatePirPrevious == HIGH && StatePir == LOW){//Se o estado novo do pir for LOW Nao tem mais movimento
                    Serial.println("SEM MOVIMENTACAO");
                    Status_LedVaranda =! Status_LedVaranda;
                    digitalWrite(LedVaranda, HIGH);
                   }
                
                  
                
                
                   // Leitura do DHT ******************************************************************
                  humidity =    dht.readHumidity();
                  temperature = dht.readTemperature();
                
                  // Atuaização do DHT LCD ***********************************************************
                  if (isnan(humidity) || isnan(temperature)) {
                    lcd.setCursor(0,0);
                    lcd.print("Falha ao ler DHT!!");
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
                      lcd.setCursor(7,0);          //linha e coluna do display
                      lcd.print(temperature,1);    //variavel e casa decimal
                      lcd.setCursor(12,0);
                      lcd.write((byte)0);     //Mostra o simbolo do grau formado pelo array
                      lcd.print("C");
                
                      lcd.setCursor(0,1);
                      lcd.print("Umid : ");
                      lcd.print(" ");
                      lcd.setCursor(7,1);
                      lcd.print(humidity,1);
                      lcd.setCursor(12,1);
                      lcd.print("%");


                    }



  /////////////////////////////////////////////////////////////////////////////////////////////////////////


  WiFiClient client = server.available();    // Ouça os clientes que chegam
   if (client) {                             // Se um novo cliente se conectar
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // imprimir uma mensagem na porta serial
    String currentLine = "";                // faça uma string para conter os dados de entrada do cliente
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // faz um loop enquanto o cliente está conectado
      currentTime = millis();
      if (client.available()) {             // se houver bytes para ler do cliente
        char c = client.read();             // leia um byte, então
        Serial.write(c);                    // imprima no monitor serial
        header += c;
        if (c == '\n') {                    // se o byte é um caractere de nova linha
          // se a linha atual estiver em branco, você terá dois caracteres de nova linha em uma linha.
          // esse é o fim da solicitação HTTP do cliente, então envie uma resposta:
          if (currentLine.length() == 0) {

            if (header.indexOf(base64Enconding) >=0) {

            
            // Os cabeçalhos HTTP sempre começam com um código de resposta (por exemplo, HTTP / 1.1 200 OK)
            // e um tipo de conteúdo para que o cliente saiba o que está por vir, em seguida, uma linha em branco:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
                



            // Verifica o caracter enviado pelo html (GET)********************************
                              //GET /?value=45& HTTP/1.1
            if(header.indexOf("GET /?value=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1+1, pos2);
              
              //Rotate the servo
              myservo.write(valueString.toInt());
              Serial.println(valueString); 
            }    
            //**************************************************
            if (header.indexOf("GET /L1") >= 0) {
              Status_LedVaranda =! Status_LedVaranda;
              digitalWrite(LedVaranda, Status_LedVaranda);
            }
             if (header.indexOf("GET /L2") >= 0) {
              Status_LedBanheiro =! Status_LedBanheiro;
              digitalWrite(LedBanheiro, Status_LedBanheiro);
            }
             if (header.indexOf("GET /L3") >= 0) {
              Status_LedQuarto =! Status_LedQuarto;
              digitalWrite(LedQuarto, Status_LedQuarto);
            }
             if (header.indexOf("GET /P1") >= 0) {
              valueString = String(0);
            }
              if (header.indexOf("GET /P2") >= 0) {
              valueString = String(90);
            }
             if (header.indexOf("GET /L4") >= 0) {
              Status_LedGaragem =! Status_LedGaragem;
              digitalWrite(LedGaragem, Status_LedGaragem);
            }
            
            




            // DEFINIÇÕES - CORPO - CÓDIGO PÁGINA HTML *****************************************************

            //**************************************************
            // Exibir a página da web em HTML

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<title> HOWTECH AUTOMACAO </title>");                     // Titulo do topo da Pagina


            //**************************************************
            // CSS para estilizar os botões e Pagina
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; background-color: #2dd2cf;}");
            client.println(".buttonPortao{ background-color: #F2AD91; border-radius: 15px; color: white; padding: 10px 20px;");
            client.println("text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;}");
            client.println(".button { background-color: #4CAF50; border-radius: 15px; color: white; padding: 10px 20px;");
            client.println("text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {  background-color: #555555;}");
            client.println(".humidity-icon{background-color: #3498db;width: 30px;height: 30px;border-radius: 50%;line-height: 36px;margin-left: 49%;}");
            client.println(".humidity-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: center;margin-left: 45%;}");
            client.println(".humidity{font-weight: 300;font-size: 60px;color: #3498db;}");
            client.println(".temperature-icon{background-color: #f39c12;width: 30px;height: 30px;border-radius: 50%;line-height: 40px;margin-left: 49%;}");
            client.println(".temperature-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: center;margin-left: 45%;}");
            client.println(".temperature{font-weight: 300;font-size: 60px;color: #f39c12;}");
            client.println("{ text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            client.println(".slider { width: 300px; }</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
                     
            client.println("</style></head>");
            


            //**************************************************
            // Título da página da web
            client.println("<body>    <h1>HOWTECH AUTOMACAO RESIDENCIAL</h1> <h3>(WEB Server ESP32)</h3>");
            
            client.println("<div id=\"webpage\">");
            client.println("<h3>Monitoramento de Temperatura e umidade</h3>");
             client.println("<div class=\"data\">");
             client.println("<div class=\"temperature-icon\">");
             client.println("<svg version=\"1.1\" id=\"Layer_1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\"");
             client.println("width=\"9.915px\" height=\"22px\" viewBox=\"0 0 9.915 22\" enable-background=\"new 0 0 9.915 22\" xml:space=\"preserve\">");
             client.println("<path fill=\"#FFFFFF\" d=\"M3.498,0.53c0.377-0.331,0.877-0.501,1.374-0.527C5.697-0.04,6.522,0.421,6.924,1.142");
             client.println("c0.237,0.399,0.315,0.871,0.311,1.33C7.229,5.856,7.245,9.24,7.227,12.625c1.019,0.539,1.855,1.424,2.301,2.491");
             client.println("c0.491,1.163,0.518,2.514,0.062,3.693c-0.414,1.102-1.24,2.038-2.276,2.594c-1.056,0.583-2.331,0.743-3.501,0.463");
             client.println("c-1.417-0.323-2.659-1.314-3.3-2.617C0.014,18.26-0.115,17.104,0.1,16.022c0.296-1.443,1.274-2.717,2.58-3.394");
             client.println("c0.013-3.44,0-6.881,0.007-10.322C2.674,1.634,2.974,0.955,3.498,0.53z\"/>");
             client.println("</svg>");
             client.println("</div>");
             client.println("<div class=\"temperature-text\">Temperature</div>");
             client.println("<div class=\"temperature\">");
             client.println((int)temperature);
             client.println("C");
             client.println("</div>");
             client.println("</div>");
             client.println("<div class=\"data\">");
             client.println("<div class=\"humidity-icon\">");
             client.println("<svg version=\"1.1\" id=\"Layer_2\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\"\n\"; width=\"12px\" height=\"17.955px\" viewBox=\"0 0 13 17.955\" enable-background=\"new 0 0 13 17.955\" xml:space=\"preserve\">");
             client.println("<path fill=\"#FFFFFF\" d=\"M1.819,6.217C3.139,4.064,6.5,0,6.5,0s3.363,4.064,4.681,6.217c1.793,2.926,2.133,5.05,1.571,7.057");
             client.println("c-0.438,1.574-2.264,4.681-6.252,4.681c-3.988,0-5.813-3.107-6.252-4.681C-0.313,11.267,0.026,9.143,1.819,6.217\"></path>");
             client.println("</svg>");
             client.println("</div>");
             client.println("<div class=\"humidity-text\">Humidity</div>");
             client.println("<div class=\"humidity\">");
             client.println((int)humidity);
             client.println("%");
             client.println("</div>");
             client.println("</div>");



            client.println("<h3>Acionamento das Lampadas</h3>");
            // Atualização do HTML ************************************************
            if (Status_LedVaranda) {
              client.print("<a href=\"/L1\"><button class=\"button button2 \">Led Varanda - OFF</button></a>");
            } else {
              client.print("<a href=\"/L1\"><button class=\"button \">Led Varanda - ON</button></a>");
            }
            client.print("<p>");
            if (Status_LedGaragem) {
              client.print("<a href=\"/L4\"><button class=\"button button2 \">Led Garagem - OFF</button></a>");
            } else {
              client.print("<a href=\"/L4\"><button class=\"button \">Led Garagem - ON</button></a>");
            }
             client.println("<P>");
            if (Status_LedBanheiro) {
              client.print("<a href=\"/L2\"><button class=\"button button2 \">Led Banheiro - OFF</button></a>");
            } else {
              client.print("<a href=\"/L2\"><button class=\"button \">Led Banheiro - ON</button></a>");
            }
             client.println("<P>");

            if (Status_LedQuarto) {
              client.print("<a href=\"/L3\"><button class=\"button button2 \">Led Quarto - OFF</button></a>");
            } else {
              client.print("<a href=\"/L3\"><button class=\"button \">Led Quarto - ON</button></a>");
            }

            client.println("<h3>Acionamento Portao Garagem</h3>");
            

            client.println("<p>Position: <span id=\"servoPos\"></span></p>");
            if (valueString)  {
              client.println("<a href=\"/P1\"><button class=\"buttonPortao \">Fechar</button></a>");
              myservo.write(valueString.toInt());
              }
            
            client.println("<input type=\"range\" min=\"0\" max=\"90\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");


            
            client.println("<script>var slider = document.getElementById(\"servoSlider\");");
            client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
            client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
            client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}</script>");
            
             if (valueString)  {
              client.println("<a href=\"/P2\"><button class=\"buttonPortao \">Abrir</button></a>");
              myservo.write(valueString.toInt());
              }

             if (valueString = "0") 
             {
              digitalWrite(LedSaidaV, HIGH);
              digitalWrite(LedEntradaV, LOW);
              }
              else if (valueString String ("90"))
              {
              digitalWrite(LedSaidaV, LOW);
              digitalWrite(LedEntradaV, HIGH);
              }
              else
              {
              digitalWrite(LedSaidaV, LOW);
              digitalWrite(LedEntradaV, LOW);
              }
             

           

            






           
            client.println("<P>");

            client.println("HOWTECH Automacao Residencial");
            client.println("<P>");


            client.println("</body></html>");






            client.println();                     // A resposta HTTP termina com outra linha em branco:
            break;
            }

            else{
              client.println("HTTP/1.1 401 Unauthorized");
              client.println("WWW-Authenticate: Basic realm=\"Secure\"");
              client.println("Content-Type: text/html");
              client.println();
              client.println("<html>Authentication failed</html>");
            
            }                                // interromper o loop while:
          } else {
            currentLine = "";                     // se você recebeu uma nova linha, limpe currentLine:
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    // Clear the header variable
    header = "";
    // fecha a conexão:
    client.stop();
    Serial.println("Client Disconnected.");
    Serial.println("------------------------------------------------------------");
  }


}
