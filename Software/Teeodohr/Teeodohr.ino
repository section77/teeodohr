// ----------------------------
// --- Teeodohr             ---
// ---  Andreas Engel 2017  ---
// ----------------------------
#include <Servo.h>
#include "pitches.h"

Servo Arm;              // Servo 1 zuweisen

// maximaler Ohr Winkel. Je nach Tassen Höhe anpassen
int max_Winkel=70;
int min_Winkel=20;

// notes in the melody:
int melody_1[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations_1[] = {4, 8, 8, 4, 4, 4, 4, 4};

int LED1=3;                 // LED 3 minuten
int LED2=5;                // LED 5 minuten
int LED3=6;               // LED 7 minuten
int speakerOut = 10;     // Piezo Lautsprecher
int Time=3;            // Time for Tea in minuten

void setup() {
  pinMode(speakerOut, OUTPUT);    // Lautsprecher Ausgang
  pinMode(LED1, OUTPUT);         // LED 1
  pinMode(LED2, OUTPUT);        // LED 2
  pinMode(LED3, OUTPUT);       // LED 3
  pinMode(A1, INPUT);        // ON and minutes select switch
  pinMode(A2, INPUT);       // Start / Stop select switch

  analogWrite(LED1, 255); // 3 Minuten LED einschalten
  analogWrite(LED2, 0);   // alle anderen aus
  analogWrite(LED3, 0);   // alle anderen aus

  Arm.attach(11);         // attaches the servo Arm to pin D11 servo object
  Arm.write(min_Winkel);          // tell servo 1 to go to position 10°
  delay(1500);
  Arm.detach();

  ON_OFF_Sound();         // Play Sound. Ready to make Tea
}

void loop() {

  if (analogRead(A1) > 900 ) SelectNextTime();    // Mit Taster 1 Zeit einstellen
  if (analogRead(A2) > 900 ) TeaTime() ;         // Mit Taster 2 Tea Time Starten

}
// ------------------------------------------------------------------
// ------------------------------------------------------------------
void SelectNextTime(){
  // Warten bis Taster losgelassen
  do { delay(20); }while (analogRead(A1)>500);

  // Zeit einstellen. 3,5,7,10 & 12 minuten
  switch (Time) {
    case 3:
      Time=5;
      analogWrite(LED1, 0);   // LED's
      analogWrite(LED2, 255);
      analogWrite(LED3, 0);
      break;
    case 5:
      Time=7;
      analogWrite(LED1, 0);   // LED's
      analogWrite(LED2, 0);
      analogWrite(LED3, 255);
      break;
    case 7:
      Time=10;
      analogWrite(LED1, 255);   // LED's
      analogWrite(LED2, 0);
      analogWrite(LED3, 255);
      break;
    case 10:
      Time=12;
      analogWrite(LED1, 0);   // LED's
      analogWrite(LED2, 255);
      analogWrite(LED3, 255);
      break;
    case 12:
      Time=3;
      analogWrite(LED1, 255);   // LED's
      analogWrite(LED2, 0);
      analogWrite(LED3, 0);
  }
}
// ----------------------------
void TeaTime () {
  // Warten bis Taster losgelassen
  do { delay(20); }while (analogRead(A2)>500);

  // Schleife für die Anzahl der Minuten
  for (int i = 1; i <= Time; i++){

    // Tee Beutel langsam abwärts bis zum eingestellten Winkel
    Arm_absenken();

    // Diese Schleife dauert 1minute.
    for (int x=0;x<=20;x++){

      // Kurz vor ende der Minute den Beutel anheben.
      if (x==19){
        Arm_anheben();
     }

      // LED's fade in /out in increments of 5 points:
      for (int fadeValue = 15 ; fadeValue <= 250; fadeValue += 5) {
         analogWrite(LED1, fadeValue);
         analogWrite(LED2, 255-fadeValue);
         analogWrite(LED3, fadeValue);
        // wait for 30 milliseconds to see the dimming effect
        delay(30);
        if (analogRead(A2) > 900 ) break;             // Tea Time Stoppen
      }

      // LED's fade in / out in increments of 5 points:
      for (int fadeValue = 250 ; fadeValue >= 15; fadeValue -= 5) {
        analogWrite(LED1, fadeValue);
        analogWrite(LED2, 255-fadeValue);
        analogWrite(LED3, fadeValue);
        // wait for 30 milliseconds to see the dimming effect
        delay(30);
        if (analogRead(A2) > 900 ) break;             // Tea Time Stoppen
      }

      if (analogRead(A2) > 900 ) break;             // Tea Time Stoppen
    }
    if (analogRead(A2) > 900 ) {
      Arm_anheben();
      break;             // Tea Time Stoppen
    }
  }

  // Arm nach oben
  Arm_anheben();

  // Warten bis Taster losgelassen
  do { delay(20); }while (analogRead(A2)>500);

  ON_OFF_Sound();         // Play sound ready to make Tea
  // Zeit wieder auf 3 min.
  Time=3;
  analogWrite(LED1, 255); // 3 Minuten LED einschalten
  analogWrite(LED2, 0);   // alle anderen aus
  analogWrite(LED3, 0);   // alle anderen aus
}

void Arm_anheben(){
  // Arm nach oben
  Arm.attach(11);
  for (int p = max_Winkel; p >=min_Winkel; p--){
    Arm.write(p);
    delay(20);
  }
  Arm.detach();
}

void Arm_absenken(){
  // Arm nach unten
  Arm.attach(11);
  for (int p = min_Winkel; p <=max_Winkel; p++){
    Arm.write(p);
    delay(20);
  }
  Arm.detach();
}

void ON_OFF_Sound(){
    for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration_1 = 1000 / noteDurations_1[thisNote];
    tone(speakerOut, melody_1[thisNote], noteDuration_1);
    int pauseBetweenNotes = noteDuration_1 * 1.30;
    delay(pauseBetweenNotes);
    noTone(speakerOut);
  }
}
