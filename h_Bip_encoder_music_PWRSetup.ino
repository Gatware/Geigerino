void Bip()   {if(biptic==1||biptic==3)tone(7,1000,15);}

void Biip()  {if(biptic)tone(7,1000,250);}

void Biiip() {if(biptic)tone(7,1000,800);}

void encoder()
{
//S=3-(PIND>>3)&B00000011; Serviva per l'encoder su PD3 e PD4.
           // PD 76543210
S=3-PIND&B00000011; // Gli I/O 0 e 1 sono PD0 e PD1, perciò non devo scorrere a destra. 
// Il valore binario di S rappresenta A e B. Il centrale dell'encoder è a massa,
                    // quindi faccio complemento a 3 (11)  
S^=S>>1; // Faccio XOR (^) fra S (gray) e il suo bit 1, facendolo scorrere a Dx: AB XOR A,
         // ottenendo un binario che per ogni scatto fa 0-1-2-3-0 oppure 0-3-2-1-0.
if (S!=So && S==0) X=0;
if (X==0)
  {
  if (So==1&&S==2)
    {E=1; X=1; if(!mute)Bip();}
  if (So==3&&S==2)
    {E=-1; X=1;if(!mute)Bip();}
  if (S==0)
    {E=0; X=0;}
  So=S;  
  }
}

void music()
{
lcd.clear();
detachInterrupt(0);
byte D1=150;
byte D2=150;
lcd.print(F("RADIO"));
tone(7,MI,D1);  delay(250);
tone(7,LA,D2);  delay(500);
lcd.print(F(" ACTIVITY"));
tone(7,LA,D1);  delay(250);
tone(7,SOL,D1); delay(250);
tone(7,LA,D1);  delay(250);
tone(7,MI,D1);  delay(250);
delay(250); // Pausa
lcd.setCursor(0,0);
lcd.print(F("Is in the air   "));
tone(7,LA,D1);  delay(250);
tone(7,DO,D2);  delay(500);
lcd.setCursor(0,1);
lcd.print(F("for you and me! "));
tone(7,SI,D1);  delay(250);
tone(7,LA,D2);  delay(1000);
attachInterrupt(0,ContaAB,FALLING);
lcd.clear();
Mask();
}

void powerSetup()
{
Biip();
lcd.print(F(" Alimentazione? "));
while(!(PIND&0x20)) // Attende che venga lasciato il pulsante.
delay(300);
while(PIND&0x20) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0) pwr+=E;
  if(pwr>1) {noTone(7); pwr=1;} // 5V
  if(pwr<0) {noTone(7); pwr=0;} // Litio diretta
  if(E!=0)  {E=0; t1=millis(); delay(20);}
  lcd.setCursor(3,1);
  if(pwr==0) lcd.print(F("Litio dir."));
    else lcd.print(F("   5V     "));
  }
if(pwr!=EEPROM.read(13)) {EEPROM.update(13,pwr); Biip(); lcd.setCursor(9,1); lcd.print(F("SET!   ")); Biip(); delay(500);}
  else Bip();
delay(300); 

// -------------------------------- Impostazione della capacità della batteria --------------------------------
lcd.clear(); lcd.print("VScarBt:");
while(!(PIND&0x20))
{delay(300);} // Attende che venga lasciato il pulsante.
while(PIND&0x20) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0){VSB+=E;E=0;delay(5);}
  if(VSB<20)VSB=20; // 20=2,0mV/h
  if(VSB>99)VSB=99; // 99=9,9mV/h
  lcd.setCursor(8,0); lcd.print(String(VSB/10)+"."+String(VSB%10)+"mV/h");
  lcd.setCursor(0,1); lcd.print("Auton. Max:" +String(int(10000/VSB)) +"h");
  };
Bip(); // Ho premuto il pulsante, perciò prosegue 
if (VSB!=EEPROM.read(17)) {EEPROM.update(17,VSB); lcd.setCursor(11,1); lcd.print(" SET!"); Biip(); delay(1200);}
delay(300);                    // Se è cambiata, memorizza l'impostazione in EEPROM 17.

// ------------------------------------------- Impostazione di Vref -------------------------------------------
lcd.clear(); lcd.print(F("Taratura di Vref"));
lcd.setCursor(1,1); lcd.print(F("Vpin21:     mV"));
while(!(PIND&0x20))
{delay(300);} // Attende che venga lasciato il pulsante.
while(PIND&0x20) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0){VrefDec+=E;}
  if(VrefDec<0){VrefDec=0; noTone(7);} // 0=1000mV
  if(VrefDec>200){VrefDec=200; noTone(7);} // 200=1200mV
  if(E){E=0; delay(10);}
  lcd.setCursor(9,1); lcd.print(1000+VrefDec);  
  };
Bip(); // Ho premuto il pulsante, perciò prosegue
if (VrefDec!=EEPROM.read(18)) {EEPROM.update(18,VrefDec); lcd.setCursor(1,1); lcd.print(F("   SET!")); Biip(); delay(1200);}
delay(300);                    // Se è cambiata, memorizza l'impostazione in EEPROM 18.
lcd.clear();

// ------------------------------------------- Test dello strumento -------------------------------------------
lcd.print(F("Test strumento?"));
while(!(PIND&0x20)) // Attende che venga lasciato il pulsante.
delay(300);
byte Test=0;

while(PIND&0x20) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(Test==0 && E==1) Test=1; // Sì
  else if(Test==1 && E==-1) Test=0; // No
  else noTone(7);
  if(E!=0)  {E=0; t1=millis(); delay(20);}
  lcd.setCursor(7,1);
  if(Test) lcd.print(F("Si"));
  else lcd.print(F("No"));
  }
  Bip();
  if(Test)
    {
    lcd.setCursor(14,0); lcd.print(' '); // Cancella il "?".
    pinMode(A2,INPUT);
    delayMicroseconds(100);
    analogRead(A2); // Prelettura
    Vcc=int(analogRead(A2)*XVref/1000L); // Legge la tensione di alimentazione.
    pinMode(A2,OUTPUT);
    muoveAgo();
    while(!(PIND&0x20)); delay(200);
    while(PIND&0x20) // Continua a leggere l'encoder finché non premo
      {
      encoder();
      if(E==1 && valore<=204) valore+=51; // Incrementa
      else if(E==-1 && valore>=51) valore-=51; // Decrementa
      else noTone(7);
      if(E!=0) muoveAgo();
      }
    Bip();
    analogWrite(6,0);
        
    /*
    for(valore=0; valore<=204; valore+=51)
      {
      lcd.setCursor(12,1);
      if     (valore==0)   lcd.print(" 0,1");
      else if(valore==51)  lcd.print("   1");
      else if(valore==102) lcd.print("  10");
      else if(valore==153) lcd.print(" 100");
      else                 lcd.print("1000");
      anOut=valore+51; // anOut=51; 102; 153; 204; 255.
      if(pwr==0)  anOut=int(anOut*635/Vcc); // Se è alimentato direttamente da Litio (605=3V) o 5V in carica.
      else anOut=int(anOut*635/1024); // Se è alimentato a 5V fissi.
      analogWrite(6,anOut);  while(!(PIND&0x20)); delay(200); while(PIND&0x20); delay(200);
      }
    analogWrite(6,0);
    */
    }
  
lcd.clear();
lcd.setCursor(4,0); lcd.print(ver);
lcd.setCursor(5,1); lcd.print(data);
while(!(PIND&0x20)){delay(1000);}
while(PIND&0x20);
Bip(); delay(500);
Riavvia();
}

void muoveAgo()
{
E=0; t1=millis(); delay(20);
lcd.setCursor(6,1);                       //      LED 54321-        LED 54321-
     if(valore==0)   {lcd.print(F("0,01")); PORTC&=B11000001; PORTC|=B00000000;}
else if(valore==51)  {lcd.print(F(" 0,1")); PORTC&=B11000011; PORTC|=B00000010;}
else if(valore==102) {lcd.print(F("   1")); PORTC&=B11000101; PORTC|=B00000100;}
else if(valore==153) {lcd.print(F("  10")); PORTC&=B11001001; PORTC|=B00001000;}
else if(valore==204) {lcd.print(F(" 100")); PORTC&=B11010001; PORTC|=B00010000;}
else                 {lcd.print(F("1000")); PORTC&=B11100001; PORTC|=B00100000;}
anOut=valore; // anOut=51; 102; 153; 204; 255.
if(pwr==0)  anOut=int(anOut*635/Vcc); // Se è alimentato direttamente da Litio (605=3V) o 5V in carica.
else anOut=int(anOut*635/1024); // Se è alimentato a 5V fissi.
analogWrite(6,anOut);
}
