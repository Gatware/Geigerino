void Azzeramento()
{
//    -------- Menu 2 - Azzeramento --------
lcd.print("   AZZERARE?    ");
t1=millis();
while(digitalRead(5)==LOW) // Attende che venga lasciato il pulsante.
{if(millis()-t1>1000) {Bip(); lcd.clear(); massimi(); return;}}  // Premuto per almeno 1 secondo (3,2 totali) salta a massimi(), poi ritorna ed esce.
t1=millis();
while(digitalRead(5)==HIGH) // Continua a leggere l'encoder finché non premo
  {
  encoder();
  if(E!=0){Azz+=E; E=0; t1=millis(); delay(20);}
  if(Azz>1) Azz=1;
  if(Azz<0) Azz=0;
  
  lcd.setCursor(7,1);
  if(Azz==0){lcd.print("No");}
  else{lcd.print("Si");}
  if(millis()-t1>4999) return; // Dopo 5 secondi di inattività esce.
  }
  
if (Azz==1) {Azz=0; Azzera();} // Azzera
else Bip();
} // END Azzeramento()

void Azzera()
{Biiip(); cp=0; tempo=0; m=1; m1=1; t3=0; D=0; DMAX=0; for(m1=1; m1<61; m1++){C[m1]=0;}}
 // END Azzera()

void massimi()
{
lcd.print("    Massimi     ");  
while(digitalRead(5)==LOW); // Attende che venga lasciato il pulsante.
t1=millis();
Mask(); lcd.setCursor(11,0); lcd.print("max"); lcd.setCursor(13,1); lcd.print("max"); 
lcd.setCursor(1,0); if(DMAX*60>ownbcpm) Imp=DMAX*60-ownbcpm; else Imp=0; printImp(); // D è in cps; printImp vuole cpm.
lcd.setCursor(0,1); Rad=float(Imp)/sens; printRad(); // uSv/h in virgola mobile. 
delay(300);
while(digitalRead(5)==HIGH){if(millis()-t1>9999) return;} // Attende che venga premuto il pulsante, ma dopo 10 secondi comunque esce.
} // END massimi()

