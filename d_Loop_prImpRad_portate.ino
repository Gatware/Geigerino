void loop()
{
P=digitalRead(5); // Legge il pulsante dell'encoder (0=Premuto)
if(Po==1 && P==0) {t1=millis(); Po=0;} // Quando viene premuto il pulsante legge il tempo
if(Po==0 && P==0) // Se era ed è premuto per almeno 1,2 secondi
  {               // salta a Integrazione, poi ritorna ed esce.
  if(millis()-t1>1200)
    {
    Po=1; Bip();
    detachInterrupt(0);         // Blocca gli interrupt per evitare che si accumulino conteggi
    Integrazione();             // che poi verrebbero divisi per un tempo brevissimo, non essendo
    while(digitalRead(5)==LOW); // stata, nel frattempo, incrementata la variabile tempo.
    Mask();
    attachInterrupt(0,ContaAB,FALLING); return;
    }
  } // END premuto per almeno 1,2 secondi
  
if(Po==0 && P==1) // se lo lascio prima, cambia unità di misura e portata analogica.
  {
  Po=1; Bip();
  detachInterrupt(0);
  lcd.clear(); portate();
  while(digitalRead(5)==LOW); 
  Mask();
  attachInterrupt(0,ContaAB,FALLING); return;
  }

// Ogni 10s legge la tensione e disegna l'icona che indica lo stato della batteria, poi ricontrolla il LED 1 (l'I/O è stato messo in input).
if(millis()-t4>9999) {t4=millis(); BattIco(); piloLED();}

mute=1; // Il Bip della funzione encoder() è disattivato.
encoder();
mute=0;

if(LCD!=LCDo) // Se l'impostazione della retroilluminazione è cambiata
  {LCDo=LCD; if(LCD==1) digitalWrite(A0,LOW);} // ed è impostata su "On", accende la retroilluminazione.
  
if(E!=0 && LCD==2) // Se è stato ruotato l'encoder e LCD sta in On/Off.
  {
  if(Lo==0) t1=millis();
  if(millis()-t1<200)
    {
    L+=E;
    if(L== 3 && digitalRead(A0)==1) {L=0; digitalWrite(A0, LOW); tone(7,1260,250);} // Encoder orario: Accende.
    if(L==-3 && digitalRead(A0)==0) {L=0; digitalWrite(A0,HIGH); tone(7,794,250);}  // Encoder antior.: Spegne.
    if(L==5) music();
    }
    else L=0;
  Lo=L; E=0;
  }


if(millis()-t5>499) // Due volte al secondo:
  {
  t5=millis();
  if(digitalRead(4)!=sinto) // Se digitalRead(4) è cambiato
    {
    detachInterrupt(0);
    lcd.setCursor(14,1);
    Azzera(); Biip();
    sinto=digitalRead(4);
    if(sinto==1) // Se l'I/O 4 sta alto è selezionata la sonda A; se sta a massa, la sonda B.
      {
      sonda=EEPROM.read(1); // Carica il tipo di sonda A.
      var=EEPROM.read(2)+EEPROM.read(3)*256; // Carica Lo-byte e Hi-byte di var del tubo A.
      ownbcpm=EEPROM.read(4); // cpm di fondo proprio del tubo A.   
      lcd.print("A");  
      } // END if(sinto==1)
      else
      {
      sonda=EEPROM.read(6); // Carica il tipo di sonda B.
      var=EEPROM.read(7)+EEPROM.read(8)*256; // Carica Lo-byte e Hi-byte di var del tubo B.
      ownbcpm=EEPROM.read(9); // cpm di fondo proprio del tubo B.
      lcd.print("B");
      } // END else
      
    if(sonda==ntipi) {sens=var;}
      else{sens=cost[sonda]; ownbcpm=ownb[sonda];}
    attachInterrupt(0,ContaAB,FALLING);
    } // END se digitalRead(4) è cambiato
  } // END 2 volte al secondo.
  
if(millis()-t3>999) // Una volta al secondo:
  {
  if(digitalRead(4)!=sinto) // Se digitalRead(4) è cambiato
    {
    detachInterrupt(0);
    lcd.setCursor(14,1);
    Azzera(); Biip();
    sinto=digitalRead(4);
    if(sinto==1) // Se l'I/O 4 sta alto è selezionata la sonda A; se sta a massa, la sonda B.
      {
      sonda=EEPROM.read(1); // Carica il tipo di sonda A.
      var=EEPROM.read(2)+EEPROM.read(3)*256; // Carica Lo-byte e Hi-byte di var del tubo A.
      ownbcpm=EEPROM.read(4); // cpm di fondo proprio del tubo A.   
      lcd.print("A");  
      } // END if(sinto==1)
      else
      {
      sonda=EEPROM.read(6); // Carica il tipo di sonda B.
      var=EEPROM.read(7)+EEPROM.read(8)*256; // Carica Lo-byte e Hi-byte di var del tubo B.
      ownbcpm=EEPROM.read(9); // cpm di fondo proprio del tubo B.
      lcd.print("B");
      } // END else
      
    if(sonda==ntipi) {sens=var;}
      else{sens=cost[sonda]; ownbcpm=ownb[sonda];}
    attachInterrupt(0,ContaAB,FALLING);
    } // END se digitalRead(4) è cambiato
    
  D=DAB;  
  t3=millis();
  if(Ti<70)
    {
    C[m]=D; if(D>DMAX){DMAX=D;} DAB=0; // DMAX: cps massimi (1 secondo).
    cp=0; 
    for(m1=1; m1<=tempo; m1++) {cp+=C[m1];} // Somma gli impulsi memorizzati nel tempo Ti.
    if(m<Ti) {m+=1;}
      else {m=1;} 
    }
    else
    {
    cp+=D; if(D>DMAX){DMAX=D;} DAB=0; // Ti=70, quindi tempo di integrazione infinito)
    }
  if(tempo<Ti) {if(long(cp*60/long(tempo-1))>ownbcpm) cpm=long(cp*60/long(tempo-1))-ownbcpm; else cpm=0;} // Impulsi al minuto (ownbcpm: cpm di background proprio del tubo). 
    else       {if(long(cp*60/long(tempo))>ownbcpm) cpm=long(cp*60/long(tempo))-ownbcpm; else cpm=0;} 
  Imp=cpm; lcd.setCursor(0,0); printImp(); // Passa i cpm a printImp.
  uSvph=float(cpm)/sens; // in virgola mobile.
  Rad=uSvph; lcd.setCursor(0,1); lcd.print("      "); lcd.setCursor(0,1); printRad(); // Passa i uSv/h a printRad.

  lcd.setCursor(10,0);
  ore=int(temposecondi/3600); if(ore>9) oref=String(ore); else oref=" "+String(ore);
  minuti=int((temposecondi%3600)/60); if(minuti>9) minutif=String(minuti); else minutif=" "+String(minuti);
  secondi=(temposecondi%3600)%60; 
  if(secondi>9) secondif=String(secondi); else secondif=" "+String(secondi);
  
  if     (tempo<60)   {lcd.print("   "+secondif+"s");}
  else if(tempo<3600)  lcd.print(minutif+"m"+secondif+"s");
  else                 lcd.print(oref+"h"+minutif+"m");
  tempo+=1; if(Ti<70 && tempo>Ti){tempo=Ti;}
  temposecondi+=1;
  if(Ti<70 && temposecondi>Ti) temposecondi-=Ti;
  // lcd.setCursor(14,1); if(int(((millis()-millisZero)/1000))%2) lcd.print(":"); else lcd.print(" "); // Fa lampeggiare ":"
  // Al posto dei : lampeggianti scrivo il nome della sonda attiva: A o B (v. sopra).
  piloLED();
  } // END una volta al secondo
} // END loop

void piloLED()
{
// PILOTAGGIO DEI LED:
//                             LED 54321-
if(LED==0)               PORTC&=B11000000;
else if(LED==1)
  {
  if     (uSvph>soglia5) {PORTC&=B11100001; PORTC|=B00100000; tone(SPK, 1000, 2000);}
  else if(uSvph>soglia4) {PORTC&=B11010001; PORTC|=B00010000; tone(SPK, 1000, 1000);}
  else if(uSvph>soglia3) {PORTC&=B11001001; PORTC|=B00001000;}
  else if(uSvph>soglia2) {PORTC&=B11000101; PORTC|=B00000100;}
  else if(uSvph>soglia1) {PORTC&=B11000011; PORTC|=B00000010;}
  else                    PORTC&=B11000001;
  }
else if(LED==2)
  {
  if     (uSvph>soglia5) {                  PORTC|=B00111110; tone(SPK, 1000, 2000);}
  else if(uSvph>soglia4) {PORTC&=B11011110; PORTC|=B00011110; tone(SPK, 1000, 1000);}
  else if(uSvph>soglia3) {PORTC&=B11001110; PORTC|=B00001110;}
  else if(uSvph>soglia2) {PORTC&=B11000110; PORTC|=B00000110;}
  else if(uSvph>soglia1) {PORTC&=B11000010; PORTC|=B00000010;}
  else                    PORTC&=B11000000;
  }
}

void printImp() // =cpm
{
if(U==1)Imp/=60; // Se viene cambiata unità di misura, diventano cps.
if     (Imp<10) cpmf="    "+String(Imp)+" ";
else if(Imp<100) cpmf="   "+String(Imp)+" ";
else if(Imp<1000) cpmf="  "+String(Imp)+" ";
else if(Imp<10000) cpmf=" "+String(Imp)+" ";
else if(Imp<100000)    cpmf=String(Imp)+" ";
else                   cpmf=String(Imp);
lcd.print(cpmf);
} // END printImp()

void printRad()
{
if(U==1)Rad/=10; // Se viene cambiata unità di misura, diventano mR/h.
if     (Rad<.1)                     lcd.print(Rad,4);  // Es.: 0,0123
else if(Rad<1)                      lcd.print(Rad,3);  // Es.: 0,245
else if(Rad<10)  {lcd.print(" ");   lcd.print(Rad,2);} // Es.:  3,56
else if(Rad<100) {lcd.print(" ");   lcd.print(Rad,1);} // Es.:  99,5
else if(Rad<1000) {lcd.print("  "); lcd.print(Rad,0);} // Es.:   512
else if(Rad<10000) {lcd.print(" "); lcd.print(Rad,0);} // Es.:  1450
else                                lcd.print(Rad,0);  // Es.:  21450
// analogOut è abilitato?
if(analogOut)
  {
  if(por==0) anOut=int(((log10(uSvph)+4)*51)); // log10(0,0001)=-4; log10(10)=1
  else       anOut=int(((log10(uSvph)+2)*51)); // log10(0,01)=-2; log10(1.000)=3
  if(anOut<0) anOut=0; if(anOut>255) anOut=255;
  analogWrite(6,anOut);
  }
} // END printRad()

void portate()
{
lcd.print("Unit"); lcd.write(1); lcd.print(" di misura");
lcd.setCursor(2,1);
if(U==0) {lcd.print("CPM - "); lcd.write(2); lcd.print("Sv/h");}
else lcd.print("CPS -  mR/h ");
while(digitalRead(5)==LOW) // Attende che venga lasciato il pulsante.
delay(300);
t1=millis();
while(digitalRead(5)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0) {U+=E; E=0; t1=millis(); delay(20);}
  if(U>1) U=1;
  if(U<0) U=0;

  lcd.setCursor(2,1);
  if(U==0) {lcd.print("CPM - "); lcd.write(2); lcd.print("Sv/h");}
  else lcd.print("CPS -  mR/h ");
  if(millis()-t1>4999) return; // Dopo 5 secondi di inattività esce.
  }
  
lcd.clear(); lcd.print(" Portata analog.");
Bip();
while(digitalRead(5)==LOW) // Attende che venga lasciato il pulsante.
delay(300);
t1=millis();
while(digitalRead(5)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0) {por+=E; E=0; t1=millis(); delay(20);}
  if(por>1) por=1;
  if(por<0) por=0;

  lcd.setCursor(0,1);
  if(por==0) {lcd.print("0,0001...10"); lcd.write(2); lcd.print("Sv/h");}
  else {lcd.print("  0,01"); lcd.write(2); lcd.print("...1mSv/h");}
  if(millis()-t1>4999) return; // Dopo 5 secondi di inattività esce.
  }
Bip();
} // END portate()
