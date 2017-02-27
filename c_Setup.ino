// while(!(PIND&0x20)) è come scrivere: while(digitalRead(5)=LOW)
// while( (PIND&0x20)) è come scrivere: digitalRead(5)=HIGH

void setup()
{
lcd.begin(16,2);
analogReference (INTERNAL); // Imposta per l'ADC il riferimento interno a 1,1V nominali (+/-10%)
pinMode(0,INPUT); // Encoder - A
pinMode(0,INPUT_PULLUP);
pinMode(1,INPUT); // Encoder - B
pinMode(1,INPUT_PULLUP);
pinMode(2,INPUT); // Ingresso sonde Ae B (INT 0).
pinMode(2,INPUT_PULLUP);
pinMode(3,OUTPUT); // Altoparlante per Tic-tic.
pinMode(4,INPUT); // Selezione sonda (a GND: Sonda B=esterna).
pinMode(4,INPUT_PULLUP);
pinMode(5,INPUT); // Encoder - Pulsante.
pinMode(5,INPUT_PULLUP);

pinMode(7,OUTPUT); // Capsula piezo.
pinMode(A0,OUTPUT); // Negativo della retroilluminazione
pinMode(A1,OUTPUT); // LED 1
pinMode(A2,OUTPUT); // LED 2
pinMode(A3,OUTPUT); // LED 3
pinMode(A4,OUTPUT); // LED 4
pinMode(A5,OUTPUT); // LED 5 + Tensione della batteria via partitore 1M/270k.
if(EEPROM.read(11)==2) digitalWrite(A0,HIGH); // Se LCD è in On/Off, spegne la retroilluminazione.

for(n=0; n<62; n++) {C[n]=0;}
lcd.createChar(1,aaccentata);
lcd.createChar(2,micro);
lcd.createChar(3,piumeno);

if(EEPROM.read(1)==255) // 
  {
  Bip(); delay(100); Bip(); delay(100); Bip(); delay(100); Biip();
  lcd.setCursor(0,0); lcd.print(" EEPROM VERGINE!");
  lcd.setCursor(0,1); lcd.print("Carico i default");
  delay(3000);
  EEPROM.update(0,40); // Lo-byte di Ti (Tempo di integrazione): 40 secondi
  EEPROM.update(15,0); // Hi-byte di Ti
  EEPROM.update(1,3);  // sonda A: 2xSBM-20  
  EEPROM.update(2,76); // var lo-byte come per 2xSBM-20:    76+
  EEPROM.update(3,1);  // var hi-byte come per 2xSBM-20: 1x256=332
  EEPROM.update(4,64); // in variabile: cpm di fondo proprio del tubo come per 2xSBM-20.
  EEPROM.update(5,0);  // TIC software: No.
  EEPROM.update(6,10); // sonda B: SBT-11  
  EEPROM.update(7,62); // var lo-byte B come per SBT-11:    62+
  EEPROM.update(8,1);  // var hi-byte B come per SBT-11: 1x256=318
  EEPROM.update(9,15); // in variabile B: cpm di fondo proprio del tubo come per SBT-11.
  EEPROM.update(10,1); // meter in modalità dot.
  EEPROM.update(11,1); // retroilluminazione dell'LCD: On
  EEPROM.update(12,3); // suoni: Bip + Tic-tic.
  EEPROM.update(13,0); // Alimentazione: Litio dir.
  EEPROM.update(14,1); // Allarme: Sì  
//              15 è l'Hi-byte di Ti.
  EEPROM.update(16,5); // Precisione %
  lcd.setCursor(0,1); lcd.print("     Fatto.     ");
  lcd.clear();
  }  
Ti=EEPROM.read(0)+EEPROM.read(15)*256; Tio=Ti; // Carica il tempo di integrazione in Ti e in Tio (valore precedente).
if(digitalRead(4)==1)
  {
  sonda=EEPROM.read(1); // Carica il tipo di sonda A.
  var=EEPROM.read(2)+EEPROM.read(3)*256; // Carica Lo-byte e Hi-byte di var del tubo A.
  ownbcpm=EEPROM.read(4); // cpm di fondo proprio del tubo A.
  sinto=1;
  }
else
  {
  sonda=EEPROM.read(6); // Carica il tipo di sonda B.
  var=EEPROM.read(7)+EEPROM.read(8)*256; // Carica Lo-byte e Hi-byte di var del tubo B.
  ownbcpm=EEPROM.read(9); // cpm di fondo proprio del tubo B.
  sinto=0;
  }
  
TS=EEPROM.read(5); // TIC software Sì/No.
LED=EEPROM.read(10); // Tipo di meter.
LCD=EEPROM.read(11); // LCD: 1:On; 2:On/Off.
biptic=EEPROM.read(12); // biptic: 0:Nssuno; 1:Bip; 2:Tic-tic; 3:Bip + Tic-tic.
pwr=EEPROM.read(13); // Alimentazione: 0:Litio dir.; 1:5V.
alm=EEPROM.read(14); // Allarme: 0:disattivato; 1:attivato.
prec=EEPROM.read(16); // Legge la precisione impostata.
valPrec=10000/sq(prec);
                       // Se il pulsante è già premuto, salta alle impostazioni, poi ritorna:
if(!(PIND&0x20)) {Bip(); lcd.clear(); dotBar(); lcd.clear(); TICSwSiNo(); lcd.clear(); TipoDiSonda();} 
if(sonda==ntipi) {sens=var;} else{sens=cost[sonda]; ownbcpm=ownb[sonda];} // var è l'ultima opzione della lista dei tipi di sonde.

lcd.clear();
lcd.setCursor(0,0); lcd.print("GEIGERINO "+ver);
lcd.setCursor(0,1); lcd.print("G.Giangreco 2016");
delay(1500); lcd.clear();
if(Ti<TMAX) lcd.print("Ti="+String(Ti)+" sec."); else if(Ti==TMAX) lcd.print("Ti=Continuo"); else {Disp2=1; lcd.print("Precisione fissa"); lcd.setCursor(7,1); lcd.print(String(prec)+"%");} Bip();
delay(1200);
attachInterrupt(0, ContaAB, FALLING); Bip(); delay(50);
Mask(); Azzera(); lcd.setCursor(14,0); lcd.print(temposecondi);
millisZero=millis();
} // END setup

// -----------------------------------------------------------------------------

void dotBar()
{
lcd.print("       LED      "); 
t1=millis();
lcd.setCursor(0,1); lcd.print(" Off   Dot   Bar");
while(!(PIND&0x20)) // Attende che venga lasciato il pulsante.
{if(millis()-t1>3000) {lcd.clear(); powerSetup();}}
delay(300);
t1=millis();
while(PIND&0x20) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0) LED+=E;
  if(LED>3) {noTone(7); LED=3;} // Al di fuori dai limiti non fa più Bip (ma rimane un brevissimo tic).
  if(LED<1) {noTone(7); LED=1;}
  if(E!=0) {E=0; t1=millis(); delay(20);}
  
  if(LED==1)      {lcd.setCursor(0,1); lcd.print("x"); lcd.setCursor(6,1); lcd.print(" "); lcd.setCursor(12,1); lcd.print(" ");}
  else if(LED==2) {lcd.setCursor(0,1); lcd.print(" "); lcd.setCursor(6,1); lcd.print("x"); lcd.setCursor(12,1); lcd.print(" ");}
  else if(LED==3) {lcd.setCursor(0,1); lcd.print(" "); lcd.setCursor(6,1); lcd.print(" "); lcd.setCursor(12,1); lcd.print("x");}
  
  if(millis()-t1>4999) Riavvia(); // Dopo 5 secondi di inattività esce.
  }
if(LED!=EEPROM.read(10)) {EEPROM.update(10,LED); Biip(); lcd.setCursor(12,1); lcd.print(" SET!"); delay(500);}
else Bip();  
} // END dotBar

void TICSwSiNo()
{
while(!(PIND&0x20)); // Attende che venga lasciato il pulsante
lcd.print(" TIC software?  ");
t1=millis();
while(PIND&0x20) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0) TS+=E;
  if(TS>1) {noTone(7); TS=1;}
  if(TS<0) {noTone(7); TS=0;}
  if(E!=0) {E=0; t1=millis(); delay(20);}
  lcd.setCursor(7,1);
  if(TS==1){lcd.print("Si");}
  else {lcd.print("No");}
  if(millis()-t1>4999) {lcd.clear(); Riavvia();} // Dopo 5 secondi di inattività esce.
  }
if(TS!=EEPROM.read(5)) // Se lo stato di TS è cambiato rispetto a quello memorizzato...
  {EEPROM.update(5,TS); Biip(); lcd.setCursor(10,1); lcd.print("  SET!  "); delay(500);}
else Bip();
} // END TicSwSiNo





