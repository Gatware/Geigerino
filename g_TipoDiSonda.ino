void TipoDiSonda()
{
//    -------- Menu 3 - Tipo di sonda --------
lcd.print(" Tipo di sonda? ");
t1=millis();
while(digitalRead(5)==LOW) // Attende che venga lasciato il pulsante.
delay(100); // Antirimbalzo 
t1=millis();
while(digitalRead(5)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0){sonda+=E; E=0; t1=millis(); delay(20);}
  if(sonda>10) sonda=10;
  if(sonda<0) sonda=0;
  
  lcd.setCursor(5,1);
  lcd.print(tipo[sonda]); lcd.print("   ");
  if(millis()-t1>9999) return; // Dopo 5 secondi di inattività esce.
  }
  
if(sonda!=EEPROM.read(1)) {EEPROM.update(1,sonda); Biip(); lcd.setCursor(5,1); lcd.print("  SET!  "); delay(500);}
else Bip();

if(sonda==10)
  {                           // variabile
  lcd.setCursor(0,0); lcd.print("  Sensibilit"); lcd.write(byte(1)); lcd.print("?  ");
  lcd.setCursor(5,1); lcd.print("         "); printVar();
  while(digitalRead(5)==LOW);
  lcd.setCursor(10,1); lcd.print("Cent.");
  delay(300);
  t1=millis();
  while(digitalRead(5)==HIGH)
    { // Centinaia
    encoder();
    if(E!=0){var+=100*E; E=0; t1=millis(); delay(20);}
    if(var>999) var-=100;
    if(var<0) var+=100;
    printVar();
    if(millis()-t1>9999) return; // Dopo 10 secondi di inattività esce.
    } // Fine dell'inserimento delle centinaia.
    
  while(digitalRead(5)==LOW);
  lcd.setCursor(10,1); lcd.print("Decine");
  Bip(); delay(300);
  t1=millis();
  while(digitalRead(5)==HIGH)
    { // Decine
    encoder();
    if(E!=0){var+=10*E; E=0; t1=millis(); delay(20);}
    if(var>999) var-=10;
    if(var<0) var+=10;
    printVar();
    if(millis()-t1>9999) return; // Dopo 10 secondi di inattività esce.
    } // Fine dell'inserimento delle decine.
    
  while(digitalRead(5)==LOW);
  lcd.setCursor(10,1); lcd.print("Unit"); lcd.write(1); lcd.print(" ");
  Bip(); delay(300);
  t1=millis();
  while(digitalRead(5)==HIGH)
    { // Unità
    encoder();
    if(E!=0){var+=E; E=0; t1=millis(); delay(20);}
    if(var>999) {var=999;}
    else if(var<1) {var=1;}
    printVar();
    if(millis()-t1>9999) return; // Dopo 10 secondi di inattività esce.
    } // Fine dell'inserimento delle unità. 

  if(var!=EEPROM.read(2)+EEPROM.read(3)*256)
    {  //      gli 8 LSB (Lo)         e gli 8 MSB (Hi)
    EEPROM.update(2,var&0xFF); EEPROM.update(3,var>>8);
    lcd.setCursor(10,1); lcd.print("  SET!  "); Biip(); delay(500);
    }
  else Bip();
    
  while(digitalRead(5)==LOW);
  lcd.clear();
  lcd.setCursor(2,0); lcd.print("CPM di fondo  ");
  delay(300);
  t1=millis();
  while(digitalRead(5)==HIGH)
    { // Cpm di background della sonda
    encoder();
    if(E>0 && ownbcpm<255){ownbcpm+=1; E=0; t1=millis(); delay(20);}
    if(E<0 && ownbcpm>0)  {ownbcpm-=1; E=0; t1=millis(); delay(20);}
    
    lcd.setCursor(6,1);
    if(ownbcpm<10) lcd.print("  "+String(ownbcpm));
    else if(ownbcpm<100) lcd.print(" "+String(ownbcpm));
    else lcd.print(ownbcpm);
    
    if(millis()-t1>9999) return; // Dopo 10 secondi di inattività esce.
    } // Fine dell'inserimento del Background proprio del tubo.
                                                      
  if(ownbcpm!=EEPROM.read(4))
    {
    EEPROM.update(4,ownbcpm);
    lcd.setCursor(10,1); lcd.print("  SET!  "); Biip(); delay(500);
    }
    else Bip();
  } // END if(sonda==10)
  
  else // IF Sonda==1...9
    {
    lcd.clear(); lcd.print("S:"+String(cost[sonda])+"cpm/("); lcd.write(byte(2)); lcd.print("Sv/h)");
    lcd.setCursor(0,1); lcd.print("Bkg:"+String(ownb[sonda])+"cpm");
    while(digitalRead(5)==LOW);
    delay(500);
    while(digitalRead(5)==HIGH);
    } // END Sonda==1...9
  
lcd.clear();
}

void printVar()
{
lcd.setCursor(6,1);
if(var<10) lcd.print("  "+String(var));
else if(var<100) lcd.print(" "+String(var));
else lcd.print(var);
}
