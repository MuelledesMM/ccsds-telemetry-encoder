#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ccsds.h"


// Configuración de la Capa de Aplicación
#define SENSOR_PAYLOAD_SIZE 2 // 2 bytes para un valor analógico de 16 bits
#define TOTAL_PACKET_SIZE (CCSDS_HEADER_SIZE + SENSOR_PAYLOAD_SIZE) // 8 bytes totales

/**
 * @brief Simula la lectura física de un sensor de temperatura (ej. mediante I2C).
 * Devuelve un valor crudo de 16 bits sin procesar.
 */
uint16_t read_hardware_sensor_register(void) {
    static uint16_t mock_raw_temp = 2500; // Valor inicial simulado (ej. 25.00 ºC)
    mock_raw_temp += 3; // Simula una pequeña variación en la lectura del sensor
    return mock_raw_temp;
}

/**
 * @brief Función envolvente (Wrapper) de la aplicación.
 * Conecta de forma limpia la adquisición del sensor con tu librería de cabeceras CCSDS,
 * automatizando el empaquetado completo sin tocar el código base del protocolo.
 */
void build_sensor_telemetry_packet(uint8_t *out_buffer, uint16_t seq_counter, uint16_t sensor_val) {
    // 1. Configurar los metadatos de la cabecera espacial
    ccsds_header_t telemetry_header = {
        .version = 0,
        .type = 0,            // 0 = Paquete de Telemetría
        .sec_hdr_flag = 0,    // Sin cabecera secundaria
        .apid = 1023,         // ID único del proceso (Sensor de Temperatura)
        .seq_flags = 3,       // Paquete completo no fragmentado (11 binario)
        .seq_count = seq_counter, // Contador autoincremental de secuencia
        .length = SENSOR_PAYLOAD_SIZE - 1 // Regla CCSDS: (Tamaño del payload en bytes - 1)
    };

    // 2. Llamar a TU librería intacta para escribir la cabecera en los primeros 6 bytes
    ccsds_encode_header(out_buffer, &telemetry_header);

    // 3. Añadir automáticamente el payload (datos del sensor) justo a continuación (Bytes 6 y 7)
    // Se utiliza formato Big-Endian, estándar en comunicaciones espaciales.
    out_buffer[6] = (uint8_t)(sensor_val >> 8);   // Byte alto del sensor
    out_buffer[7] = (uint8_t)(sensor_val & 0xFF); // Byte bajo del sensor
}

int main() {
    uint8_t tx_buffer[TOTAL_PACKET_SIZE];
    uint16_t sequence_number = 0;

    printf("=== SIMULADOR DE TELEMETRÍA ESPACIAL (Librería CCSDS + Capa Sensor) ===\n\n");

    // Simulamos un bucle periódico de adquisición (como el que ejecutaría un microcontrolador)
    for (int cycle = 1; cycle <= 3; cycle++) {
        // Paso 1: Leer el registro físico del sensor en crudo
        uint16_t raw_temp = read_hardware_sensor_register();

        // Paso 2: Generar la trama unificada de forma automática con el wrapper
        build_sensor_telemetry_packet(tx_buffer, sequence_number, raw_temp);

        // Paso 3: Visualizar el resultado listo para transmitir por la antena o bus de datos
        printf("[Ciclo de Adquisición %d]\n", cycle);
        printf("  -> Valor crudo obtenido del sensor: 0x%04X (%u)\n", raw_temp, raw_temp);
        printf("  -> Trama binaria completa en Hex:   ");
        for (int i = 0; i < TOTAL_PACKET_SIZE; i++) {
            printf("%02X ", tx_buffer[i]);
        }
        printf("\n  -> Desglose estructural:          [ 6 bytes Cabecera CCSDS ] + [ 2 bytes Payload Sensor ]\n\n");

        // Incrementar el contador de secuencia espacial para la siguiente iteración
        sequence_number++;
    }

    return 0;
}