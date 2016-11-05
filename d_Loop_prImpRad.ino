void loop()
{
P=digitalRead(5); // Legge il pulsante dell'encoder (0=Premuto)
if(Po==1 && P==0) {t1=millis(); Po=0;} // Quando viene premuto il pulsante legge il tempo
if(Po==0 && P==0) // Se era ed è premuto per almeno 1,2 secondi
  {               // salta a Integrazione, poi ritorna ed esce.
  if(millis()-t1>1200)
    {Po=1; Bip();
    detachInterrupt(0); detachInterrupt(1); // Blocca gli interrupt per evitare che si accumulino conteggi
    Integrazione();                         // che poi verrebbero divisi per un tempo brevissimo, non essendo
    while(digitalRead(5)==LOW);             // stata, nel frattempo, incrementata la variabile tempo.
    Mask();
    attachInterrupt(0,ContaA,FALLING); attachInterrupt(1,ContaB,FALLING); return;}
  }
if(Po==0 && P==1) // se lo lascio prima, cambia unità di misura.
  {Po=1; Bip(); U=1-U; Mask();}
  
if(millis()-t4>9999) {t4=millis(); BattIco();} // Disegna l'icona che indica lo stato della batteria.

if(millis()-t3>999) // Una volta al secondo:
  {
  if(digitalRead(4)!=sinto) // Se digitalRead(4) è cambiato
    {
    Azzera();
    sinto=digitalRead(4);
    if(sinto==1) // Se l'I/O 4 sta alto è selezionata la sonda A; se sta a massa, la sonda B.
      {
      sonda=EEPROM.read(1); // Carica il tipo di sonda A.
      var=EEPROM.read(2)+EEPROM.read(3)*256; // Carica Lo-byte e Hi-byte di var del tubo A.
      ownbcpm=EEPROM.read(4); // cpm di fondo proprio del tubo A.     
      } // END if(sinto==1)
    else
      {
      sonda=EEPROM.read(6); // Carica il tipo di sonda B.
      var=EEPROM.read(7)+EEPROM.read(8)*256; // Carica Lo-byte e Hi-byte di var del tubo B.
      ownbcpm=EEPROM.read(9); // cpm di fondo proprio del tubo B.      
      } // END else
      
    if(sonda==10) {sens=var;}
    else{sens=cost[sonda]; ownbcpm=ownb[sonda];}
    } // END se digitalRead(4) è cambiato
    
  if(digitalRead(4)) D=DA; else D=DB;
    
  t3=millis();
  if(Ti<70)
    {
    C[m]=D; if(D>DMAX){DMAX=D;} DA=0; DB=0; // DMAX: cps massimi (1 secondo).
    if(m<Ti) {m+=1;}
    else {m=1;}
    cp=0;
    for(m1=1; m1<tempo+1; m1++) {cp+=C[m1];} // Somma gli impulsi memorizzati nel tempo Ti.
    }
  else {cp+=D; if(D>DMAX){DMAX=D;} DA=0; DB=0;} // Ti=70, quindi tempo di integrazione infinito)
    
  if(long(cp*60/tempo)>ownbcpm) cpm=long(cp*60/tempo)-ownbcpm; else cpm=0; // Impulsi al minuto (ownbcpm: cpm di background proprio del tubo). 
  
  Imp=cpm; lcd.setCursor(0,0); printImp(); // Passa i cpm a printImp.
  uSvph=float(cpm)/sens; // in virgola mobile.
  Rad=uSvph; lcd.setCursor(0,1); lcd.print("      "); lcd.setCursor(0,1); printRad(); // Passa i uSv/h a printRad.

  lcd.setCursor(10,0);
  ore=int(tempo/3600); if(ore>9) oref=String(ore); else oref=" "+String(ore);
  minuti=int((tempo%3600)/60); if(minuti>9) minutif=String(minuti); else minutif=" "+String(minuti);
  secondi=(tempo%3600)%60; if(secondi>9) secondif=String(secondi); else secondif=" "+String(secondi);
  if     (tempo<60)   {lcd.print("   "+secondif+"s");}
  else if(tempo<3600)  lcd.print(minutif+"m"+secondif+"s");
  else                 lcd.print(oref+"h"+minutif+"m");
  tempo+=1; if(Ti<70 && tempo>Ti){tempo=Ti;}
  lcd.setCursor(14,1); if(int(((millis()-millisZero)/1000))%2) lcd.print(":"); else lcd.print(" ");
  
// PILOTAGGIO DEI LED:
  if     (Rad>soglia5) {PORTC&=B11100000; PORTC|=B00100000;}
  else if(Rad>soglia4) {PORTC&=B11010000; PORTC|=B00010000;}
  else if(Rad>soglia3) {PORTC&=B11001000; PORTC|=B00001000;}
  else if(Rad>soglia2) {PORTC&=B11000100; PORTC|=B00000100;}
  else if(Rad>soglia1) {PORTC&=B11000010; PORTC|=B00000010;}
  } // END una volta al secondo
} // END loop

void printImp() // =cpm
{
if(U==1)Imp/=60; // Se viene cambiata unità di misura, diventano cps.
if     (Imp<10) cpmf="    "+String(Imp);
else if(Imp<100) cpmf="   "+String(Imp);
else if(Imp<1000) cpmf="  "+String(Imp);
else if(Imp<10000) cpmf=" "+String(Imp);
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

if(analogOut)
  {
  anOut=(log(Rad)+3)*40;
  if(anOut<0) anOut=0;
  analogWrite(6,anOut);
  }
} // END printRad()
