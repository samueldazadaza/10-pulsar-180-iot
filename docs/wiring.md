# Diagrama de Conexiones — Pulsar 180 IoT (v1)

## Conexión actual (v1 — solo ESP32)

```
Cargador USB 12V → 5V (de la moto)
         │
         └── USB ── ESP32 DevKit (puerto USB)
```

El ESP32 se alimenta directamente del cargador USB que ya tienes instalado en la moto.

---

## Pines reservados para futuros sensores (v2+)

| Componente         | Pin ESP32 | Tipo       | Notas                                  |
|--------------------|-----------|------------|----------------------------------------|
| Sensor gasolina    | GPIO 34   | ADC (analógico) | 0-3.3V desde el divisor de tensión   |
| Relé alarma        | GPIO 32   | Digital OUT     | Activa/alarma (NC/NO)                |
| Buzzer / sirena    | GPIO 33   | Digital OUT     | PWM para tonos                       |
| Sensor vibración   | GPIO 35   | Digital IN      | Detección de movimiento              |

---

## Sensor de gasolina (v2)

El sensor original de la Pulsar 180 GT es un flotador con resistencia variable
(~0-90 Ω vacío, ~90-100 Ω lleno).

### Circuito recomendado

```
 3.3V (ESP32)
    │
    ├── Sensor gasolina (resistencia variable)
    │       │
    │       └── GPIO 34 (lectura ADC)
    │       │
    └── Resistencia fija (120 Ω, 1%)
            │
           GND
```

### Rango esperado

| Nivel    | Resistencia | Voltaje en ADC |
|----------|-------------|----------------|
| Vacío    | ~0 Ω        | ~0.5 V         |
| Medio    | ~45 Ω       | ~1.8 V         |
| Lleno    | ~100 Ω      | ~2.8 V         |

> **Nota:** Estos valores son orientativos. Cada moto puede variar.
> Se recomienda calibrar midiendo con un multímetro.

---

## Especificaciones eléctricas

| Parámetro               | Valor              |
|-------------------------|--------------------|
| Alimentación ESP32      | 5V vía USB         |
| Corriente (v1 activo)   | ~55 mA @ 5V        |
| Corriente (desde 12V)   | ~25 mA (incluye pérdidas del cargador USB) |
| Consumo en 8 horas      | ~200 mAh desde batería 12V |
| Consumo en 24 horas     | ~600 mAh desde batería 12V |
