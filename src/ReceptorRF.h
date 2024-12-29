#ifndef RECEPTORRF_H
#define RECEPTORRF_H

#include <Arduino.h>

class ReceptorRF {
public:
    // Constructor: permite especificar el pin de datos y la cantidad de bits de la trama
    ReceptorRF(uint8_t datos = 13, uint8_t tramaBits = 24);

    // Destructor: libera memoria asignada dinámicamente
    ~ReceptorRF();

    // Configura el receptor
    void configurarRF();

    // Obtiene el código decodificado; retorna 1 si tiene éxito, 0 en caso de error
    int obtenerCodigo(uint8_t* codigo);

    // Deshabilita el receptor
    void deshabilitarRF();

    // Habilita el receptor
    void habilitarRF();

private:
    uint8_t _datos;         		// Pin donde se conecta la salida del receptor
    uint8_t _tramaBits;       		// Cantidad de bits de la trama (12, 24 u otra)
    uint8_t* _codigo;         		// Almacena los bits del código decodificado
    uint8_t _estado;          		// Estado actual del receptor
    uint8_t _cantBits;        		// Cantidad de bits leídos
    unsigned long _tiempoBit; 		// Tiempo entre bits
    unsigned long _tiempoCero;		// Tiempo de un nivel lógico bajo
    unsigned long _tiempoEntreTramas; 	// Tiempo entre tramas

    // definiciones
    #define MAX_TIEMPO_BIT 2500
    #define MAX_TIEMPO_CERO 2000
    #define TOLERANCIA 100

    // Estados del receptor
    enum {
        LIBRE,
        ESPACIO_TRAMA,
        CONTAR_BITS,
        DECODIFICAR,
        CODIGO_OK,
        CODIGO_ERROR,
        OCUPADO
    };
};

#endif // RECEPTORRF_H
