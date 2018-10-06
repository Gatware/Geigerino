void Azzeramento()
{
//    -------- Menu 2 - Azzeramento --------
lcd.print(F("   AZZERARE?    "));
delay(300);
while(!(PIND&0x20)) // Attende che venga lasciato il pulsante.
t1=millis();
while(PIND&0x20) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(Azz==0 && E==1) Azz=1;
  else if(Azz==1 && E==-1) Azz=0;
  else noTone(7);
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
{for(m1=1; m1<61; m1++){C[m1]=0;} m=1; m1=1; cp=0; tempo=0; temposecondi=0; dose=0; t3=0; DAB=0; D=0; cpmMAX=0; suonoFine=0;}
 // END Azzera()

void massimi()
{
lcd.print(F(" Massimi e tot.")); 
while(!(PIND&0x20)) // Attende che venga lasciato il pulsante.
t1=millis();
Mask(); lcd.setCursor(11,1); lcd.print("max  "); lcd.setCursor(11,0); lcd.print("max"); // Sulla riga 1 (in basso) cancella
                      // con 2 spazi la lettera della sonda attiva e l'icona della batteria, appena visualizzati da mask(). 
lcd.setCursor(0,0); Imp=cpmMAX; printImp();
lcd.setCursor(0,1); Rad=float(Imp)/sens; printRad(); // uSv/h in virgola mobile. 
while(PIND&0x20); // Attende che venga premuto il pulsante.
lcd.clear(); lcd.print(F("Impulsi contati"));
lcd.setCursor(0,1); lcd.print(cp);
while(!(PIND&0x20)) // Attende che venga lasciato il pulsante.
delay(300);
while(PIND&0x20); // Attende che venga premuto il pulsante.
delay(300);
} // END massimi()

void suoni()
{
lcd.print("     Suoni      ");
while(!(PIND&0x20)) // Attende che venga lasciato il pulsante.
delay(300);
t1=millis();
mute=1;
while(PIND&0x20) // Continua a leggere l'encoder finché non premo
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
if(biptic!=EEPROM.read(12)) {EEPROM.update(12,biptic); Biip(); lcd.setCursor(11,1); lcd.print(F(" SET!")); delay(500);}
else Bip();  
} // END suoni()

void allarme()
{
lcd.print(F("    Allarme      "));
while(!(PIND&0x20)) // Attende che venga lasciato il pulsante.
delay(300);
t1=millis();
while(PIND&0x20) // Continua a leggere l'encoder finché non premo
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
while(!(PIND&0x20)) // Attende che venga lasciato il pulsante.
{delay(300);}
t1=millis();
while(PIND&0x20) // Continua a leggere l'encoder finché non premo
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
while(!(PIND&0x20)) // Attende che venga lasciato il pulsante.
{delay(300);}
t1=millis();
while(PIND&0x20) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0) prec+=E;
  if(prec>15){noTone(7); prec=15;}
  if(prec<1) {noTone(7); prec=1;}
  if(E!=0) {E=0; t1=millis(); delay(20);}
  lcd.setCursor(6,1);
  if(prec<10) lcd.print(' ');
  lcd.print(prec); lcd.print('%');
  
  if(millis()-t1>4999) return; // Dopo 5 secondi di inattività esce.
  }
if(prec!=EEPROM.read(16)) {valPrec=10000/sq(prec); EEPROM.update(16,prec); Biip(); lcd.setCursor(12,1); lcd.print(F("SET!")); delay(500);}
else Bip();  
}

void autonomia()
{
unsigned long t=millis()+500;
lcd.print(F("V batt.:    V"));
lcd.setCursor(0,1); lcd.print("Autonomia:");
scriveTensione(); // Inizialmente usa il valore corrente di Vb.
pinMode(A1,INPUT);
analogRead(A1); // Prima lettura per averne, poi, una precisa e stabile.
BattIco();
while(!(PIND&0x20)) // Attende che venga lasciato il pulsante.
{delay(300);}

while(PIND&0x20) // Finché non viene premuto il pulsante:
  {
  if(millis()-t>1000) // 1 volta al secondo
    {
    t=millis();
    BattIco();
    scriveTensione();
    }
  }
pinMode(A1,OUTPUT);
Bip();
}

void scriveTensione()
{
lcd.setCursor(8,0);
if(((500*Vb/1023)%100)>9) {lcd.print(int(500*Vb/102300)); lcd.print('.'); lcd.print((500*Vb/1023)%100);}
else{lcd.print(int(500*Vb/102300)); lcd.print(".0"); lcd.print((500*Vb/1023)%100);}
lcd.setCursor(10,1); lcd.print(String(int(Vb*10000/205 - 32000) /VSB*Vb/859) + "h ");
}
