# BadUSB — Proof of Concept Educativo

> Aviso legal: Este proyecto fue desarrollado exclusivamente con fines educativos en el marco de la materia Seguridad e Integridad de la Universidad Argentina de la Empresa (UADE). Todos los experimentos fueron realizados en entornos controlados y con autorización explícita. El uso de estas técnicas fuera de un entorno autorizado puede constituir un delito bajo la Ley 26.388 (Delitos Informáticos) de la República Argentina.

---

## Descripción

Proyecto de investigación sobre ataques BadUSB basado en el microcontrolador ESP32-S3, que actúa como dispositivo HID (Human Interface Device) malicioso al ser reconocido por el sistema operativo como un teclado USB legítimo.

El repositorio contiene tres payloads independientes, cada uno diseñado para demostrar una técnica de ataque distinta mediante inyección de keystrokes vía PowerShell.

---

## Hardware utilizado

- Microcontrolador: ESP32-S3
- Framework: Arduino (libraries USB.h y USBHIDKeyboard.h)
- Conexión: USB-A — el dispositivo se enumera como teclado HID ante el sistema operativo

---

## Estructura del repositorio

badusb-uade/
├── esp32s3-badusb-wifi/          # Payload 1 — Reconocimiento del sistema
├── esp32s3-badusb-phishing/      # Payload 2 — Phishing de credenciales
├── esp32s3-badusb-robodocumento/ # Payload 3 — Búsqueda y exfiltración de archivos
└── README.md

---

## Payloads

### 1. Reconocimiento del sistema (wifi)

Recolecta información del host comprometido y la exfiltra a un servidor remoto.

Datos recopilados:
- Nombre y arquitectura del sistema operativo (Get-ComputerInfo)
- Direcciones IPv4 activas (Get-NetIPAddress)
- Puertos TCP en estado LISTEN (Get-NetTCPConnection)

Flujo:
1. Abre PowerShell vía Win + R
2. Escribe los datos en un archivo temporal
3. Realiza un HTTP POST con el archivo al servidor
4. Elimina el archivo del disco

---

### 2. Phishing de credenciales (phishing)

Genera una ventana de inicio de sesión falsa mediante System.Windows.Forms, simulando un prompt legítimo del sistema.

Flujo:
1. Abre PowerShell vía Win + R
2. Escribe un script .ps1 en %TEMP% y lo ejecuta en background (-WindowStyle Hidden)
3. Muestra un formulario con campos User y Password
4. Al presionar OK, envía las credenciales vía HTTP POST a un endpoint remoto

---

### 3. Exfiltración de archivos (robodocumento)

Busca archivos que coincidan con un patrón de nombre específico en el directorio del usuario y los sube a un servidor remoto.

Flujo:
1. Abre PowerShell vía Win + R
2. Recorre $env:USERPROFILE recursivamente con Get-ChildItem
3. Sube cada archivo encontrado vía HTTP multipart POST usando System.Net.Http.HttpClient

---

## Técnica común — HID Injection

Todos los payloads comparten la misma base técnica:

ESP32-S3 conectado por USB
        ↓
Sistema operativo lo reconoce como teclado HID
        ↓
El dispositivo "tipea" comandos a velocidad de máquina
        ↓
PowerShell ejecuta el payload antes de que el usuario reaccione

La función tipearSeguro() envía los caracteres con un delay(1ms) entre cada uno para evitar pérdida de caracteres en sistemas lentos.

---

## MITRE ATT&CK Mapping

| Técnica                                              | ID         | Descripción                                          |
|------------------------------------------------------|------------|------------------------------------------------------|
| Hardware Additions                                   | T1200      | Dispositivo USB presentado como teclado HID legítimo |
| Command and Scripting Interpreter: PowerShell        | T1059.001  | Ejecución de comandos vía PowerShell inyectado       |
| System Information Discovery                         | T1082      | Recolección de OS, IPs y puertos abiertos            |
| Credentials from Password Stores / Input Capture     | T1056      | Formulario falso para captura de credenciales        |
| Exfiltration Over C2 Channel                         | T1041      | Envío de datos a servidor externo vía HTTP POST      |
| Data from Local System                               | T1005      | Búsqueda y lectura de archivos por nombre            |

---

## Impacto — Tríada CIA

| Pilar             | Impacto | Detalle                                                                 |
|-------------------|---------|-------------------------------------------------------------------------|
| Confidencialidad  | Alto    | Credenciales, archivos sensibles e información del sistema expuestos    |
| Integridad        | Medio   | El script es escrito y ejecutado en disco sin consentimiento del usuario |
| Disponibilidad    | Bajo    | El ataque no interrumpe el funcionamiento del sistema                   |

Controles afectados según ISO/IEC 27001: A.9 (Control de acceso), A.12 (Seguridad operacional), A.14 (Adquisición y mantenimiento de sistemas).

---

## Contramedidas

- Deshabilitar la ejecución automática de dispositivos HID no autorizados (USB lockdown policies)
- Aplicar políticas de ExecutionPolicy restrictivas en PowerShell
- Implementar control de acceso físico a puertos USB (soluciones tipo USB Guard)
- Monitorear procesos powershell.exe lanzados desde explorer.exe o rundll32.exe
- Concientización de usuarios sobre dispositivos USB de origen desconocido

---

## Contexto académico

Proyecto desarrollado para la materia Seguridad e Integridad — UADE
Año: 2026
