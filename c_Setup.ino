void setup()
{
lcd.begin(16,2);
Serial.begin(9600);

pinMode(2,INPUT); // Ingresso (sonda)(INT 0)
pinMode(2,INPUT_PULLUP);
pinMode(3,INPUT); // Encoder - A
pinMode(3,INPUT_PULLUP);
pinMode(4,INPUT); // Encoder - B
pinMode(4,INPUT_PULLUP);
pinMode(5,INPUT); // Encoder - Pulsante
pinMode(5,INPUT_PULLUP);

pinMode(7,OUTPUT); // Capsula piezo
pinMode(A1,OUTPUT); // LED 1
pinMode(A2,OUTPUT); // LED 2
pinMode(A3,OUTPUT); // LED 3
pinMode(A4,OUTPUT); // LED 4
pinMode(A5,OUTPUT); // LED 5

for(n=0; n<62; n++) {C[n]=0;}
lcd.createChar(1,aaccentata);
lcd.createChar(2,micro);

if(EEPROM.read(0)==255)
  {
  Bip(); delay(100); Bip(); delay(100); Bip(); delay(100); Bip();
  lcd.setCursor(0,0); lcd.print(" EEPROM VERGINE!");
  lcd.setCursor(0,1); lcd.print("Carico i default");
  EEPROM.update(0,10); // Ti (Tempo di integrazione): 10 secondi
  EEPROM.update(1,1);  // Sonda: SBM-20  
  EEPROM.update(2,166); // var lo-byte come per SBM-20:   166+
  EEPROM.update(3,0);   // var hi-byte come per SBM-20: 0x256=166
  EEPROM.update(4,45); // in variabile: cpm di fondo proprio del tubo.
  delay(4000);
  lcd.clear();
  }  
Ti=EEPROM.read(0); Tio=Ti; // Carica il tempo di integrazione in Ti e in Tio (valore precedente).
sonda=EEPROM.read(1); // Carica il tipo di sonda
var=EEPROM.read(2)+EEPROM.read(3)*256; // Carica Lo-byte e Hi-byte di var.
ownbcpm=EEPROM.read(4); // cpm di fondo proprio del tubo.

if(digitalRead(5)==0) {Bip(); lcd.clear(); TipoDiSonda();} // Se il pulsante è già premuto, salta all'impostazione
if(sonda==10) {sens=var;} else{sens=cost[sonda]; ownbcpm=ownb[sonda];}
lcd.clear();
lcd.setCursor(0,0); lcd.print("   GEIGERINO    ");
lcd.setCursor(5,1); lcd.print(ver);
delay(2000);
lcd.setCursor(0,0); lcd.print("G.Giangreco 2016");
lcd.setCursor(0,1); lcd.print("                ");
delay(1500);
attachInterrupt(0, Conta, FALLING); Bip();
Mask(); tempo=0; lcd.setCursor(14,0); lcd.print(tempo);
millisZero=millis();
}   
