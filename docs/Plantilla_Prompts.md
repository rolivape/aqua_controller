**Contexto del Proyecto (Breve)**: AquaControllerUSB_v5.2 (ESP-IDF v5.5, ESP32-S3). Sigue documento maestro v5.2, mapa componentes/APIs, Reglas Diseño (Capítulo 9), Anexo 10.5 librerías válidas. Optimiza IRAM <80%, NVS configs, MQTT/NCM, SAFE MODE autónomo. Anti-patrones: No nube/Wi-Fi crítico, no dependencias pesadas.

**Tarea Específica**: [Breve, e.g., Genera config_aq.c para NVS init y roles].

**Requisitos Clave** (Lista numerada, max 5):
1. Librerías: [e.g., nvs_flash nativo].
2. Diseño: [e.g., /components/config_aq/, sufijo _aq, no hardcode].
3. Optimizaciones: [e.g., PSRAM buffers, reconexión <5s].
4. APIs: [Lista funciones].
5. Integración/Testing: [e.g., Llama desde main.c; stubs Unity].

**Auto-Validación Requerida**:
- Confirma cumplimiento IRAM, v5.5 compatibilidad, y anti-patrones.
- Simula output idf.py size (conceptual).

**Salida Esperada**:
- Código C completo (#includes, funciones, error handling).
- Comentarios.
- Verificación final: [Explica cumplimiento].

**Referencias**: [Enlaces docs.espressif.com específicos].

Genera y confirma.
