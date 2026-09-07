#include "ccsds.h"

void ccsds_encode_header(uint8_t *buffer, const ccsds_header_t *header) {
    // Byte 0 (VERSION 3, TIPO 1, HEADER 1, APID 3)
    buffer[0] = ((header->version & 0x07) << 5) | 
                ((header->type & 0x01) << 4) | 
                ((header->sec_hdr_flag & 0x01) << 3) | 
                ((header->apid >> 8) & 0x07);

    // Byte 1: APID LSB 8
    buffer[1] = (header->apid & 0xFF);

    //Byte 2: SEQUENCE FLAGS 2,  SEQUENCE COUNT 6
    buffer[2] = ((header->seq_flags & 0x03) << 6) | 
                ((header->seq_count >> 8) & 0x3F);

    //Byte 3: SEQUENCE COUNT 8
    buffer[3] = (header->seq_count & 0xFF);

    //Byte 4: Packet Length MSB 8
    buffer[4] = (header->length >> 8) & 0xFF;

    //Byte 5: Packet Length LSB 8
    buffer[5] = (header->length & 0xFF);
}

void ccsds_decode_header(const uint8_t *buffer, ccsds_header_t *header) {
    /* Extraer los campos aplicando máscaras y desplazamientos inversos */
    
    header->version      = (buffer[0] >> 5) & 0x07;
    header->type         = (buffer[0] >> 4) & 0x01;
    header->sec_hdr_flag = (buffer[0] >> 3) & 0x01;
    
    /* Reconstruir el APID combinando los 3 bits menos significativos del Byte 0 con el Byte 1 */
    header->apid         = ((buffer[0] & 0x07) << 8) | buffer[1];
    
    header->seq_flags    = (buffer[2] >> 6) & 0x03;
    
    /* Reconstruir el Sequence Count combinando los 6 bits menos significativos del Byte 2 con el Byte 3 */
    header->seq_count    = ((buffer[2] & 0x3F) << 8) | buffer[3];
    
    /* Reconstruir la longitud combinando Byte 4 y Byte 5 */
    header->length       = (buffer[4] << 8) | buffer[5];
}