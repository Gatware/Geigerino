# Geigerino
La mia versione personale di Contatore Geiger

													GEIGERINO v.1.5U
                                                     
IMPOSTAZIONI DI BASE (SETUP)

Le impostazioni di base si effettuano accendendo Geigerino tenendo premuto l'encoder.

   Prima impostazione: LED Off, Dot, Bar. Permette di scegliere se i 5 LED segnalatori del livello di pericolosità devono rimanere sempre spenti (per ridurre il consumo) oppure si devono accendere uno per volta oppure a barra.
   
   Seconda impostazione: TIC software. Se l'hardware non ha un avvisatore acustico per ogni particella rilevata, è possibile far emettere un breve "Bip" al cicalino ogni volta che viene rilevata una particella.
   
   La terza impostazione riguarda il modello di sonda interna e il modello di sonda esterna. Entrambe sono selezionabili fra SBM-19, SBM-20, 2xSBM-20, SBM-21, SBT-9, SBT-11,  SI-1G,  SI-3BG,  SI-22G,  SI-29BG,  LND-712, Variabile. Altri modelli possono essere aggiunti, purché il byte ntipi sia pari al numero totale di opzioni (compresa Variabile). Se è scelto un modello preimpostato, vengono visualizzati i valori richiamati. Se, invece, è scelto Variabile, bisogna impostare le sensibilità (in cpm/(uSv/h)) e il fondo proprio della sonda.
   Dove non specificato diversamente, le impostazioni modificate vengono immediatamente memorizzate nella EEPROM. Ciò viene evidenziato mediante la scritta “SET!”. Se un’impostazione non viene modificata o, comunque, viene selezionato il valore o l’opzione apparsi nel momento in cui si è entrati nell’impostazione, la memorizzazione non viene effettuata.
   
   
   IMPOSTAZIONE DELL’HARDWARE
   
   
   Tenendo premuto l’encoder all’accensione per 3 secondi si entra nel menu “Alimentazione?”, che serve per correggere il duty cycle del PWM dell’uscita per lo strumento analogico secondo la tensione di alimentazione: se Geigerino è alimentato direttamente da un elemento al Litio, il PWM deve essere costantemente corretto in base alla tensione assunta dall’elemento; se, invece, viene alimentato a 5V tramite un convertitore o regolatore di tensione, il valore di Vb seguirà comunque la tensione della batteria, ma il duty cycle dovrà essere costantemente proporzionato alla tensione di alimentazione di 5V.Il fondo scala dello strumento deve essere 3V, realizzabile con uno strumento da 100uA con una resistenza in serie pari a 30kohm-Ri, dove Ri è la resistenza interna dello strumento.
   
   
   FUNZIONAMENTO
   
   
   Geigerino effettua sempre una media continua dei valori rilevati. Se il tempo di integrazione (Ti) è impostato fra 10 e 60 secondi, però, allo scadere di Ti i valori vengono sovrascritti, quindi il valore visualizzato è la media degli ultimi Ti secondi, scartando tutte le letture precedenti. Il tempo visualizzato sul display riparte ciclicamente da zero per comodità di lettura, ma la media è continua. Se, invece, il tempo è impostato su "Continuo", la media viene aggiornata continuamente tenendo conto di tutti i valori rilevati a partire dall’ultimo azzeramento e il tempo, inizialmente in secondi, viene poi visualizzato in minuti e secondi e poi in ore e minuti.
   
   
   MENU (durante il funzionamento)
   
   
   Premendo per un attimo l’encoder è possibile selezionare l’unità di misura: CPM e uSv/h oppure CPS e mR/h. Alla riaccensione, Geigerino si ridispone sempre in CPM e uSv/h.
	Premendo nuovamente l’encoder è possibile scegliere la portata analogica fra 0,0001…10usV/h e 0,01…1mSv/h. Alla riaccensione, Geigerino si ridispone sempre sulla portata 0,0001…10usV/h.
	Tenendo premuto l’encoder per almeno 1 secondo, va all’impostazione del tempo di integrazione, selezionabile fra 10 e 60 secondi a passi di 10 e Continuo.
	Se, invece, viene tenuto premuto per 2 secondi, va al menu “AZZERARE?” Sì/No, per azzerare i conteggi, il tempo e il massimo. N.B.: lo stesso risultato si ottiene commutando e ricommutando la sonda.
	Tenendolo premuto per 3 secondi vengono visualizzati i valori massimi rilevati di CPM e uSv/h.
	Tenendolo premuto per 4 secondi si arriva alla selezione della retroilluminazione: sempre accesa (On), oppure comandata dalla rotazione dell’encoder (On/Off). Nel primo caso, la retroilluminazione è sempre accesa (il consumo è minore di 2mA), anche quando Geigerino viene spento e riacceso; impostato su On/Off, invece, se durante il normale funzionamento l’encoder viene ruotato di almeno 3 scatti in un verso o nell’altro, la retroilluminazione viene accesa o spenta e, alla riaccensione, Geigerino si disporrà con retroilluminazione spenta. Solo l’impostazione On – On/Off viene memorizzata nella EEPROM.
	Tenendolo premuto per 5 secondi si entra nel menu Suoni, dove ci sono le seguenti opzioni:
-	Nessun suono: nessun suono viene emesso tranne un leggerissimo “Tic” al posto del Bip
-	Bip: viene emesso solo il Bip dei menu.
Se il Tic software è attivato nel Setup, vengono visualizzate anche le opzioni:
-	Tic-tic: viene emesso solo il Tic software; 
-	Bip + Tic-tic: vengono emessi il Bip e il Tic software.
	
