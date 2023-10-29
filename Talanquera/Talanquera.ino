#include <Servo.h>
Servo ioe;

int trig = 2;
int echo = 3;
int tiempo;
int distancia;
int ledVerde = 8;
int ledRojo = 9;

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  ioe.attach(4);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRojo, OUTPUT);
}

void loop() {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  tiempo = pulseIn(echo, HIGH);
  distancia = tiempo / 58.2;

  if (distancia <= 10) {
    ioe.write(120);
    digitalWrite(ledVerde, HIGH);  // LED verde encendido cuando está abierta
    digitalWrite(ledRojo, LOW);     // LED rojo apagado
    delay(2000);
  } else {
    ioe.write(45);
    digitalWrite(ledVerde, LOW);    // LED verde apagado
    digitalWrite(ledRojo, HIGH);    // LED rojo encendido cuando está cerrada
  }

  delay(500);
}
