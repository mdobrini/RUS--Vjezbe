### Upravljanje višestrukim prekidima i njihovim prioritetima  

#### Cilj zadatka  
- [x] Proučiti koncept višestrukih prekida i njihovih prioriteta na odabranom procesoru
      Korišten je procesor: 
- [x] Implementirati program koji efikasno upravlja različitim događajima koristeći odgovarajuće strategije rukovanja prekidima  

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
- [x] Postaviti različite prioritete za prekide kako bi važniji događaji imali prednost pri obradi
      
   - Postavljanje različitih prioriteta rješava se korištenjem vanjskih prekida `INT0` i `INT1`. Prioritet tih prekida ovisi o hardverskoj konfiguraciji mikrokontrolera, gdje `INT0` ima viši prioritet od `INT1`, što znači da će ISR za `ECHO` biti obrađen prije ISR za `BUTTON2`.
      - Timer prekid `TIMER1` je konfiguriran za treći nivi prioriteta -> izvršava se nakon vanjskih prekida, jer je postavljen an najniži među tri glavna prekida.
```cpp
// Vanjski prekidi
attachInterrupt(digitalPinToInterrupt(BUTTON2), ISR_button2, FALLING); // INT1
attachInterrupt(digitalPinToInterrupt(ECHO), ISR_sensor, RISING);      // INT0
```
      
- [x] Omogućiti preklapanje prekida (nested interrupts) ako razvojna platforma podržava tu funkcionalnost
      
   - Dok se obrada jednog prekida vrši, drugi prekid može biti pozvan i odmah obrađen. Za to se koristi funkcija `sei()` unutar ISR-ova, koja omogućava da se prekidi ne blokiraju međusobno.
```cpp
// ISR za tipkalo 2 (pokreće slanje ultrazvučnog impulsa) - Srednji prioritet
void ISR_button2() {
  sei(); // Omogući nested interrupts
  // Logika za tipkalo 2
}
```
- `sei()` se koristi unutar ISR-a da omogući "nested interrupts". Ova linija omogućava obradu drugih prekida dok je jedan ISR aktivan, čime se omogućava da važniji prekidi (s višim prioritetom) ne budu blokirani od strane onih s nižim prioritetom.

```cpp
// ISR za Echo signal s HC-SR04 (detektira reflektirani val) - Najviši prioritet
void ISR_sensor() {
  sei(); // Omogući nested interrupts
  sensor_triggered = true;
  nested_occurred = true;

...

noInterrupts();
if (nested_occurred) {
  processed_nested = true;
  nested_occurred = false;
  interrupts();
  Serial.println("### LOOP: DETEKTIRANO PREKLAPANJE (nested_occurred flag) ###");
}
else {
  interrupts();
}
}
```
- `nested_occured` flag služi za praćenje situacija kada jedan prekid (npr. `ISR_sensor()`) preklapa drugi prekid (npr. `ISR(TIMER1_COMPA_vect)`). Ovdje je važno da se svi prekidi pravilo obrade bez ometanja važnijih zadataka.



##### Efikasno upravljanje resursima  
- [x] Spriječiti konflikte pristupa resursima korištenjem odgovarajućih mehanizama (semafori, kritične sekcije, zastavice)

- Zaštićene sekcije koriste funckije `noInterrupts()` i `interrupts()`. Ove funkcije omogućuju da se prekidi onemoguće dok se čita ili mijenja kritična globalna varijabla, čime se sprječava istovremeni pristup iz različitih ISR funkcija.
- Zastavice poput `button2_pressed`, `sensor_triggered`, i `timer_flag` koriste se za signalizaciju da su događaji u prekidu obradeni. Ove zastavice se postavljaju unutar ISR-a i čitaju u glavnoj petlji `loop().` Korištenjem kritičnih sekcija, osigurava se da samo jedna ISR može pristupiti i postaviti te zastavice u isto vrijeme.

  ```cpp
  void ISR_sensor() {
  sei(); // Omogući nested interrupts
  noInterrupts(); // Onemogući prekide dok mijenjamo flag
  sensor_triggered = true;  // Postavi flag za signalizaciju
  nested_occurred = true;  // Označava preklapanje
  interrupts();  // Omogući prekide
}

  ```
```  
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
