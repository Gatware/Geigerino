void TipoDiSonda()
{
//    -------- Menu 3 - Tipo di sonda --------
lcd.print(F("Tipo di sonda A? ")); // ---------- sonda A ----------
sonda=EEPROM.read(1); // Carica il tipo di sonda A.
var=EEPROM.read(2)+EEPROM.read(3)*256; // Carica Lo-byte e Hi-byte di var del tubo A.
ownbcpm=EEPROM.read(4); // cpm di fondo proprio del tubo A.
t1=millis();
while(!(PIND&0x20))// Attende che venga lasciato il pulsante.
{delay(300);} // Antirimbalzo 
t1=millis();
while(PIND&0x20) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0){sonda+=E; E=0; t1=millis(); delay(10);}
  if(sonda>ntipi) {sonda=ntipi; noTone(7);}
  if(sonda<1) {sonda=1; noTone(7);}
  
  lcd.setCursor(5,1);
  lcd.print(tipo[sonda]); lcd.print("   ");
  if(millis()-t1>9999) Riavvia(); // Dopo 10 secondi di inattività esce.
  }
  
if(sonda!=EEPROM.read(1)) {EEPROM.update(1,sonda); Biip(); lcd.setCursor(5,1); lcd.print("  SET!   "); delay(500);}
else Bip();

if(tipo[sonda]=="variabile" || tipo[sonda]=="Variabile")
  {                           // variabile
  lcd.setCursor(0,0); lcd.print(F("  Sensibilit")); lcd.write(1); lcd.print(F("?  "));
  lcd.setCursor(5,1); lcd.print(F("         ")); printVar();
  while(!(PIND&0x20))
  lcd.setCursor(0,1); lcd.print(F("Centin."));
  delay(300);
  t1=millis();
  while(PIND&0x20)
    { // Centinaia
    encoder();
    if(E!=0){var+=100*E; E=0; t1=millis(); delay(10);}
    if(var>999) var-=100;
    if(var<0) var+=100;
    printVar();
    if(millis()-t1>9999) Riavvia(); // Dopo 10 secondi di inattività esce.
    } // Fine dell'inserimento delle centinaia.
    
  while(!(PIND&0x20));
  lcd.setCursor(0,1); lcd.print(F("Decine "));
  Bip(); delay(300);
  t1=millis();
  while(PIND&0x20)
    { // Decine
    encoder();
    if(E!=0){var+=10*E; E=0; t1=millis(); delay(10);}
    if(var>999) var-=10;
    if(var<0) var+=10;
    printVar();
    if(millis()-t1>9999) Riavvia(); // Dopo 10 secondi di inattività esce.
    } // Fine dell'inserimento delle decine.
    
  while(!(PIND&0x20));
  lcd.setCursor(0,1); lcd.print(" Unit"); lcd.write(1); lcd.print(" ");
  Bip(); delay(300);
  t1=millis();
  while(PIND&0x20)
    { // Unità
    encoder();
    if(E!=0){var+=E; E=0; t1=millis(); delay(10);}
    if(var>999) {var=999;}
    else if(var<1) {var=1;}
    printVar();
    if(millis()-t1>9999) Riavvia(); // Dopo 10 secondi di inattività esce.
    } // Fine dell'inserimento delle unità. 

  if(var!=EEPROM.read(2)+EEPROM.read(3)*256)
    {  //      gli 8 LSB (Lo)         e gli 8 MSB (Hi)
    EEPROM.update(2,var&0xFF); EEPROM.update(3,var>>8);
    lcd.setCursor(10,1); lcd.print(F("  SET!   ")); Biip(); delay(500);
    }
  else Bip();
    
  while(!(PIND&0x20));
  lcd.clear();
  lcd.setCursor(2,0); lcd.print(F("CPM di fondo  "));
  delay(300);
  t1=millis();
  while(PIND&0x20)
    { // Cpm di background della sonda
    encoder();
    if(E>0 && ownbcpm<255){ownbcpm+=1; E=0; t1=millis(); delay(10);}
    if(E<0 && ownbcpm>0)  {ownbcpm-=1; E=0; t1=millis(); delay(10);}
    
    lcd.setCursor(6,1);
    if(ownbcpm<10) lcd.print("  "+String(ownbcpm));
    else if(ownbcpm<100) lcd.print(" "+String(ownbcpm));
    else lcd.print(ownbcpm);
    
    if(millis()-t1>9999) Riavvia(); // Dopo 10 secondi di inattività esce.
    } // Fine dell'inserimento del Background proprio del tubo.
                                                      
  if(ownbcpm!=EEPROM.read(4))
    {
    EEPROM.update(4,ownbcpm);
    lcd.setCursor(10,1); lcd.print(F("  SET!  ")); Biip(); delay(500);
    }
  else Bip();
  } // END if(tipo[sonda]==Variabile)
  
  else // IF Sonda==1...ntipi-1
    {
    lcd.clear(); lcd.print("S:"+String(cost[sonda])+"cpm/"); lcd.write(byte(2)); lcd.write(byte(5)); lcd.write(byte(6));
    lcd.setCursor(0,1); lcd.print("Bkg:"+String(ownb[sonda])+"cpm");
    while(!(PIND&0x20));
    delay(500);
    while(PIND&0x20);
    Bip();
    } // END Sonda==1...ntipi-1
  
lcd.clear();

lcd.print(F("Tipo di sonda B? ")); // ---------- sonda B ----------
sonda=EEPROM.read(6); // Carica il tipo di sonda B.
var=EEPROM.read(7)+EEPROM.read(8)*256; // Carica Lo-byte e Hi-byte di var del tubo B.
ownbcpm=EEPROM.read(9); // cpm di fondo proprio del tubo B.
t1=millis();
while(!(PIND&0x20)) // Attende che venga lasciato il pulsante.
{delay(300);} // Antirimbalzo 
t1=millis();
while(PIND&0x20) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0){sonda+=E; E=0; t1=millis(); delay(10);}
  if(sonda>ntipi) sonda=ntipi;
  if(sonda<1) sonda=1;
  
  lcd.setCursor(5,1);
  lcd.print(tipo[sonda]); lcd.print("   ");
  if(millis()-t1>9999) Riavvia(); // Dopo 10 secondi di inattività esce.
  }
  
if(sonda!=EEPROM.read(6)) {EEPROM.update(6,sonda); Biip(); lcd.setCursor(5,1); lcd.print("  SET!    "); delay(500);}
else Bip();

if(tipo[sonda]=="variabile" || tipo[sonda]=="Variabile")
  {                           // variabile
  lcd.setCursor(0,0); lcd.print(F("  Sensibilit")); lcd.write(1); lcd.print(F("?  "));
  lcd.setCursor(5,1); lcd.print(F("         ")); printVar();
  while(!(PIND&0x20));
  lcd.setCursor(0,1); lcd.print(F("Centin."));
  delay(300);
  t1=millis();
  while(PIND&0x20)
    { // Centinaia
    encoder();
    if(E!=0){var+=100*E; E=0; t1=millis(); delay(10);}
    if(var>999) var-=100;
    if(var<0) var+=100;
    printVar();
    if(millis()-t1>9999) Riavvia(); // Dopo 10 secondi di inattività esce.
    } // Fine dell'inserimento delle centinaia.
    
  while(!(PIND&0x20));
  lcd.setCursor(0,1); lcd.print(F("Decine "));
  Bip(); delay(300);
  t1=millis();
  while(PIND&0x20)
    { // Decine
    encoder();
    if(E!=0){var+=10*E; E=0; t1=millis(); delay(10);}
    if(var>999) var-=10;
    if(var<0) var+=10;
    printVar();
    if(millis()-t1>9999) Riavvia(); // Dopo 10 secondi di inattività esce.
    } // Fine dell'inserimento delle decine.
    
  while(!(PIND&0x20));
  lcd.setCursor(0,1); lcd.print(F(" Unit")); lcd.write(1); lcd.print(F(" ")); // lcd.write(1): à
  Bip(); delay(300);
  t1=millis();
  while(PIND&0x20)
    { // Unità
    encoder();
    if(E!=0){var+=E; E=0; t1=millis(); delay(10);}
    if(var>999) {var=999;}
    else if(var<1) {var=1;}
    printVar();
    if(millis()-t1>9999) Riavvia(); // Dopo 10 secondi di inattività esce.
    } // Fine dell'inserimento delle unità. 

  if(var!=EEPROM.read(7)+EEPROM.read(8)*256)
    {  //      gli 8 LSB (Lo)         e gli 8 MSB (Hi)
    EEPROM.update(7,var&0xFF); EEPROM.update(8,var>>8);
    lcd.setCursor(10,1); lcd.print(F("  SET!   ")); Biip(); delay(500);
    }
  else Bip();
    
  while(!(PIND&0x20));
  lcd.clear();
  lcd.setCursor(2,0); lcd.print(F("CPM di fondo  "));
  delay(300);
  t1=millis();
  while(PIND&0x20)
    { // Cpm di background della sonda
    encoder();
    if(E>0 && ownbcpm<255){ownbcpm+=1; E=0; t1=millis(); delay(10);}
    if(E<0 && ownbcpm>0)  {ownbcpm-=1; E=0; t1=millis(); delay(10);}
    
    lcd.setCursor(6,1);
    if(ownbcpm<10) lcd.print("  "+String(ownbcpm));
    else if(ownbcpm<100) lcd.print(" "+String(ownbcpm));
    else lcd.print(ownbcpm);
    
    if(millis()-t1>9999) Riavvia(); // Dopo 10 secondi di inattività esce.
    } // Fine dell'inserimento del Background proprio del tubo.
                                                      
  if(ownbcpm!=EEPROM.read(9))
    {
    EEPROM.update(9,ownbcpm);
    lcd.setCursor(10,1); lcd.print(F("  SET!  ")); Biip(); delay(500);
    }
    else Bip();
  } // END if(tipo[sonda]==Variabile)
  
else // IF Sonda==1...ntipi-1
  {
  lcd.clear(); lcd.print("S:"+String(cost[sonda])+"cpm/"); lcd.write(byte(2)); lcd.write(byte(5)); lcd.write(byte(6));
  lcd.setCursor(0,1); lcd.print("Bkg:"+String(ownb[sonda])+"cpm");
  while(!(PIND&0x20));
  delay(500);
  while(PIND&0x20);
  Bip();
  } // END Sonda==1...9
  
lcd.clear();
}

void printVar()
{
lcd.setCursor(7,1);
if(var<10) lcd.print("  "+String(var));
else if(var<100) lcd.print(" "+String(var));
else lcd.print(var);
}

