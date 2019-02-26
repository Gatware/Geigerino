				              GEIGERINO 1.9iUX 260219

				   La mia versione personale di Contatore Geiger

IMPOSTAZIONI DI BASE (SETUP)

Le impostazioni di base si effettuano accendendo Geigerino tenendo premuto l'encoder.
Prima impostazione: LED Off, Dot, Bar. Permette di scegliere se i 5 LED segnalatori del livello di pericolosità devono rimanere sempre spenti (per ridurre il consumo) oppure si devono accendere uno per volta oppure a barra.
Seconda impostazione: Tic-tic software. Se l'hardware non ha un avvisatore acustico per ogni particella rilevata, è possibile far emettere un breve "Bip" al cicalino ogni volta che viene rilevata una particella.
La terza impostazione riguarda il modello di sonda interna e il modello di sonda esterna. Entrambe sono selezionabili fra SBM-19, SBM-20, 2xSBM-20, SBM-21, SBT-9, SBT-11, SI-1G, SI-3BG, SI-22G, SI-29BG, LND-712, Variabile. Altri modelli possono essere aggiunti, purché il byte ntipi sia pari al numero totale di opzioni (compresa Variabile). Se è scelto un modello preimpostato, vengono visualizzati i valori richiamati. Se, invece, è scelto Variabile, bisogna impostare le sensibilità (in cpm/(μSv/h)) e il fondo proprio della sonda.
Dove non specificato diversamente, le impostazioni modificate vengono immediatamente memorizzate nella EEPROM. Ciò viene evidenziato mediante la scritta “SET!”. Se un’impostazione non viene modificata o, comunque, viene selezionato il valore o l’opzione apparsi nel momento in cui si è entrati nell’impostazione, la memorizzazione non viene effettuata.	

IMPOSTAZIONI DELL’HARDWARE E TEST DELLO STRUMENTO

Si accede a queste impostazioni tenendo premuto l’encoder all’accensione per 3 secondi.
Prima impostazione: “Alimentazione?”. Serve per correggere il duty cycle del PWM dell’uscita per lo strumento analogico secondo la tensione di alimentazione: se Geigerino è alimentato direttamente da un elemento al Litio, il PWM deve essere costantemente corretto in base alla tensione assunta dall’elemento; se, invece, viene alimentato a 5V tramite un convertitore o regolatore di tensione, il valore di Vb seguirà comunque la tensione della batteria, ma il duty cycle dovrà rimanere immutato, proporzionato alla tensione di alimentazione fissa a 5V. Lo strumento analogico, da 100uA, viene pilotato con scala logaritmica da 0,01 (in posizione di riposo) a 1000uSv/h. Avendo aggiunto il Mosfet per la commutazione dell’alimentazione quando è collegato il cavo USB, dalla versione 1.9UX è stata introdotta la lettura della tensione di alimentazione indipendente dalla tensione della batteria per permettere comunque il corretto funzionamento del PWM. L’aggiunta del Mosfet si è resa necessaria per permettere l’interruzione della carica, quando è completa, anche se Geigerino è in funzione: precedentemente, infatti, l’assorbimento di Geigerino impediva il riconoscimento della fine della carica alla schedina carica batteria, che poteva interrompere la carica solo se Geigerino era spento. Lasciando a lungo Geigerino acceso con il connettore USB collegato per impedire la scarica della batteria, il carica batteria avrebbe continuato sempre a fornire la tensione danneggiando pericolosamente, a lungo andare, l’elemento al Litio. Con il Mosfet, quando viene fornita alimentazione tramite USB, Geigerino viene scollegato dalla batteria e collegato ai +5V USB.
Seconda impostazione: velocità di scarica della batteria. Può essere impostata fra 2,0 e 9,9mV/h. In base a questa, Geigerino calcola l'autonomia residua. Ruotando l'encoder varia la velocità di scarica e la corrispondente autonomia a piena carica stimata. La velocità di scarica si determina misurando con precisione la tensione dell'elemento una prima volta; poi, di nuovo, dopo averlo tenuto in funzione per circa un'ora e dividendo l'abbassamento di tensione per il tempo trascorso in ore e decimali: ad es. 45 minuti=0,75h; 1h30=1,5h.
Terza impostazione: taratura di Vref. Bisogna leggere con un multimetro digitale la tensione presente sul pin 21 dell’ATmega328 e inserirla. La tensione di riferimento dell’integrato ha un valore dichiarato di 1,1V +/-10%. Se la taratura non viene effettuata, viene impostato il valore di 1100mV.
TEST dello strumento: se viene selezionato “Sì”, lo strumento si porterà a 0,1uSv/h e, ruotando l’encoder, indicherà i valori 0,01 - 0,1 - 1 - 10 - 100 - 1000 (fondo scala). Serve per verificare la taratura del fondo scala e il movimento dell’ago senza problemi meccanici. Allo stesso tempo, i 5 LED si accenderanno in sequenza a partire da 0,1uSv/h.
Alla pressione successiva dell’encoder, verranno visualizzate versione e data del software.

FUNZIONAMENTO

All’accensione normale, appaiono di seguito:	
- Il nome “GEIGERINO”, la versione e il nome dell’autore	
- Le sigle delle due sonde (A e B, interna ed esterna) attualmente selezionate (o “Variabile” se una o entrambe sono impostate manualmente)	
- Il tempo di integrazione attualmente impostato.
Dopo la visualizzazione del tempo di integrazione appare la pagina principale e inizia il conteggio.
Geigerino effettua sempre una media continua dei valori rilevati. Se il tempo di integrazione (Ti) è im-postato fra 5 e 60 secondi, però, allo scadere di Ti i valori vengono sovrascritti, quindi il valore visualizzato è la media degli ultimi Ti secondi, scartando tutte le letture precedenti. Il tempo visualizzato sul display ri-parte ciclicamente da zero per comodità di lettura, ma in ogni momento la media è sugli ultimi Ti secondi. Se, invece, il tempo è impostato su "Continuo", la media viene aggiornata continuamente tenendo conto di tutti i valori rilevati a partire dall’ultimo azzeramento e il tempo, inizialmente in secondi, viene poi visualizzato in minuti e secondi e poi in ore e minuti. Premendo brevemente l'encoder viene visualizzato l'errore stimato; premendolo nuovamente, viene visualizzata la dose totale dall’accensione.
Se il tempo è impostato su "Precisione fissa" (più avanti abbreviato PF), il conteggio va avanti finché l'errore scende al di sotto del valore impostato; contemporaneamente viene visualizzato il tempo stimato rimanente al raggiungimento della precisione impostata (preceduto dal segno "-"). Raggiunto il valore im-postato, inizia un suono intermittente intervallato dal ticchettio. Premendo l'encoder una prima volta, viene tacitato l’allarme; premendolo una seconda volta, si torna alla visualizzazione dei uSv/h (o dei cpm, se erano già visualizzati i uSv/h), della dstd% e del tempo trascorso (preceduto dal segno "+").
Nel funzionamento a tempo o Continuo i valori stimati hanno un intervallo di confidenza del 68% (SD), mentre nel funzionamento a precisione fissa l'intervallo di confidenza è del 95% (1,96*SD), dove SD è la deviazione standard.
Il simbolo in basso a destra indica lo stato della batteria. Quando è completamente scarica e Geigerino deve essere spento, la retroilluminazione del display lampeggia.
MENU (durante il funzionamento)
Premendo per un attimo l’encoder, si passa dalla visualizzazione in uSv/h alla visualizzazione della deviazione standard (in cpm e in percentuale), poi alla visualizzazione in mR/h (per permettere facili confronti con apparati classici) e alla dose in mSv (tranne in PF), aggiornate continuamente.
Tenendo premuto l’encoder per almeno 1 secondo, si passa al menu. Ruotando l'encoder, scorrono le voci; premendo l'encoder, si accede alla voce:
1.	Tempo di integrazione, impostabile fra 5, 10…60 secondi a passi di 10, Continuo e Precisione fissa.

2.	“AZZERARE?” Sì/No, per azzerare i conteggi, il tempo e il massimo. N.B.: lo stesso risultato si ottiene commutando e ricommutando la sonda.	

3.	Valori massimi visualizzati di cpm e μSv/h a partire dal 5° secondo dall’azzeramento (per evitare possibili alti valori erronei), a cui segue la visualizzazione del totale degli impulsi contati e dello stesso dopo aver detratto il fondo proprio della sonda in uso.	

4.	Suoni, dove ci sono le seguenti opzioni:	 
- Nessun suono: nessun suono viene emesso tranne un leggerissimo “Tic” al posto del Bip. In questo caso, il LED rosso emette un lampo in corrispondenza di ogni radiazione rilevata. 	
- Bip: viene emesso solo il Bip dei menu. Anche in questo caso, il LED rosso lampeggia a ogni radiazione rilevata.	
     Se il Tic-tic software è attivato nel Setup, vengono visualizzate anche le opzioni: 	
- Tic-tic: viene emesso solo il Tic-tic software; 	
- Bip + Tic-tic: vengono emessi il Bip e il Tic-tic software.	

5.	Allarme: On/Off. Se è posto in On, al raggiungimento della soglia del secondo LED giallo viene emesso un suono di sirena d’allarme.	

6.	Retroilluminazione: sempre accesa (On), oppure comandata dalla rotazione dell’encoder (On/Off). Nel primo caso, la retroilluminazione è sempre accesa (il consumo è minore di 2mA), anche quando Geigerino viene spento e riacceso; impostato su On/Off, invece, se durante il normale funzionamento l’encoder viene ruotato di almeno 3 scatti in un verso o nell’altro, la retroilluminazione viene accesa o spenta e, alla riaccensione, Geigerino si disporrà con retroilluminazione spenta. Solo l’impostazione On – On/Off viene memorizzata nella EEPROM.	

7.	Precisione: ruotando l'encoder si sceglie la precisione di fine misura per le misure a precisione fissa tra l'1% e il 15%.	

8.	Autonomia: appaiono la tensione dell'elemento e l'autonomia residua stimata, aggiornate ogni secondo.
