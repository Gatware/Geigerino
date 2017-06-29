/*
***********************************************************************
*   ******************** G E I G E R I N O ************************   *
*         ******* Un Contatore Geiger con Arduino ********            *
*   *********  Copyright Gianluca Giangreco 2016-2017  ************   *
****** https://github.com/Gatware/Geigerino/tree/GEIGERINO_1.8U *******                              
*                                                                     *
* Questo software è a puro scopo didattico, pertanto  non assumo res- *
* ponsabilità sulla precisione e veridicità  dei valori riportati. Mi *
* sono impegnato  nello scriverlo e ringrazio fin d'ora  chi  volesse *
* aiutarmi con correzioni, miglioramenti o suggerimenti.              *
*                                                                     *
* GEIGERINO is free software:you can redistribute it and/or modify it *
* under the terms of  the GNU General Public License, as published by *
* the  Free Software Foundation,  either version 3 of the License, or *
* (at your option) any later version.                                 *
*                                                                     *
* GEIGERINO  is distributed  to share  my work  and my experience and *
* to improve it together, but WITHOUT ANY WARRANTY relating to proper *
* working and accuracy. See the GNU General Public License  for  more * 
* details.                                                            *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                                                     *
*          See "a_commenti" tab for connections details.              *
*                                                                     *
*      Realizzato e collaudato con Arduino Uno (ATMEGA 328P).         *
*                                                                     *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
#define soglia1 0.1  // V
#define soglia2 1    // V
#define soglia3 10   // G
#define soglia4 100  // G
#define soglia5 1000 // R
#define analogOut 1 // 1:Sì; 0:No.
#define MI  659
#define SOL 784
#define LA  880
#define SI  988
#define DO 1046
#define TMAX 310 // 300 secondi; 310 = Continuo.

// SPK 3: Altoparlante separato
// SPK 7: Usa la stessa piezo che fa Bip
#define SPK 7 // per Tic-tic e allarmi

#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
LiquidCrystal lcd(8,9,10,11,12,13); // RS,EN,D4,D5,D6,D7
void(* Riavvia)(void) = 0; // Riavvia() riavvia il Contatore Geiger (usato per uscire dalle impostazioni di setup dopo ...secondi).

unsigned long XVref=0.940*1072; // 0,94 * Vref in mV. Inserire qui il valore letto con il multimetro sul pin 21 dell'ATmega328P
                                // o il valore del riferimento di tensione esterno montato.
unsigned long Vb; // Tensione della batteria, letta su A3. Deve essere long per permettere i calcoli per la lettura della tensione.
byte bat=7;  // Stato corrente della batteria (indice dell'icona).

byte P=1; // Stato del pulsante
byte Po=1; // Stato precedente del pulsante
int Azz=0; // Contatore sì/no per l'encoder in Azzerare.
int pwr=0; // Contatore per alimentazione Litio diretta/5V (1:5V).
int TS=0; // 1: fa TIC-TIC software
unsigned long millisZero=0; // Lettura del tempo iniziale per far lampeggiare sempre i due punti sui secondi dispari:
                            // a volte, dopo aver impostato il tipo di sonda, lampeggiavano al contrario.
//             t:      Variabile locale in Autonomia per la lettura ogni secondo.             
unsigned long t1=0; // Lettura del tempo per la durata della pressione del pulsante e per la rotazione dell'encoder per On/Off.
unsigned long t2=0; // Lettura del tempo per il Bip
unsigned long t3=0; // Lettura del tempo per C[m] (1 volta al secondo).
unsigned long t4=0; // Lettura del tempo per la misura e l'icona dello stato della batteria.
unsigned long t5=0; // Lettura del tempo per la lettura del commutatore della sonda (2 volte al secondo).
unsigned long t6=0; // Lettura del tempo per la modulazione del suono dell'allarme.
unsigned long t7=0; // Lettura del tempo per il lampeggio del LED rosso se biptic==0.
int E; // Risultato della routine encoder(): 1, -1, 0.
byte S; // Lettura dei due valori dell'encoder.
byte So;// Lettura precedente dell'encoder.
int X; // Usato in encoder() per evitare letture multiple.

int U=0; // Unità di misura. 0: CPM e uSv/h; 1: CPS e mR/h.
int por=0; // Portata analogica: 0: range 0,0001...10uSv/h
           //                    1: range  10uSv/h...1Sv/h
volatile unsigned int DAB=0; // Accumula gli impulsi nell'interrupt 0 (sonda A) (impulsi in 1 secondo).
unsigned long D=0; // Copia DA o DB, secondo quale sonda è selezionata.
unsigned long C[61]; // Impulsi (Count) per ogni secondo (60 secondi MAX; non uso C[0]).
float uSvph=0; // microSievert/h in virgola mobile.
float Rad=0; // Valore che viene passato a prRad() per la visualizzazione.
unsigned long uSvphInt=0; // Parte intera dei microSievert/h.
String uSvphf; // uSvh formattato.
float anOut=0; // Valore per il pwm dell'uscita analogica (deve contenere anche i decimali nel calcolo intermedio).
byte m=1; // Contatore per C[m] (da 1 a Ti).
byte m1=1;// Contatore per i cpm (da 1 a Ti).
byte n=0; // Contatore per l'azzeramento iniziale di N[] e per l'elenco delle sonde.
unsigned long  cp=0; // Impulsi nel tempo Ti.
unsigned long cpm=0; // Impulsi al minuto.
unsigned long cpmMAX=0; // cpm massimi visualizzati.
unsigned long Imp=0; // Valore che viene passato a prCp() per la visualizzazione.
String cpmf; // cpm formattato.
byte ore;
byte minuti; // ore, minuti e secondi calcolati dai secondi trascorsi.
byte secondi;
String oref;
String minutif; // ore, minuti e secondi formattati con zeri iniziali.
String secondif;
unsigned long tempo=0; // Conta il tempo della misurazione da 0 a Ti o più a lungo se è continua.
unsigned long temposecondi=0; // Serve per contare i secondi ciclicamente quando Ti<TMAX (cioè non continuo).
String tempof; // tempo formattato.
unsigned int Ti=0; // Tempo di integrazione. Se Ti=TMAX: Continuo; se Ti=TMAX+10: Precisione fissa.
unsigned int Tio=0; // Tempo di integrazione precedente.
unsigned int tempoRes=0; // Tempo residuo al raggiungimento dell'errore impostato.
byte Er=0; // Errore ammesso in centesimi: 2=0,02=2%
int sonda=0; // Tipo di sonda
byte sinto=1; // Valore precedente dello stato sonda interna (sonda A) = digitalRead(4) (1: interna).
byte LED=2; // 1:meter off; 2:meter dot; 3:meter bar.
byte LCD=2; // Retroilluminazione dell'LCD: 1:On 2:On/Off
byte LCDo=2; // Stato precedente di LCD.
int L=0; // Conteggio dell'encoder per la retroilluminazione On/Off.
int Lo=0; // Valore precedente di L.
byte mute=0; // 1:non fa fare Bip quando si ruota l'encoder (quando LCD=1 (On)).
int biptic=1; // 0:Nessun suono; 1:Bip; 2:Tic; 3:Bip+Tic.
byte TicSi=1; // Viene impostata 1 volta al secondo per far fare o no il Tic nell'interrupt.
int alm=0; // 1:Allarme attivo
byte ALLARME=0; // 1: L'allarme sta suonando!
byte ALLARMEo=0; // Stato precedente.
int f=100; // Frequenza iniziale del tono d'allarme.
int fc=f; // Variabile ausiliaria per la frequenza.
byte Disp2=0; // 1:Visualizza la deviazione standard al posto dei uSv/h.
byte Disp2o=0; // Valore precedente di Disp2.
long dstd=0; // Deviazione standard (Poisson).
byte dstdPerc=0; // Deviazione standard percentuale.
String spazio=""; // Per la Deviazione standard se cpm>=100.000
const String PROGMEM voce[]={"","Tempo d'int.","Azzeramento ","Massimi     ","Suoni       ","Allarme     ","Retroillum. ","Precisione  ","Autonomia   ","            "};
byte cv=1; // Contatore per le voci del menu.
byte suonoFine=0; // precisione fissa: 1:suono per errore giunto al valore impostato.
byte prec=0; // Precisione impostata in %.
unsigned int valPrec=0; // per 5%: valPrec=1/(0,05^2)=400
byte restoPrecedente=0; // Usata localmente per far lampeggiare la retroilluminazione quando la batteria è scarica.
byte particella=0; // 1:se biptic==0, fa lampeggiare il LED rosso.
byte particellao=0; // Stato precedente di particella.
int VSB=0; // Velocità di scarica della batteria in 10*mV/h per determinare l'autonomia

/*  1= SBM-19
 *  2= SBM-20
 *  3= 2xSBM-20
 *  4= SBM-21
 *  5= SI-1G
 *  6= SI-3BG
 *  7= SI-22G
 *  8= SI-29BG
 *  9= SBT-9
 * 10= SBT-11
 * 11= LND-712
 * 12= Variabile
*/
//Tipo di sonda -      1         2           3         4        5         6         7          8          9          10          11         12
String tipo[]={"", "SBM-19", "SBM-20", "2xSBM-20", "SBM-21", "SBT-9", "SBT-11", " SI-1G", " SI-3BG", " SI-22G", " SI-29BG",  "LND-712", "Variabile"};
int    cost[]={0,     21,      166,       332,        667,       92,     318,      167,        2,        583,       100,        168};
byte   ownb[]={0,    110,       32,        47,         12,       10,      15,       25,       12,         70,        15,         20}; // Own Background in cpm.
// 2xSBM-20: con ownb=47 il fondo ambientale è simile a quello della SBT-11. 
byte ntipi=12; // Scrivere qui il numero dell'ultima voce nella lista dei tipi di sonde (il primo elemento di ogni array è nullo).

int sens=0; // Sensibilità della sonda in uso (cost[] o var) in cpm/(uSv/h).
int var=0; // Fattore tra 1 e 999 per la sensibilità variabile della sonda.
byte ownbcpm=0; // Valore di Own Background letto dall'array ownb[].

void ContaAB() // INTERRUPT 0
{
detachInterrupt(0);  
DAB++;
if(TicSi) tone(SPK,1000,5); // TIC-TIC SOFTWARE
particella=1;
//while(!(PIND&0x20)); // Usando "FALLING" (fronte di discesa) non dovrebbe servire.
attachInterrupt(0,ContaAB,FALLING);
}


