/*
0.1  15/10/16 Cerco di fare tutta la selezione dei menu nel loop.
0.2  16/10/16 Torno ai salti da un menu all'altro come in Limiti.
0.2a 17/10/16 Rinomino menu1, menu2 e menu3 con il nome della funzione contenuta.
              Sposto TipoDiSonda (era menu3) nel setup, tenendo premuto il pulsante all'accensione.
              Agiungo la lettura della tensione di alimentazione e le icone della batteria.
0.2c 18/10/16 Vado avanti. Cambio "flag" con "Azz".
0.2d 19/10/16 Aggiungo la "à", il "micro" e la memorizzazione su EEPROM della sensibilità.
0.2e 20/10/16 Aggiungo il conteggio dei cpm.
0.2h 21/10/16 Proseguo e comincio a introdurre la misurazione continua.
0.3  22/10/16 Cambio tempo in unsigned long e continuo con la misurazione continua. 
              Giustifico a destra tutte le letture in uSv/h e mR/h.
0.3a 24/10/16 Ho messo il conteggio del tempo in ore, minuti e secondi e ":" che lampeggia e il pilotaggio dei LED con le soglie.
0.3b 24/10/16 Ho aggiunto l'uscita analogica logaritmica.
0.3c 25/10/16 Ho aggiunto la sottrazione dei cpm di background propri per il tubo selezionato o variabile.
0.3d 25/10/16 In TipoDiSonda aggiungo la visualizzazione di sensibilità e background del tubo caricato.
0.3d126/10/16 Ho fatto delle correzioni su tempo nel loop, poiché il conteggio del tempo non partiva da 0s ma da 1s
              e la prima visualizzazione veniva immediatamente sovrascritta da quella che calcola anche minuti e secondi.
0.3d231/10/16 In modo continuo i secondi andavano a passi di 2, perché incrementavo tempo 2 volte, e il conteggio si fermava a 1m11s.  
0.3e 31/10/16 I calcoli di Hi-byte di var erano sbagliati, perché dividevo e moltiplicavo per 16, anziché per 256!
              A volte, dopo aver impostato il tipo di sonda, i due punti lampeggiavano sui secondi pari anziché dispari: ho introdotto
              millisZero, ottenendo un azzeramento iniziale del tempo letto dalla funzione dei due punti.
              Riduco l'antirimbalzo di Centinaia-Decine-Unità della sensibilità variabile da 500mS a 300mS.
0.3f  3/11/16 Come la 0.3e, ma con le connessioni come nella seguente 0.4: Encoder A e B da PD3 e PD4 a PD0 e PD1 per liberare l'interrupt 1.
0.3g  4/11/16 Nella 0.3f non avevo messo gli I/O 0 e 1 nel setup per INPUT e PULLUP. Corretti. 
              Dovrebbe essere la 0.3 definitiva.
0.4 temp3     Riparte dalla v0.3e. 
0.4 temp4     Sposto l'encoder sugli I/O 0 e 1 per lasciare libero l'altro interrupt in per la sonda B.
              Introduco TIC software Si/No, con impostazione tramite encoder nel setup.
0.4 temp5     Riparto dalla 0.3g, perché nella temp4 i valori aumentano in continuazione, anche con Ti=10 secondi!
              Dovevo solo azzerare DA e DB dopo ogni lettura. Azzerare D è inutile, perché è la copia di uno dei due!
              Aggiungo qui sotto i valori 1M/270k del partitore per la lettura della tensione della batteria.
1.0   5/11/16 Commpleto la sondaB, con le impostazioni in TipoDiSonda e tutto il resto.
              Forse c'è tutto (a parte la lettura della tensione della batteria in Volt).
              Ho messo long(tempo) nel calcolo dei CPM.
1.0   6/11/16 Ho modificato il pilotaggio dei LED da modo punto a barra.
1.1   6/11/16 Ho aggiunto il menu del modo dei LED: Off/Dot/Bar
              Ho aggiunto il menu del pilotaggio della retroilluminazione dell'LCD: On o On/Off. In On/Off è comandata dall'encoder.
              Ho spostato la lettura della tensione della batteria da A0 (ora destinato alla retroillum.) ad A1, condiviso con un LED V.
1.2  18/11/16 Aggiungo tra le sonde la doppia SBM-20.
              Aggiungo l'avvisatore acustico al 4° e 5° livello (4° e 5° LED).
*/  

String ver=" v1.2";

/*
 * ----------- LCD -----------
 * LCD RS pin > I/O 8
 * LCD En pin > I/O 9
 * LCD D4 pin > I/O 10
 * LCD D5 pin > I/O 11
 * LCD D6 pin > I/O 12
 * LCD D7 pin > I/O 13
 * LCD R/W pin > ground
 * LCD VSS pin > ground
 * LCD V0 pin  > trimmer
 * LCD VCC pin > +5V
 * 
 * I/O 0 (pin 2) Encoder - A
 * I/O 1 (pin 3) Encoder - B
 * I/O 2 (pin 4) Ingresso sonda A - INT 0
 * I/O 3~(pin 5) Ingresso sonda B - INT 1
 * I/O 4 (pin 6) Sel. sonda B (a GND): 1=sonda A.
 * I/O 5~(pin 11) Encoder - Pulsante
 * I/O 6~(pin 12) Analog out
 * I/O 7 (pin 13) Capsula piezo
 * I/O 8  LCD RS (pin 14)
 * I/O 9~ LCD En (pin 15)
 * I/O 10~LCD D4 (pin 16)
 * I/O 11~LCD D5 (pin 17) -MOSI
 * I/O 12 LCD D6 (pin 18) -MISO
 * I/O 13 LCD D7 (pin 19) -SCK
 * A0 (pin 23) Negativo della retroilluminazione dell'LCD.
 * A1 (pin 24) LED 1 + Tensione della batteria via partitore 1M/270k.
 * A2 (pin 25) LED 2
 * A3 (pin 26) LED 3
 * A4 (pin 27) LED 4
 * A5 (pin 28) LED 5
 * 
 * ***** EEPROM *****
 * 0 Ti: Tempo di integrazione
 * 1 sonda: Tipo di sonda A
 * 2 var lo-byte: Sensibilità variabile A (lo-byte)
 * 3 var hi-byte: Sensibilità variabile A (hi-byte)
 * 4 owbcpm: cpm di background proprio del tubo A
 * 5 TS: TIC software Sì/No
 * 6 sonda: Tipo di sonda B 
 * 7 var lo-byte: Sensibilità variabile B (lo-byte)
 * 8 var hi-byte: Sensibilità variabile B (hi-byte)
 * 9 owbcpm: cpm di background proprio del tubo B
 *10 LED: 0:meter off; 1:meter dot; 2:meter bar.
 *11 LCD: retroilluminazione dell'LCD: 1:On; 2:Auto.
 */