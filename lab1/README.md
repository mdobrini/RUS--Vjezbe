### Upravljanje višestrukim prekidima i njihovim prioritetima  

#### Cilj zadatka  
- [ ] Proučiti koncept višestrukih prekida i njihovih prioriteta na odabranom procesoru  
- [ ] Implementirati program koji efikasno upravlja različitim događajima koristeći odgovarajuće strategije rukovanja prekidima  

#### Zadatak  

##### Implementacija višestrukih interrupta  
- [ ] Definirati i implementirati više prekidnih rutina (ISR – Interrupt Service Routines) koje će reagirati na različite događaje:  
  - [ ] Pritisak različitih tipkala  
  - [ ] Aktivaciju internih timera  
  - [ ] Očitavanje različitih senzora  
  - [ ] Serijsku komunikaciju ili druge vanjske prekide  

##### Postavljanje prioriteta prekida  
- [ ] Postaviti različite prioritete za prekide kako bi važniji događaji imali prednost pri obradi  
- [ ] Omogućiti preklapanje prekida (nested interrupts) ako razvojna platforma podržava tu funkcionalnost  

##### Efikasno upravljanje resursima  
- [ ] Spriječiti konflikte pristupa resursima korištenjem odgovarajućih mehanizama (semafori, kritične sekcije, zastavice)  
- [ ] Minimizirati vrijeme izvršavanja ISR funkcija kako bi se izbjegle blokade drugih prekida  

##### Demonstracija rada s vanjskim sklopovima  
- [ ] Koristiti dodatne vanjske sklopove (senzore, tipkala, LED diode, serijske module, eksterne kontrolere)  
- [ ] Implementirati logiku koja pokazuje kako različiti prioriteti utječu na obradu događaja  

##### Dokumentacija i testiranje  
- [ ] Jasno dokumentirati način rada programa, uključujući opis svakog prekida i njegovog prioriteta  
- [ ] Provesti testove kako bi se osiguralo pravilno reagiranje sustava na različite događaje  

#### Dodatni zahtjevi (po izboru jedan)  
- [ ] Implementirati mehanizam detekcije i rješavanja situacija gdje se višestruki prekidi javljaju istovremeno  
- [ ] Koristiti DMA (Direct Memory Access) ako je dostupno kako bi se smanjio utjecaj prekida na glavni procesor  
