/*
   Código para el control de la iluminación con arduino
   Autor: Daniel Mauricio Pineda Tobón
   Version: 1.0
   Fecha: 13042020
*/

String strPuerto = ""; // Almacena el texto que indica el puerto que se debe activar
boolean strCompleto = false;
int LEDstate = LOW; // Indicara si se enciende o se apaga el LED
int LED = 0;
float inten = 100; // Valor de intensidad para los LEDs

// Longitud del arreglo
#define ARRAYSIZE 22
int Puerto[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50};
// Vector con el porcentaje de tiempo activo de cada LED
float Porcen[] = {100, 100, 70, 100, 65, 100, 100, 40, 30, 30, 45, 25, 100, 100, 30};
//############### L0   L1   L2  L3   L4  L5   L6   L7  L8  L9  L10 L11 L12  L13  L14

// Para el PWM
float periodo = 200; // En microsegundos
int tEnc = 0;
int tApa = 0;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 22; i++) {
    pinMode(Puerto[i], OUTPUT);
  }
  //Reservamos unos 10 bytes para el texto que se recibe
  strPuerto.reserve(10);
}

void loop() {
  if (strCompleto == true && LEDstate == HIGH) {
    LED = strPuerto.toInt(); // Convierto el dato recibido en entero
    strPuerto = ""; // Reinicio la variable que almacena los datos recibidos
    strCompleto = false; // Reinicio la variable que indica si se termino de recibir datos
    Serial.print("LED");
    Serial.print(LED);
    Serial.print("ON");
    Serial.print("\r\n");
    if (LED == 100) apagarTodo();
    pwmPin(Puerto[LED], Porcen[LED], periodo);
  }
  if (strCompleto == true && LEDstate == LOW) {
    LED = strPuerto.toInt(); // Convierto el dato recibido en entero
    strPuerto = ""; // Reinicio la variable que almacena los datos recibidos
    strCompleto = false; // Reinicio la variable que indica si se termino de recibir datos
    Serial.println("ALL OFF");
    apagarTodo();
  }
}

void serialEvent() {
  while (Serial.available()) {
    // Tomamos el caracter que entró
    char inChar = (char)Serial.read();
    // Añadimos el caracter al string
    strPuerto += inChar;
    // Si el caracter que se recibe es una nueva linea
    // significa que finalizo. (Condicion para Python).
    if (inChar == 'H' || inChar == 'h') {
      strCompleto = true;
      LEDstate = HIGH;
    } else if (inChar == 'L' || inChar == "l") {
      strCompleto = true;
      LEDstate = LOW;
    }
  }
}

void apagarTodo() {
  for (int i = 0; i < 22; i++) {
    digitalWrite(Puerto[i], LOW);
  }
}

void pwmPin(int pin, float porc, int periodo) {
  porc = porc / 100.;
  tEnc = int(porc * periodo);
  tApa = int(periodo - tEnc);
  while (LEDstate == HIGH) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(tEnc);
    digitalWrite(pin, LOW);
    delayMicroseconds(tApa);

    while (Serial.available()) {
      // Tomamos el caracter que entró
      char inChar = (char)Serial.read();
      // Añadimos el caracter al string
      strPuerto += inChar;
      // Si el caracter que se recibe es una nueva línea
      // significa que finalizo. (Condicion para Python).
      if (inChar == 'L' || inChar == 'l') {
        strCompleto = true;
        LEDstate = LOW;
      }

      if (inChar == 'P' || inChar == 'p') {
        inten = strPuerto.toInt();
        strPuerto = "";
        porc = inten / 100.;
        tEnc = int(porc * periodo);
        tApa = int(periodo - tEnc);
        break;
      }
    }
  }
}
