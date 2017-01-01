void setup()
{
lcd.begin(16,2);

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
digitalWrite(A0,HIGH); // Spegne la retroilluminazione.

for(n=0; n<62; n++) {C[n]=0;}
lcd.createChar(1,aaccentata);
lcd.createChar(2,micro);

if(EEPROM.read(0)==255)
  {
  Bip(); delay(100); Bip(); delay(100); Bip(); delay(100); Biip();
  lcd.setCursor(0,0); lcd.print(" EEPROM VERGINE!");
  lcd.setCursor(0,1); lcd.print("Carico i default");
  EEPROM.update(0,10); // Ti (Tempo di integrazione): 10 secondi
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
  delay(4000);
  lcd.clear();
  }  
Ti=EEPROM.read(0); Tio=Ti; // Carica il tempo di integrazione in Ti e in Tio (valore precedente).
if(digitalRead(4)==1)
  {
  sonda=EEPROM.read(1); // Carica il tipo di sonda A.
  var=EEPROM.read(2)+EEPROM.read(3)*256; // Carica Lo-byte e Hi-byte di var del tubo A.
  ownbcpm=EEPROM.read(4); // cpm di fondo proprio del tubo A.
  }
else
  {
  sonda=EEPROM.read(6); // Carica il tipo di sonda B.
  var=EEPROM.read(7)+EEPROM.read(8)*256; // Carica Lo-byte e Hi-byte di var del tubo B.
  ownbcpm=EEPROM.read(9); // cpm di fondo proprio del tubo B.
  }
  
TS=EEPROM.read(5); // TIC software Sì/No.
LED=EEPROM.read(10); // Tipo di meter.
LCD=EEPROM.read(11); // LCD: 1:On; 2:On/Off.
biptic=EEPROM.read(12); // biptic: 0:Nssuno; 1:Bip; 2:Tic-tic; 3:Bip + Tic-tic.

                       // Se il pulsante è già premuto, salta alle impostazioni, poi ritorna.
if(digitalRead(5)==0) {Bip(); lcd.clear(); TipoDiSonda(); lcd.clear(); dotBar(); lcd.clear(); TICSwSiNo();} 
if(sonda==ntipi) {sens=var;} else{sens=cost[sonda]; ownbcpm=ownb[sonda];} // var è l'ultima opzione della lista dei tipi di sonde.

lcd.clear();
lcd.setCursor(0,0); lcd.print("Contatore Geiger");
lcd.setCursor(5,1); lcd.print(ver);
delay(2000);
lcd.setCursor(0,0); lcd.print("G.Giangreco 2016");
lcd.setCursor(0,1); lcd.print("                ");
delay(1500);
attachInterrupt(0, ContaAB, FALLING); Bip();
Mask(); tempo=0; temposecondi=0; lcd.setCursor(14,0); lcd.print(temposecondi);
millisZero=millis();
} // END setup

void TICSwSiNo()
{
while(digitalRead(5)==0); // Attende che venga lasciato il pulsante
lcd.print(" TIC software?  ");
t1=millis();
while(digitalRead(5)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0){TS+=E; E=0; t1=millis(); delay(20);}
  if(TS>1) TS=1;
  if(TS<0) TS=0;
  
  lcd.setCursor(7,1);
  if(TS==1){lcd.print("Si");}
  else {lcd.print("No");}
  if(millis()-t1>4999) {lcd.clear(); Riavvia();} // Dopo 5 secondi di inattività esce.
  }
 
if(TS!=EEPROM.read(5)) // Se lo stato di TS è cambiato rispetto a quello memorizzato...
  {
  EEPROM.update(5,TS); // ...aggiorna
  lcd.setCursor(10,1); lcd.print("  SET!  "); Biip(); delay(500);
  }
else Bip();
} // END TicSwSiNo

void dotBar()
{
lcd.print("       LED      "); 
t1=millis();
while(digitalRead(5)==LOW); // Attende che venga lasciato il pulsante.
delay(300);
lcd.setCursor(0,1); lcd.print(" Off   Dot   Bar");
t1=millis();
while(digitalRead(5)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0){LED+=E; E=0; t1=millis(); delay(20);}
  if(LED>2) LED=2;
  if(LED<0) LED=0;

  if(LED==0)      {lcd.setCursor(0,1); lcd.print("x"); lcd.setCursor(6,1); lcd.print(" "); lcd.setCursor(12,1); lcd.print(" ");}
  else if(LED==1) {lcd.setCursor(0,1); lcd.print(" "); lcd.setCursor(6,1); lcd.print("x"); lcd.setCursor(12,1); lcd.print(" ");}
  else if(LED==2) {lcd.setCursor(0,1); lcd.print(" "); lcd.setCursor(6,1); lcd.print(" "); lcd.setCursor(12,1); lcd.print("x");}
  
  if(millis()-t1>4999) Riavvia(); // Dopo 5 secondi di inattività esce.
  }
if(LED!=EEPROM.read(10)) {EEPROM.update(10,LED); Biip(); lcd.setCursor(12,1); lcd.print(" SET!"); delay(500);}
else Bip();  
} // END dotBar



