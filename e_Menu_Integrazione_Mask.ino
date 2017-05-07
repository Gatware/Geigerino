void Menu()
{
lcd.clear(); lcd.print("> "+String(voce[cv]));
while(!(PIND&0x20)) // Attende che venga lasciato il pulsante.
{delay(200);}
t1=millis();
while(PIND&0x20) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0) {cv+=E; E=0; t1=millis(); delay(20);}
  if(cv>8) {noTone(7); cv=8;}
  if(cv<1) {noTone(7); cv=1;}
  lcd.setCursor(2,0); lcd.print (voce[cv]); lcd.setCursor(4,1); lcd.print (voce[cv+1]);
  if(millis()-t1>4999) return; // Dopo 5 secondi di inattività esce.
  }
delay(200); lcd.clear();
switch(cv)
  {
  case 1: Integrazione(); break;
  case 2: Azzeramento(); break;
  case 3: massimi(); break;
  case 4: suoni(); break;
  case 5: allarme(); break;
  case 6: retroillum(); break;
  case 7: precisione(); break;
  case 8: autonomia(); break;
  }
}

void Integrazione()
{
//    -------- Tempo d'integrazione --------
lcd.print(F("Tempo d'integr.?"));
t1=millis();
while(!(PIND&0x20)){delay(200);} // Attende che venga lasciato il pulsante.
{if(millis()-t1>999) {Bip(); lcd.clear(); Azzeramento(); return;}}
// Premuto per almeno 1 secondo (2 totali) salta a Azzeramento, poi ritorna ed esce.
t1=millis();
while(PIND&0x20) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0)  Ti+=10*E;
  if(Ti>TMAX+10) {noTone(7); Ti=10;}
  if(Ti<10) {noTone(7); Ti=TMAX+10;}
  if(E!=0){E=0; t1=millis(); delay(20);}
  
  lcd.setCursor(0,1);
  if(Ti<TMAX) {lcd.print("    "+String(Ti)+" sec.     "); Disp2=0;}
  else if(Ti==TMAX) {lcd.print(F("    Continuo    ")); Disp2=0;} // Se Ti=TMAX, integra continuamente a tempo infinito.
  else {lcd.print(F("Precisione fissa")); Disp2=1;}
  if(millis()-t1>4999) return; // Dopo 5 secondi di inattività esce.
  }
delay(200);
if(Ti>TMAX) {lcd.setCursor(0,1); lcd.print("       "+String(prec)+"%       "); delay(1200);}
if(Ti!=Tio) {lcd.setCursor(4,1); lcd.print(" ZERO   ");} // Se il tempo impostato è diverso dal precedente, segnala che azzera.
if(Ti!=EEPROM.read(0)+EEPROM.read(15)*256) {EEPROM.update(0,Ti&0xFF); EEPROM.update(15,Ti>>8); Biip(); lcd.setCursor(10,1); lcd.print("  SET!"); delay(500);}
  else Bip();
if(Ti!=Tio) {Biip(); Azzera();} // Se è cambiato, azzera e fa Biip.
Tio=Ti;
}

void Mask()
{
lcd.clear();                   // byte(2): simbolo "micro"
lcd.setCursor(6,0); if(U==0) lcd.print("c"); lcd.write(4); lcd.print(" "); // else lcd.print("cps"); // 4: piemme
lcd.setCursor(6,1); if(U==0) {lcd.write(byte(2)); lcd.write(byte(5)); lcd.write(byte(6));} // else lcd.print("mR/h");  
lcd.setCursor(14,1); if(digitalRead(4)==1) {lcd.print("A");} else {lcd.print("B");}
BattIco();
}



