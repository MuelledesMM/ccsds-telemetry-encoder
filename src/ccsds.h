#ifndef CCSDS_H
#define CCSDS_H

#include <stdint.h>

#define CCSDS_HEADER_SIZE 6

/* ESTRUCTURA CCSDS
 * No bit-fields para evitar problemas de Endianness. */
typedef struct {
    uint8_t  version;       /* 3 bits: Versión del paquete (Normalmente 0) */
    uint8_t  type;          /* 1 bit: 0 = Telemetría, 1 = Telecomando */
    uint8_t  sec_hdr_flag;  /* 1 bit: 1 = Cabecera secundaria presente, 0 = Ausente */
    uint16_t apid;          /* 11 bits: Application Process ID (Identificador de la aplicación) */
    uint8_t  seq_flags;     /* 2 bits: Sequence flags (Normalmente 3 o 0b11 para paquete no fragmentado) */
    uint16_t seq_count;     /* 14 bits: Sequence count (Contador de paquetes) */
    uint16_t length;        /* 16 bits: Tamaño del payload en bytes - 1 */
} ccsds_header_t;

/**
 * Empaqueta una estructura ccsds_header_t en un buffer de bytes (Big-Endian).
 * 
 * @param buffer Puntero al array de salida (debe tener al menos 6 bytes de espacio).
 * @param header Puntero a la estructura con los datos de la cabecera.
 */
void ccsds_encode_header(uint8_t *buffer, const ccsds_header_t *header);

/**
 * Desempaqueta un buffer de bytes (Big-Endian) en una estructura ccsds_header_t.
 * 
 * @param buffer Puntero al array de entrada (debe contener los 6 bytes de la cabecera).
 * @param header Puntero a la estructura donde se guardarán los datos parseados.
 */
void ccsds_decode_header(const uint8_t *buffer, ccsds_header_t *header);

#endif /* CCSDS_H */