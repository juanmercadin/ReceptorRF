//Proyecto completo en: https://github.com/juanmercadin/ReceptorRF.git

#include <ReceptorRF.h>

// Definiciones

#define PIN_DATOS 13 // Pin donde está conectado el receptor RF
#define CANT_BITS 24 // Cantidad de bits de la trama (puedes ajustar este valor según tu necesidad)

// Variables
uint8_t codigo[3]; // Arreglo para almacenar el código recibido para 24 bits son 3 bytes

// Crear un objeto receptor de la clase ReceptorRF
ReceptorRF receptor(PIN_DATOS, CANT_BITS);

void setup() {
  Serial.begin(9600); // Iniciar comunicación serial para depuración
  receptor.configurarRF(); // Configurar el receptor RF
  Serial.println("Receptor configurado. Esperando códigos...");
}

void loop() {
  if (receptor.obtenerCodigo(codigo)) { // Intentar obtener un código del receptor
    Serial.print("Código recibido en hexadecimal: ");
    for (uint8_t i = 0; i < 3; i++) {
      Serial.print(codigo[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
    Serial.print("Código recibido en binario: ");
    for (uint8_t i = 0; i < 3; i++) {
      Serial.print(codigo[i], BIN);
    }
    Serial.println();
  }
}