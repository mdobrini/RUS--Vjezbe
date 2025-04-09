/**
 * @file sleep_watchdog_interrupt.ino
 * @mainpage AVR Sleep i Watchdog Prekid
 *
 * @section overview Pregled modula
 * Ovaj primjer demonstrira kako koristiti sleep mode i watchdog prekide na AVR mikrokontroleru
 * kako bi se smanjila potrošnja energije. Uređaj se budi iz "power-down" moda korištenjem watchdog
 * timera ili eksternog prekida uz pomoć tipkala na INT0 pinu. 
 *
 * @section funkcionalnosti Ključne funkcionalnosti
 * - Sleep mode: Uređaj ulazi u SLEEP_MODE_PWR_DOWN radi štednje energije
 * - Watchdog timer: Automatsko buđenje nakon ~8 sekundi
 * - Eksterni prekid: Buđenje putem INT0 prekida (tipkalo)
 *
 * @section konfiguracija Shema spajanja
 * - LED dioda na pin 13
 * - Tipkalo povezano na pin 2 (INT0), s internim pull-up otporom
 *
 * @section autor Autor
 * Mislav Dobrinić
 * @date 9. travnja 2025.
 * @version 1.0
 * @note Licenca: MIT Licenca
 */

#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

const int ledPin = 13;     ///< Pin za LED indikator
const int buttonPin = 2;   ///< Pin za eksterni prekid (INT0)

volatile bool wakeUpFlag = false; ///< Zastavica za buđenje iz sleep moda

/**
 * @brief ISR za vanjski prekid INT0
 * Postavlja zastavicu za buđenje.
 */
ISR(INT0_vect) {
  wakeUpFlag = true;
}

/**
 * @brief ISR za Watchdog prekid
 * Postavlja zastavicu za buđenje.
 */
ISR(WDT_vect) {
  wakeUpFlag = true;
}

/**
 * @brief Inicijalizacija periferije i prekida
 */
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  // Konfiguracija vanjskog prekida na padajući brid
  EICRA |= (1 << ISC01);
  EIMSK |= (1 << INT0);

  sei(); // Omogući globalne prekide
}

/**
 * @brief Glavna petlja programa
 * LED dioda svijetli 5 sekundi, zatim se mikrokontroler uspava do idućeg buđenja.
 */
void loop() {
  digitalWrite(ledPin, HIGH);
  delay(5000);
  digitalWrite(ledPin, LOW);

  enterSleepMode(); // Ulazak u sleep mode
}

/**
 * @brief Ulazak u power-down sleep mode
 * Uključuje watchdog timer i deaktivira ADC radi štednje, zatim se mikrokontroler uspava.
 */
void enterSleepMode() {
  wakeUpFlag = false;
  setupWatchdog(); // Omogući watchdog ako se koristi

  ADCSRA &= ~(1 << ADEN); // Isključi ADC radi uštede
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_cpu();

  while (!wakeUpFlag) {}

  sleep_disable();
  ADCSRA |= (1 << ADEN); // Ponovno uključi ADC
  wdt_disable();         // Isključi watchdog nakon buđenja
}

/**
 * @brief Konfiguracija Watchdog timera
 * Postavlja watchdog da generira prekid nakon ~8 sekundi.
 */
void setupWatchdog() {
  MCUSR &= ~(1 << WDRF);
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = (1 << WDIE) | (1 << WDP3) | (1 << WDP0); // Timeout ~8s
}