void setup()
{
lcd.begin(16,2);

pinMode(0,INPUT); // Encoder - A
pinMode(0,INPUT_PULLUP);
pinMode(1,INPUT); // Encoder - B
pinMode(1,INPUT_PULLUP);
pinMode(2,INPUT); // Ingresso A (sonda A)(INT 0).
pinMode(2,INPUT_PULLUP);
pinMode(3,INPUT); // Ingresso B (sonda B)(INT 1).
pinMode(3,INPUT_PULLUP);
pinMode(4,INPUT); // Selezione sonda (a GND: Sonda esterna).
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

for(n=0; n<62; n++) {C[n]=0;}
lcd.createChar(1,aaccentata);
lcd.createChar(2,micro);

if(EEPROM.read(0)==255)
  {
  Bip(); delay(100); Bip(); delay(100); Bip(); delay(100); Bip();
  lcd.setCursor(0,0); lcd.print(" EEPROM VERGINE!");
  lcd.setCursor(0,1); lcd.print("Carico i default");
  EEPROM.update(0,10);    // Ti (Tempo di integrazione): 10 secondi
  EEPROM.update(1,1);  // sonda A: SBM-20  
  EEPROM.update(2,166);// var lo-byte come per SBM-20:   166+
  EEPROM.update(3,0);  // var hi-byte come per SBM-20: 0x256=166
  EEPROM.update(4,45); // in variabile: cpm di fondo proprio del tubo.
  EEPROM.update(5,0);     // TIC software: No.
  EEPROM.update(6,8);  // sonda B: SBT-11  
  EEPROM.update(7,62); // var lo-byte B come per SBM-20:     62+
  EEPROM.update(8,1);  // var hi-byte B come per SBM-20: 1x256=318
  EEPROM.update(9,45); // in variabile B: cpm di fondo proprio del tubo.
  EEPROM.update(10,1); // meter in modalità dot.
  EEPROM.update(11,1); // retroilluminazione dell'LCD: On
  delay(4000);
  lcd.clear();
  }  
Ti=EEPROM.read(0); Tio=Ti; // Carica il tempo di integrazione in Ti e in Tio (valore precedente).
sonda=EEPROM.read(1); // Carica il tipo di sonda A.
var=EEPROM.read(2)+EEPROM.read(3)*256; // Carica Lo-byte e Hi-byte di var del tubo A.
ownbcpm=EEPROM.read(4); // cpm di fondo proprio del tubo A.
TS=EEPROM.read(5); // TIC software Sì/No.
LED=EEPROM.read(10); // Tipo di meter.
LCD=EEPROM.read(11); // LCD: 1:On; 2:On/Off.

if(digitalRead(5)==0) {Bip(); lcd.clear(); TipoDiSonda(); lcd.clear(); TICSwSiNo();} // Se il pulsante è già premuto, salta alle impostazioni, poi ritorna.
if(sonda==ntipi) {sens=var;} else{sens=cost[sonda]; ownbcpm=ownb[sonda];} // var è l'ultima opzione della lista dei tipi di sonde.
  
lcd.clear();
lcd.setCursor(0,0); lcd.print("Contatore Geiger");
lcd.setCursor(5,1); lcd.print(ver);
delay(2000);
lcd.setCursor(0,0); lcd.print("G.Giangreco 2016");
lcd.setCursor(0,1); lcd.print("                ");
delay(1500);
attachInterrupt(0, ContaA, FALLING);
attachInterrupt(1, ContaB, FALLING); Bip();
Mask(); tempo=0; lcd.setCursor(14,0); lcd.print(tempo);
millisZero=millis();
}

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
  if(millis()-t1>4999) {lcd.clear(); return;} // Dopo 5 secondi di inattività esce.
  }
 
if(TS!=EEPROM.read(5)) // Se lo stato di TS è cambiato rispetto a quello memorizzato...
  {
  EEPROM.update(5,TS); // ...aggiorna
  lcd.setCursor(10,1); lcd.print("  SET!  "); Biip(); delay(500);
  }
else Bip();
}
