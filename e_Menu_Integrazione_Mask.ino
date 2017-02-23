void Menu()
{
lcd.clear(); lcd.print("> "+String(voce[cv]));
while(digitalRead(5)==LOW) // Attende che venga lasciato il pulsante.
{delay(200);}
t1=millis();
while(digitalRead(5)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0) {cv+=E; E=0; t1=millis(); delay(20);}
  if(cv>6) {noTone(7); cv=6;}
  if(cv<1) {noTone(7); cv=1;}
  lcd.setCursor(2,0); lcd.print (voce[cv]); lcd.setCursor(4,1); lcd.print (voce[cv+1]);
  if(millis()-t1>4999) return; // Dopo 5 secondi di inattività esce.
  }
delay(200);
switch(cv)
  {
  case 1: Integrazione(); break;
  case 2: Azzeramento(); break;
  case 3: massimi(); break;
  case 4: suoni(); break;
  case 5: allarme(); break;
  case 6: retroillum(); break;
  }
}

void Integrazione()
{
//    -------- Tempo d'integrazione --------
lcd.clear();
lcd.print(F("Tempo d'integr.?"));
t1=millis();
while(digitalRead(5)==LOW) // Attende che venga lasciato il pulsante.
{if(millis()-t1>999) {Bip(); lcd.clear(); Azzeramento(); return;}}
// Premuto per almeno 1 secondo (2 totali) salta a Azzeramento, poi ritorna ed esce.
t1=millis();
while(digitalRead(5)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0)  Ti+=10*E;
  if(Ti>TMAX) {noTone(7); Ti=TMAX;}
  if(Ti<10) {noTone(7); Ti=10;}
  if(E!=0){E=0; t1=millis(); delay(20);}
  
  lcd.setCursor(3,1);
  if(Ti<TMAX) lcd.print(" "+String(Ti)+" sec. ");
  else lcd.print(F(" Continuo")); // Se Ti=TMAX, integra continuamente a tempo infinito.
  if(millis()-t1>4999) return; // Dopo 5 secondi di inattività esce.
  }
delay(200);
if(Ti!=Tio) {lcd.setCursor(4,1); lcd.print(" ZERO   ");} // Se il tempo impostato è diverso dal precedente, segnala che azzera.
if(Ti!=EEPROM.read(0)+EEPROM.read(15)*256) {EEPROM.update(0,Ti&0xFF); EEPROM.update(15,Ti>>8); Biip(); lcd.setCursor(10,1); lcd.print("  SET!"); delay(500);}
  else Bip();
if(Ti!=Tio) {Biip(); Azzera();} // Se è cambiato, azzera e fa Biip.
Tio=Ti;
}

void Mask()
{
lcd.clear();                   // byte(2): simbolo "micro"
lcd.setCursor(6,0); if(U==0) lcd.print("cpm"); else lcd.print("cps");
lcd.setCursor(6,1); if(U==0) {lcd.write(byte(2)); lcd.print("Sv/h  ");} else lcd.print("mR/h");  
lcd.setCursor(14,1); if(digitalRead(4)==1) {lcd.print("A");} else {lcd.print("B");}
BattIco();
}



