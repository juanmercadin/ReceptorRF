/*
Este programa básico bloqueante permite obtener el código en hexadecimal y en 
binario de cualquier control remoto que cumpla con las caracteristicas de tener
la cantidad de bits declarados para su trama, una vez recibido el código, el 
receptor se deshabilita por 2 segundos, bloqueando la posibilidad de que se
puedan recibir nuevos códigos desde el receptor. Por defecto se establece la trama
en 24 bits y el pin de ingreso de datos el número 13, estos valores pueden ser
modificados por el usuario a su conveniencia. La libreria no funciona con
interrupciones por lo que se puede usar cualquier pin del Arduino disponible.
*/
#include <ReceptorRF.h>

// Definiciones

#define PIN_DATOS 13 // Pin donde está conectado el receptor RF
#define CANT_BITS 24 // Cantidad de bits de la trama (puedes ajustar este valor según tu necesidad)
#define INTERVALO 2000 // Tiempo de espera que bloquea al receptor entre cada recepción de código

// Variables
uint8_t codigo[3]; // Arreglo para almacenar el código recibido, para 24 bits son 3 bytes
bool deshabilitado = false; // Bandera para indicar que se deshabilito el receptor
long tiempoAnterior; // Variable para guardar los tiempos de espera

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
    receptor.deshabilitarRF();
    deshabilitado = true;
    tiempoAnterior = millis();
  }
  if (deshabilitado){ // Se vuelve a habilitar luego de transcurrido el tiempo establecido en el intervalo
    if ((millis() - tiempoAnterior) > INTERVALO){
      receptor.habilitarRF();
      deshabilitado = false;
    }
  }
}