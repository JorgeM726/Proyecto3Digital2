/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server 
  Basándose en los ejemplos de: 
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <WebServer.h>
#include <HardwareSerial.h>
//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "Angela";  // Ingresa tu SSID aquí
const char* password = "Angela121928";  // Ingresa tu contraseña aquí

WebServer server(80);  // Objeto WebServer (puerto HTTP, 80 es el predeterminado)

HardwareSerial SerialTiva1(1);  // Define UART para comunicarse con la primera Tiva en el puerto 1
HardwareSerial SerialTiva2(2);  // Define UART para comunicarse con la segunda Tiva en el puerto 2

// Variables para la primera Tiva
int parqueo1_tiva1 = 0;
int parqueo2_tiva1 = 0;
int parqueo3_tiva1 = 0;
int parqueo4_tiva1 = 0;

// Variables para la segunda Tiva
int parqueo1_tiva2 = 0;
int parqueo2_tiva2 = 0;
int parqueo3_tiva2 = 0;
int parqueo4_tiva2 = 0;

//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(115200);   // Inicia el puerto serial de la ESP32 a 9600 baudios
  SerialTiva1.begin(115200, SERIAL_8N1, 16, 17); // Inicia el puerto serial para la primera Tiva (puerto 1), con pines 16 (Rx) y 17 (Tx) a 9600 baudios
  SerialTiva2.begin(115200, SERIAL_8N1, 18, 19); // Inicia el puerto serial para la segunda Tiva (puerto 2), con pines 18 (Rx) y 19 (Tx) a 9600 baudios

  Serial.println("Intentando conectarse a ");
  Serial.println(ssid);

  // Conéctate a tu módem Wi-Fi
  WiFi.begin(ssid, password);

  // Verifica que la conexión Wi-Fi esté conectada a la red Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conexión Wi-Fi exitosa");
  Serial.print("IP obtenida: ");
  Serial.println(WiFi.localIP());  // Muestra la IP de la ESP32 en el puerto serial

  server.on("/", handle_OnConnect);
  server.on("/status", HTTP_GET, handle_Status);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("Servidor HTTP iniciado");
  delay(100);
}

//************************************************************************************************
// Loop principal
//************************************************************************************************
void loop() {
  server.handleClient();
  if (SerialTiva1.available() >= 4) {
    char receivedData[4];
    SerialTiva1.readBytes(receivedData, 4);
    delay(10);
    // Actualizar las variables individuales para la primera Tiva
    parqueo1_tiva1 = (receivedData[0] == '1') ? 1 : 0;
    parqueo2_tiva1 = (receivedData[1] == '1') ? 1 : 0;
    parqueo3_tiva1 = (receivedData[2] == '1') ? 1 : 0;
    parqueo4_tiva1 = (receivedData[3] == '1') ? 1 : 0;
    // Imprimir el estado de cada parqueo para la primera Tiva
    Serial.print("Estado de los parqueos (Tiva1): ");
    Serial.print("P1: "); Serial.print(parqueo1_tiva1);
    Serial.print(" P2: "); Serial.print(parqueo2_tiva1);
    Serial.print(" P3: "); Serial.print(parqueo3_tiva1);
    Serial.print(" P4: "); Serial.println(parqueo4_tiva1);
  }

  // Ejemplo de recepción de datos desde la segunda Tiva
  if (SerialTiva2.available() >= 4) {
    char receivedData[4];
    SerialTiva2.readBytes(receivedData, 4);
    delay(10);
    // Actualizar las variables individuales para la segunda Tiva
    parqueo1_tiva2 = (receivedData[0] == '1') ? 1 : 0;
    parqueo2_tiva2 = (receivedData[1] == '1') ? 1 : 0;
    parqueo3_tiva2 = (receivedData[2] == '1') ? 1 : 0;
    parqueo4_tiva2 = (receivedData[3] == '1') ? 1 : 0;
    // Imprimir el estado de cada parqueo para la segunda Tiva
    Serial.print("Estado de los parqueos (Tiva2): ");
    Serial.print("P1: "); Serial.print(parqueo1_tiva2);
    Serial.print(" P2: "); Serial.print(parqueo2_tiva2);
    Serial.print(" P3: "); Serial.print(parqueo3_tiva2);
    Serial.print(" P4: "); Serial.println(parqueo4_tiva2);
  }
}

//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************
void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(parqueo1_tiva1, parqueo2_tiva1, parqueo3_tiva1, parqueo4_tiva1, parqueo1_tiva2, parqueo2_tiva2, parqueo3_tiva2, parqueo4_tiva2));
}

void handle_Status() {
  server.send(200, "text/html", SendHTML(parqueo1_tiva1, parqueo2_tiva1, parqueo3_tiva1, parqueo4_tiva1, parqueo1_tiva2, parqueo2_tiva2, parqueo3_tiva2, parqueo4_tiva2));
}

void handle_NotFound() {
  server.send(404, "text/plain", "No encontrado");
}

String SendHTML(int p1_t1, int p2_t1, int p3_t1, int p4_t1, int p1_t2, int p2_t2, int p3_t2, int p4_t2) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>Control de Parqueos</title>\n";
  ptr +="<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js\"></script>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center; background-color: #222222; }\n";
  ptr +="body{margin-top: 50px;} h1 {color: #c4c4c4;margin: 50px auto 30px;} h3 {color: #c4c4c4;margin-bottom: 50px;}\n";
  ptr +=".status {font-size: 20px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr += "<h1>Control de Parqueos &#128664</h1>\n"; 
  ptr +="<div class=\"status\" id=\"status-container\">";
  
  //ptr +="<p>Tiva 1 - Parqueos:</p>";
  ptr += getStatusHTML("1", p1_t1);
  ptr += getStatusHTML("2", p2_t1);
  ptr += getStatusHTML("3", p3_t1);
  ptr += getStatusHTML("4", p4_t1);

  //ptr +="<p>Tiva 2 - Parqueos:</p>";
  ptr += getStatusHTML("5", p1_t2);
  ptr += getStatusHTML("6", p2_t2);
  ptr += getStatusHTML("7", p3_t2);
  ptr += getStatusHTML("8", p4_t2);

  //ptr += "<img src='img/Logo.jpg'>";

  ptr +="</div>\n";
  ptr +="<script>\n";
  ptr +="function updateStatus() {\n";
  ptr +="  $.ajax({\n";
  ptr +="    url: '/status',\n";
  ptr +="    type: 'GET',\n";
  ptr +="    success: function(data) {\n";
  ptr +="      $('#status-container').html(data);\n";
  ptr +="    }\n";
  ptr +="  });\n";
  ptr +="}\n";
  ptr +="setInterval(updateStatus, 5000); // Actualiza cada 5 segundos\n";
  ptr +="</script>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

String getStatusHTML(String label, int status) {
  String result = "<p> &#128663 Parqueo " + label + ": ";
  result += (status) ? "Ocupado &#10060" : "Libre &#9989";
  result += "</p>\n";
  return result;
}
