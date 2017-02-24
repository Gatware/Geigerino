void Azzeramento()
{
//    -------- Menu 2 - Azzeramento --------
lcd.print(F("   AZZERARE?    "));
t1=millis();
delay(300);
while(digitalRead(5)==LOW) // Attende che venga lasciato il pulsante.
{if(millis()-t1>999) {Bip(); lcd.clear(); massimi(); return;}}  // Premuto per almeno 1 secondo (3 totali) salta a massimi(), poi ritorna ed esce.
t1=millis();
while(digitalRead(5)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0)   Azz+=E;
  if(Azz>1) {noTone(7); Azz=1;}
  if(Azz<0) {noTone(7); Azz=0;}
  if(E!=0)  {E=0; t1=millis(); delay(20);}
    
  lcd.setCursor(7,1);
  if(Azz==0){lcd.print("No");}
  else{lcd.print("Si");}
  if(millis()-t1>4999) return; // Dopo 5 secondi di inattività esce.
  }
  
if (Azz==1) {Azz=0; Biip(); detachInterrupt(0); Azzera(); attachInterrupt(0,ContaAB,FALLING);} // Azzera
else Bip();
} // END Azzeramento()

void Azzera()
{for(m1=1; m1<61; m1++){C[m1]=0;} m=1; m1=1; cp=0; tempo=0; temposecondi=0; t3=0; DAB=0; D=0; DMAX=0; suonoFine=0;}
 // END Azzera()

void massimi()
{
lcd.print(F("    Massimi     ")); 
t1=millis();
while(digitalRead(5)==LOW) // Attende che venga lasciato il pulsante.
{if(millis()-t1>999) {Bip(); lcd.clear(); suoni(); return;}}  // Premuto per almeno 1 secondo (4 totali) salta a retroillum(), poi ritorna ed esce.
t1=millis();
Mask(); lcd.setCursor(11,0); lcd.print("max"); lcd.setCursor(13,1); lcd.print("max"); 
lcd.setCursor(0,0); if(DMAX*60>ownbcpm) Imp=DMAX*60-ownbcpm; else Imp=0; printImp(); // D è in cps; printImp vuole cpm.
lcd.setCursor(0,1); Rad=float(Imp)/sens; printRad(); // uSv/h in virgola mobile. 
delay(300);
while(digitalRead(5)==HIGH){if(millis()-t1>9999) return;} // Attende che venga premuto il pulsante, ma dopo 10 secondi comunque esce.
} // END massimi()

void suoni()
{
lcd.print("     Suoni      ");
t1=millis();
while(digitalRead(5)==LOW) // Attende che venga lasciato il pulsante.
{if(millis()-t1>999) {Bip(); lcd.clear(); allarme(); return;}}  // Premuto per almeno 1 secondo (5 totali) salta ad allarme(), poi ritorna ed esce.
delay(300);
t1=millis();
mute=1;
while(digitalRead(5)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0) biptic+=E;
  if(biptic>1+2*TS) {tone(7,1000,1); noTone(7); E=0; biptic=1+2*TS;} // Se TS (Tic-Tic Software) è disattivato, elenca solo Nessun suono e Bip (dei menu)
  if(biptic<0) {tone(7,1000,1); noTone(7); E=0; biptic=0;}
  if(E!=0){tone(7,1000,15); E=0; t1=millis(); delay(20);}
  lcd.setCursor(1,1);
  
  if(biptic==0)      lcd.print(F(" Nessun suono"));
  else if(biptic==1) lcd.print(F("     Bip     "));
  else if(biptic==2) lcd.print(F("   Tic-tic   "));
  else if(biptic==3) lcd.print(F("Bip + Tic-tic"));
  if(millis()-t1>4999) return; // Dopo 5 secondi di inattività esce.  
  }
mute=0;
if(biptic!=EEPROM.read(12)) {EEPROM.update(12,biptic); Biip(); lcd.setCursor(10,1); lcd.print(F(". SET!")); delay(500);}
else Bip();  
} // END suoni()

void allarme()
{
lcd.print(F("    Allarme      "));
t1=millis();
while(digitalRead(5)==LOW) // Attende che venga lasciato il pulsante.
{if(millis()-t1>999) {Bip(); lcd.clear(); retroillum(); return;}}  // Premuto per almeno 1 secondo (5 totali) salta a retroillum(), poi ritorna ed esce.
delay(300);
t1=millis();
while(digitalRead(5)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0)   alm+=E;
  if(alm>1) {noTone(7); alm=1;}
  if(alm<0) {noTone(7); alm=0;}
  if(E!=0)  {E=0; t1=millis(); delay(20);}
    
  lcd.setCursor(7,1);
  if(alm==0){lcd.print("No");}
  else{lcd.print("Si");}
  if(millis()-t1>4999) return; // Dopo 5 secondi di inattività esce.
  }
if(alm!=EEPROM.read(14)) {EEPROM.update(14,alm); Biip(); lcd.setCursor(12,1); lcd.print("SET!"); delay(500);}
else Bip();  
} // END allarme()

void retroillum()
{
lcd.print(F(" Retroillumin.  "));
t1=millis();
while(digitalRead(5)==LOW) // Attende che venga lasciato il pulsante.
{delay(300);}
t1=millis();
while(digitalRead(5)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0) LCD+=E;
  if(LCD>2){noTone(7); LCD=2;}
  if(LCD<1){noTone(7); LCD=1;}
  if(E!=0) {E=0; t1=millis(); delay(20);}
  
  lcd.setCursor(5,1); if(LCD==1) lcd.print(F("  On  ")); else lcd.print(F("On/Off"));
  if(millis()-t1>4999) return; // Dopo 5 secondi di inattività esce.
  }
if(LCD!=EEPROM.read(11)) {EEPROM.update(11,LCD); Biip(); lcd.setCursor(12,1); lcd.print(F("SET!")); delay(500);}
else Bip();  
} // END retroillum()

void precisione()
{
lcd.print(F("   Precisione   "));
t1=millis();
while(digitalRead(5)==LOW) // Attende che venga lasciato il pulsante.
{delay(300);}
t1=millis();
while(digitalRead(5)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0) prec+=E;
  if(prec>10){noTone(7); prec=10;}
  if(prec<1) {noTone(7); prec=1;}
  if(E!=0) {E=0; t1=millis(); delay(20);}
  lcd.setCursor(5,1); if(prec<10) lcd.print(" "+String(prec)+"%"); else lcd.print(String(prec)+"%");
  if(millis()-t1>4999) return; // Dopo 5 secondi di inattività esce.
  }
if(prec!=EEPROM.read(16)) {valPrec=10000/prec^2; EEPROM.update(16,prec); Biip(); lcd.setCursor(12,1); lcd.print(F("SET!")); delay(500);}
else Bip();  
}

