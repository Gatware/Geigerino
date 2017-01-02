void Integrazione()
{
//    -------- Menu 1 - Tempo d'integrazione --------
lcd.clear();
lcd.print("Tempo d'integr.?");
t1=millis();
while(digitalRead(5)==LOW) // Attende che venga lasciato il pulsante.
{if(millis()-t1>999) {Bip(); lcd.clear(); Azzeramento(); return;}}
// Premuto per almeno 1 secondo (2 totali) salta a Azzeramento, poi ritorna ed esce.
t1=millis();
while(digitalRead(5)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0)  Ti+=10*E;
  if(Ti>70) {noTone(7); Ti=70;}
  if(Ti<10) {noTone(7); Ti=10;}
  if(E!=0){E=0; t1=millis(); delay(20);}
  
  lcd.setCursor(4,1);
  if(Ti<70) lcd.print(" "+String(Ti)+" sec.");
  else lcd.print("Continuo"); // Se Ti=70, integra continuamente a tempo infinito.
  if(millis()-t1>4999) return; // Dopo 5 secondi di inattività esce.
  }
delay(200);
if(Ti!=Tio) {lcd.setCursor(4,1); lcd.print(" ZERO   ");} // Se il tempo impostato è diverso dal precedente, segnala che azzera.
if(Ti!=EEPROM.read(0)) {EEPROM.update(0,Ti); Biip(); lcd.setCursor(10,1); lcd.print("  SET!"); delay(500);}
  else Bip();
if(Ti!=Tio) {Biip(); Azzera();} // Se è cambiato, azzera e fa Biip.
Tio=Ti;
}

void Mask()
{
lcd.clear();                   // byte(2): simbolo "micro"
lcd.setCursor(6,0); if(U==0) lcd.print("CPM"); else lcd.print("CPS");
lcd.setCursor(6,1); if(U==0) {lcd.write(byte(2)); lcd.print("Sv/h");} else lcd.print("mR/h");  
lcd.setCursor(14,1); if(digitalRead(4)==1) {lcd.print("A");} else {lcd.print("B");}
BattIco();
}

