# CCSDS Space Packet Protocol - Telemetry Encoder/Decoder

Este repositorio contiene una implementación ligera y robusta de la cabecera primaria del **CCSDS (Consultative Committee for Space Data Systems)** Space Packet Protocol, un estándar de comunicaciones fundamental en el software de vuelo y estaciones terrenas de satélites.

El proyecto está diseñado específicamente para entornos embebidos (sistemas *bare-metal* o RTOS) y demuestra prácticas de programación segura a bajo nivel.

## Características Técnicas Clave

* **Independencia de Endianness (Endian-agnostic):** En lugar de utilizar *bit-fields* de C (cuyo comportamiento en memoria depende del compilador y la arquitectura del microcontrolador), el empaquetado se realiza estrictamente mediante operaciones de desplazamiento y máscaras bit a bit (`<<`, `>>`, `&`, `|`). Esto garantiza que los datos siempre se transmitan en el formato **Big-Endian** exigido por el estándar CCSDS, independientemente de si se ejecuta en una CPU Little-Endian (como la familia ARM Cortex-M).
* **Eficiencia y Alineación:** La estructura no requiere empaquetado forzado del compilador (`__attribute__((packed))`), evitando penalizaciones de rendimiento por desalineación de memoria en arquitecturas restrictivas.
* **Validación Integrada:** Incluye pruebas unitarias básicas para garantizar la integridad de los datos en el ciclo de codificación y decodificación.
* **Compilación Estricta:** Se exigen los flags `-Wall -Wextra -std=c99` para asegurar código libre de advertencias (*warnings*).

## Estructura del Directorio

```text
.
├── src/
│   ├── ccsds.h       # Definiciones de la estructura y firmas de funciones
│   └── ccsds.c       # Lógica de codificación/decodificación bit a bit
├── tests/
│   └── test_main.c   # Pruebas unitarias automatizadas
└── Makefile          # Scripts de compilación
