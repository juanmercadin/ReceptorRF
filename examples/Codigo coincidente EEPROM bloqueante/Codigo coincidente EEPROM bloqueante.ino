/*
Este programa permite activar o desactivar una salida luego de validarse
el código a partir de la recepción de un código válido previamente
almacenado en la EEPROM, se utiliza el led incorporado en la placa de Arduino
UNO para visualizar el estado de la activación. Este programa bloquea la recepción
para nuevos códigos para evitar el efecto rebote.
*/
#include <ReceptorRF.h>
#include "EEPROM.h"    //Librería para almacenar y leer datos en la Eeprom del micro

// Definiciones

#define PIN_DATOS 12 // Pin donde está conectado el receptor RF
#define CANT_BITS 24 // Cantidad de bits de la trama (puedes ajustar este valor según tu necesidad)
#define LED 13 // Led onboard de Arduino UNO
#define BOTON 11 // Boton para grabar un código en la EEPROM
#define INTERVALO 2000 // Tiempo de espera que bloquea al receptor entre cada recepción de código

// Variables
uint8_t codigo[3]; // Arreglo para almacenar el código recibido, para 24 bits son 3 bytes
uint8_t codigoValido[3]; // Vector donde guardo el código válido
bool activacion = false; // Bandera para utilizar cuando se debe activar o desactivar
bool deshabilitado = false; // Bandera para indicar que se deshabilito el receptor
long tiempoAnterior; // Variable para guardar los tiempos de espera

// Crear un objeto receptor de la clase ReceptorRF
ReceptorRF receptor(PIN_DATOS, CANT_BITS);

// Declaracion de los prototipos de funciones
bool compararCodigos (byte codigo1[], byte codigo2[]);

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BOTON, INPUT_PULLUP);
  receptor.configurarRF(); // Configurar el receptor RF
  for (int i = 0; i < 3; i++){ // Cargo los bytes guardado en la EEPROM
    codigoValido[i] = EEPROM.read(i);
  }
  Serial.begin(9600);
}

void loop() {
  if (receptor.obtenerCodigo(codigo)) { // Intentar obtener un código del receptor
    if (compararCodigos(codigoValido, codigo)){
      activacion = !activacion;
      receptor.deshabilitarRF();
      deshabilitado = true;
      tiempoAnterior = millis();
    }
  }
  if (deshabilitado){ // Se vuelve a habilitar luego de transcurrido el tiempo establecido en el intervalo
    if ((millis() - tiempoAnterior) > INTERVALO){
      receptor.habilitarRF();
      deshabilitado = false;
    }
  }
  if (activacion){ // Prendo o apago el led si el código recibido es válido
    digitalWrite(LED, HIGH);
  }
  else{
    digitalWrite(LED, LOW);
  }
  if (digitalRead(BOTON) == false){
    while(!receptor.obtenerCodigo(codigoValido)){ // Obtengo el código a guardar en la EEPROM
      Serial.println("Esperando");
    } 
    for (int i = 0; i < 3; i++){ // Guardo los bytes en las primeras posiciones de la EEPROM
      EEPROM.put(i, codigoValido[i]);
    }
    Serial.println("Grabado");
  }
}

bool compararCodigos (byte codigo1[], byte codigo2[]){ // Función para comparar ambos códigos
  if(codigo1[0] != codigo2[0]) return false;
  if(codigo1[1] != codigo2[1]) return false;
  if(codigo1[2] != codigo2[2]) return false;
  return true;
}