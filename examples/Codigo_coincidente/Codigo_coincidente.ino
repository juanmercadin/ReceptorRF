/*
Este programa permite activar o desactivar una salida luego de validarse
el código a partir de la recepción de un código válido previamente
almacenado en el código, se utiliza el led incorporado en la placa de Arduino
UNO para visualizar el estado de la activación. Este programa carece de algún
mecanismo para evitar el efecto rebote.
*/
#include <ReceptorRF.h>

// Definiciones

#define PIN_DATOS 12 // Pin donde está conectado el receptor RF
#define CANT_BITS 24 // Cantidad de bits de la trama (puedes ajustar este valor según tu necesidad)
#define LED 13 // Led onboard de Arduino UNO

// Variables
uint8_t codigo[3]; // Arreglo para almacenar el código recibido, para 24 bits son 3 bytes
const uint8_t codigoValido[3] = {0xF7, 0xD8, 0xA8}; // Código válido para activar/desactivar
bool activacion = false; // Bandera para utilizar cuando se debe activar o desactivar

// Crear un objeto receptor de la clase ReceptorRF
ReceptorRF receptor(PIN_DATOS, CANT_BITS);

// Declaracion de los prototipos de funciones
bool compararCodigos (byte codigo1[], byte codigo2[]);

void setup() {
  pinMode(LED, OUTPUT);
  receptor.configurarRF(); // Configurar el receptor RF
}

void loop() {
  if (receptor.obtenerCodigo(codigo)) { // Intentar obtener un código del receptor
    if (compararCodigos(codigoValido, codigo)){
      activacion = !activacion;
    }
  }
  if (activacion){ // Prendo o apago el led si el código recibido es válido
    digitalWrite(LED, HIGH);
  }
  else{
    digitalWrite(LED, LOW);
  }
}

bool compararCodigos (byte codigo1[], byte codigo2[]){ // Función para comparar ambos códigos
  if(codigo1[0] != codigo2[0]) return false;
  if(codigo1[1] != codigo2[1]) return false;
  if(codigo1[2] != codigo2[2]) return false;
  return true;
}