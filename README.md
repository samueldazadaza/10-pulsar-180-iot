# PULSAR 180 GT — Sistema de Proximidad IoT

Sistema de proximidad para moto **Pulsar 180 GT** basado en **ESP32**. Crea una red WiFi a la que te conectas con tu teléfono; el ESP32 mide la intensidad de la señal (RSSI) para estimar qué tan cerca o lejos está la moto y lo muestra en un dashboard web.

---

## 📦 Componentes necesarios (v1)

| Componente | Propósito |
|---|---|
| **ESP32 DevKit** (la clásica de AliExpress) | Cerebro del sistema |
| Cable USB-micro | Alimentación desde el cargador USB de la moto |
| Cargador USB 12V→5V (ya lo tienes) | Convierte los ~12V de la batería a 5V para el ESP32 |
| VS Code + PlatformIO | Para programar el ESP32 |

> **Para v2+** necesitarás: resistencias, relé, buzzer, sensor de vibración.

---

## 🧠 ¿Cómo funciona?

1. El ESP32 se conecta al cargador USB de tu moto (5V estables).
2. Al encender la moto (o si el cargador está siempre conectado), el ESP32 arranca.
3. Crea una red WiFi llamada **`Pulsar-180`**.
4. Tu teléfono se conecta a esa red.
5. El ESP32 mide cada 10 segundos la intensidad de señal (RSSI) de tu teléfono conectado.
6. Abres `http://192.168.4.1` en el navegador y ves el dashboard con la distancia estimada.
7. El historial se guarda automáticamente en el almacenamiento local del teléfono (localStorage).

### Relación señal vs distancia

El ESP32 usa la siguiente fórmula para estimar metros:  
**distancia = 10 ^ ((RSSI - RSSI_AT_1M) / (-10 × PATH_LOSS_N))**

> Los parámetros `RSSI_AT_1M` (-40 dBm) y `PATH_LOSS_N` (2.5) se pueden ajustar en `config.h` para calibrar según tu entorno.

| RSSI (dBm) | Distancia aprox. | Indicador |
|---|---|---|
| -30 a -50 | 0 — 3 m | 🟢 Muy cerca |
| -50 a -65 | 3 — 10 m | 🟢 Cerca |
| -65 a -75 | 10 — 25 m | 🟡 Medio |
| -75 a -85 | 25 — 50 m | 🟠 Lejos |
| Sin conexión | > 50 m | 🔴 Sin señal |

---

## 🚀 Instalación paso a paso

### 1. Instalar VS Code + PlatformIO

1. Descarga e instala [VS Code](https://code.visualstudio.com/)
2. Abre VS Code, ve a la pestaña de extensiones (Ctrl+Shift+X)
3. Busca **PlatformIO IDE** e instálalo
4. Espera a que termine la instalación (la primera vez descarga herramientas)

### 2. Abrir el proyecto

1. En VS Code con PlatformIO, haz clic en **"Open Project"**
2. Selecciona la carpeta `firmware/` de este repositorio
3. PlatformIO detectará automáticamente la configuración

### 3. Conectar el ESP32

1. Conecta tu ESP32 a la PC mediante cable USB
2. En PlatformIO, selecciona el puerto correcto
3. Haz clic en **→ (Upload and Monitor)** en la barra inferior

### 4. Verificar que funciona

1. Abre el **Monitor Serie** (icono de enchufe en la barra inferior)
2. Deberías ver algo como:

```
=== PULSAR 180 GT - Sistema de Proximidad ===
Iniciando...
[WiFi] AP iniciado: Pulsar-180
[WiFi] IP: 192.168.4.1
[HTTP] Servidor iniciado en http://192.168.4.1
Sistema listo.
Conectate a la red WiFi: Pulsar-180
Abre el navegador en: http://192.168.4.1
```

3. Conecta tu teléfono a la red WiFi **Pulsar-180** (contraseña: `pulsar180`)
4. Abre el navegador y ve a **http://192.168.4.1**
5. Verás el dashboard en tiempo real
6. En el monitor serie verás la distancia en metros:
```
[Lectura] RSSI: -62 dBm | Clientes: 1 | Distancia: 6.3 m (Cerca)
```
6. Ejemplo de salida en monitor serie:
```
[Lectura] RSSI: -62 dBm | Clientes: 1 | Distancia: 6.3 m (Cerca)
```

---

## 📊 Dashboard Web

La página web se ve así:

```
┌──────────────────────────────────┐
│      PULSAR 180 GT                │
│  Sistema de Proximidad            │
│                                   │
│  ┌──────────────────────────┐    │
│  │    ● CERCA               │    │
│  │  ████████░░  -55 dBm     │    │
│  │  Distancia: 6.3 m        │    │
│  └──────────────────────────┘    │
│                                   │
│  ─── Proximidad ───              │
│  ●━━━━━━━━━━━━○━━━               │
│  Muy cerca    Lejos   Sin señal   │
│                                   │
│  ┌──────────────────────────┐    │
│  │ 📊 Historial de Señal     │    │
│  │                          │    │
│  │ 30 ┤╲╲             ┐ 1m │    │
│  │ 50 ┤  ╲╲╱╲╱╲      │ 6m │    │
│  │ 70 ┤     ╲╱╲╱╲    │20m │    │
│  │ 90 ┤        ╲╱╲  ┘50m │    │
│  │    └─────────────────  │    │
│  │    21:30  21:31  21:32 │    │
│  │ ─── RSSI ─── Distancia │    │
│  └──────────────────────────┘    │
│                                   │
│  Clientes: 1     Activo: 02:34    │
│                                   │
│  ┌──────────────────────────┐    │
│  │     ARMAR ALARMA          │    │
│  └──────────────────────────┘    │
│                                   │
│  Próxima: 7s · 150 registros     │
│  v1.1 — Pulsar IoT                │
└──────────────────────────────────┘
```

**Características:**
- Diseño claro y moderno, responsive para celular
- Distancia en metros (con decimales si < 10 m)
- Gráfico histórico con doble eje Y: RSSI (azul) + Distancia (verde)
- Datos guardados en **localStorage** del teléfono (no se pierden al cerrar)
- Incluye **fecha y hora** exacta de cada lectura
- Indicador de distancia con código de colores (verde/amarillo/rojo)
- Barra de señal visual
- Control deslizante de proximidad animado
- Botón "ARMAR/DESARMAR alarma" (placeholder para v2)
- Actualización automática cada 10 segundos con cuenta regresiva

---

## 🔧 Estructura del proyecto

```
pulsar-180-iot/
├── firmware/                        # Código del ESP32
│   ├── src/
│   │   ├── main.cpp                 # Punto de entrada
│   │   ├── config.h                 # Configuración (SSID, umbrales, pines)
│   │   ├── dashboard.h              # HTML + CSS + JS (embebido en flash)
│   │   ├── wifi_manager.h/.cpp      # Gestión WiFi AP + lectura RSSI
│   │   └── web_server.h/.cpp        # Servidor web + API JSON
│   ├── platformio.ini               # Configuración de PlatformIO
│   └── .gitignore
├── docs/
│   └── wiring.md                    # Diagrama de conexiones
├── test/                            # Tests (futuro)
├── scripts/                         # Scripts auxiliares (futuro)
└── README.md                        # Este archivo
```

---

## ⚡ Consumo eléctrico

| Situación | Corriente (desde batería 12V) | En 8 horas | % batería 9Ah |
|---|---|---|---|
| ESP32 activo (v1) | ~25 mA | ~200 mAh | ~2.2% |
| Cargador USB solo | ~20 mA | ~160 mAh | ~1.8% |
| **Total sistema** | ~45 mA | ~360 mAh | ~4% |

Tu batería de Pulsar 180 GT tiene típicamente **9Ah**. Con 4% de consumo en 8 horas, el arranque no se ve afectado.

> El cargador USB con voltímetro que ya tienes instalado consume **más** que el ESP32 mismo (~20 mA vs ~13 mA con optimizaciones).

---

## 🛠 Optimizaciones de bajo consumo aplicadas

| Optimización | Detalle |
|---|---|
| CPU a 80 MHz | Reduce consumo del microcontrolador |
| WiFi Modem Sleep | Pausa el WiFi entre transmisiones |
| TX Power reducido | Potencia de transmisión mínima (~10 dBm) |
| Lectura cada 10s | En lugar de tiempo real continuo |
| Delay en loop | El ESP32 no satura el CPU |

---

## 🗺 Hoja de ruta (futuras versiones)

- [x] **v1** — Dashboard WiFi con indicador de proximidad
- [x] **v1.1** — Distancia en metros + gráfico histórico + localStorage
- [ ] **v2** — Sensor de gasolina (ADC) + dashboard con gauge
- [ ] **v3** — Alarma con sensor de vibración + relé corta corriente
- [ ] **v4** — Modo sleep profundo con interrupción por movimiento
- [ ] **v5** — Notificaciones vía Telegram/IFTTT
- [ ] **v6** — GPS vía NEO-6M para seguimiento

---

## 🔌 API REST (para integraciones)

```json
GET /api/status

{
  "rssi": -55,
  "clients": 1,
  "distance": "Cerca",
  "distance_meters": 6.3,
  "uptime": 1234
}
```

| Campo | Tipo | Descripción |
|---|---|---|
| `rssi` | int | Intensidad de señal en dBm |
| `clients` | int | Número de teléfonos conectados |
| `distance` | string | Etiqueta de distancia |
| `distance_meters` | float | Distancia estimada en metros (-1 si sin conexión) |
| `uptime` | int | Segundos desde el encendido |

---

## ❓ Preguntas frecuentes

### ¿Se puede dejar conectado 24/7 a la batería?
Sí. El consumo total (cargador USB + ESP32) es de ~45 mA, equivalente a ~1Ah en 24h (~11% de una batería de 9Ah). Si la usas a diario, la batería se recarga con el alternador.

### ¿El ESP32 no se daña con las vibraciones de la moto?
Se recomienda montarlo en espuma anti-vibración o en una caja estanca con soporte de goma. Las vibraciones extremas podrían aflojar conexiones.

### ¿Qué pasa si no hay teléfono conectado?
El dashboard muestra "Sin conexión" y la barra de señal desaparece. El ESP32 sigue funcionando normalmente.

### ¿Puedo cambiar el nombre de la red WiFi?
Sí, edita `WIFI_SSID` en `firmware/src/config.h` y vuelve a flashear.

---

## 📝 Licencia

Este proyecto es de uso personal y educativo. Siéntete libre de modificarlo y mejorarlo.

---

Hecho con ⚡ para la comunidad Pulsar 180 GT
