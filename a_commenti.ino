
/*
0.1  15/10/16 Cerco di fare tutta la selezione dei menu nel loop.
0.2  16/10/16 Torno ai salti da un menu all'altro come in Limiti.
0.2a 17/10/16 Rinomino menu1, menu2 e menu3 con il nome della funzione contenuta.
              Sposto TipoDiSonda (era menu3) nel setup, tenendo premuto il pulsante all'accensione.
              Aggiungo la lettura della tensione di alimentazione e le icone della batteria.
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
1.2a 22/11/16 Integrazione: metto l'azzeramento anche se il tempo aumenta, perché l'improvvisa aggiunta di elementi nulli abbassava il valore.
              Corretto un errore in TipoDiSonda - Sonda B: if(sonda>ntipi) sonda=ntipi; (non elencava Variabile).
              Ho aggiunto uno spazio dopo akcuni "SET!" che lasciavano un carattere della scritta precedente.
              Ho aggiunto un lcd.clear() nel Setup fra TipoDiSonda() e TICSwSiNo().
1.2b 22/11/16 Azzera: sposto il Biiip() fuori dalla funzione, perché se no quando commuto la sonda è troppo lungo.
              Abbasso il Bkg della SBT-11/11A da 60cpm a 15cpm, come da datasheet (0,25cps).
              Il menu saltava massimi() e meter() (LED dot/bar) perché mancava t1=millis() all'inizio delle due funzioni. Corretto.
              Ho rinominato meter() in dotBar() e l'ho spostato nel setup.
1.2c 23/11/16 Ho modificato l'algoritmo dell'uscita analogica: adesso lo 0 è 0,0001uSv/h e il f.s. è 1Sv/h (10x a ogni decimo della scala)
              Anticipo la cancellazione di uno spazio in Tempo di integrazione >SET!
              Aggiungo delay(300) all'inizio di Azzerare?
1.3  24/11/16 Ho messo la doppia scala analogica: 0,0001...10uSv/h e 0,01...1mSv/h.
              Adesso la scala analogica legge solo i uSv/h, anche visualizzando mR/h sul display.
              Dopo una visualizzazione di 100000cpm o più sul display rimaneva l'ultima cifra: risolto.
              Qualche altro aggiustamento.
1.4   1/12/16 Aggiungo il menu di selezione Bip e Tic-tic (il Tic-tic SW è però anche disattivabile definitivamente dal setup, se presente HW).
              Un #define permette di selezionare Altoparlante indipendente oppure la stessa piezo che fa Bip.
1.4U  1/12/16 Versione con ingresso unico.
1.5U  8/12/16 Ho introdotto la variabile temposecondi, che conta i secondi insieme a tempo ma ogni volta che supera Ti viene posta a 1 per
              visualizzare ciclicamente i secondi quando non è in modalità "Continuo".
     10/12/16 Ho prolungato il "Tic-tic" da 2 a 10mS.
     24/12/16 Ho cambiato i valori di soglia ei LED.
     25/12/16 Metto il pilotaggio dei LED nella funzione piloLED(), che richiamo anche dopo aver letto la tensione della batteria ogni 10s,
              poiché per leggere la tensione devo porre l'I/O del LED 1 in modo input e poi rimane basso.
     26/12/16 Ho fatto una correzione nell'impostazione della conda B nel setup e nel caricamento all'avviamento.
              Al posto dei ":" lampeggianti ho messo l'indicazione di sonda A o B.
              Ho cambiato l'ordine dei tipi di sonde, mettendo le Sxx in ordine alfabetico.
              La media cresceva lentamente, poiché la divisione viene effettuata per un numero di secondi pari a tempo+1: adesso un 
              if(tempo<Ti) fa dividere per tempo-1 e sembra risolto. Non ho trovato una soluzione migliore :-(
     28/12/16 Velocizzo la lettura del commutatore di sonda da 1 a 2 volte a secondo (il commutatore è comodo anche per azzerare).
     31/12/16 Adesso per accendere e spegnere la retroilluminazione (se in modalità On/Off) bisogna ruotare l'encoder di 3 scatti in 200mS
              e i suoni per On e Off sono diversi dal Bip().
              Adesso la numerazione delle opzioni delle sonde parte da 1 anziché da 0 e non è più necessario che Variabile (o variabile) sia
              l'ultima opzione, poiché non viene più letto l'indice (che era l'ultimo) ma il nome (Variabile o variabile).
              Ho aggiunto Riavvia() per usarla al posto di return nel menu di setup: riavvia tutto anziché passare al menu seguente.
              Adesso gli azzeramenti fanno Biip().
              Ho aggiunto music().
       1/1/17 Aggiungo nel menu Setup la selezione Alimentazione Litio dir./5V: con l'alimentazione diretta da Litio la tensione di alimentazione
              è più bassa e varia, quindi bisogna compensare il PWM dello strumento analogico.
       7/2/17 Ho corretto la cancellazione finale del display in printRad.
              Ho cambiato da int a long le variabili per i cps.
       8/2/17 Ho cambiato il suono di allarme con AUUUuuu... AUUUuuu..., ho aggiunto il menu per escluderlo e ho disattivato il Tic-tic durante l'allarme.
      11/2/17 Correzione: in loop riga 57 ho messo noTone in else, altrimenti nei momenti di silenzio ronzava! (iniziava a suonare e un attimo dopo arrivava noTone).
1.6U  18/2/17 Facendo click, anzichè al menu delle portate, visualizza la deviazione standard.
      19/2/17 Adesso visualizza la deviazione standard anche in percentuale.
      20/2/17 Ho aggiunto 0,5*60 ai cpm prima di dividere per il tempo e 0,5*10 alla dstd prima di calcolare la percentuale per fare le approssimazioni.
1.7U  21/2/17 Cambio sistema di menu: metto un menu con selezione scorrevole del sottomenu.
              Ho messo F() a gran parte degli lcd.print per risparmiare RAM. Mettendolo anche a quelli nel setup e all'inizio del loop dava caratteri danneggiati...
ridotto       Ho tolto quello che non uso per ridurre l'occupazione di memoria.
1.7U          Ho modificato la variabile LED da int a byte, facendola variare fra 1 e 3: così durante la rotazione dell'encoder può solo arrivare a 0 e va bene byte.
              Lo chiamo semplicemente 1.7U.
1.8   23/2/17 Comincio a introdurre le misure a precisione costante. Se Ti=TMAX+10: Precisione fissa. Al 5% preimpostato funziona.
      24/2/17 Aggiungo la voce del menu per inserire la precisione desiderata.    
      28/2/17 Al raggiungimento della precisione impostata, premendo, se stava emettendo suono, diventava continuo. Corretto.
              Ho messo i segni + e - ai tempi trascorso e rimanente.
              Ho introdotto la scritta cpm con il carattere "piemme" (char 4).
       1/3/17 Ho corretto il calcolo della percentuale di errore (approssimazione) e del tempo residuo.
       5/3/17 A volte la misura a precisione fissa finiva subito, con cpm ancora a zero. Adesso la misura può finire solo se cpm>0 e cp>20.
       6/3/17 Ho aggiunto l'intermittenza della retroilluminazione quando la batteria è scarica.
       7/3/17 Adesso, finita la misura a precisione fissa e tacitato l'avvisatore, compaiono anche i uSv/h.
      11/3/17 Ho aggiunto la rotazione continua dei tempi di integrazione, ricominciando dall'inizio dopo Precisione fissa.
              Ho aggiunto il lampeggio del LED rosso per ogni particella rilevata quando Suoni è impostato su Nessun suono. 
              Ho aggiunto la lettura di Vbat e il calcolo dell'autonomia.
       2/4/17 Uscita analogica: già non c'era più la selezione della portata; ho messo portata fissa da 0,01uSv/h a 1mSv/h.
              Ho spostato la parte dell'uscita analogica, perché funzionava solo quando erano visualizzati i uSv/h, ma non quando erano visualizzati i cpm (Disp2).
       5/5/17 In modalità a precisione fissa, raggiunta la precisione impostata la prima pressione dell'encoder tacita solo il suono; la seconda cambia la visualizzazione
              da cpm e dstd% a uSv/h o, adesso, anche viceversa se erano visualizzati i uSv/h; la terza azzera e fa ripartire il conteggio.
              Ho esteso il valore massimo della precisione impostabile dal 10% al 15%.
*/
 
String  ver=" v1.8U";
String data="050517";
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
 * I/O 2 (pin 4) Ingresso unico sonde A e B- INT 0
 * I/O 3~(pin 5) Usc. Altoparlante Tic-tic  (INT 1)
 * I/O 4 (pin 6) Sel. sonda B (a GND): 1=sonda A.
 * I/O 5~(pin 11) Encoder - Pulsante
 * I/O 6~(pin 12) Analog out (3V fs)
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
 * ***** CARATTERI *****
 * 0 Simboli Batt_0...7
 * 1 Lettera "à"
 * 2 Lettera "μ"
 * 3 10 simboli della percentuale del tempo
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
 *12 biptic: 0:Nssuno; 1:Bip; 2:Tic-tic; 3:Bip + Tic-tic.
 *13 pwr: Alimentazione: 0:Litio diretta; 1:5V.
 *14 Allarme
 *15
 *17 VSB: Velocità di scarica della batteria in mV/h.
 */
