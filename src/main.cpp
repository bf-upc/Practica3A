#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

const char* ssid = "ssid"; //censurado
const char* password = "contraseña"; //censurado 
WebServer server(80);


void handleRoot() {
  File file = LittleFS.open("/index.html", "r");
  if (!file) {
    String message = "Archivo no encontrado. Intentando abrir: /index.html\n";
    message += "Verifica que el archivo existe en la carpeta /data\n";
    server.send(404, "text/plain", message);
    return;
  }
  
  server.streamFile(file, "text/html");
  file.close();
}

void setup() {
  Serial.begin(115200);
  delay(1000); // Esperar a que el serial se inicie
  
  Serial.println("\n=== INICIANDO ESP32 ===");
  
  // Montar LittleFS con formateo automático si falla
  if (!LittleFS.begin(true)) {
    Serial.println("Error al montar LittleFS");
    return;
  }
  Serial.println("✅ LittleFS montado correctamente");
  
  // Listar archivos para verificar
  
  // Conectar a WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Conectado a WiFi");
    Serial.print("📡 Dirección IP: ");
    Serial.println(WiFi.localIP());
    
    server.on("/", handleRoot);
    server.begin();
    Serial.println("✅ Servidor HTTP iniciado");
  } else {
    Serial.println("\n❌ Error al conectar a WiFi");
  }
}

void loop() {
  server.handleClient();
}