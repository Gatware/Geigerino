void Bip()   {if(biptic==1||biptic==3)tone(7,1000,15);}

void Biip()  {if(biptic)tone(7,1000,250);}

void Biiip() {if(biptic)tone(7,1000,800);}

void Tic()   {if(biptic==2||biptic==3)tone(SPK,1000,10);}

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
lcd.print("RADIO");
tone(7,MI,D1);
delay(250);
tone(7,LA,D2);
delay(500);
lcd.print(" ACTIVITY");
tone(7,LA,D1);
delay(250);
tone(7,SOL,D1);
delay(250);
tone(7,LA,D1);
delay(250);
tone(7,MI,D1);
delay(250);
delay(250); // Pausa
lcd.setCursor(0,0);
lcd.print("Is in the air   ");
tone(7,LA,D1);
delay(250);
tone(7,DO,D2);
delay(500);
lcd.setCursor(0,1);
lcd.print("for you and me! ");
tone(7,SI,D1);
delay(250);
tone(7,LA,D2);
delay(1000);
attachInterrupt(0,ContaAB,FALLING);
lcd.clear();
Mask();
}

void powerSetup()
{
Biip();
lcd.print(" Alimentazione? ");
while(digitalRead(5)==LOW)// Attende che venga lasciato il pulsante.
delay(300);
while(digitalRead(5)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0) pwr+=E;
  if(pwr>1) {noTone(7); pwr=1;} // 5V
  if(pwr<0) {noTone(7); pwr=0;} // Litio diretta
  if(E!=0)  {E=0; t1=millis(); delay(20);}
  lcd.setCursor(3,1);
  if(pwr==0) lcd.print("Litio dir.");
    else lcd.print("   5V     ");
  }
if(pwr!=EEPROM.read(13)) {EEPROM.update(13,pwr); Biip(); lcd.setCursor(9,1); lcd.print("SET!   "); delay(500);}
  else Bip();
delay(300); Riavvia();
}

