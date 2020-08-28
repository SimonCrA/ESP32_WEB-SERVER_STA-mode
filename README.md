# ESP32_WEB-SERVER_STA-mode


###  WEB SERVER GPIO
---
Este código le permite al ESP32 crear un web server que servirá un página web.
El ESP32 esta programado para conectarse a una red WiFi.

```
const char* ssid = "noShar3";
const char* password = "holacomoestas";
```
---

Al conectarse a la red wifi el esp32 nos dará a través del puerto serial una dirección ip que es a la que debemos acceder para poder visualizar la página WEB.

En la página WEB nos encontraremos con dos botones que cambian el estado de las GPIO:
```
GPIO 26
GPIO 27
```
Debemos conectar un led en cada PIN del ESP32.

---
Para finalizar debemos accionar los botones, y veremos cómo los LEDs cambiarán su estado (ON/OFF).
