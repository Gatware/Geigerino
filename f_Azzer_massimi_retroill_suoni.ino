void Azzeramento()
{
//    -------- Menu 2 - Azzeramento --------
lcd.print("   AZZERARE?    ");
t1=millis();
delay(300);
while(digitalRead(5)==LOW) // Attende che venga lasciato il pulsante.
{if(millis()-t1>1000) {Bip(); lcd.clear(); massimi(); return;}}  // Premuto per almeno 1 secondo (3,2 totali) salta a massimi(), poi ritorna ed esce.
t1=millis();
while(digitalRead(5)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0){Azz+=E; E=0; t1=millis(); delay(20);}
  if(Azz>1) Azz=1;
  if(Azz<0) Azz=0;
  
  lcd.setCursor(7,1);
  if(Azz==0){lcd.print("No");}
  else{lcd.print("Si");}
  if(millis()-t1>4999) return; // Dopo 5 secondi di inattività esce.
  }
  
if (Azz==1) {Azz=0; Biiip(); Azzera();} // Azzera
else Bip();
} // END Azzeramento()

void Azzera()
{cp=0; tempo=0; temposecondi=0; m=1; m1=1; t3=0; DAB=0; D=0; DMAX=0; for(m1=1; m1<61; m1++){C[m1]=0;}}
 // END Azzera()

void massimi()
{
lcd.print("    Massimi     "); 
t1=millis();
while(digitalRead(5)==LOW) // Attende che venga lasciato il pulsante.
{if(millis()-t1>1000) {Bip(); lcd.clear(); retroillum(); return;}}  // Premuto per almeno 1 secondo (3,2 totali) salta a retroillum(), poi ritorna ed esce.
t1=millis();
Mask(); lcd.setCursor(11,0); lcd.print("max"); lcd.setCursor(13,1); lcd.print("max"); 
lcd.setCursor(0,0); if(DMAX*60>ownbcpm) Imp=DMAX*60-ownbcpm; else Imp=0; printImp(); // D è in cps; printImp vuole cpm.
lcd.setCursor(0,1); Rad=float(Imp)/sens; printRad(); // uSv/h in virgola mobile. 
delay(300);
while(digitalRead(5)==HIGH){if(millis()-t1>9999) return;} // Attende che venga premuto il pulsante, ma dopo 10 secondi comunque esce.
} // END massimi()

void retroillum()
{
lcd.print(" Retroillumin.  ");
t1=millis();
while(digitalRead(5)==LOW) // Attende che venga lasciato il pulsante.
{if(millis()-t1>1000) {Bip(); lcd.clear(); suoni(); return;}}  // Premuto per almeno 1 secondo (3,2 totali) salta a retroillum(), poi ritorna ed esce.
delay(300);
t1=millis();
while(digitalRead(5)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0){LCD+=E; E=0; t1=millis(); delay(20);}
  if(LCD>2) LCD=2;
  if(LCD<1) LCD=1;

  lcd.setCursor(5,1); if(LCD==1) lcd.print("On    "); else lcd.print("On/Off");
  if(millis()-t1>4999) return; // Dopo 5 secondi di inattività esce.
  }
if(LCD!=EEPROM.read(11)) {EEPROM.update(11,LCD); Biip(); lcd.setCursor(12,1); lcd.print(" SET!"); delay(500);}
else Bip();  
} // END retroillum()

void suoni()
{
lcd.print("     Suoni      ");
t1=millis();
while(digitalRead(5)==LOW); // Attende che venga lasciato il pulsante.
delay(300);
t1=millis();
while(digitalRead(5)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0){biptic+=E; E=0; t1=millis(); delay(20);}
  if(biptic>1+2*TS) biptic=1+2*TS; // Se TS (Tic-Tic Software) è disattivato, elenca solo Nessun suono e Bip (dei menu)
  if(biptic<0) biptic=0;

  lcd.setCursor(2,1);
  if(biptic==0)      lcd.print("Nessun suono ");
  else if(biptic==1) lcd.print("Bip          ");
  else if(biptic==2) lcd.print("Tic-tic      ");
  else if(biptic==3) lcd.print("Bip + Tic-tic");
  if(millis()-t1>4999) return; // Dopo 5 secondi di inattività esce.  
  }
if(biptic!=EEPROM.read(12)) {EEPROM.update(12,biptic); Biip(); lcd.setCursor(12,1); lcd.print(" SET!"); delay(500);}
else Bip();  
} // END suoni()

