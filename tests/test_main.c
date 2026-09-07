#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "ccsds.h"

int main() {
    // Configurar los datos originales de prueba 
    ccsds_header_t original = {
        .version = 0,           // Versión 1 (000)
        .type = 0,              // Telemetría
        .sec_hdr_flag = 0,      // Sin cabecera secundaria
        .apid = 1023,           // ID del proceso (Ej. sensor de temperatura: 0x3FF)
        .seq_flags = 3,         // Paquete no fragmentado (11 binario)
        .seq_count = 42,        // Paquete número 42
        .length = 15            // 16 bytes de payload (15 + 1, según el estándar)
    };

    uint8_t buffer[CCSDS_HEADER_SIZE];
    memset(buffer, 0, CCSDS_HEADER_SIZE); // Limpiar memoria por seguridad

    // Empaquetar a bytes 
    ccsds_encode_header(buffer, &original);

    // Mostrar por consola cómo se ven los bytes reales que viajarían al espacio /
    printf("Buffer generado (Hex): ");
    for(int i = 0; i < CCSDS_HEADER_SIZE; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");

    // Desempaquetar desde los bytes a una nueva estructura vacía 
    ccsds_header_t decoded;
    ccsds_decode_header(buffer, &decoded);

    // Validar que todo coincide usando aserciones
    assert(decoded.version == original.version);
    assert(decoded.type == original.type);
    assert(decoded.sec_hdr_flag == original.sec_hdr_flag);
    assert(decoded.apid == original.apid);
    assert(decoded.seq_flags == original.seq_flags);
    assert(decoded.seq_count == original.seq_count);
    assert(decoded.length == original.length);

    printf("ÉXITO: Todas las pruebas de empaquetado/desempaquetado CCSDS han pasado correctamente.\n");

    return 0;
}