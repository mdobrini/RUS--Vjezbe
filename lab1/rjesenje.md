### Upravljanje višestrukim prekidima i njihovim prioritetima  

#### Cilj zadatka  
- [x] Proučiti koncept višestrukih prekida i njihovih prioriteta na odabranom procesoru
      Korišten je procesor: 
- [ ] Implementirati program koji efikasno upravlja različitim događajima koristeći odgovarajuće strategije rukovanja prekidima  

#### Rjesenje zadataka  

##### Implementacija višestrukih interrupta  
- [x] Definirati i implementirati više prekidnih rutina (ISR – Interrupt Service Routines) koje će reagirati na različite događaje:  
  - [x] Pritisak različitih tipkala  
```cpp
attachInterrupt(digitalPinToInterrupt(BUTTON2), ISR_button2, FALLING);
```
  - [x] Aktivaciju internih timera
- Timer1 je kofiguriran za CTC mod (Clear Timer on Compare Match)
- Postavljen je da generira prekid svake sekunde (1hz) pomoću `OCR1A = 15624`
- Kada dođe do match-a, poziva se ISR `TIMER1_COMPA_vect`, gdje se postavlja `timer_flag = true`, a LED-ica trepće u `loop()`

```cpp
ISR(TIMER1_COMPA_vect) {
  timer_flag = true;
}

```
  - [x] Očitavanje različitih senzora
- Koristi se `HC-SR04` ultrazvučni senzor.
- ISR `ISR_sensor` se aktivira na RISING rub Echo pina, kad se signal vrati s prepreke.
- Ovdje se ne mjeri udaljenost, nego se detektira prisustvo reflektiranog signala putem prekida.
```cpp
attachInterrupt(digitalPinToInterrupt(ECHO), ISR_sensor, RISING);

void ISR_sensor() {
  sensor_triggered = true;
}

```  
  - [x] Serijsku komunikaciju ili druge vanjske prekide
- `serialEvent()` je automatski pozvana funkcija kad stigne novi podatak putem serijske komunikacije.
- Iako nije "klasičan" ISR, ponaša se kao softverski prekid povezan s UART-om.
- Obrađuje podatke pristigle preko USB serijske veze s računalom.
```cpp
void serialEvent() {
  while (Serial.available()) {
    char c = Serial.read();
    Serial.print("Primljeno: ");
    Serial.println(c);
  }
}

```

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
