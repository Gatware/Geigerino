/*
*           ********************************************
*           ************ G E I G E R I N O *************
*           ***** Un Contatore Geiger con Arduino ******
***************  Copyright Gianluca Giangreco 2016   ******************
******* https://github.com/Gatware/Geigerino/tree/GEIGERINO-1.1 *******                              
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
* proper working and accuracy. See the GNU General Public License for * 
* more details.                                                       *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                                                     *
*          See "a_commenti" tab for connections details.              *
*                                                                     *
*      Realizzato e collaudato con Arduino Uno (ATMEGA 328P).         *
*                                                                     *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
#define soglia1 0.5 // V
#define soglia2 2.5 // V
#define soglia3 12  // G
#define soglia4 60  // R
#define soglia5 300 // R
#define analogOut 1 // 1:Sì; 0:No.

#include <LiquidCrystal.h>
#include <EEPROM.h>
LiquidCrystal lcd(8,9,10,11,12,13); // RS,EN,D4,D5,D6,D7
unsigned long XVref=0.940*1080; // 0,94 * Vref in mV. Inserire qui il valore letto con il multimetro sul pin 21 dell'ATmega328P
                                // o il valore del riferimento di tensione esterno montato.
unsigned long Vb; // Tensione della batteria, letta su A3. Deve essere long per permettere i calcoli per la lettura della tensione.
byte bat=7;  // Stato corrente della batteria (indice dell'icona).

byte P=1; // Stato del pulsante
byte Po=1; // Stato precedente del pulsante
int Azz=0; // Contatore sì/no per l'encoder in Azzerare.
int TS=0; // 1: fa TIC-TIC software
unsigned long millisZero=0; // Lettura del tempo iniziale per far lampeggiare sempre i due punti sui secondi dispari:
                            // a volte, dopo aver impostato il tipo di sonda, lampeggiavano al contrario.
unsigned long t1=0; // Lettura del tempo per la durata della pressione del pulsante
unsigned long t2=0; // Lettura del tempo per il Bip
unsigned long t3=0; // Lettura del tempo per C[m].
unsigned long t4=0; // Lettura del tempo per la misura e l'icona dello stato della batteria.
int E; // Risultato della routine encoder(): 1, -1, 0.
byte S; // Lettura dei due valori dell'encoder.
byte So;// Lettura precedente dell'encoder.
int X; // Usato in encoder() per evitare letture multiple.

int U=0; // Unità di misura. 0: CPM e uSv/h; 1: CPS e mR/h.
int por=0; // Portata analogica: 0: range 0,0001...10uSv/h
           //                    1: range  10uSv/h...1Sv/h
volatile unsigned int DAB=0; // Accumula gli impulsi nell'interrupt 0 (sonda A) (impulsi in 1 secondo).
unsigned int D=0; // Copia DA o DB, secondo quale sonda è selezionata.
unsigned int DMAX=0; // D MAX conteggiati (valore massimo dei cps [in 1 secondo]).
unsigned int C[61]; // Impulsi (Count) per ogni secondo (60 secondi MAX; non uso C[0]).
float uSvph=0; // microSievert/h in virgola mobile.
float Rad=0; // Valore che viene passato a prRad() per la visualizzazione.
unsigned long uSvphInt=0; // Parte intera dei microSievert/h.
String uSvphf; // uSvh formattato.
int anOut=0; // Valore per il pwm dell'uscita analogica.
byte m=1; // Contatore per C[m] (da 1 a Ti).
byte m1=1;// Contatore per i cpm (da 1 a Ti).
byte n=0; // Contatore per l'azzeramento iniziale di N[] e per l'elenco delle sonde.
unsigned long  cp=0; // Impulsi nel tempo Ti.
unsigned long cpm=0; // Impulsi al minuto.
unsigned long Imp=0; // Valore che viene passato a prCp() per la visualizzazione.
String cpmf; // cpm formattato.
byte ore;
byte minuti; // ore, minuti e secondi calcolati dai secondi trascorsi.
byte secondi;
String oref;
String minutif; // ore, minuti e secondi formattati con zeri iniziali.
String secondif;
unsigned long tempo=0; // Conta il tempo della misurazione da 0 a Ti o più a lungo se è continua.
String tempof; // tempo formattato.
byte Ti=0; // Tempo di integrazione.
byte Tio=0; // Tempo di integrazione precedente.
int sonda=0; // Tipo di sonda
byte sinto=1; // Valore precedente dello stato sonda interna (sonda A) = digitalRead(4) (1: interna).
int LED=1; // 0:meter off; 1:meter dot; 2:meter bar (può scendere a -1).
byte LCD=1; // Retroilluminazione dell'LCD: 1:On 2:On/Off
byte LCDo=1; // Stato precedente di LCD.
byte mute=0; // 1:non fa fare Bip quando si ruota l'encoder (quando LCD=1 (On)).
int biptic=1; // 0:Nessun suono; 1:Bip; 2:Tic; 3:Bip+Tic.
         
/*  0= SBM-19
 *  1= SBM-20
 *  2= 2xSBM-20
 *  3= SBM-21
 *  4= SI-1G
 *  5= SI-3BG
 *  6= SI-22G
 *  7= SI-29BG
 *  8= SBT-9
 *  9= SBT-11
 * 10= LND-712
 * 11= Variabile
*/
// Tipo di sonda   0         1         2           3          4         5          6          7         8         9         10         11
String tipo[]={"SBM-19", "SBM-20", "2xSBM-20", "SBM-21", " SI-1G", " SI-3BG", " SI-22G", " SI-29BG", "SBT-9", "SBT-11", "LND-712", "Variabile"};
int    cost[]={   21,      166,       332,        667,      167,        2,        583,       100,      92,       318,      168};
byte   ownb[]={  110,       32,        64,         12,       25,       12,         70,        15,      10,        15,       20}; // Own Background in cpm.
byte ntipi=11; // Scrivere qui il numero dell'ultima voce nella lista dei tipi di sonde (la prima ha numero zero).

int sens=0; // Sensibilità della sonda in uso (cost[] o var) in cpm/(uSv/h).
int var=0; // Fattore tra 1 e 999 per la sensibilità variabile della sonda.
byte ownbcpm=0; // Valore di Own Background letto dall'array ownb[].

void ContaAB() // INTERRUPT 0
{
detachInterrupt(0);  
DAB++;
if(TS) Tic(); // TIC-TIC SOFTWARE
//while(digitalRead(5)==0); // Usando "FALLING" (fronte di discesa) non dovrebbe servire.
attachInterrupt(0,ContaAB,FALLING);
}


