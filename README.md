Librería ReceptorRF

Librería para poder recibir códigos de controles remotos RF o IR de tipo código fijo de 12, 24 u otra cantidad de bits. Por defecto la cantidad de bits esta establecida en 24 bits.

CONEXIONES

Por defecto se establece el pin 13 para Arduino UNO, pero este pin se puede cambiar según se desee al momento de declarar el objeto ReceptorRF.

FUNCIONES

configurarRF()

Configura el pin de entrada de datos del receptor con el valor declarado en el objeto ReceptorRF.

obtenerCodigo(codigo)

Retorna el valor  "0"  si no encuentra ningún código válido o "1" si encuentra un código válido, en caso de encontrar un código válido de un control remoto, lo entrega en una array que debe tener la cantidad de bytes necesarios para poder almacenar el código según sea la cantidad de bit que contenga la trama (12, 24 u otra cantidad).

deshabilitarRF()

Deshabilita el recptor para que no pueda seguir leyendo códigos. Cuando se encuentra deshabilitado el receptor la función obtenerCodigo() devuelve el valor "0".

habilitarRF()

Habilita el receptor para que pueda volver a leer códigos.

LICENCIA

Esta librería fue desarrollada por el Ing. Juan Manuel Mercadin, y la misma puede ser copiada, modificada, adaptada y utilizada por cualquier persona que así lo desee.
