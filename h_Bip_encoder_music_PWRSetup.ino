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
  if(E!=0){VSB+=E;E=0;delay(10);}
  if(VSB<20)VSB=20; // 20=2,0mV/h
  if(VSB>99)VSB=99; // 99=9,9mV/h
  lcd.setCursor(8,0); lcd.print(String(VSB/10)+"."+String(VSB%10)+"mV/h");
  lcd.setCursor(0,1); lcd.print("Auton. Max:" +String(int(10000/VSB)) +"h");
  };
Bip(); // Ho premuto il pulsante, perciò prosegue 
if (VSB!=EEPROM.read(17)) {EEPROM.update(17,VSB); lcd.setCursor(11,1); lcd.print(" SET!"); Biip(); delay(1200);}
delay(300);                    // Se è cambiata, memorizza l'impostazione in EEPROM 17.

// ------------------------------------------- Impostazione di Vref -------------------------------------------
lcd.clear(); lcd.print("Taratura di Vref");
lcd.setCursor(1,1); lcd.print("Vpin21:     mV");
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
if (VrefDec!=EEPROM.read(18)) {EEPROM.update(18,VrefDec); lcd.setCursor(1,1); lcd.print("   SET!"); Biip(); delay(1200);}
delay(300);                    // Se è cambiata, memorizza l'impostazione in EEPROM 18.

lcd.clear();
lcd.setCursor(4,0); lcd.print(ver);
lcd.setCursor(5,1); lcd.print(data);
while(PIND&0x20);
while(!(PIND&0x20)){delay(300);}

Riavvia();
}

