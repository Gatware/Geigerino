void loop()
{ // PIND&0x20)>>5 è come dire digitalRead(5).
P=(PIND&0x20)>>5; // Legge il pulsante dell'encoder (0=Premuto)
if(Po==1 && P==0) {t1=millis(); Po=0;} // Quando viene premuto il pulsante legge il tempo
if(Po==0 && P==0)     // Se era ed è premuto per almeno 1 secondo
  {                   // salta a Menu, poi ritorna ed esce.
  if(millis()-t1>999) // 
    {
    Po=1; Bip();
    detachInterrupt(0); // Blocca gli interrupt per evitare che si accumulino conteggi
    Menu();             // che poi verrebbero divisi per un tempo brevissimo, non essendo
    while(!(PIND&0x20)) // stata, nel frattempo, incrementata la variabile tempo.
    delay(200);
    Mask();
    attachInterrupt(0,ContaAB,FALLING); return;
    }
  } // END premuto per almeno 1 secondo
  
if(Po==0 && P==1) // se lo lascio prima, cambia uSv/h > deviazione standard (+/-cpm) > mR/h > Dose.
  {
  Po=1; Dispo=Disp; Disp+=1; if(Disp==4) Disp=0; // 0:uSv/h; 1:deviazione standard (+/-cpm); 2:mR/h; 3:Dose
  if (Disp==2 && Ti==TMAX+10) {Disp=0; Dispo=2;} // nel modo a precisione fissa salta la Dose, che non ha senso.
  delay(100);    
  }

if(biptic<2 && particella==1)
  {
  if(particellao==0) {t7=millis(); PORTC|=B00100000; particellao=1;} // Accende il LED rosso.
  else if(millis()-t7>10) {PORTC&=B11011111; particella=0; particellao=0;} // Spegne il LED rosso.
  }

    
// ------------------- DUE VOLTE AL SECONDO -------------------

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
      lcd.print('A');  
      } // END if(sinto==1)
    else
      {
      sonda=EEPROM.read(6); // Carica il tipo di sonda B.
      var=EEPROM.read(7)+EEPROM.read(8)*256; // Carica Lo-byte e Hi-byte di var del tubo B.
      ownbcpm=EEPROM.read(9); // cpm di fondo proprio del tubo B.
      lcd.print('B');
      } // END else
      
    if(tipo[sonda]=="variabile" || tipo[sonda]=="Variabile") sens=var;
    else{sens=cost[sonda]; ownbcpm=ownb[sonda];}
    attachInterrupt(0,ContaAB,FALLING);
    } // END se digitalRead(4) è cambiato
  } // END 2 volte al secondo.
  
// ------------------- UNA VOLTA AL SECONDO -------------------

if(millis()-t3>999) // Una volta al secondo:
  {
  if(TS && (biptic==2||biptic==3) && alm+ALLARME!=2 && suonoFine==0) TicSi=1; else TicSi=0;
  D=DAB;
  if(60*D>ownbcpm) dose+=float(D-ownbcpm/60)/sens/60;
  t3=millis();
  if(Ti<TMAX)
    {
    C[m]=D; DAB=0;
    cp=0; 
    for(m1=1; m1<=tempo; m1++) {cp+=C[m1];} // Somma gli impulsi memorizzati nel tempo Ti.
    if(m<Ti) {m+=1;}
    else {m=1;} 
    }
  else
    {
    cp+=D; DAB=0; // Ti=TMAX, quindi tempo di integrazione infinito, o Ti=TMAX+10, quindi precisione fissa.
    }
  if(tempo<Ti && Ti<TMAX) {if(long((cp*60+30)/long(tempo-1))>ownbcpm) cpm=long((cp*60+30)/long(tempo-1))-ownbcpm; else cpm=0;} // Impulsi al minuto (ownbcpm: cpm di background proprio del tubo). 
  else       {if(long((cp*60+30)/long(tempo))>ownbcpm) cpm=long((cp*60+30)/long(tempo))-ownbcpm; else cpm=0;} //+30: Aggiunge 0,5 ai cpm per l'arrotondamento
  if (cpm>cpmMAX && tempo>4) cpmMAX=cpm;
  Imp=cpm; lcd.setCursor(0,0); printImp(); // Passa i cpm a printImp.
  uSvph=float(cpm)/sens; // in virgola mobile.
  calcDstdEDstdPerc();
  if(Disp==0)
    {
    if(Dispo){Dispo=0; lcd.setCursor(6,1); lcd.write(byte(2)); lcd.write(byte(5)); lcd.write(byte(6));} // Se è appena stato commutato, scrive uSv/h.
    Rad=uSvph; lcd.setCursor(0,1); lcd.print("      "); lcd.setCursor(0,1); printRad(); // Se non è in modo Disp2, passa i uSv/h a printRad.
    lcd.setCursor(9,0); if(Ti==TMAX+10) lcd.print('+'); else lcd.print(' '); visualSecondi(temposecondi); 
    }                       // TMAX+10: Precisione fissa.
  else if(Disp==1) // Se è in modo Disp=1, scrive la deviazione standard
    {
    if(tempo>1) // Ho dovuto mettere tempo>1 perché poco dopo l'azzeramento appariva ± -2622.
      {
      lcd.setCursor(9,0);
      if(Ti==TMAX+10) {lcd.print('-'); unsigned long residuo=230.5*valPrec/cpm-tempo; if(residuo>0) visualSecondi(residuo); else visualSecondi(0);} // 1,96^2*60=230,4;  per 5%: valPrec=1/(5%^2)=400
      else {lcd.print(' '); visualSecondi(temposecondi);}
      scriveDstd(); scriveDstdPerc();
      }
    else {lcd.setCursor(1,1); lcd.print("   0 ");} // Se tempo<=1 
    }
  else if(Disp==2) // mR/h
    {
    if(Dispo!=2){Dispo=2; lcd.setCursor(6,1); lcd.print("mR"); lcd.write(byte(6));} // Se è appena stato commutato, scrive mR/h.
    Rad=uSvph/10; lcd.setCursor(0,1); lcd.print("      "); lcd.setCursor(0,1); printRad();
    }
  else // Disp=3: Dose; nel modo a precisione fissa non viene visualizzata.
    {
    Rad=dose/1000;
    lcd.setCursor(0,1); printRad(); lcd.print("mSv");
    lcd.setCursor(9,0); lcd.print(' '); visualSecondi(temposecondi); 
    }

  pinMode(A2,INPUT);
  delayMicroseconds(100);
  analogRead(A2); // Prelettura
  Vcc=int(analogRead(A2)*XVref/1000L); // Legge la tensione di alimentazione.
  pinMode(A2,OUTPUT);
  piloLED(); // Ripristina lo stato dei LED.

  // analogOut è abilitato?
  if(analogOut)
    {
    anOut=((log10(uSvph)+2)*51); // log10(0,01)=-2; log10(1000)=3
    // if(por==0) anOut=((log10(uSvph)+2)*51); // log10(0,0001)=-4; log10(10)=1
    //   else     anOut=((log10(uSvph)+2)*51); // log10(0,01)=-2; log10(1.000)=3   
    if(pwr==0) anOut=int(anOut*635/Vcc); // Se è alimentato direttamente da Litio (605=3V) o 5V in carica.
    else     anOut=int(anOut*635/1024); // Se è alimentato a 5V fissi.
    if(anOut<0) anOut=0; else if(anOut>255) anOut=255;
    analogWrite(6,anOut); 
    }
  calcDstdEDstdPerc(); scriveDstdPerc();   
  if(Ti==TMAX+10)
    {
    if(dstdPerc<=prec && cpm>0 && cp>20)
      {
      suonoFine=1;
      while(PIND&0x20) {if(millis()%2000>1000) tone(7,1000); else noTone(7);} // Finché non viene premeuto l'encoder fa Biiip - Biiip - Biiip...
      noTone(7); while(!(PIND&0x20)); delay (300); // Premuto l'encoder, si tacita e attende che venga lasciato il pulsante.
      while(PIND&0x20); // Attende di essere premuto di nuovo per cambiare la visualizzazione.
      delay(200);
      if(Disp==1)
        {
        Rad=uSvph; lcd.setCursor(0,1); lcd.print("      "); lcd.setCursor(0,1); printRad(); lcd.setCursor(6,1); lcd.write(byte(2)); lcd.write(byte(5)); lcd.write(byte(6));
        lcd.setCursor(9,0); lcd.print('+'); visualSecondi(temposecondi);                                                // micro               Sv                  /h
        }
      else scriveDstd();
      while(!(PIND&0x20)) {delay(200);} // Attende che venga lasciato l'encoder
      while(PIND&0x20) {delay(200);} // Attende una nuova pressione dell'encoder per azzerare.
      delay(200);
      Azzera();
      if(!Disp){lcd.setCursor(6,1); lcd.write(byte(2)); lcd.write(byte(5)); lcd.write(byte(6));} // Se è impostato per visualizzare i uSv/h, lo riscrive.
      } 
    }  
  tempo+=1;
  if(Ti<TMAX && tempo>Ti) tempo=Ti;
  temposecondi+=1;
  if(Ti<TMAX && temposecondi>Ti) temposecondi-=Ti;
  // lcd.setCursor(14,1); if(int(((millis()-millisZero)/1000))%2) lcd.print(":"); else lcd.print(" "); // Fa lampeggiare ":"
  // Al posto dei : lampeggianti scrivo il nome della sonda attiva: A o B (v. sopra).
  piloLED();
  
  // Se la batteria è scarica, fa lampeggiare la retroilluminazione:
  byte resto=millis()%2000; if(Vb<676 && resto!=restoPrecedente) {digitalWrite(A0,!digitalRead(A0)); restoPrecedente=resto;} 
  } // END una volta al secondo
  
 
// --------------------- OGNI 10 SECONDI ---------------------
// Legge la tensione e disegna l'icona che indica lo stato della batteria, poi ricontrolla il LED 1 (l'I/O è stato messo in input).
if(millis()-t4>9999) {t4=millis(); BattIco();}
// -----------------------------------------------------------

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

if(ALLARME && alm)
  {
  ALLARMEo=1;
  if(f>0)tone(7,f); else noTone(7);
  if     (fc<421)  f=fc;  // Frequenza crescente fino a 420 (=420Hz)
  else if(fc<521)  f=420; // Fino a 520 rimane a 420Hz 
  else if(fc<661)  f=0;  // Fino a 660 fa silenzio
  else             fc=100;  // Quando arriva a 601 riparte da 100Hz.
  if(millis()-t6>7) {t6=millis(); fc*=1.01;}
  }
  else if(ALLARMEo==1) {ALLARMEo=0; fc=255; noTone(7);}
} // END loop


// --------------------------------- FUNZIONI ---------------------------------

void visualSecondi(long S)
{
if(S<0) S=0;
ore=int(S/3600);
minuti=int((S%3600)/60);
secondi=(S%3600)%60; 
if      (S<60)   {lcd.print("   "); printSecondi(); lcd.print('s');}
else if (S<3600) {printMinuti(); lcd.print('m'); printSecondi(); lcd.print('s');}
else             {printOre(); lcd.print('h'); printMinuti(); lcd.print('m');}
}

void printOre()
{
if(ore<10) lcd.print(' ');
lcd.print(ore);
}
void printMinuti()
{
if(minuti<10) lcd.print(' ');
lcd.print(minuti);
}
void printSecondi()
{
if(secondi<10) lcd.print(' ');
lcd.print(secondi);
}

void piloLED()
{
// PILOTAGGIO DEI LED:
//                              LED 54321-
if(LED==1)                PORTC&=B11000000;
else if(LED==2)
  { //                          LED 54321-        LED 54321-
  if     (uSvph>soglia5) {PORTC&=B11100001; PORTC|=B00100000; ALLARME=1;}
  else if(uSvph>soglia4) {PORTC&=B11010001; PORTC|=B00010000; ALLARME=1;}
  else if(uSvph>soglia3) {PORTC&=B11001001; PORTC|=B00001000; ALLARME=0;}
  else if(uSvph>soglia2) {PORTC&=B11000101; PORTC|=B00000100; ALLARME=0;}
  else if(uSvph>soglia1) {PORTC&=B11000011; PORTC|=B00000010; ALLARME=0;}
  else                   {PORTC&=B11000001;                   ALLARME=0;}
  }
else if(LED==3)
  { //                          LED 54321-        LED 54321-
  if     (uSvph>soglia5) {                  PORTC|=B00111110; ALLARME=1;}
  else if(uSvph>soglia4) {PORTC&=B11011110; PORTC|=B00011110; ALLARME=1;}
  else if(uSvph>soglia3) {PORTC&=B11001110; PORTC|=B00001110; ALLARME=0;}
  else if(uSvph>soglia2) {PORTC&=B11000110; PORTC|=B00000110; ALLARME=0;}
  else if(uSvph>soglia1) {PORTC&=B11000010; PORTC|=B00000010; ALLARME=0;}
  else                   {PORTC&=B11000000;                   ALLARME=0;}
  }
}

void printImp() // =cpm
{
if(U==1)Imp/=60; // Se viene cambiata unità di misura, diventano cps.
if     (Imp<10) {lcd.print("    "); lcd.print(Imp); lcd.print(' ');}
else if(Imp<100) {lcd.print("   "); lcd.print(Imp); lcd.print(' ');}
else if(Imp<1000) {lcd.print("  "); lcd.print(Imp); lcd.print(' ');}
else if(Imp<10000) {lcd.print(' '); lcd.print(Imp); lcd.print(' ');}
else if(Imp<100000)    {lcd.print(Imp); lcd.print(' ');}
else                    lcd.print(Imp);
} // END printImp()

void printRad()
{
if(U==1)Rad/=10; // Se viene cambiata unità di misura, diventano mR/h.
     if(Rad<.1)                     lcd.print(Rad,4);                   // Es.: 0,0120
else if(Rad<1)                     {lcd.print(Rad,3); lcd.print(' ');}  // Es.: 0,245
else if(Rad<10)    {lcd.print(' '); lcd.print(Rad,2); lcd.print(' ');}  // Es.:  3,56
else if(Rad<100)   {lcd.print(' '); lcd.print(Rad,1); lcd.print(' ');}  // Es.:  99,5
else if(Rad<1000) {lcd.print(F("  ")); lcd.print(Rad,0); lcd.print(' ');}  // Es.:   512
else if(Rad<10000) {lcd.print(' '); lcd.print(Rad,0); lcd.print(' ');}  // Es.:  1450
else                                lcd.print(Rad,0);                   // Es.: 21450
} // END printRad()

void calcDstdEDstdPerc()
{
dstd=(10+(Ti==TMAX+10)*9.6)*sqrt(cpm*60/(tempo-(tempo<Ti))); // Adotto coefficiente 1 (*10 perché sono decimi) per considerare una confidenza del 68%, 
  //                                           oppure 1,96 (*10) nel caso della precisione fissa (Ti==TMAX+10)=1 per considerare una confidenza del 95%.
dstdPerc=10*dstd/cpm+1; // +1: Aggiunge 1 alla percentuale (dstd è già moltiplicata per 10) per arrotondare.
} // END calcDstdEDtsdPerc()
  
void scriveDstd()
{
lcd.setCursor(0,1); lcd.write(3);
if(cpm>=100000) spazio=" "; else spazio="";
if     (dstd<1)     {lcd.print("   0 c"); lcd.write(4); lcd.print("   ");}
else if(dstd<100)   {lcd.print(' '); lcd.print(spazio); lcd.print(int(dstd/10)); lcd.print(','); lcd.print(dstd%10); lcd.print(' ');}
else if(dstd<1000)  {lcd.print("  "); lcd.print(spazio); lcd.print(int(dstd/10)); lcd.print(' ');}
else if(dstd<10000) {lcd.print(' ');  lcd.print(spazio); lcd.print(int(dstd/10)); lcd.print(' ');}
else if(dstd<100000){lcd.print(spazio); lcd.print(int(dstd/10)); lcd.print(' ');}
else if(dstd>999999){lcd.print(spazio); lcd.print(int(dstd/10)); lcd.print(' ');}
lcd.setCursor(6,1); lcd.print('c'); lcd.write(4); lcd.print(' '); // Cancella il carattere /h.
} // END scriveDstd()

void scriveDstdPerc()
{
if(cpm!=0)
  {
  lcd.setCursor(10,1);             
  if(dstdPerc>99) lcd.print("++");
  else if(dstdPerc>9) lcd.print(dstdPerc);
  else {lcd.print(' '); lcd.print(dstdPerc);}
  lcd.print('%'); if(dstd>99) lcd.print(' ');
  }
  else {lcd.setCursor(10,1); lcd.print(" 0 ");}
} // END scriveDstdPerc()
