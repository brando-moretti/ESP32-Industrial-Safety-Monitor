#include <Arduino.h>
enum stato_sicurezza {SICURO, ALLARME
};
#define LED_ALLARME 2
#define PIN_PULSANTE 5
#define PIN_POTENZIOMETRO 34
struct dati_potenziometro {
  int valore_p;
   stato_sicurezza stato;
};

void funzione_stampa (struct dati_potenziometro*LetturaAttuale)
{
  Serial.println("Valore del Potenziomentro:");
  Serial.println(LetturaAttuale->valore_p);
  Serial.println("Stato della Sicurezza:");
  Serial.println(LetturaAttuale->stato);
}
    void setup() {
  Serial.begin(115200);
  pinMode(LED_ALLARME, OUTPUT);
  pinMode(PIN_PULSANTE, INPUT_PULLUP);
}

void loop() {
  static bool STATO_PRECEDENTE_PULSANTE = HIGH;
  static int i = 0;
  static unsigned long TempoAttuale = 0;
  static dati_potenziometro LetturaAttuale;
  static dati_potenziometro Cronologia[5];
  if (millis() - TempoAttuale >= 2000) {
    TempoAttuale = millis();
    LetturaAttuale.valore_p = analogRead(PIN_POTENZIOMETRO);
   if (LetturaAttuale.valore_p<3000){
     LetturaAttuale.stato = SICURO;
     digitalWrite(LED_ALLARME, HIGH);
     
   }
   else
   {
     LetturaAttuale.stato = ALLARME;
     digitalWrite(LED_ALLARME, LOW);
     
   }
   Cronologia[i] = LetturaAttuale;
   i++;

   if(i>=5){
     i = 0;
   }
  }
  bool StatoAttualePulsante = digitalRead(PIN_PULSANTE);
  if (StatoAttualePulsante== LOW && STATO_PRECEDENTE_PULSANTE == HIGH)
  {
    Serial.println("--- STAMPA MEMORIA ---");
    for (int j = 0; j < 5; j++) {
      funzione_stampa(&Cronologia[j]);
    }
    delay(50); 
  }

  STATO_PRECEDENTE_PULSANTE = StatoAttualePulsante;
  }


