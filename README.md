# ESP32 Web Server con LittleFS

![ESP32](https://img.shields.io/badge/ESP32-Desarrollo-blue)
![PlatformIO](https://img.shields.io/badge/PlatformIO-IDE-orange)
![LittleFS](https://img.shields.io/badge/LittleFS-Sistema_de_Archivos-green)

Servidor web para ESP32 que utiliza LittleFS (sistema de archivos en flash) para almacenar y servir archivos HTML/CSS/JS. Este proyecto separa el código C++ del contenido web, facilitando el mantenimiento y la escalabilidad.

## 📋 Características

- ✨ Servidor web asíncrono en ESP32
- 📁 Sistema de archivos LittleFS para almacenar contenido web
- 🌐 Página HTML con JavaScript interactivo
- 🔧 Configuración sencilla con PlatformIO
- 📱 Interfaz responsive

## 🚀 Requisitos Previos

- [PlatformIO](https://platformio.org/install) instalado en VS Code
- Placa ESP32 (cualquier modelo compatible)
- Cable USB para programación
- Conexión WiFi

## 📦 Estructura del Proyecto

```
esp32-web-server/
├── .gitignore
├── platformio.ini          # Configuración de PlatformIO
├── README.md
├── src/
│   └── main.cpp            # Código principal del servidor
└── data/
    └── index.html          # Página web estática
```

## ⚙️ Configuración Rápida

### 1. Clonar el repositorio

```bash
git clone https://github.com/tuusuario/esp32-web-server.git
cd esp32-web-server
```

### 2. Configurar credenciales WiFi

Edita el archivo `src/main.cpp` y reemplaza las credenciales WiFi:

```cpp
const char* ssid = "TU_SSID";
const char* password = "TU_PASSWORD";
```

### 3. Personalizar el contenido web

Modifica el archivo `data/index.html` según tus necesidades:

```html
<h1>Mi Página Personalizada</h1>
<!-- Agrega tu contenido aquí -->
```

## 🚀 Subida a la placa ESP32

### Subir el sistema de archivos (LittleFS)

```bash
# IMPORTANTE: Cierra el monitor serie antes de ejecutar
pio run --target uploadfs
```

### Compilar y subir el código

```bash
pio run --target upload
```

### Ver el monitor serie

```bash
pio device monitor
```

## 💻 Código Principal

### Configuración de PlatformIO (`platformio.ini`)

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
board_build.filesystem = littlefs
monitor_speed = 115200
```

### Servidor Web (`src/main.cpp`)

```cpp
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
```

## 🔧 Solución de Problemas

### Error: "file not found"

1. **Verifica la estructura de carpetas**
   ```
   tu_proyecto/
   ├── data/
   │   └── index.html    # ¿Está aquí?
   └── src/
       └── main.cpp
   ```

2. **Sube el sistema de archivos primero**
   ```bash
   pio run --target uploadfs
   ```

3. **Cierra el monitor serie** antes de subir archivos

4. **Ejecuta el script de diagnóstico**
   ```cpp
   // Agrega esto a tu código para listar archivos
   File root = LittleFS.open("/");
   File file = root.openNextFile();
   while (file) {
     Serial.println(file.name());
     file = root.openNextFile();
   }
   ```

### Error de compilación con LittleFS

Si tienes problemas con LittleFS, verifica que en `platformio.ini` tengas:

```ini
board_build.filesystem = littlefs
```

## 📚 Personalización Avanzada

### Servir archivos CSS y JS

Puedes agregar más archivos en la carpeta `data`:

```
data/
├── index.html
├── style.css
├── script.js
└── images/
    └── logo.png
```

Y en tu código:

```cpp
void handleCSS() {
  File file = LittleFS.open("/style.css", "r");
  server.streamFile(file, "text/css");
  file.close();
}

void setup() {
  // ...
  server.on("/style.css", handleCSS);
}
```

### Múltiples rutas

```cpp
server.on("/", handleRoot);
server.on("/about", handleAbout);
server.on("/api/data", handleAPIData);
```

