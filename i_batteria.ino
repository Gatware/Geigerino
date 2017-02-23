void BattIco()
{
pinMode(A1,INPUT);
delayMicroseconds(100);
Vb=int(analogRead(A1)*XVref/1000L);
pinMode(A1,OUTPUT);

// (Il display può caricare in memoria un massimo di 8 simboli: carico solo quello che mi serve se lo stato è diverso dal precedente)
     if(Vb>838) {bat=7;} // 4,1V  Batt_7 100%
else if(Vb>808) {bat=6;} // 3,95V Batt_6  83% 
else if(Vb>777) {bat=5;} // 3,8V  Batt_5  67% 
else if(Vb>745) {bat=4;} // 3,65V Batt_4  50% 
else if(Vb>716) {bat=3;} // 3,5V  Batt_3  33%
else if(Vb>695) {bat=2;} // 3,4V  Batt_2  22%
else if(Vb>675) {bat=1;} // 3,3V  Batt_1  11%
else            {bat=0;} // 3,2V  Batt_0   0%

switch (bat)
  {
  case 0: lcd.createChar(0, Batt_0); break;
  case 1: lcd.createChar(0, Batt_1); break;
  case 2: lcd.createChar(0, Batt_2); break;
  case 3: lcd.createChar(0, Batt_3); break;
  case 4: lcd.createChar(0, Batt_4); break;
  case 5: lcd.createChar(0, Batt_5); break;
  case 6: lcd.createChar(0, Batt_6); break;
  case 7: lcd.createChar(0, Batt_7); break;
  }
lcd.setCursor(15,1); lcd.write(byte(0));
}

