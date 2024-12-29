#include "ReceptorRF.h"

ReceptorRF::ReceptorRF(uint8_t datos, uint8_t tramaBits)
    : _datos(datos), _tramaBits(tramaBits), _estado(LIBRE), _cantBits(0), _tiempoBit(0), _tiempoCero(0), _tiempoEntreTramas(0) {
    _codigo = new uint8_t[(tramaBits + 7) / 8](); // Asignar suficiente memoria para los bits de la trama
}

ReceptorRF::~ReceptorRF() {
    delete[] _codigo; // Liberar memoria asignada
}

void ReceptorRF::configurarRF() {
    pinMode(_datos, INPUT);
    _estado = LIBRE;
}

int ReceptorRF::obtenerCodigo(uint8_t* codigo) {
    while (_estado != CODIGO_OK && _estado != CODIGO_ERROR) {
        switch (_estado) {
            case LIBRE: {
                _tiempoEntreTramas = 0;
                _cantBits = 0;
                memset(_codigo, 0, (_tramaBits + 7) / 8);
                _estado = ESPACIO_TRAMA;
		
            } break;

            case ESPACIO_TRAMA: {
                for (uint8_t i = 0; i < _tramaBits; i++) {
                    while (digitalRead(_datos) == HIGH);
                    unsigned long tiempoInicial = micros();
                    while (digitalRead(_datos) == LOW);
                    unsigned long anchoPulso = micros() - tiempoInicial;
                    if (_tiempoEntreTramas < anchoPulso) {
                        _tiempoEntreTramas = anchoPulso;
                    }
                }
		for (int i = 0; i < _tramaBits; i++) {
        	    while (digitalRead(_datos) == HIGH);
        	    unsigned long tiempoInicial = micros();
        	    while (digitalRead(_datos) == LOW);
        	    unsigned long anchoPulso = micros() - tiempoInicial;
        	    if (((_tiempoEntreTramas - TOLERANCIA) < anchoPulso) && (anchoPulso < (_tiempoEntreTramas + TOLERANCIA))) {
            	    	_estado = CONTAR_BITS;
            	    	break; // Salir del bucle
        	    }
        	    if (i == _tramaBits - 1) {
            		_estado = CODIGO_ERROR;
        	    }
		}
            } break;

            case CONTAR_BITS: {
		unsigned long tiempoInicial = micros();
    		while (digitalRead(_datos) == HIGH);
    		while (digitalRead(_datos) == LOW);
    		_tiempoBit = micros() - tiempoInicial;
    		if (_tiempoBit < MAX_TIEMPO_BIT) {
        	    _tiempoBit /= 2;
    		} else {
        	    _cantBits = _tramaBits;
    		}
		unsigned long anchoPulso;
    		do {
        	    while (digitalRead(_datos) == HIGH);
        	    unsigned long tiempoInicial = micros(); // Reiniciar el temporizador
        	    while (digitalRead(_datos) == LOW);
        	    anchoPulso = micros() - tiempoInicial;
        	    _cantBits++;
    		} while (((_tiempoEntreTramas - TOLERANCIA) > anchoPulso || anchoPulso > (_tiempoEntreTramas + TOLERANCIA)) && _cantBits < _tramaBits);
    		if (_cantBits == _tramaBits) {
        	    _estado = DECODIFICAR;
    		} else {
        	    _estado = CODIGO_ERROR;
    		}
            } break;

            case DECODIFICAR: {
                for (uint8_t i = 0; i < _cantBits; i++) {
                    while (digitalRead(_datos) == HIGH);
                    unsigned long tiempoInicial = micros();
                    while (digitalRead(_datos) == LOW);
                    _tiempoCero = micros() - tiempoInicial;

                    if (_tiempoCero < MAX_TIEMPO_CERO) {
                        uint8_t bitValue = (_tiempoCero > _tiempoBit) ? 0 : 1;
                        _codigo[i / 8] |= (bitValue << (7 - (i % 8)));
                    } else {
                        _estado = CODIGO_ERROR;
                        break;
                    }

                    if (i == _cantBits - 1) {
                        _estado = CODIGO_OK;
                    }
                }
            } break;

	    case OCUPADO: {
		return 0;
	    }

            default: {
                _estado = LIBRE;
            }
        }
    }

    if (_estado == CODIGO_OK) {
        memcpy(codigo, _codigo, (_tramaBits + 7) / 8);
        _estado = LIBRE;
        return 1;
    }

    if (_estado == CODIGO_ERROR) {
        _estado = LIBRE;
        return 0;
    }

    return 0;
}

void ReceptorRF::deshabilitarRF() {
    _estado = OCUPADO;
}

void ReceptorRF::habilitarRF() {
    _estado = LIBRE;
}
