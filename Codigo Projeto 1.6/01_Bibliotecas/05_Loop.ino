void loop () {




  // Leitura e programação do sensor PIR
  StatePirPrevious = StatePir; // passa o estado de leitura atual para o passado
  StatePir = digitalRead(sensorPir); // realiza a leitura do sensor
  delay(500);

  if (StatePirPrevious == LOW && StatePir == HIGH) { //se estado do pir anterior for low e o novo for high tem movimento
    Serial.println ("MOVIMENTO DETECTADO");
    Status_LedVaranda = ! Status_LedVaranda;
    digitalWrite(LedVaranda, LOW);
  }
  else if (StatePirPrevious == HIGH && StatePir == LOW) { //Se o estado novo do pir for LOW Nao tem mais movimento
    Serial.println("SEM MOVIMENTACAO");
    Status_LedVaranda = ! Status_LedVaranda;
    digitalWrite(LedVaranda, HIGH);
  }




  // Leitura do DHT ******************************************************************
  humidity =    dht.readHumidity();
  temperature = dht.readTemperature();

  // Atuaização do DHT LCD ***********************************************************
  if (isnan(humidity) || isnan(temperature)) {
    lcd.setCursor(0, 0);
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

    lcd.setCursor(0, 0);
    lcd.print("Temp : ");
    lcd.print(" ");
    lcd.setCursor(7, 0);         //linha e coluna do display
    lcd.print(temperature, 1);   //variavel e casa decimal
    lcd.setCursor(12, 0);
    lcd.write((byte)0);     //Mostra o simbolo do grau formado pelo array
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Umid : ");
    lcd.print(" ");
    lcd.setCursor(7, 1);
    lcd.print(humidity, 1);
    lcd.setCursor(12, 1);
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



            // Os cabeçalhos HTTP sempre começam com um código de resposta (por exemplo, HTTP / 1.1 200 OK)
            // e um tipo de conteúdo para que o cliente saiba o que está por vir, em seguida, uma linha em branco:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();




            // Verifica o caracter enviado pelo html (GET)********************************
            //GET /?value=45& HTTP/1.1
            if (header.indexOf("GET /?value=") >= 0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1 + 1, pos2);

              //Rotate the servo
              myservo.write(valueString.toInt());
              Serial.println(valueString);
            }
            //**************************************************
            if (header.indexOf("GET /L1") >= 0) {
              Status_LedVaranda = ! Status_LedVaranda;
              digitalWrite(LedVaranda, Status_LedVaranda);
            }
            if (header.indexOf("GET /L2") >= 0) {
              Status_LedBanheiro = ! Status_LedBanheiro;
              digitalWrite(LedBanheiro, Status_LedBanheiro);
            }
            if (header.indexOf("GET /L3") >= 0) {
              Status_LedQuarto = ! Status_LedQuarto;
              digitalWrite(LedQuarto, Status_LedQuarto);
            }
            if (header.indexOf("GET /P1") >= 0) {
              valueString = String(0);
            }
            if (header.indexOf("GET /P2") >= 0) {
              valueString = String(90);
            }
            if (header.indexOf("GET /L4") >= 0) {
              Status_LedGaragem = ! Status_LedGaragem;
              digitalWrite(LedGaragem, Status_LedGaragem);
            }






            // DEFINIÇÕES - CORPO - CÓDIGO PÁGINA HTML *****************************************************

            //**************************************************
            // Exibir a página da web em HTML

            client.println("<!DOCTYPE html lang=pt-br><html>");
            client.println("<meta http-equiv=X-UA-Compatible content=IE=edge>");
            client.println("<meta charset=UTF-8>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<title>HowTechSystem - Automação Residencial</title>");                     // Titulo do topo da Pagina


            //**************************************************
            // CSS para estilizar os botões e Pagina
            client.println("<style>html { font-family: Lexend, sans-serif; display: inline-block; margin: 0px auto; text-align: center; background-color: #2dd2cf;}");
            client.println("@charset UTF-8");
            client.println("@import url('https://fonts.googleapis.com/css2?family=Lexend:wght@300;600&family=Montserrat&display=swap')");
            client.println("@import url('https://fonts.googleapis.com/css2?family=Lexend:wght@300;400;500;600;700&display=swap')");
            client.println("* { margin: 0; padding: 0;}");
            client.println(".portao-titulo { display: flex; justify-content: center; align-items: center; margin: 0 auto; color: var(--corBoxBranco); font-size: 1.4rem; text-transform: uppercase;}");
            client.println(".btn-portao { margin: 25px; width: 50%; padding: 9px; border-radius: 20px; font-family:'Lexend', sans-serif; background-color:#F7F7F7;");
            client.println(" font-weight: 600; text-transform: lowercase; font-size: 1.2rem; color:#1E1E1E; box-shadow: 3px 3px 6px rgba(0, 0, 0, 0.07); cursor: pointer; transition: 0.2s ease-in; border: none;}");
            client.println(".botao-container-portao { display: flex; justify-content: center; align-items: center;}");
            client.println(".btn-portao:active { transform: scale(1.02); transition: 0.1s ease-in;}");
            client.println("text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;}");
            client.println(".humidity-icon{background-color: #3498db;width: 30px;height: 30px;border-radius: 50%;line-height: 36px;margin-left: 49%;}");
            client.println(".humidity-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: center;margin-left: 45%;}");
            client.println(".humidity{font-weight: 300;font-size: 60px;color: #3498db;}");
            client.println(".temperature-icon{background-color: #f39c12;width: 30px;height: 30px;border-radius: 50%;line-height: 40px;margin-left: 49%;}");
            client.println(".temperature-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: center;margin-left: 45%;}");
            client.println(".temperature{font-weight: 300;font-size: 60px;color: #f39c12;}");
            client.println("{ text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            client.println(".slider-fundo {display: flex;justify-content: center;align-items: center;background-color:#F7F7F7;border-radius: 30px;width: 300px;height: 65px;}");
            client.println(".slider-container {width: 300px;margin: 25px auto;text-align: center;}");
            client.println(".slider {width: 80%; -webkit-appearance: none; appearance: none; height: 5px; background-color:#1E1E1E; border-radius: 15px; outline: none; opacity: 0.7;-webkit-transition: .2s;transition: opacity .2s;}");
            client.println(".slider::-webkit-slider-thumb {-webkit-appearance: none;appearance: none;width: 15px;height: 15px;background-color: #2DC2BF;cursor: pointer;border-radius: 50%;}");
            client.println(".slider::-moz-range-thumb {width: 15px;height: 15px;background-color: #4CAF50;cursor: pointer;}");
            client.println(".luzes-titulo { display: flex; justify-content: center; align-items: center; margin: 30px auto; color: #F7F7F7; font-size: 1.4rem; text-transform: uppercase;}");
            client.println(".luzes-container { display: flex; justify-content: center; align-items: center; flex-direction: column; margin-top: 20px; text-align: center; font-size: 1.7rem;}");
            client.println("luzes-fundo { display: grid; grid-template-columns: repeat(2, 1fr); gap: 20px; justify-items: center;}");
            client.println(".luz { display: flex; flex-direction: column; align-items: center;}");
            client.println(".btn-luz { width: 100px; height: 100px; border-radius: 50%; background-color: #F7F7F7; border: none; outline: none; cursor: pointer; transition: background-color 0.3s ease; margin: 20px; overflow: hidden; max-width: 100%; max-height: 100%;}");
            client.println(".btn-luz2 { width: 100px; height: 100px; border-radius: 50%; background-color: #FFFF00; border: none; outline: none; cursor: pointer; transition: background-color 0.3s ease; margin: 20px; overflow: hidden; max-width: 100%; max-height: 100%;}");
            client.println(".btn-luz:hover { background-color: #2DC2BF;}");
            client.println(".imgLuz { max-width: 100%; width: 60px; max-height: 100%; margin-bottom: 10px; overflow: hidden; box-sizing: border-box; margin: 0 5px; object-fit: contain;}");
            client.println(".grid-container { display: grid; grid-template-columns: repeat(2, 1fr); grid-gap: 10px;}");
            client.println(".grid-item { display: flex; justify-content: center; align-items: center; background-color: #2dd2cf; border-radius: 20px; padding: 10px;}");
            client.print(".temperatura-titulo { display: flex; justify-content: center; align-items: center; margin: 0 auto; margin-top: 50px; color: #F7F7F7; font-size: 1.4rem; text-transform: uppercase;}");
            client.print(".temperatura-container { display: flex; justify-content: center; align-items: center; flex-direction: column; margin-top: 50px; margin-bottom: 100px; text-align: center; font-size: 1.4rem; text-transform: uppercase;}");
            client.print(".temperatura-fundo { display: flex; justify-content: center; align-items: center; background-color: #F7F7F7; border-radius: 30px; width: 180px; height: 50px; margin-top: 20px; margin-bottom: 20px; text-align: center; font-size: 1.7rem; margin-left: auto; margin-right: auto; padding: 15px; overflow: hidden;}");
            client.print(".imgTemperatura { max-width: 100%; max-height: 100%; margin-bottom: 10px; overflow: hidden; box-sizing: border-box; margin: 0 5px; object-fit: contain;}");



            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");


            client.println("</style></head>");



            //**************************************************
            // Título da página da web
            client.println("<body>");
            client.println("<div class=\"portao-titulo\">");
            client.println("<h1>HowTech System - Automação Residencial</h1>");
            client.println("</div>");



            client.println("<div id=\"webpage\">");
            client.println("<div class=\"temperatura-titulo\">");
            
            
            client.println("<h1>Temperatura</h1>");
            client.println("</div>");
 client.println("<div class=\"temperatura-container\">");
client.println("<div class=\"termometro\">");
client.println("<div class=\"temperatura-fundo\">");
client.println("<svg version=\"1.0\" xmlns=\"http://www.w3.org/2000/svg\" width=\"640.000000pt\" height=\"50.000000pt\"");
client.println("viewBox=\"0 0 640.000000 1280.000000\" preserveAspectRatio=\"xMidYMid meet\">");
client.println("<g transform=\"translate(0.000000,1280.000000) scale(0.100000,-0.100000)\" fill=\"#232323\" stroke=\"none\">");
client.println("<path d=\"M3033 12785 c-93 -17 -162 -40 -248 -83 -255 -127 -432 -345 -507\"");
client.println("-623 l-23 -84 -5 -4232 -5 -4232 -85 -56 c-437 -292 -714 -707 -821 -1233 -27");
client.println("-130 -36 -423 -19 -570 94 -797 686 -1452 1471 -1626 990 -219 1982 385 2238");
client.println("1365 50 191 56 247 56 489 0 239 -8 301 -61 499 -91 337 -299 673 -560 903");
client.println("-85 76 -213 171 -281 209 l-43 24 0 518 0 517 325 0 325 0 0 120 0 120 -325 0");
client.println("-325 0 0 765 0 765 325 0 325 0 0 120 0 120 -325 0 -325 0 0 765 0 765 325 0");
client.println("325 0 0 120 0 120 -322 2 -323 3 -3 763 -2 762 325 0 325 0 0 125 0 125 -325");
client.println("0 -325 0 0 913 c0 850 -2 918 -19 1002 -38 186 -127 345 -270 485 -219 213");
client.println("-524 309 -818 255z m340 -349 c189 -60 340 -209 405 -400 l27 -81 3 -4312 2");
client.println("-4312 118 -69 c310 -184 502 -381 646 -664 254 -498 227 -1079 -72 -1543 -228");
client.println("-355 -580 -595 -1005 -687 -150 -32 -446 -32 -600 0 -484 102 -870 399 -1092");
client.println("841 -210 419 -208 947 5 1372 142 285 353 504 654 680 l126 73 0 4266 c0 2729");
client.println("4 4289 10 4333 21 140 75 249 178 352 67 67 179 133 262 153 25 6 54 14 65 16");
client.println("38 9 220 -3 268 -18z\" />");
client.println("<g fill=\"#FF3E4D\" stroke=\"none\">");
client.println("<path d=\"M2830 5136 l0 -1953 -77 -28 c-436 -154 -766 -531 -864 -985 -33\"");
client.println("-153 -33 -402 -1 -549 122 -545 540 -950 1082 -1047 125 -22 335 -22 460 0");
client.println("418 75 778 343 967 721 54 107 87 200 114 326 33 148 33 397 0 549 -98 454");
client.println("-428 831 -863 985 l-78 28 0 1953 0 1954 -370 0 -370 0 0 -1954z\"/>");
client.println("</g>");
client.println("</g>");
client.println("</svg>");
client.println((int)temperature);
client.println("</div>");
client.println("</div>");

client.println("<div class=\"humidade\">");
client.println("<div class=\"temperatura-fundo\">");
client.println("<svg xmlns=\"http://www.w3.org/2000/svg\" xml:space=\"preserve\" width=\"5%\" height=\"5%\" version=\"1.1\" style=\"shape-rendering:geometricPrecision; text-rendering:geometricPrecision; image-rendering:optimizeQuality; fill-rule:evenodd; clip-rule:evenodd\"");
client.println("viewBox=\"0 0 500 500\"");
client.println("xmlns:xlink=\"http://www.w3.org/1999/xlink\">");
client.println("<defs>");
client.println("<style type=\"text/css\">");
client.println("<![CDATA[");
client.println(".fil0 {fill:#008cff}");
client.println(".fil2 {fill:#55b0fa}");
client.println(".fil1 {fill:#b6e3ff;fill-rule:nonzero}");
client.println("]]>");
client.println("</style>");
client.println("</defs>");
client.println("<g id=\"Layer_x0020_1\">");
client.println("<metadata id=\"CorelCorpID_0Corel-Layer\"/>");
client.println("<path class=\"fil0\" d=\"M265 35c54,112 142,179 156,263 12,75 -48,182 -171,182l0 -460c5,0 10,5 15,15z\"/>");
client.println("<path class=\"fil1\" d=\"M387 345c-5,21 -15,41 -30,56 -14,15 -33,26 -54,32 -7,2 -13,1 -19,-3 -7,-5 -9,-11 -9,-20 -1,-10 6,-19 16,-22 12,-4 24,-10 32,-20 9,-9 15,-20 19,-33 2,-12 12,-18 23,-18 8,0 14,3 18,9 5,5 6,12 4,19z\"/>");
client.println("<path class=\"fil2\" d=\"M235 35c-54,112 -142,179 -156,263 -12,75 48,182 171,182l0 -460c-5,0 -10,5 -15,15z\"/>");
client.println("</g>");
client.println("</svg>");
client.println((int)humidity);
client.println("</div>");
client.println("</div>");
client.println("</div>");

            


            client.println("<div class=\"luzes-titulo\"><h1>Luzes<h1></div>");


            client.println("<div class=\"luzes-container\">");
            client.println("<div class=\"luzes-fundo\">");
            client.println("<div class=\"grid-container\">");

            // Botão 1

            client.println("<div class=\"grid-item\">");
            client.println("<h5>Varanda</h5>");
            if (Status_LedVaranda) {
                client.println("<a href=\"/L1\">");
                client.println("<button class=\"btn-luz\">");
                client.println("<svg");
                client.println("xmlns=\"http://www.w3.org/2000/svg\"x=\"0px\"y=\"0px\"width=\"100\"height=\"100\"viewBox=\"0,0,256,256\"style=\"fill:000000;\"<gfill=\"none\"fillrule=\"nonzero\"stroke=\"none\"strokewidth=\"1\"strokelinecap=\"butt\"strokelinejoin=\"miter\"strokemiterlimit=\"10\" roke-dasharray=\"\" stroke-dashoffset=\"0\" font-family=\"none\" font-weight=\"none\" font-size=\"none\" text-anchor=\"none\" style=\"mix-blend-mode: normal\"><g transform=\"scale(2.56,2.56)\"><path d=\"M59.888,70.332c2.031,-1.277 2.875,-4.239 2.1422,-5.765l-0.541,-4.415c8.007,-4.104 13.918,-12.381 13.918,-21.997c0,-13.68 -11.509,-24.81 -25.189,-24.81c-13.68,0 -24.769,11.09 -24.769,24.769c0,9.616 5.491,17.933 13.498,22.037l-0.541,4.415c0,2.568 1.674,4.764 3.986,5.551\" fill=\"#ffea25\"></path><path d=\"M59.889,71.332c-0.332,0 -0.657,-0.166 -0.848,-0.468c-0.294,-0.468 -0.153,-1.085 0.314,-1.379c1.621,-1.019 2.328,-3.513 1.996,-4.634c-0.016,-0.053 -0.026,-0.107 -0.033,-0.162l-0.542,-4.415c-0.052,-0.417 0.163,-0.821 0.536,-1.012c8.124,-4.164 13.374,-12.449 13.374,-21.107c0,-13.128 -10.851,-23.81 -24.188,-23.81c-13.106,0 -23.77,10.663 -23.77,23.769c0,8.949 4.964,17.053 12.954,21.147c0.373,0.191 0.588,0.596 0.536,1.012l-0.534,4.351c0.024,2.057 1.348,3.879 3.309,4.546c0.522,0.178 0.803,0.746 0.624,1.269c-0.177,0.523 -0.74,0.804 -1.269,0.625c-2.79,-0.949 -4.664,-3.56 -4.664,-6.498c0,-0.041 0.003,-0.082 0.008,-0.122l0.458,-3.731c-8.297,-4.535 -13.422,-13.13 -13.422,-22.599c0,-14.209 11.561,-25.769 25.77,-25.769c14.44,0 26.188,11.578 26.188,25.81c0,9.178 -5.41,17.958 -13.842,22.559l0.449,3.656c0.546,2.04 -0.547,5.346 -2.874,6.809c-0.164,0.104 -0.349,0.153 -0.53,0.153z\" fill=\"#232323\"></path><path d=\"M47.542,67.739c-0.016,0 -0.03,0 -0.045,-0.002c-0.275,-0.024 -0.479,-0.267 -0.454,-0.542c0.522,-5.9 0.102,-10.628 -1.361,-14.719c-1.56,0.421 -3.202,0.194 -4.427,-0.692c-1.09,-0.789 -1.752,-1.862 -1.817,-2.946c-0.054,-0.902 0.302,-1.734 1.029,-2.407c0.373,-0.345 0.77,-0.555 1.18,-0.623c0.954,-0.162 1.965,0.448 2.698,1.625c0.752,1.207 1.392,2.457 1.922,3.766c0.271,-0.125 0.535,-0.276 0.785,-0.452c0.354,-0.249 0.671,-0.529 0.955,-0.831c-0.001,0 -0.001,-0.001 -0.002,-0.002c-0.888,-1.08 -1.058,-2.601 -0.507,-4.521c0.238,-0.831 1.009,-1.626 1.792,-1.849c0.512,-0.144 0.996,-0.045 1.361,0.288c0.529,0.479 0.659,1.259 0.389,2.318c-0.28,1.094 -0.777,2.453 -1.655,3.664c0.794,0.514 1.959,0.735 3.316,0.623c1.114,-0.094 2.149,-0.604 2.949,-1.444c-1.386,-0.92 -2.019,-2.167 -1.753,-3.519c0.172,-0.874 0.95,-1.797 1.812,-2.148c0.566,-0.232 1.116,-0.193 1.553,0.106c0.594,0.407 0.665,1.093 0.723,1.645c0.129,1.24 -0.171,2.509 -0.832,3.593c0.391,0.171 0.795,0.308 1.198,0.414c0.926,-1.753 1.888,-3.012 2.795,-3.362c0.422,-0.162 1.234,-0.291 1.944,0.682c1.084,1.482 0.788,2.427 0.349,2.958c-0.796,0.963 -2.685,1.176 -4.541,0.854c-2.403,5.039 -4.737,13.858 -4.443,16.665c0.029,0.275 -0.17,0.521 -0.445,0.549c-0.265,0.031 -0.521,-0.17 -0.549,-0.445c-0.297,-2.834 1.864,-11.615 4.437,-16.996c-0.473,-0.133 -0.929,-0.3 -1.35,-0.497c-0.988,1.131 -2.318,1.819 -3.762,1.94c-1.633,0.14 -3.048,-0.167 -4.038,-0.844c-0.331,0.354 -0.703,0.684 -1.121,0.977c-0.319,0.225 -0.655,0.416 -1.001,0.573c1.513,4.227 1.95,9.091 1.414,15.145c-0.025,0.26 -0.242,0.456 -0.498,0.456zM41.953,46.783c-0.047,0 -0.095,0.004 -0.143,0.012c-0.214,0.036 -0.437,0.16 -0.664,0.371c-0.508,0.469 -0.747,1.012 -0.711,1.613c0.047,0.784 0.56,1.583 1.405,2.195c0.938,0.68 2.238,0.859 3.479,0.556c-0.506,-1.241 -1.112,-2.425 -1.824,-3.568c-0.364,-0.587 -0.949,-1.179 -1.542,-1.179zM59.365,49.286c1.513,0.2072.854,-0.0193.303,-0.562c0.387,-0.4670.004,-1.199 -0.385,-1.731c-0.321,-0.437 -0.562,-0.421 -0.778,-0.338c-0.653,.734,44.48c-0.048,0 -0.104,0.007 -0.17,0.026c-0.456,0.13 -0.962,0.662 -1.105,1.163c-0.425,1.482 -0.359,2.64 0.195,3.447c0.741,-1.061 1.17,-2.25 1.417,-3.213c0.118,-0.4610.198,-1.066 -0.092,-1.33c-0.032,-0.029 -0.102,-0.093 -0.245,-0.093zM56.423,44.175c-0.101,0 -0.213,0.025 -0.336,0.076c-0.548,0.224 -1.102,0.872 -1.209,1.416c-0.232,1.185 0.588,2.018 1.389,2.533c0.57,-0.909 0.832,-1.977 0.724,-3.013c-0.05,-0.477 -0.097,-0.795 -0.294,-0.931c-0.079,-0.054 -0.171,-0.081 -0.274,-0.081z\" fill=\"#232323\"></path><path d=\"M57.228,76.614h-13.636c-1.573,0 -2.861,-1.35 -2.861,-3v0c0,-1.65 1.287,-3 2.861,-3h14.325c1.573,0 2.861,1.35 2.861,3v0c0,0.895 0,0.895 -0.598,1.72z\" fill=\"#8f9eb6\"></path><path d=\"M57.228,77.614h-13.636c-2.129,0 -3.86,-1.794 -3.86,-4c0,-2.206 1.731,-4 3.86,-4h14.325c2.129,0 3.86,1.794 3.86,4c0,1.151 -0.089,1.342 -0.788,2.307c-0.324,0.447 -0.949,0.547 -1.396,0.224c-0.447,-0.324 -0.547,-0.949 -0.223,-1.396c0.407,-0.563 0.407,-0.563 0.407,-1.134c0,-1.103 -0.835,-2 -1.86,-2h-14.325c-1.025,0 -1.86,0.897 -1.86,2c0,1.103 0.835,2 1.86,2h13.637c0.553,0 1,0.448 1,1c0,0.552 -0.448,0.999 -1.001,0.999z\" fill=\"#232323\"></path><path d=\"M57.682,82.614h-12.907c-1.401,0 -2.547,-1.146 -2.547,-2.547v-0.907c0,-1.401 1.146,-2.547 2.547,-2.547h12.907c1.401,0 2.547,1.146 2.547,2.547v0.907c-0.001,1.401 -1.147,2.547 -2.547,2.547z\" fill=\"#8f9eb6\"></path><path d=\"M57.681,83.614h-12.906c-1.956,0 -3.547,-1.591 -3.547,-3.547v-0.906c0,-1.956 1.591,-3.547 3.547,-3.547h12.906c1.956,0 3.547,1.591 3.547,3.547v0.906c0,1.956 -1.59,3.547 -3.547,3.547zM44.775,77.614c-0.853,0 -1.547,0.694 -1.547,1.547v0.906c0,0.853 0.694,1.547 1.547,1.547h12.906c0.853,0 1.547,-0.694 1.547,-1.547v-0.906c0,-0.853 -0.694,-1.547 -1.547,-1.547z\" fill=\"#232323\"></path><path d=\"M56.874,83.073c0,2.833 -4.427,3.552 -5.745,3.552c-3.106,0 -5.623,-1.646 -5.623,-3.677\" fill=\"#6474a6\"></path><path d=\"M51.129,87.625c-3.714,0 -6.623,-2.054 -6.623,-4.677c0,-0.552 0.447,-1 1,-1c0.553,0 1,0.448 1,1c0,1.451 2.117,2.677 4.623,2.677c1.047,0 4.745,-0.611 4.745,-2.552c0,-0.552 0.447,-1 1,-1c0.553,0 1,0.448 1,1c0,3.439 -4.544,4.552 -6.745,4.552zM70.103,38.739c-0.276,0 -0.5,-0.224 -0.5,-0.5c0,-1.682 -0.233,-3.345 -0.692,-4.944c-0.076,-0.2660.077,-0.5420.343,-0.619c0.264,-0.0780.542,0.0770.618,0.342c0.485,1.6880.731,3.4450.731,5.22c0,0.278 -0.223,0.501 -0.5,0.501zM68.148,30.482c-0.183,0 -0.358,-0.101 -0.446,-0.275c-2.562,-5.085 -7.434,-8.674 -13.03,-9.602c-0.966,-0.16 -1.957,-0.241 -2.943,-0.241c-0.276,0 -0.5,-0.224 -0.5,-0.5c0,-0.2760.224,-0.50.5,-0.5.041,0 2.087,0.086 3.107,0.255c5.91,0.979 11.054,4.769 13.759,10.138c0.124,0.247 0.025,0.547 -0.222,0.671c-0.071,0.037 -0.149,0.054 -0.225,0.054z\" fill=\"#232323\"></path></g></g>");
                client.println("</svg>");
                client.println("</button>");
                client.println("</a>");
              } else {
                client.println("<a href=\"/L1\">");
                client.println("<button class=\"btn-luz\">");
                client.println("<svg");
                client.println("xmlns=\"http://www.w3.org/2000/svg\" x=\"0px\" y=\"0px\" width=\"100\" height=\"100\" viewBox=\"0,0,256,256\"style=\"fill:#000000;\"><g fill=\"#232323\" fill-rule=\"nonzero\" stroke=\"none\" stroke-width=\"1\" stroke-linecap=\"butt\" stroke-linejoin=\"miter\" stroke-miterlimit=\"10\" stroke-dasharray=\"\" stroke-dashoffset=\"0\" font-family=\"none\" font-weight=\"none\" font-size=\"none\" text-anchor=\"none\" style=\"mix-blend-mode: normal\"><g transform=\"scale(2.56,2.56)\"><path d=\"M50.49805,12.3457c-14.209,0 -25.76953,11.56053 -25.76953,25.76953c0,9.469 5.12487,18.06461 13.42188,22.59961l-0.45703,3.73047c-0.005,0.04 -0.00781,0.08009 -0.00781,0.12109c0,2.49935 1.36374,4.75285 3.48633,5.95703c-0.87119,0.73395 -1.43945,1.83806 -1.43945,3.08984c0,1.55242 0.86531,2.88611 2.11523,3.54883c-0.39048,0.5698 -0.61914,1.25656 -0.61914,1.99805v0.90625c0,1.88656 1.48368,3.41988 3.34375,3.52734c0.4333,2.30615 3.15399,4.03125 6.55664,4.03125c2.08427,0 6.26309,-1.00114 6.70117,-4.02734c1.88662,-0.07988 3.39844,-1.62618 3.39844,-3.53125v-0.90625c0,-1.956 -1.59087,-3.54687 -3.54687,-3.54687h-12.90625c-0.00668,0 -0.01286,0.00192 -0.01953,0.00195h-1.16406c-1.025,0 -1.85937,-0.897 -1.85937,-2c0,-1.103 0.83438,-2 1.85938,-2h14.32617c1.025,0 1.85938,0.897 1.85938,2c0,0.571 -0.0012,0.56981 -0.4082,1.13281c-0.324,0.447 -0.22239,1.07248 0.22461,1.39648c0.447,0.323 1.07053,0.22434 1.39453,-0.22266c0.699,-0.965 0.78906,-1.15759 0.78906,-2.30859c0,-2.206 -1.73038,-4 -3.85937,-4h-14.32617c-0.02276,0 -0.04374,0.0074 -0.06641,0.00781c-0.11711,-0.20127 -0.29393,-0.36829 -0.53125,-0.44922c-1.961,-0.667 -3.28459,-2.48987 -3.30859,-4.54687l0.5332,-4.35156c0.052,-0.416 -0.16216,-0.82072 -0.53516,-1.01172c-7.99,-4.094 -12.95508,-12.19748 -12.95508,-21.14648c0,-13.106 10.66353,-23.76953 23.76953,-23.76953c13.337,0 24.18945,10.68255 24.18945,23.81055c0,8.658 -5.251,16.94342 -13.375,21.10742c-0.373,0.191 -0.58716,0.59472 -0.53516,1.01172l0.5332,4.35156c-0.016,1.283 -0.52913,2.48649 -1.45312,3.39648c-0.394,0.388 -0.39777,1.02201 -0.00977,1.41602c0.388,0.393 1.02006,0.39777 1.41406,0.00977c1.321,-1.303 2.04883,-3.03586 2.04883,-4.88086c0,-0.04 -0.00281,-0.08205 -0.00781,-0.12305l-0.45703,-3.73047c8.432,-4.601 13.8418,-13.38059 13.8418,-22.55859c0,-14.232 -11.74945,-25.81055 -26.18945,-25.81055zM51.72852,19.36328c-0.276,0 -0.5,0.224 -0.5,0.5c0,0.276 0.224,0.5 0.5,0.5c0.986,0 1.97736,0.08219 2.94336,0.24219c5.596,0.928 10.4673,4.51656 13.0293,9.60156c0.088,0.174 0.26426,0.27539 0.44727,0.27539c0.076,0 0.15361,-0.01769 0.22461,-0.05469c0.247,-0.124 0.34666,-0.42292 0.22266,-0.66992c-2.705,-5.369 -7.84977,-9.15967 -13.75977,-10.13867c-1.02,-0.169 -2.06642,-0.25586 -3.10742,-0.25586zM69.45117,32.66016c-0.06437,-0.0077 -0.13127,-0.00388 -0.19727,0.01563c-0.266,0.077 -0.41975,0.35314 -0.34375,0.61914c0.459,1.599 0.69336,3.26136 0.69336,4.94336c0,0.276 0.224,0.5 0.5,0.5c0.277,0 0.5,-0.222 0.5,-0.5c0,-1.775 -0.24742,-3.5327 -0.73242,-5.2207c-0.057,-0.19875 -0.2268,-0.33431 -0.41992,-0.35742zM56.46289,43.16016c-0.2895,-0.01625 -0.59816,0.04278 -0.91016,0.17578c-0.838,0.358 -1.51739,1.17528 -1.65039,1.98828c-0.227,1.391 0.23033,2.88367 1.23633,4.13867c-0.709,0.561 -1.5485,0.89961 -2.4375,0.97461c-0.994,0.087 -2.11,-0.32984 -3,-1.08984c0.685,-1.084 1.09484,-2.24126 1.33984,-3.19727c0.271,-1.059 0.14033,-1.83936 -0.38867,-2.31836c-0.404,-0.368 -0.96717,-0.48141 -1.57617,-0.31641c-0.812,0.216 -1.54425,0.87933 -1.78125,1.61133c-0.431,1.332 -0.08664,2.84072 0.94336,4.13672c0.053,0.066 0.10611,0.13231 0.16211,0.19531c-0.375,0.477 -0.82066,0.91711 -1.34766,1.28711c-0.25,0.176 -0.51416,0.32813 -0.78516,0.45313c-0.53,-1.309 -1.16987,-2.55862 -1.92187,-3.76562c-0.733,-1.177 -1.74522,-1.787 -2.69922,-1.625c-0.41,0.068 -0.80669,0.27805 -1.17969,0.62305c-0.727,0.673 -1.0833,1.50425 -1.0293,2.40625c0.065,1.084 0.72836,2.15631 1.81836,2.94531c1.225,0.886 2.86578,1.11436 4.42578,0.69336c1.463,4.091 1.88333,8.81875 1.36133,14.71875c-0.025,0.275 0.17813,0.51702 0.45313,0.54102c0.015,0.002 0.03088,0.00195 0.04688,0.00195c0.256,0 0.472,-0.19603 0.5,-0.45703c0.536,-6.054 0.09894,-10.91753 -1.41406,-15.14453c0.346,-0.157 0.68295,-0.34727 1.00195,-0.57227c0.582,-0.408 1.07323,-0.8893 1.49023,-1.4043c1.08,0.894 2.43097,1.37548 3.66797,1.27148c1.113,-0.093 2.1592,-0.52338 3.0332,-1.23437c0.01,0.01 0.02025,0.0193 0.03125,0.0293c0.482,0.446 1.01041,0.8137 1.56641,1.0957c-0.09396,0.16793 -0.172,0.32439 -0.26172,0.49023c-0.18609,-0.04847 -0.3859,0.00518 -0.49414,0.23633c-0.50399,1.07715 -0.89555,2.16429 -1.21875,3.25977c-0.04972,0.114 -0.17596,0.38603 -0.20117,0.44531c-0.04591,0.1084 -0.04261,0.22117 -0.01367,0.32617c-0.91667,3.52488 -1.05266,7.14913 -0.98047,10.9082c0.012,0.643 1.012,0.645 1,0c-0.07327,-3.85878 0.05366,-7.61271 1.09766,-11.24023c0.53934,-1.23276 1.23424,-2.6666 2.01172,-4.04102c0.964,0.307 1.97006,0.35905 2.91406,0.12305c0.302,-0.142 2.96425,-1.43653 3.28125,-3.26953c0.087,-0.507 0.02447,-1.27645 -0.76953,-2.06445c-0.453,-0.451 -1.00842,-0.62677 -1.60742,-0.50977c-1.414,0.278 -2.97476,2.3177 -4.25977,4.4707c-0.484,-0.243 -0.95095,-0.56208 -1.37695,-0.95508c0.04,-0.045 0.07919,-0.09267 0.11719,-0.13867c1.002,-1.211 1.48613,-2.77011 1.32813,-4.28711c-0.058,-0.552 -0.12866,-1.23753 -0.72266,-1.64453c-0.24,-0.164 -0.51128,-0.25523 -0.80078,-0.27148zM56.36328,44.1582c0.114,0 0.22908,0.02666 0.33008,0.09766c0.198,0.136 0.24492,0.45269 0.29492,0.92969c0.13,1.245 -0.27152,2.53502 -1.10352,3.54102c-0.009,0.011 -0.01834,0.02416 -0.02734,0.03516c-0.789,-1.018 -1.1467,-2.19239 -0.9707,-3.27539c0.078,-0.47 0.54064,-1.01047 1.05664,-1.23047c0.12,-0.052 0.26992,-0.09766 0.41992,-0.09766zM49.60352,44.44336c0.132,0 0.268,0.03191 0.375,0.12891c0.29,0.264 0.2098,0.86908 0.0918,1.33008c-0.207,0.809 -0.54298,1.77927 -1.08398,2.69727c-0.792,-1.019 -1.06319,-2.17006 -0.74219,-3.16406c0.132,-0.407 0.60889,-0.82612 1.08789,-0.95312c0.081,-0.022 0.17549,-0.03906 0.27148,-0.03906zM41.95313,46.7832c0.593,0 1.17702,0.59269 1.54102,1.17969c0.712,1.143 1.31822,2.32541 1.82422,3.56641c-1.241,0.303 -2.54051,0.12531 -3.47852,-0.55469c-0.845,-0.612 -1.3573,-1.41131 -1.4043,-2.19531c-0.036,-0.601 0.20294,-1.14428 0.71094,-1.61328c0.227,-0.211 0.45006,-0.33509 0.66406,-0.37109c0.048,-0.008 0.09558,-0.01172 0.14258,-0.01172zM62.5625,46.95313c0.18567,0.00802 0.35063,0.08966 0.51563,0.25391c0.397,0.394 0.55728,0.78159 0.48828,1.18359c-0.193,1.122 -2.02995,2.20995 -2.62695,2.50195c-0.653,0.159 -1.36236,0.13155 -2.06836,-0.06445c1.216,-2.012 2.51705,-3.66737 3.49805,-3.85937c0.06675,-0.0135 0.13147,-0.0183 0.19336,-0.01562zM44.77539,77.61328h12.45313h0.45313c0.853,0 1.54688,0.69388 1.54688,1.54688v0.90625c0,0.853 -0.69387,1.54688 -1.54687,1.54688h-12.90625c-0.853,0 -1.54687,-0.69388 -1.54687,-1.54687v-0.90625c0,-0.853 0.69387,-1.54687 1.54688,-1.54687zM46.67383,83.61328h9.08594c-0.60907,1.51351 -3.68868,2.01172 -4.63086,2.01172c-2.10939,0 -3.92747,-0.87141 -4.45508,-2.01172z\"></path></g></g>");
                client.println("</svg>");
                client.println("</button>");
                client.println("</a>");
              }
            client.println("</div>");

            // Botão 2

            client.println("<div class=\"grid-item\">");
            client.println("<h5>Banheiro</h5>");
            if (Status_LedBanheiro) {
                client.println("<a href=\"/L2\">");
                client.println("<button class=\"btn-luz\">");
                client.println("<svg");
                client.println("xmlns=\"http://www.w3.org/2000/svg\"x=\"0px\"y=\"0px\"width=\"100\"height=\"100\"viewBox=\"0,0,256,256\"style=\"fill:000000;\"<gfill=\"none\"fillrule=\"nonzero\"stroke=\"none\"strokewidth=\"1\"strokelinecap=\"butt\"strokelinejoin=\"miter\"strokemiterlimit=\"10\" roke-dasharray=\"\" stroke-dashoffset=\"0\" font-family=\"none\" font-weight=\"none\" font-size=\"none\" text-anchor=\"none\" style=\"mix-blend-mode: normal\"><g transform=\"scale(2.56,2.56)\"><path d=\"M59.888,70.332c2.031,-1.277 2.875,-4.239 2.1422,-5.765l-0.541,-4.415c8.007,-4.104 13.918,-12.381 13.918,-21.997c0,-13.68 -11.509,-24.81 -25.189,-24.81c-13.68,0 -24.769,11.09 -24.769,24.769c0,9.616 5.491,17.933 13.498,22.037l-0.541,4.415c0,2.568 1.674,4.764 3.986,5.551\" fill=\"#ffea25\"></path><path d=\"M59.889,71.332c-0.332,0 -0.657,-0.166 -0.848,-0.468c-0.294,-0.468 -0.153,-1.085 0.314,-1.379c1.621,-1.019 2.328,-3.513 1.996,-4.634c-0.016,-0.053 -0.026,-0.107 -0.033,-0.162l-0.542,-4.415c-0.052,-0.417 0.163,-0.821 0.536,-1.012c8.124,-4.164 13.374,-12.449 13.374,-21.107c0,-13.128 -10.851,-23.81 -24.188,-23.81c-13.106,0 -23.77,10.663 -23.77,23.769c0,8.949 4.964,17.053 12.954,21.147c0.373,0.191 0.588,0.596 0.536,1.012l-0.534,4.351c0.024,2.057 1.348,3.879 3.309,4.546c0.522,0.178 0.803,0.746 0.624,1.269c-0.177,0.523 -0.74,0.804 -1.269,0.625c-2.79,-0.949 -4.664,-3.56 -4.664,-6.498c0,-0.041 0.003,-0.082 0.008,-0.122l0.458,-3.731c-8.297,-4.535 -13.422,-13.13 -13.422,-22.599c0,-14.209 11.561,-25.769 25.77,-25.769c14.44,0 26.188,11.578 26.188,25.81c0,9.178 -5.41,17.958 -13.842,22.559l0.449,3.656c0.546,2.04 -0.547,5.346 -2.874,6.809c-0.164,0.104 -0.349,0.153 -0.53,0.153z\" fill=\"#232323\"></path><path d=\"M47.542,67.739c-0.016,0 -0.03,0 -0.045,-0.002c-0.275,-0.024 -0.479,-0.267 -0.454,-0.542c0.522,-5.9 0.102,-10.628 -1.361,-14.719c-1.56,0.421 -3.202,0.194 -4.427,-0.692c-1.09,-0.789 -1.752,-1.862 -1.817,-2.946c-0.054,-0.902 0.302,-1.734 1.029,-2.407c0.373,-0.345 0.77,-0.555 1.18,-0.623c0.954,-0.162 1.965,0.448 2.698,1.625c0.752,1.207 1.392,2.457 1.922,3.766c0.271,-0.125 0.535,-0.276 0.785,-0.452c0.354,-0.249 0.671,-0.529 0.955,-0.831c-0.001,0 -0.001,-0.001 -0.002,-0.002c-0.888,-1.08 -1.058,-2.601 -0.507,-4.521c0.238,-0.831 1.009,-1.626 1.792,-1.849c0.512,-0.144 0.996,-0.045 1.361,0.288c0.529,0.479 0.659,1.259 0.389,2.318c-0.28,1.094 -0.777,2.453 -1.655,3.664c0.794,0.514 1.959,0.735 3.316,0.623c1.114,-0.094 2.149,-0.604 2.949,-1.444c-1.386,-0.92 -2.019,-2.167 -1.753,-3.519c0.172,-0.874 0.95,-1.797 1.812,-2.148c0.566,-0.232 1.116,-0.193 1.553,0.106c0.594,0.407 0.665,1.093 0.723,1.645c0.129,1.24 -0.171,2.509 -0.832,3.593c0.391,0.171 0.795,0.308 1.198,0.414c0.926,-1.753 1.888,-3.012 2.795,-3.362c0.422,-0.162 1.234,-0.291 1.944,0.682c1.084,1.482 0.788,2.427 0.349,2.958c-0.796,0.963 -2.685,1.176 -4.541,0.854c-2.403,5.039 -4.737,13.858 -4.443,16.665c0.029,0.275 -0.17,0.521 -0.445,0.549c-0.265,0.031 -0.521,-0.17 -0.549,-0.445c-0.297,-2.834 1.864,-11.615 4.437,-16.996c-0.473,-0.133 -0.929,-0.3 -1.35,-0.497c-0.988,1.131 -2.318,1.819 -3.762,1.94c-1.633,0.14 -3.048,-0.167 -4.038,-0.844c-0.331,0.354 -0.703,0.684 -1.121,0.977c-0.319,0.225 -0.655,0.416 -1.001,0.573c1.513,4.227 1.95,9.091 1.414,15.145c-0.025,0.26 -0.242,0.456 -0.498,0.456zM41.953,46.783c-0.047,0 -0.095,0.004 -0.143,0.012c-0.214,0.036 -0.437,0.16 -0.664,0.371c-0.508,0.469 -0.747,1.012 -0.711,1.613c0.047,0.784 0.56,1.583 1.405,2.195c0.938,0.68 2.238,0.859 3.479,0.556c-0.506,-1.241 -1.112,-2.425 -1.824,-3.568c-0.364,-0.587 -0.949,-1.179 -1.542,-1.179zM59.365,49.286c1.513,0.2072.854,-0.0193.303,-0.562c0.387,-0.4670.004,-1.199 -0.385,-1.731c-0.321,-0.437 -0.562,-0.421 -0.778,-0.338c-0.653,.734,44.48c-0.048,0 -0.104,0.007 -0.17,0.026c-0.456,0.13 -0.962,0.662 -1.105,1.163c-0.425,1.482 -0.359,2.64 0.195,3.447c0.741,-1.061 1.17,-2.25 1.417,-3.213c0.118,-0.4610.198,-1.066 -0.092,-1.33c-0.032,-0.029 -0.102,-0.093 -0.245,-0.093zM56.423,44.175c-0.101,0 -0.213,0.025 -0.336,0.076c-0.548,0.224 -1.102,0.872 -1.209,1.416c-0.232,1.185 0.588,2.018 1.389,2.533c0.57,-0.909 0.832,-1.977 0.724,-3.013c-0.05,-0.477 -0.097,-0.795 -0.294,-0.931c-0.079,-0.054 -0.171,-0.081 -0.274,-0.081z\" fill=\"#232323\"></path><path d=\"M57.228,76.614h-13.636c-1.573,0 -2.861,-1.35 -2.861,-3v0c0,-1.65 1.287,-3 2.861,-3h14.325c1.573,0 2.861,1.35 2.861,3v0c0,0.895 0,0.895 -0.598,1.72z\" fill=\"#8f9eb6\"></path><path d=\"M57.228,77.614h-13.636c-2.129,0 -3.86,-1.794 -3.86,-4c0,-2.206 1.731,-4 3.86,-4h14.325c2.129,0 3.86,1.794 3.86,4c0,1.151 -0.089,1.342 -0.788,2.307c-0.324,0.447 -0.949,0.547 -1.396,0.224c-0.447,-0.324 -0.547,-0.949 -0.223,-1.396c0.407,-0.563 0.407,-0.563 0.407,-1.134c0,-1.103 -0.835,-2 -1.86,-2h-14.325c-1.025,0 -1.86,0.897 -1.86,2c0,1.103 0.835,2 1.86,2h13.637c0.553,0 1,0.448 1,1c0,0.552 -0.448,0.999 -1.001,0.999z\" fill=\"#232323\"></path><path d=\"M57.682,82.614h-12.907c-1.401,0 -2.547,-1.146 -2.547,-2.547v-0.907c0,-1.401 1.146,-2.547 2.547,-2.547h12.907c1.401,0 2.547,1.146 2.547,2.547v0.907c-0.001,1.401 -1.147,2.547 -2.547,2.547z\" fill=\"#8f9eb6\"></path><path d=\"M57.681,83.614h-12.906c-1.956,0 -3.547,-1.591 -3.547,-3.547v-0.906c0,-1.956 1.591,-3.547 3.547,-3.547h12.906c1.956,0 3.547,1.591 3.547,3.547v0.906c0,1.956 -1.59,3.547 -3.547,3.547zM44.775,77.614c-0.853,0 -1.547,0.694 -1.547,1.547v0.906c0,0.853 0.694,1.547 1.547,1.547h12.906c0.853,0 1.547,-0.694 1.547,-1.547v-0.906c0,-0.853 -0.694,-1.547 -1.547,-1.547z\" fill=\"#232323\"></path><path d=\"M56.874,83.073c0,2.833 -4.427,3.552 -5.745,3.552c-3.106,0 -5.623,-1.646 -5.623,-3.677\" fill=\"#6474a6\"></path><path d=\"M51.129,87.625c-3.714,0 -6.623,-2.054 -6.623,-4.677c0,-0.552 0.447,-1 1,-1c0.553,0 1,0.448 1,1c0,1.451 2.117,2.677 4.623,2.677c1.047,0 4.745,-0.611 4.745,-2.552c0,-0.552 0.447,-1 1,-1c0.553,0 1,0.448 1,1c0,3.439 -4.544,4.552 -6.745,4.552zM70.103,38.739c-0.276,0 -0.5,-0.224 -0.5,-0.5c0,-1.682 -0.233,-3.345 -0.692,-4.944c-0.076,-0.2660.077,-0.5420.343,-0.619c0.264,-0.0780.542,0.0770.618,0.342c0.485,1.6880.731,3.4450.731,5.22c0,0.278 -0.223,0.501 -0.5,0.501zM68.148,30.482c-0.183,0 -0.358,-0.101 -0.446,-0.275c-2.562,-5.085 -7.434,-8.674 -13.03,-9.602c-0.966,-0.16 -1.957,-0.241 -2.943,-0.241c-0.276,0 -0.5,-0.224 -0.5,-0.5c0,-0.2760.224,-0.50.5,-0.5.041,0 2.087,0.086 3.107,0.255c5.91,0.979 11.054,4.769 13.759,10.138c0.124,0.247 0.025,0.547 -0.222,0.671c-0.071,0.037 -0.149,0.054 -0.225,0.054z\" fill=\"#232323\"></path></g></g>");
                client.println("</svg>");
                client.println("</button>");
                client.println("</a>");
              } else {
                client.println("<a href=\"/L2\">");
                client.println("<button class=\"btn-luz\">");
                client.println("<svg");
                client.println("xmlns=\"http://www.w3.org/2000/svg\" x=\"0px\" y=\"0px\" width=\"100\" height=\"100\" viewBox=\"0,0,256,256\"style=\"fill:#000000;\"><g fill=\"#232323\" fill-rule=\"nonzero\" stroke=\"none\" stroke-width=\"1\" stroke-linecap=\"butt\" stroke-linejoin=\"miter\" stroke-miterlimit=\"10\" stroke-dasharray=\"\" stroke-dashoffset=\"0\" font-family=\"none\" font-weight=\"none\" font-size=\"none\" text-anchor=\"none\" style=\"mix-blend-mode: normal\"><g transform=\"scale(2.56,2.56)\"><path d=\"M50.49805,12.3457c-14.209,0 -25.76953,11.56053 -25.76953,25.76953c0,9.469 5.12487,18.06461 13.42188,22.59961l-0.45703,3.73047c-0.005,0.04 -0.00781,0.08009 -0.00781,0.12109c0,2.49935 1.36374,4.75285 3.48633,5.95703c-0.87119,0.73395 -1.43945,1.83806 -1.43945,3.08984c0,1.55242 0.86531,2.88611 2.11523,3.54883c-0.39048,0.5698 -0.61914,1.25656 -0.61914,1.99805v0.90625c0,1.88656 1.48368,3.41988 3.34375,3.52734c0.4333,2.30615 3.15399,4.03125 6.55664,4.03125c2.08427,0 6.26309,-1.00114 6.70117,-4.02734c1.88662,-0.07988 3.39844,-1.62618 3.39844,-3.53125v-0.90625c0,-1.956 -1.59087,-3.54687 -3.54687,-3.54687h-12.90625c-0.00668,0 -0.01286,0.00192 -0.01953,0.00195h-1.16406c-1.025,0 -1.85937,-0.897 -1.85937,-2c0,-1.103 0.83438,-2 1.85938,-2h14.32617c1.025,0 1.85938,0.897 1.85938,2c0,0.571 -0.0012,0.56981 -0.4082,1.13281c-0.324,0.447 -0.22239,1.07248 0.22461,1.39648c0.447,0.323 1.07053,0.22434 1.39453,-0.22266c0.699,-0.965 0.78906,-1.15759 0.78906,-2.30859c0,-2.206 -1.73038,-4 -3.85937,-4h-14.32617c-0.02276,0 -0.04374,0.0074 -0.06641,0.00781c-0.11711,-0.20127 -0.29393,-0.36829 -0.53125,-0.44922c-1.961,-0.667 -3.28459,-2.48987 -3.30859,-4.54687l0.5332,-4.35156c0.052,-0.416 -0.16216,-0.82072 -0.53516,-1.01172c-7.99,-4.094 -12.95508,-12.19748 -12.95508,-21.14648c0,-13.106 10.66353,-23.76953 23.76953,-23.76953c13.337,0 24.18945,10.68255 24.18945,23.81055c0,8.658 -5.251,16.94342 -13.375,21.10742c-0.373,0.191 -0.58716,0.59472 -0.53516,1.01172l0.5332,4.35156c-0.016,1.283 -0.52913,2.48649 -1.45312,3.39648c-0.394,0.388 -0.39777,1.02201 -0.00977,1.41602c0.388,0.393 1.02006,0.39777 1.41406,0.00977c1.321,-1.303 2.04883,-3.03586 2.04883,-4.88086c0,-0.04 -0.00281,-0.08205 -0.00781,-0.12305l-0.45703,-3.73047c8.432,-4.601 13.8418,-13.38059 13.8418,-22.55859c0,-14.232 -11.74945,-25.81055 -26.18945,-25.81055zM51.72852,19.36328c-0.276,0 -0.5,0.224 -0.5,0.5c0,0.276 0.224,0.5 0.5,0.5c0.986,0 1.97736,0.08219 2.94336,0.24219c5.596,0.928 10.4673,4.51656 13.0293,9.60156c0.088,0.174 0.26426,0.27539 0.44727,0.27539c0.076,0 0.15361,-0.01769 0.22461,-0.05469c0.247,-0.124 0.34666,-0.42292 0.22266,-0.66992c-2.705,-5.369 -7.84977,-9.15967 -13.75977,-10.13867c-1.02,-0.169 -2.06642,-0.25586 -3.10742,-0.25586zM69.45117,32.66016c-0.06437,-0.0077 -0.13127,-0.00388 -0.19727,0.01563c-0.266,0.077 -0.41975,0.35314 -0.34375,0.61914c0.459,1.599 0.69336,3.26136 0.69336,4.94336c0,0.276 0.224,0.5 0.5,0.5c0.277,0 0.5,-0.222 0.5,-0.5c0,-1.775 -0.24742,-3.5327 -0.73242,-5.2207c-0.057,-0.19875 -0.2268,-0.33431 -0.41992,-0.35742zM56.46289,43.16016c-0.2895,-0.01625 -0.59816,0.04278 -0.91016,0.17578c-0.838,0.358 -1.51739,1.17528 -1.65039,1.98828c-0.227,1.391 0.23033,2.88367 1.23633,4.13867c-0.709,0.561 -1.5485,0.89961 -2.4375,0.97461c-0.994,0.087 -2.11,-0.32984 -3,-1.08984c0.685,-1.084 1.09484,-2.24126 1.33984,-3.19727c0.271,-1.059 0.14033,-1.83936 -0.38867,-2.31836c-0.404,-0.368 -0.96717,-0.48141 -1.57617,-0.31641c-0.812,0.216 -1.54425,0.87933 -1.78125,1.61133c-0.431,1.332 -0.08664,2.84072 0.94336,4.13672c0.053,0.066 0.10611,0.13231 0.16211,0.19531c-0.375,0.477 -0.82066,0.91711 -1.34766,1.28711c-0.25,0.176 -0.51416,0.32813 -0.78516,0.45313c-0.53,-1.309 -1.16987,-2.55862 -1.92187,-3.76562c-0.733,-1.177 -1.74522,-1.787 -2.69922,-1.625c-0.41,0.068 -0.80669,0.27805 -1.17969,0.62305c-0.727,0.673 -1.0833,1.50425 -1.0293,2.40625c0.065,1.084 0.72836,2.15631 1.81836,2.94531c1.225,0.886 2.86578,1.11436 4.42578,0.69336c1.463,4.091 1.88333,8.81875 1.36133,14.71875c-0.025,0.275 0.17813,0.51702 0.45313,0.54102c0.015,0.002 0.03088,0.00195 0.04688,0.00195c0.256,0 0.472,-0.19603 0.5,-0.45703c0.536,-6.054 0.09894,-10.91753 -1.41406,-15.14453c0.346,-0.157 0.68295,-0.34727 1.00195,-0.57227c0.582,-0.408 1.07323,-0.8893 1.49023,-1.4043c1.08,0.894 2.43097,1.37548 3.66797,1.27148c1.113,-0.093 2.1592,-0.52338 3.0332,-1.23437c0.01,0.01 0.02025,0.0193 0.03125,0.0293c0.482,0.446 1.01041,0.8137 1.56641,1.0957c-0.09396,0.16793 -0.172,0.32439 -0.26172,0.49023c-0.18609,-0.04847 -0.3859,0.00518 -0.49414,0.23633c-0.50399,1.07715 -0.89555,2.16429 -1.21875,3.25977c-0.04972,0.114 -0.17596,0.38603 -0.20117,0.44531c-0.04591,0.1084 -0.04261,0.22117 -0.01367,0.32617c-0.91667,3.52488 -1.05266,7.14913 -0.98047,10.9082c0.012,0.643 1.012,0.645 1,0c-0.07327,-3.85878 0.05366,-7.61271 1.09766,-11.24023c0.53934,-1.23276 1.23424,-2.6666 2.01172,-4.04102c0.964,0.307 1.97006,0.35905 2.91406,0.12305c0.302,-0.142 2.96425,-1.43653 3.28125,-3.26953c0.087,-0.507 0.02447,-1.27645 -0.76953,-2.06445c-0.453,-0.451 -1.00842,-0.62677 -1.60742,-0.50977c-1.414,0.278 -2.97476,2.3177 -4.25977,4.4707c-0.484,-0.243 -0.95095,-0.56208 -1.37695,-0.95508c0.04,-0.045 0.07919,-0.09267 0.11719,-0.13867c1.002,-1.211 1.48613,-2.77011 1.32813,-4.28711c-0.058,-0.552 -0.12866,-1.23753 -0.72266,-1.64453c-0.24,-0.164 -0.51128,-0.25523 -0.80078,-0.27148zM56.36328,44.1582c0.114,0 0.22908,0.02666 0.33008,0.09766c0.198,0.136 0.24492,0.45269 0.29492,0.92969c0.13,1.245 -0.27152,2.53502 -1.10352,3.54102c-0.009,0.011 -0.01834,0.02416 -0.02734,0.03516c-0.789,-1.018 -1.1467,-2.19239 -0.9707,-3.27539c0.078,-0.47 0.54064,-1.01047 1.05664,-1.23047c0.12,-0.052 0.26992,-0.09766 0.41992,-0.09766zM49.60352,44.44336c0.132,0 0.268,0.03191 0.375,0.12891c0.29,0.264 0.2098,0.86908 0.0918,1.33008c-0.207,0.809 -0.54298,1.77927 -1.08398,2.69727c-0.792,-1.019 -1.06319,-2.17006 -0.74219,-3.16406c0.132,-0.407 0.60889,-0.82612 1.08789,-0.95312c0.081,-0.022 0.17549,-0.03906 0.27148,-0.03906zM41.95313,46.7832c0.593,0 1.17702,0.59269 1.54102,1.17969c0.712,1.143 1.31822,2.32541 1.82422,3.56641c-1.241,0.303 -2.54051,0.12531 -3.47852,-0.55469c-0.845,-0.612 -1.3573,-1.41131 -1.4043,-2.19531c-0.036,-0.601 0.20294,-1.14428 0.71094,-1.61328c0.227,-0.211 0.45006,-0.33509 0.66406,-0.37109c0.048,-0.008 0.09558,-0.01172 0.14258,-0.01172zM62.5625,46.95313c0.18567,0.00802 0.35063,0.08966 0.51563,0.25391c0.397,0.394 0.55728,0.78159 0.48828,1.18359c-0.193,1.122 -2.02995,2.20995 -2.62695,2.50195c-0.653,0.159 -1.36236,0.13155 -2.06836,-0.06445c1.216,-2.012 2.51705,-3.66737 3.49805,-3.85937c0.06675,-0.0135 0.13147,-0.0183 0.19336,-0.01562zM44.77539,77.61328h12.45313h0.45313c0.853,0 1.54688,0.69388 1.54688,1.54688v0.90625c0,0.853 -0.69387,1.54688 -1.54687,1.54688h-12.90625c-0.853,0 -1.54687,-0.69388 -1.54687,-1.54687v-0.90625c0,-0.853 0.69387,-1.54687 1.54688,-1.54687zM46.67383,83.61328h9.08594c-0.60907,1.51351 -3.68868,2.01172 -4.63086,2.01172c-2.10939,0 -3.92747,-0.87141 -4.45508,-2.01172z\"></path></g></g>");
                client.println("</svg>");
                client.println("</button>");
                client.println("</a>");
              }
            client.println("</div>");

            // Botão 3
            client.println("<div class=\"grid-item\">");
            client.println("<h5>Quarto</h5>");

            if (Status_LedQuarto) {
                client.println("<a href=\"/L3\">");
                client.println("<button class=\"btn-luz\">");
                client.println("<svg");
                client.println("xmlns=\"http://www.w3.org/2000/svg\"x=\"0px\"y=\"0px\"width=\"100\"height=\"100\"viewBox=\"0,0,256,256\"style=\"fill:000000;\"<gfill=\"none\"fillrule=\"nonzero\"stroke=\"none\"strokewidth=\"1\"strokelinecap=\"butt\"strokelinejoin=\"miter\"strokemiterlimit=\"10\" roke-dasharray=\"\" stroke-dashoffset=\"0\" font-family=\"none\" font-weight=\"none\" font-size=\"none\" text-anchor=\"none\" style=\"mix-blend-mode: normal\"><g transform=\"scale(2.56,2.56)\"><path d=\"M59.888,70.332c2.031,-1.277 2.875,-4.239 2.1422,-5.765l-0.541,-4.415c8.007,-4.104 13.918,-12.381 13.918,-21.997c0,-13.68 -11.509,-24.81 -25.189,-24.81c-13.68,0 -24.769,11.09 -24.769,24.769c0,9.616 5.491,17.933 13.498,22.037l-0.541,4.415c0,2.568 1.674,4.764 3.986,5.551\" fill=\"#ffea25\"></path><path d=\"M59.889,71.332c-0.332,0 -0.657,-0.166 -0.848,-0.468c-0.294,-0.468 -0.153,-1.085 0.314,-1.379c1.621,-1.019 2.328,-3.513 1.996,-4.634c-0.016,-0.053 -0.026,-0.107 -0.033,-0.162l-0.542,-4.415c-0.052,-0.417 0.163,-0.821 0.536,-1.012c8.124,-4.164 13.374,-12.449 13.374,-21.107c0,-13.128 -10.851,-23.81 -24.188,-23.81c-13.106,0 -23.77,10.663 -23.77,23.769c0,8.949 4.964,17.053 12.954,21.147c0.373,0.191 0.588,0.596 0.536,1.012l-0.534,4.351c0.024,2.057 1.348,3.879 3.309,4.546c0.522,0.178 0.803,0.746 0.624,1.269c-0.177,0.523 -0.74,0.804 -1.269,0.625c-2.79,-0.949 -4.664,-3.56 -4.664,-6.498c0,-0.041 0.003,-0.082 0.008,-0.122l0.458,-3.731c-8.297,-4.535 -13.422,-13.13 -13.422,-22.599c0,-14.209 11.561,-25.769 25.77,-25.769c14.44,0 26.188,11.578 26.188,25.81c0,9.178 -5.41,17.958 -13.842,22.559l0.449,3.656c0.546,2.04 -0.547,5.346 -2.874,6.809c-0.164,0.104 -0.349,0.153 -0.53,0.153z\" fill=\"#232323\"></path><path d=\"M47.542,67.739c-0.016,0 -0.03,0 -0.045,-0.002c-0.275,-0.024 -0.479,-0.267 -0.454,-0.542c0.522,-5.9 0.102,-10.628 -1.361,-14.719c-1.56,0.421 -3.202,0.194 -4.427,-0.692c-1.09,-0.789 -1.752,-1.862 -1.817,-2.946c-0.054,-0.902 0.302,-1.734 1.029,-2.407c0.373,-0.345 0.77,-0.555 1.18,-0.623c0.954,-0.162 1.965,0.448 2.698,1.625c0.752,1.207 1.392,2.457 1.922,3.766c0.271,-0.125 0.535,-0.276 0.785,-0.452c0.354,-0.249 0.671,-0.529 0.955,-0.831c-0.001,0 -0.001,-0.001 -0.002,-0.002c-0.888,-1.08 -1.058,-2.601 -0.507,-4.521c0.238,-0.831 1.009,-1.626 1.792,-1.849c0.512,-0.144 0.996,-0.045 1.361,0.288c0.529,0.479 0.659,1.259 0.389,2.318c-0.28,1.094 -0.777,2.453 -1.655,3.664c0.794,0.514 1.959,0.735 3.316,0.623c1.114,-0.094 2.149,-0.604 2.949,-1.444c-1.386,-0.92 -2.019,-2.167 -1.753,-3.519c0.172,-0.874 0.95,-1.797 1.812,-2.148c0.566,-0.232 1.116,-0.193 1.553,0.106c0.594,0.407 0.665,1.093 0.723,1.645c0.129,1.24 -0.171,2.509 -0.832,3.593c0.391,0.171 0.795,0.308 1.198,0.414c0.926,-1.753 1.888,-3.012 2.795,-3.362c0.422,-0.162 1.234,-0.291 1.944,0.682c1.084,1.482 0.788,2.427 0.349,2.958c-0.796,0.963 -2.685,1.176 -4.541,0.854c-2.403,5.039 -4.737,13.858 -4.443,16.665c0.029,0.275 -0.17,0.521 -0.445,0.549c-0.265,0.031 -0.521,-0.17 -0.549,-0.445c-0.297,-2.834 1.864,-11.615 4.437,-16.996c-0.473,-0.133 -0.929,-0.3 -1.35,-0.497c-0.988,1.131 -2.318,1.819 -3.762,1.94c-1.633,0.14 -3.048,-0.167 -4.038,-0.844c-0.331,0.354 -0.703,0.684 -1.121,0.977c-0.319,0.225 -0.655,0.416 -1.001,0.573c1.513,4.227 1.95,9.091 1.414,15.145c-0.025,0.26 -0.242,0.456 -0.498,0.456zM41.953,46.783c-0.047,0 -0.095,0.004 -0.143,0.012c-0.214,0.036 -0.437,0.16 -0.664,0.371c-0.508,0.469 -0.747,1.012 -0.711,1.613c0.047,0.784 0.56,1.583 1.405,2.195c0.938,0.68 2.238,0.859 3.479,0.556c-0.506,-1.241 -1.112,-2.425 -1.824,-3.568c-0.364,-0.587 -0.949,-1.179 -1.542,-1.179zM59.365,49.286c1.513,0.2072.854,-0.0193.303,-0.562c0.387,-0.4670.004,-1.199 -0.385,-1.731c-0.321,-0.437 -0.562,-0.421 -0.778,-0.338c-0.653,.734,44.48c-0.048,0 -0.104,0.007 -0.17,0.026c-0.456,0.13 -0.962,0.662 -1.105,1.163c-0.425,1.482 -0.359,2.64 0.195,3.447c0.741,-1.061 1.17,-2.25 1.417,-3.213c0.118,-0.4610.198,-1.066 -0.092,-1.33c-0.032,-0.029 -0.102,-0.093 -0.245,-0.093zM56.423,44.175c-0.101,0 -0.213,0.025 -0.336,0.076c-0.548,0.224 -1.102,0.872 -1.209,1.416c-0.232,1.185 0.588,2.018 1.389,2.533c0.57,-0.909 0.832,-1.977 0.724,-3.013c-0.05,-0.477 -0.097,-0.795 -0.294,-0.931c-0.079,-0.054 -0.171,-0.081 -0.274,-0.081z\" fill=\"#232323\"></path><path d=\"M57.228,76.614h-13.636c-1.573,0 -2.861,-1.35 -2.861,-3v0c0,-1.65 1.287,-3 2.861,-3h14.325c1.573,0 2.861,1.35 2.861,3v0c0,0.895 0,0.895 -0.598,1.72z\" fill=\"#8f9eb6\"></path><path d=\"M57.228,77.614h-13.636c-2.129,0 -3.86,-1.794 -3.86,-4c0,-2.206 1.731,-4 3.86,-4h14.325c2.129,0 3.86,1.794 3.86,4c0,1.151 -0.089,1.342 -0.788,2.307c-0.324,0.447 -0.949,0.547 -1.396,0.224c-0.447,-0.324 -0.547,-0.949 -0.223,-1.396c0.407,-0.563 0.407,-0.563 0.407,-1.134c0,-1.103 -0.835,-2 -1.86,-2h-14.325c-1.025,0 -1.86,0.897 -1.86,2c0,1.103 0.835,2 1.86,2h13.637c0.553,0 1,0.448 1,1c0,0.552 -0.448,0.999 -1.001,0.999z\" fill=\"#232323\"></path><path d=\"M57.682,82.614h-12.907c-1.401,0 -2.547,-1.146 -2.547,-2.547v-0.907c0,-1.401 1.146,-2.547 2.547,-2.547h12.907c1.401,0 2.547,1.146 2.547,2.547v0.907c-0.001,1.401 -1.147,2.547 -2.547,2.547z\" fill=\"#8f9eb6\"></path><path d=\"M57.681,83.614h-12.906c-1.956,0 -3.547,-1.591 -3.547,-3.547v-0.906c0,-1.956 1.591,-3.547 3.547,-3.547h12.906c1.956,0 3.547,1.591 3.547,3.547v0.906c0,1.956 -1.59,3.547 -3.547,3.547zM44.775,77.614c-0.853,0 -1.547,0.694 -1.547,1.547v0.906c0,0.853 0.694,1.547 1.547,1.547h12.906c0.853,0 1.547,-0.694 1.547,-1.547v-0.906c0,-0.853 -0.694,-1.547 -1.547,-1.547z\" fill=\"#232323\"></path><path d=\"M56.874,83.073c0,2.833 -4.427,3.552 -5.745,3.552c-3.106,0 -5.623,-1.646 -5.623,-3.677\" fill=\"#6474a6\"></path><path d=\"M51.129,87.625c-3.714,0 -6.623,-2.054 -6.623,-4.677c0,-0.552 0.447,-1 1,-1c0.553,0 1,0.448 1,1c0,1.451 2.117,2.677 4.623,2.677c1.047,0 4.745,-0.611 4.745,-2.552c0,-0.552 0.447,-1 1,-1c0.553,0 1,0.448 1,1c0,3.439 -4.544,4.552 -6.745,4.552zM70.103,38.739c-0.276,0 -0.5,-0.224 -0.5,-0.5c0,-1.682 -0.233,-3.345 -0.692,-4.944c-0.076,-0.2660.077,-0.5420.343,-0.619c0.264,-0.0780.542,0.0770.618,0.342c0.485,1.6880.731,3.4450.731,5.22c0,0.278 -0.223,0.501 -0.5,0.501zM68.148,30.482c-0.183,0 -0.358,-0.101 -0.446,-0.275c-2.562,-5.085 -7.434,-8.674 -13.03,-9.602c-0.966,-0.16 -1.957,-0.241 -2.943,-0.241c-0.276,0 -0.5,-0.224 -0.5,-0.5c0,-0.2760.224,-0.50.5,-0.5.041,0 2.087,0.086 3.107,0.255c5.91,0.979 11.054,4.769 13.759,10.138c0.124,0.247 0.025,0.547 -0.222,0.671c-0.071,0.037 -0.149,0.054 -0.225,0.054z\" fill=\"#232323\"></path></g></g>");
                client.println("</svg>");
                client.println("</button>");
                client.println("</a>");
              } else {
                client.println("<a href=\"/L3\">");
                client.println("<button class=\"btn-luz\">");
                client.println("<svg");
                client.println("xmlns=\"http://www.w3.org/2000/svg\" x=\"0px\" y=\"0px\" width=\"100\" height=\"100\" viewBox=\"0,0,256,256\"style=\"fill:#000000;\"><g fill=\"#232323\" fill-rule=\"nonzero\" stroke=\"none\" stroke-width=\"1\" stroke-linecap=\"butt\" stroke-linejoin=\"miter\" stroke-miterlimit=\"10\" stroke-dasharray=\"\" stroke-dashoffset=\"0\" font-family=\"none\" font-weight=\"none\" font-size=\"none\" text-anchor=\"none\" style=\"mix-blend-mode: normal\"><g transform=\"scale(2.56,2.56)\"><path d=\"M50.49805,12.3457c-14.209,0 -25.76953,11.56053 -25.76953,25.76953c0,9.469 5.12487,18.06461 13.42188,22.59961l-0.45703,3.73047c-0.005,0.04 -0.00781,0.08009 -0.00781,0.12109c0,2.49935 1.36374,4.75285 3.48633,5.95703c-0.87119,0.73395 -1.43945,1.83806 -1.43945,3.08984c0,1.55242 0.86531,2.88611 2.11523,3.54883c-0.39048,0.5698 -0.61914,1.25656 -0.61914,1.99805v0.90625c0,1.88656 1.48368,3.41988 3.34375,3.52734c0.4333,2.30615 3.15399,4.03125 6.55664,4.03125c2.08427,0 6.26309,-1.00114 6.70117,-4.02734c1.88662,-0.07988 3.39844,-1.62618 3.39844,-3.53125v-0.90625c0,-1.956 -1.59087,-3.54687 -3.54687,-3.54687h-12.90625c-0.00668,0 -0.01286,0.00192 -0.01953,0.00195h-1.16406c-1.025,0 -1.85937,-0.897 -1.85937,-2c0,-1.103 0.83438,-2 1.85938,-2h14.32617c1.025,0 1.85938,0.897 1.85938,2c0,0.571 -0.0012,0.56981 -0.4082,1.13281c-0.324,0.447 -0.22239,1.07248 0.22461,1.39648c0.447,0.323 1.07053,0.22434 1.39453,-0.22266c0.699,-0.965 0.78906,-1.15759 0.78906,-2.30859c0,-2.206 -1.73038,-4 -3.85937,-4h-14.32617c-0.02276,0 -0.04374,0.0074 -0.06641,0.00781c-0.11711,-0.20127 -0.29393,-0.36829 -0.53125,-0.44922c-1.961,-0.667 -3.28459,-2.48987 -3.30859,-4.54687l0.5332,-4.35156c0.052,-0.416 -0.16216,-0.82072 -0.53516,-1.01172c-7.99,-4.094 -12.95508,-12.19748 -12.95508,-21.14648c0,-13.106 10.66353,-23.76953 23.76953,-23.76953c13.337,0 24.18945,10.68255 24.18945,23.81055c0,8.658 -5.251,16.94342 -13.375,21.10742c-0.373,0.191 -0.58716,0.59472 -0.53516,1.01172l0.5332,4.35156c-0.016,1.283 -0.52913,2.48649 -1.45312,3.39648c-0.394,0.388 -0.39777,1.02201 -0.00977,1.41602c0.388,0.393 1.02006,0.39777 1.41406,0.00977c1.321,-1.303 2.04883,-3.03586 2.04883,-4.88086c0,-0.04 -0.00281,-0.08205 -0.00781,-0.12305l-0.45703,-3.73047c8.432,-4.601 13.8418,-13.38059 13.8418,-22.55859c0,-14.232 -11.74945,-25.81055 -26.18945,-25.81055zM51.72852,19.36328c-0.276,0 -0.5,0.224 -0.5,0.5c0,0.276 0.224,0.5 0.5,0.5c0.986,0 1.97736,0.08219 2.94336,0.24219c5.596,0.928 10.4673,4.51656 13.0293,9.60156c0.088,0.174 0.26426,0.27539 0.44727,0.27539c0.076,0 0.15361,-0.01769 0.22461,-0.05469c0.247,-0.124 0.34666,-0.42292 0.22266,-0.66992c-2.705,-5.369 -7.84977,-9.15967 -13.75977,-10.13867c-1.02,-0.169 -2.06642,-0.25586 -3.10742,-0.25586zM69.45117,32.66016c-0.06437,-0.0077 -0.13127,-0.00388 -0.19727,0.01563c-0.266,0.077 -0.41975,0.35314 -0.34375,0.61914c0.459,1.599 0.69336,3.26136 0.69336,4.94336c0,0.276 0.224,0.5 0.5,0.5c0.277,0 0.5,-0.222 0.5,-0.5c0,-1.775 -0.24742,-3.5327 -0.73242,-5.2207c-0.057,-0.19875 -0.2268,-0.33431 -0.41992,-0.35742zM56.46289,43.16016c-0.2895,-0.01625 -0.59816,0.04278 -0.91016,0.17578c-0.838,0.358 -1.51739,1.17528 -1.65039,1.98828c-0.227,1.391 0.23033,2.88367 1.23633,4.13867c-0.709,0.561 -1.5485,0.89961 -2.4375,0.97461c-0.994,0.087 -2.11,-0.32984 -3,-1.08984c0.685,-1.084 1.09484,-2.24126 1.33984,-3.19727c0.271,-1.059 0.14033,-1.83936 -0.38867,-2.31836c-0.404,-0.368 -0.96717,-0.48141 -1.57617,-0.31641c-0.812,0.216 -1.54425,0.87933 -1.78125,1.61133c-0.431,1.332 -0.08664,2.84072 0.94336,4.13672c0.053,0.066 0.10611,0.13231 0.16211,0.19531c-0.375,0.477 -0.82066,0.91711 -1.34766,1.28711c-0.25,0.176 -0.51416,0.32813 -0.78516,0.45313c-0.53,-1.309 -1.16987,-2.55862 -1.92187,-3.76562c-0.733,-1.177 -1.74522,-1.787 -2.69922,-1.625c-0.41,0.068 -0.80669,0.27805 -1.17969,0.62305c-0.727,0.673 -1.0833,1.50425 -1.0293,2.40625c0.065,1.084 0.72836,2.15631 1.81836,2.94531c1.225,0.886 2.86578,1.11436 4.42578,0.69336c1.463,4.091 1.88333,8.81875 1.36133,14.71875c-0.025,0.275 0.17813,0.51702 0.45313,0.54102c0.015,0.002 0.03088,0.00195 0.04688,0.00195c0.256,0 0.472,-0.19603 0.5,-0.45703c0.536,-6.054 0.09894,-10.91753 -1.41406,-15.14453c0.346,-0.157 0.68295,-0.34727 1.00195,-0.57227c0.582,-0.408 1.07323,-0.8893 1.49023,-1.4043c1.08,0.894 2.43097,1.37548 3.66797,1.27148c1.113,-0.093 2.1592,-0.52338 3.0332,-1.23437c0.01,0.01 0.02025,0.0193 0.03125,0.0293c0.482,0.446 1.01041,0.8137 1.56641,1.0957c-0.09396,0.16793 -0.172,0.32439 -0.26172,0.49023c-0.18609,-0.04847 -0.3859,0.00518 -0.49414,0.23633c-0.50399,1.07715 -0.89555,2.16429 -1.21875,3.25977c-0.04972,0.114 -0.17596,0.38603 -0.20117,0.44531c-0.04591,0.1084 -0.04261,0.22117 -0.01367,0.32617c-0.91667,3.52488 -1.05266,7.14913 -0.98047,10.9082c0.012,0.643 1.012,0.645 1,0c-0.07327,-3.85878 0.05366,-7.61271 1.09766,-11.24023c0.53934,-1.23276 1.23424,-2.6666 2.01172,-4.04102c0.964,0.307 1.97006,0.35905 2.91406,0.12305c0.302,-0.142 2.96425,-1.43653 3.28125,-3.26953c0.087,-0.507 0.02447,-1.27645 -0.76953,-2.06445c-0.453,-0.451 -1.00842,-0.62677 -1.60742,-0.50977c-1.414,0.278 -2.97476,2.3177 -4.25977,4.4707c-0.484,-0.243 -0.95095,-0.56208 -1.37695,-0.95508c0.04,-0.045 0.07919,-0.09267 0.11719,-0.13867c1.002,-1.211 1.48613,-2.77011 1.32813,-4.28711c-0.058,-0.552 -0.12866,-1.23753 -0.72266,-1.64453c-0.24,-0.164 -0.51128,-0.25523 -0.80078,-0.27148zM56.36328,44.1582c0.114,0 0.22908,0.02666 0.33008,0.09766c0.198,0.136 0.24492,0.45269 0.29492,0.92969c0.13,1.245 -0.27152,2.53502 -1.10352,3.54102c-0.009,0.011 -0.01834,0.02416 -0.02734,0.03516c-0.789,-1.018 -1.1467,-2.19239 -0.9707,-3.27539c0.078,-0.47 0.54064,-1.01047 1.05664,-1.23047c0.12,-0.052 0.26992,-0.09766 0.41992,-0.09766zM49.60352,44.44336c0.132,0 0.268,0.03191 0.375,0.12891c0.29,0.264 0.2098,0.86908 0.0918,1.33008c-0.207,0.809 -0.54298,1.77927 -1.08398,2.69727c-0.792,-1.019 -1.06319,-2.17006 -0.74219,-3.16406c0.132,-0.407 0.60889,-0.82612 1.08789,-0.95312c0.081,-0.022 0.17549,-0.03906 0.27148,-0.03906zM41.95313,46.7832c0.593,0 1.17702,0.59269 1.54102,1.17969c0.712,1.143 1.31822,2.32541 1.82422,3.56641c-1.241,0.303 -2.54051,0.12531 -3.47852,-0.55469c-0.845,-0.612 -1.3573,-1.41131 -1.4043,-2.19531c-0.036,-0.601 0.20294,-1.14428 0.71094,-1.61328c0.227,-0.211 0.45006,-0.33509 0.66406,-0.37109c0.048,-0.008 0.09558,-0.01172 0.14258,-0.01172zM62.5625,46.95313c0.18567,0.00802 0.35063,0.08966 0.51563,0.25391c0.397,0.394 0.55728,0.78159 0.48828,1.18359c-0.193,1.122 -2.02995,2.20995 -2.62695,2.50195c-0.653,0.159 -1.36236,0.13155 -2.06836,-0.06445c1.216,-2.012 2.51705,-3.66737 3.49805,-3.85937c0.06675,-0.0135 0.13147,-0.0183 0.19336,-0.01562zM44.77539,77.61328h12.45313h0.45313c0.853,0 1.54688,0.69388 1.54688,1.54688v0.90625c0,0.853 -0.69387,1.54688 -1.54687,1.54688h-12.90625c-0.853,0 -1.54687,-0.69388 -1.54687,-1.54687v-0.90625c0,-0.853 0.69387,-1.54687 1.54688,-1.54687zM46.67383,83.61328h9.08594c-0.60907,1.51351 -3.68868,2.01172 -4.63086,2.01172c-2.10939,0 -3.92747,-0.87141 -4.45508,-2.01172z\"></path></g></g>");
                client.println("</svg>");
                client.println("</button>");
                client.println("</a>");
              }
            client.println("</div>");

            // Botão 4
            client.println("<div class=\"grid-item\">");
            client.println("<h5>Garagem</h5>");

            if (Status_LedGaragem) {
                client.println("<a href=\"/L4\">");
                client.println("<button class=\"btn-luz\">");
                client.println("<svg");
                client.println("xmlns=\"http://www.w3.org/2000/svg\"x=\"0px\"y=\"0px\"width=\"100\"height=\"100\"viewBox=\"0,0,256,256\"style=\"fill:000000;\"<gfill=\"none\"fillrule=\"nonzero\"stroke=\"none\"strokewidth=\"1\"strokelinecap=\"butt\"strokelinejoin=\"miter\"strokemiterlimit=\"10\" roke-dasharray=\"\" stroke-dashoffset=\"0\" font-family=\"none\" font-weight=\"none\" font-size=\"none\" text-anchor=\"none\" style=\"mix-blend-mode: normal\"><g transform=\"scale(2.56,2.56)\"><path d=\"M59.888,70.332c2.031,-1.277 2.875,-4.239 2.1422,-5.765l-0.541,-4.415c8.007,-4.104 13.918,-12.381 13.918,-21.997c0,-13.68 -11.509,-24.81 -25.189,-24.81c-13.68,0 -24.769,11.09 -24.769,24.769c0,9.616 5.491,17.933 13.498,22.037l-0.541,4.415c0,2.568 1.674,4.764 3.986,5.551\" fill=\"#ffea25\"></path><path d=\"M59.889,71.332c-0.332,0 -0.657,-0.166 -0.848,-0.468c-0.294,-0.468 -0.153,-1.085 0.314,-1.379c1.621,-1.019 2.328,-3.513 1.996,-4.634c-0.016,-0.053 -0.026,-0.107 -0.033,-0.162l-0.542,-4.415c-0.052,-0.417 0.163,-0.821 0.536,-1.012c8.124,-4.164 13.374,-12.449 13.374,-21.107c0,-13.128 -10.851,-23.81 -24.188,-23.81c-13.106,0 -23.77,10.663 -23.77,23.769c0,8.949 4.964,17.053 12.954,21.147c0.373,0.191 0.588,0.596 0.536,1.012l-0.534,4.351c0.024,2.057 1.348,3.879 3.309,4.546c0.522,0.178 0.803,0.746 0.624,1.269c-0.177,0.523 -0.74,0.804 -1.269,0.625c-2.79,-0.949 -4.664,-3.56 -4.664,-6.498c0,-0.041 0.003,-0.082 0.008,-0.122l0.458,-3.731c-8.297,-4.535 -13.422,-13.13 -13.422,-22.599c0,-14.209 11.561,-25.769 25.77,-25.769c14.44,0 26.188,11.578 26.188,25.81c0,9.178 -5.41,17.958 -13.842,22.559l0.449,3.656c0.546,2.04 -0.547,5.346 -2.874,6.809c-0.164,0.104 -0.349,0.153 -0.53,0.153z\" fill=\"#232323\"></path><path d=\"M47.542,67.739c-0.016,0 -0.03,0 -0.045,-0.002c-0.275,-0.024 -0.479,-0.267 -0.454,-0.542c0.522,-5.9 0.102,-10.628 -1.361,-14.719c-1.56,0.421 -3.202,0.194 -4.427,-0.692c-1.09,-0.789 -1.752,-1.862 -1.817,-2.946c-0.054,-0.902 0.302,-1.734 1.029,-2.407c0.373,-0.345 0.77,-0.555 1.18,-0.623c0.954,-0.162 1.965,0.448 2.698,1.625c0.752,1.207 1.392,2.457 1.922,3.766c0.271,-0.125 0.535,-0.276 0.785,-0.452c0.354,-0.249 0.671,-0.529 0.955,-0.831c-0.001,0 -0.001,-0.001 -0.002,-0.002c-0.888,-1.08 -1.058,-2.601 -0.507,-4.521c0.238,-0.831 1.009,-1.626 1.792,-1.849c0.512,-0.144 0.996,-0.045 1.361,0.288c0.529,0.479 0.659,1.259 0.389,2.318c-0.28,1.094 -0.777,2.453 -1.655,3.664c0.794,0.514 1.959,0.735 3.316,0.623c1.114,-0.094 2.149,-0.604 2.949,-1.444c-1.386,-0.92 -2.019,-2.167 -1.753,-3.519c0.172,-0.874 0.95,-1.797 1.812,-2.148c0.566,-0.232 1.116,-0.193 1.553,0.106c0.594,0.407 0.665,1.093 0.723,1.645c0.129,1.24 -0.171,2.509 -0.832,3.593c0.391,0.171 0.795,0.308 1.198,0.414c0.926,-1.753 1.888,-3.012 2.795,-3.362c0.422,-0.162 1.234,-0.291 1.944,0.682c1.084,1.482 0.788,2.427 0.349,2.958c-0.796,0.963 -2.685,1.176 -4.541,0.854c-2.403,5.039 -4.737,13.858 -4.443,16.665c0.029,0.275 -0.17,0.521 -0.445,0.549c-0.265,0.031 -0.521,-0.17 -0.549,-0.445c-0.297,-2.834 1.864,-11.615 4.437,-16.996c-0.473,-0.133 -0.929,-0.3 -1.35,-0.497c-0.988,1.131 -2.318,1.819 -3.762,1.94c-1.633,0.14 -3.048,-0.167 -4.038,-0.844c-0.331,0.354 -0.703,0.684 -1.121,0.977c-0.319,0.225 -0.655,0.416 -1.001,0.573c1.513,4.227 1.95,9.091 1.414,15.145c-0.025,0.26 -0.242,0.456 -0.498,0.456zM41.953,46.783c-0.047,0 -0.095,0.004 -0.143,0.012c-0.214,0.036 -0.437,0.16 -0.664,0.371c-0.508,0.469 -0.747,1.012 -0.711,1.613c0.047,0.784 0.56,1.583 1.405,2.195c0.938,0.68 2.238,0.859 3.479,0.556c-0.506,-1.241 -1.112,-2.425 -1.824,-3.568c-0.364,-0.587 -0.949,-1.179 -1.542,-1.179zM59.365,49.286c1.513,0.2072.854,-0.0193.303,-0.562c0.387,-0.4670.004,-1.199 -0.385,-1.731c-0.321,-0.437 -0.562,-0.421 -0.778,-0.338c-0.653,.734,44.48c-0.048,0 -0.104,0.007 -0.17,0.026c-0.456,0.13 -0.962,0.662 -1.105,1.163c-0.425,1.482 -0.359,2.64 0.195,3.447c0.741,-1.061 1.17,-2.25 1.417,-3.213c0.118,-0.4610.198,-1.066 -0.092,-1.33c-0.032,-0.029 -0.102,-0.093 -0.245,-0.093zM56.423,44.175c-0.101,0 -0.213,0.025 -0.336,0.076c-0.548,0.224 -1.102,0.872 -1.209,1.416c-0.232,1.185 0.588,2.018 1.389,2.533c0.57,-0.909 0.832,-1.977 0.724,-3.013c-0.05,-0.477 -0.097,-0.795 -0.294,-0.931c-0.079,-0.054 -0.171,-0.081 -0.274,-0.081z\" fill=\"#232323\"></path><path d=\"M57.228,76.614h-13.636c-1.573,0 -2.861,-1.35 -2.861,-3v0c0,-1.65 1.287,-3 2.861,-3h14.325c1.573,0 2.861,1.35 2.861,3v0c0,0.895 0,0.895 -0.598,1.72z\" fill=\"#8f9eb6\"></path><path d=\"M57.228,77.614h-13.636c-2.129,0 -3.86,-1.794 -3.86,-4c0,-2.206 1.731,-4 3.86,-4h14.325c2.129,0 3.86,1.794 3.86,4c0,1.151 -0.089,1.342 -0.788,2.307c-0.324,0.447 -0.949,0.547 -1.396,0.224c-0.447,-0.324 -0.547,-0.949 -0.223,-1.396c0.407,-0.563 0.407,-0.563 0.407,-1.134c0,-1.103 -0.835,-2 -1.86,-2h-14.325c-1.025,0 -1.86,0.897 -1.86,2c0,1.103 0.835,2 1.86,2h13.637c0.553,0 1,0.448 1,1c0,0.552 -0.448,0.999 -1.001,0.999z\" fill=\"#232323\"></path><path d=\"M57.682,82.614h-12.907c-1.401,0 -2.547,-1.146 -2.547,-2.547v-0.907c0,-1.401 1.146,-2.547 2.547,-2.547h12.907c1.401,0 2.547,1.146 2.547,2.547v0.907c-0.001,1.401 -1.147,2.547 -2.547,2.547z\" fill=\"#8f9eb6\"></path><path d=\"M57.681,83.614h-12.906c-1.956,0 -3.547,-1.591 -3.547,-3.547v-0.906c0,-1.956 1.591,-3.547 3.547,-3.547h12.906c1.956,0 3.547,1.591 3.547,3.547v0.906c0,1.956 -1.59,3.547 -3.547,3.547zM44.775,77.614c-0.853,0 -1.547,0.694 -1.547,1.547v0.906c0,0.853 0.694,1.547 1.547,1.547h12.906c0.853,0 1.547,-0.694 1.547,-1.547v-0.906c0,-0.853 -0.694,-1.547 -1.547,-1.547z\" fill=\"#232323\"></path><path d=\"M56.874,83.073c0,2.833 -4.427,3.552 -5.745,3.552c-3.106,0 -5.623,-1.646 -5.623,-3.677\" fill=\"#6474a6\"></path><path d=\"M51.129,87.625c-3.714,0 -6.623,-2.054 -6.623,-4.677c0,-0.552 0.447,-1 1,-1c0.553,0 1,0.448 1,1c0,1.451 2.117,2.677 4.623,2.677c1.047,0 4.745,-0.611 4.745,-2.552c0,-0.552 0.447,-1 1,-1c0.553,0 1,0.448 1,1c0,3.439 -4.544,4.552 -6.745,4.552zM70.103,38.739c-0.276,0 -0.5,-0.224 -0.5,-0.5c0,-1.682 -0.233,-3.345 -0.692,-4.944c-0.076,-0.2660.077,-0.5420.343,-0.619c0.264,-0.0780.542,0.0770.618,0.342c0.485,1.6880.731,3.4450.731,5.22c0,0.278 -0.223,0.501 -0.5,0.501zM68.148,30.482c-0.183,0 -0.358,-0.101 -0.446,-0.275c-2.562,-5.085 -7.434,-8.674 -13.03,-9.602c-0.966,-0.16 -1.957,-0.241 -2.943,-0.241c-0.276,0 -0.5,-0.224 -0.5,-0.5c0,-0.2760.224,-0.50.5,-0.5.041,0 2.087,0.086 3.107,0.255c5.91,0.979 11.054,4.769 13.759,10.138c0.124,0.247 0.025,0.547 -0.222,0.671c-0.071,0.037 -0.149,0.054 -0.225,0.054z\" fill=\"#232323\"></path></g></g>");
                client.println("</svg>");
                client.println("</button>");
                client.println("</a>");
              } else {
                client.println("<a href=\"/L4\">");
                client.println("<button class=\"btn-luz\">");
                client.println("<svg");
                client.println("xmlns=\"http://www.w3.org/2000/svg\" x=\"0px\" y=\"0px\" width=\"100\" height=\"100\" viewBox=\"0,0,256,256\"style=\"fill:#000000;\"><g fill=\"#232323\" fill-rule=\"nonzero\" stroke=\"none\" stroke-width=\"1\" stroke-linecap=\"butt\" stroke-linejoin=\"miter\" stroke-miterlimit=\"10\" stroke-dasharray=\"\" stroke-dashoffset=\"0\" font-family=\"none\" font-weight=\"none\" font-size=\"none\" text-anchor=\"none\" style=\"mix-blend-mode: normal\"><g transform=\"scale(2.56,2.56)\"><path d=\"M50.49805,12.3457c-14.209,0 -25.76953,11.56053 -25.76953,25.76953c0,9.469 5.12487,18.06461 13.42188,22.59961l-0.45703,3.73047c-0.005,0.04 -0.00781,0.08009 -0.00781,0.12109c0,2.49935 1.36374,4.75285 3.48633,5.95703c-0.87119,0.73395 -1.43945,1.83806 -1.43945,3.08984c0,1.55242 0.86531,2.88611 2.11523,3.54883c-0.39048,0.5698 -0.61914,1.25656 -0.61914,1.99805v0.90625c0,1.88656 1.48368,3.41988 3.34375,3.52734c0.4333,2.30615 3.15399,4.03125 6.55664,4.03125c2.08427,0 6.26309,-1.00114 6.70117,-4.02734c1.88662,-0.07988 3.39844,-1.62618 3.39844,-3.53125v-0.90625c0,-1.956 -1.59087,-3.54687 -3.54687,-3.54687h-12.90625c-0.00668,0 -0.01286,0.00192 -0.01953,0.00195h-1.16406c-1.025,0 -1.85937,-0.897 -1.85937,-2c0,-1.103 0.83438,-2 1.85938,-2h14.32617c1.025,0 1.85938,0.897 1.85938,2c0,0.571 -0.0012,0.56981 -0.4082,1.13281c-0.324,0.447 -0.22239,1.07248 0.22461,1.39648c0.447,0.323 1.07053,0.22434 1.39453,-0.22266c0.699,-0.965 0.78906,-1.15759 0.78906,-2.30859c0,-2.206 -1.73038,-4 -3.85937,-4h-14.32617c-0.02276,0 -0.04374,0.0074 -0.06641,0.00781c-0.11711,-0.20127 -0.29393,-0.36829 -0.53125,-0.44922c-1.961,-0.667 -3.28459,-2.48987 -3.30859,-4.54687l0.5332,-4.35156c0.052,-0.416 -0.16216,-0.82072 -0.53516,-1.01172c-7.99,-4.094 -12.95508,-12.19748 -12.95508,-21.14648c0,-13.106 10.66353,-23.76953 23.76953,-23.76953c13.337,0 24.18945,10.68255 24.18945,23.81055c0,8.658 -5.251,16.94342 -13.375,21.10742c-0.373,0.191 -0.58716,0.59472 -0.53516,1.01172l0.5332,4.35156c-0.016,1.283 -0.52913,2.48649 -1.45312,3.39648c-0.394,0.388 -0.39777,1.02201 -0.00977,1.41602c0.388,0.393 1.02006,0.39777 1.41406,0.00977c1.321,-1.303 2.04883,-3.03586 2.04883,-4.88086c0,-0.04 -0.00281,-0.08205 -0.00781,-0.12305l-0.45703,-3.73047c8.432,-4.601 13.8418,-13.38059 13.8418,-22.55859c0,-14.232 -11.74945,-25.81055 -26.18945,-25.81055zM51.72852,19.36328c-0.276,0 -0.5,0.224 -0.5,0.5c0,0.276 0.224,0.5 0.5,0.5c0.986,0 1.97736,0.08219 2.94336,0.24219c5.596,0.928 10.4673,4.51656 13.0293,9.60156c0.088,0.174 0.26426,0.27539 0.44727,0.27539c0.076,0 0.15361,-0.01769 0.22461,-0.05469c0.247,-0.124 0.34666,-0.42292 0.22266,-0.66992c-2.705,-5.369 -7.84977,-9.15967 -13.75977,-10.13867c-1.02,-0.169 -2.06642,-0.25586 -3.10742,-0.25586zM69.45117,32.66016c-0.06437,-0.0077 -0.13127,-0.00388 -0.19727,0.01563c-0.266,0.077 -0.41975,0.35314 -0.34375,0.61914c0.459,1.599 0.69336,3.26136 0.69336,4.94336c0,0.276 0.224,0.5 0.5,0.5c0.277,0 0.5,-0.222 0.5,-0.5c0,-1.775 -0.24742,-3.5327 -0.73242,-5.2207c-0.057,-0.19875 -0.2268,-0.33431 -0.41992,-0.35742zM56.46289,43.16016c-0.2895,-0.01625 -0.59816,0.04278 -0.91016,0.17578c-0.838,0.358 -1.51739,1.17528 -1.65039,1.98828c-0.227,1.391 0.23033,2.88367 1.23633,4.13867c-0.709,0.561 -1.5485,0.89961 -2.4375,0.97461c-0.994,0.087 -2.11,-0.32984 -3,-1.08984c0.685,-1.084 1.09484,-2.24126 1.33984,-3.19727c0.271,-1.059 0.14033,-1.83936 -0.38867,-2.31836c-0.404,-0.368 -0.96717,-0.48141 -1.57617,-0.31641c-0.812,0.216 -1.54425,0.87933 -1.78125,1.61133c-0.431,1.332 -0.08664,2.84072 0.94336,4.13672c0.053,0.066 0.10611,0.13231 0.16211,0.19531c-0.375,0.477 -0.82066,0.91711 -1.34766,1.28711c-0.25,0.176 -0.51416,0.32813 -0.78516,0.45313c-0.53,-1.309 -1.16987,-2.55862 -1.92187,-3.76562c-0.733,-1.177 -1.74522,-1.787 -2.69922,-1.625c-0.41,0.068 -0.80669,0.27805 -1.17969,0.62305c-0.727,0.673 -1.0833,1.50425 -1.0293,2.40625c0.065,1.084 0.72836,2.15631 1.81836,2.94531c1.225,0.886 2.86578,1.11436 4.42578,0.69336c1.463,4.091 1.88333,8.81875 1.36133,14.71875c-0.025,0.275 0.17813,0.51702 0.45313,0.54102c0.015,0.002 0.03088,0.00195 0.04688,0.00195c0.256,0 0.472,-0.19603 0.5,-0.45703c0.536,-6.054 0.09894,-10.91753 -1.41406,-15.14453c0.346,-0.157 0.68295,-0.34727 1.00195,-0.57227c0.582,-0.408 1.07323,-0.8893 1.49023,-1.4043c1.08,0.894 2.43097,1.37548 3.66797,1.27148c1.113,-0.093 2.1592,-0.52338 3.0332,-1.23437c0.01,0.01 0.02025,0.0193 0.03125,0.0293c0.482,0.446 1.01041,0.8137 1.56641,1.0957c-0.09396,0.16793 -0.172,0.32439 -0.26172,0.49023c-0.18609,-0.04847 -0.3859,0.00518 -0.49414,0.23633c-0.50399,1.07715 -0.89555,2.16429 -1.21875,3.25977c-0.04972,0.114 -0.17596,0.38603 -0.20117,0.44531c-0.04591,0.1084 -0.04261,0.22117 -0.01367,0.32617c-0.91667,3.52488 -1.05266,7.14913 -0.98047,10.9082c0.012,0.643 1.012,0.645 1,0c-0.07327,-3.85878 0.05366,-7.61271 1.09766,-11.24023c0.53934,-1.23276 1.23424,-2.6666 2.01172,-4.04102c0.964,0.307 1.97006,0.35905 2.91406,0.12305c0.302,-0.142 2.96425,-1.43653 3.28125,-3.26953c0.087,-0.507 0.02447,-1.27645 -0.76953,-2.06445c-0.453,-0.451 -1.00842,-0.62677 -1.60742,-0.50977c-1.414,0.278 -2.97476,2.3177 -4.25977,4.4707c-0.484,-0.243 -0.95095,-0.56208 -1.37695,-0.95508c0.04,-0.045 0.07919,-0.09267 0.11719,-0.13867c1.002,-1.211 1.48613,-2.77011 1.32813,-4.28711c-0.058,-0.552 -0.12866,-1.23753 -0.72266,-1.64453c-0.24,-0.164 -0.51128,-0.25523 -0.80078,-0.27148zM56.36328,44.1582c0.114,0 0.22908,0.02666 0.33008,0.09766c0.198,0.136 0.24492,0.45269 0.29492,0.92969c0.13,1.245 -0.27152,2.53502 -1.10352,3.54102c-0.009,0.011 -0.01834,0.02416 -0.02734,0.03516c-0.789,-1.018 -1.1467,-2.19239 -0.9707,-3.27539c0.078,-0.47 0.54064,-1.01047 1.05664,-1.23047c0.12,-0.052 0.26992,-0.09766 0.41992,-0.09766zM49.60352,44.44336c0.132,0 0.268,0.03191 0.375,0.12891c0.29,0.264 0.2098,0.86908 0.0918,1.33008c-0.207,0.809 -0.54298,1.77927 -1.08398,2.69727c-0.792,-1.019 -1.06319,-2.17006 -0.74219,-3.16406c0.132,-0.407 0.60889,-0.82612 1.08789,-0.95312c0.081,-0.022 0.17549,-0.03906 0.27148,-0.03906zM41.95313,46.7832c0.593,0 1.17702,0.59269 1.54102,1.17969c0.712,1.143 1.31822,2.32541 1.82422,3.56641c-1.241,0.303 -2.54051,0.12531 -3.47852,-0.55469c-0.845,-0.612 -1.3573,-1.41131 -1.4043,-2.19531c-0.036,-0.601 0.20294,-1.14428 0.71094,-1.61328c0.227,-0.211 0.45006,-0.33509 0.66406,-0.37109c0.048,-0.008 0.09558,-0.01172 0.14258,-0.01172zM62.5625,46.95313c0.18567,0.00802 0.35063,0.08966 0.51563,0.25391c0.397,0.394 0.55728,0.78159 0.48828,1.18359c-0.193,1.122 -2.02995,2.20995 -2.62695,2.50195c-0.653,0.159 -1.36236,0.13155 -2.06836,-0.06445c1.216,-2.012 2.51705,-3.66737 3.49805,-3.85937c0.06675,-0.0135 0.13147,-0.0183 0.19336,-0.01562zM44.77539,77.61328h12.45313h0.45313c0.853,0 1.54688,0.69388 1.54688,1.54688v0.90625c0,0.853 -0.69387,1.54688 -1.54687,1.54688h-12.90625c-0.853,0 -1.54687,-0.69388 -1.54687,-1.54687v-0.90625c0,-0.853 0.69387,-1.54687 1.54688,-1.54687zM46.67383,83.61328h9.08594c-0.60907,1.51351 -3.68868,2.01172 -4.63086,2.01172c-2.10939,0 -3.92747,-0.87141 -4.45508,-2.01172z\"></path></g></g>");
                client.println("</svg>");
                client.println("</button>");
                client.println("</a>");
              }
            client.println("</div>");

            client.println("</div>");  // Fim do grid-container
            client.println("</div>");  // Fim do luzes-fundo
            client.println("</div>");  // Fim do luzes-container

            client.println("</div>");

            client.println("</div>");
            client.println("</div>");


            client.println("<div class=\"portao-titulo\">");
            client.println("<h1>Portão</h1>");
            client.println("</div>");


            client.println("<p>Position: <span id=\"servoPos\"></span> ° </p>");

            client.println("<div class=\"slider-container\">");
            client.println("<div class=\"slider-fundo\">");
            client.println("<input type=\"range\" min=\"0\" max=\"90\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\"" + valueString + "\"/>");
            client.println("</div>");
            client.println("</div>");



            client.println("<script>var slider = document.getElementById(\"servoSlider\");");
            client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
            client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
            client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
            client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}</script>");

            if (valueString)  {
              client.println("<div class=\"botao-container-portao\">");
              client.println("<a href=\"/P1\"><button class=\"btn-portao \">Fechar</button></a>");
              client.println("<a href=\"/P2\"><button class=\"btn-portao \">Abrir</button></a>");
              client.println("</div>");
              myservo.write(valueString.toInt());
            }


            client.println("<P>");

            client.println("HOWTECH Automacao Residencial");
            client.println("<P>");


            client.println("</body></html>");






            client.println();                     // A resposta HTTP termina com outra linha em branco:
            break;

            // interromper o loop while:
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
