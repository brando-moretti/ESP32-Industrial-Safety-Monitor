#include <Arduino.h>

enum stato_sicurezza { SICURO, ALLARME };

#define LED_ALLARME 2
#define PIN_PULSANTE 5
#define PIN_POTENZIOMETRO 34

struct dati_potenziometro {
  int valore_p;
  stato_sicurezza stato;
};

// 1. Variabile volatile per comunicare tra la ISR e il loop.
// Il compilatore ora sa che questa variabile può cambiare in qualsiasi
// millisecondo.
volatile bool richiestaStampaMemoria = false;
volatile unsigned long ultimoTempoInterrupt =
    0; // Serve per il debouncing hardware

// 2. La ISR (Interrupt Service Routine). Salvata nella RAM veloce.
void IRAM_ATTR gestionePulsanteISR() {
  unsigned long tempoAttualeISR = millis();
  // Debouncing: ignora i rimbalzi meccanici del pulsante nei primi 200 ms
  if (tempoAttualeISR - ultimoTempoInterrupt > 200) {
    richiestaStampaMemoria = true; // Alza la bandierina (il "post-it")
    ultimoTempoInterrupt = tempoAttualeISR;
  }
}

void funzione_stampa(struct dati_potenziometro *LetturaAttuale) {
  Serial.println("Valore del Potenziometro:");
  Serial.println(LetturaAttuale->valore_p);
  Serial.println("Stato della Sicurezza:");
  Serial.println(LetturaAttuale->stato);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_ALLARME, OUTPUT);
  pinMode(PIN_PULSANTE, INPUT_PULLUP);

  // 3. Agganciamo il campanello. Quando il pin 5 va LOW (FALLING), esegui la
  // ISR.
  attachInterrupt(digitalPinToInterrupt(PIN_PULSANTE), gestionePulsanteISR,
                  FALLING);
}

void loop() {
  static int i = 0;
  static unsigned long TempoAttuale = 0;
  static dati_potenziometro LetturaAttuale;
  static dati_potenziometro Cronologia[5];

  // Il tuo codice originale basato su millis() continua a girare indisturbato
  if (millis() - TempoAttuale >= 2000) {
    TempoAttuale = millis();
    LetturaAttuale.valore_p = analogRead(PIN_POTENZIOMETRO);

    if (LetturaAttuale.valore_p < 3000) {
      LetturaAttuale.stato = SICURO;
      digitalWrite(LED_ALLARME, HIGH);
    } else {
      LetturaAttuale.stato = ALLARME;
      digitalWrite(LED_ALLARME, LOW);
    }

    Cronologia[i] = LetturaAttuale;
    i++;
    if (i >= 5) {
      i = 0;
    }
  }

  // 4. Il loop controlla la bandierina alzata dall'Interrupt
  if (richiestaStampaMemoria) {
    Serial.println("--- STAMPA MEMORIA ---");
    for (int j = 0; j < 5; j++) {
      funzione_stampa(&Cronologia[j]);
    }

    // Abbassiamo la bandierina per non ripetere la stampa al prossimo giro di
    // loop
    richiestaStampaMemoria = false;
  }
}