/*
   Code for control of LED light in MEDUSA
   Author: Daniel Mauricio Pineda Tobón
   Version: 1.0
   Date: 13042020
*/

// The following vector has the percentage of time the port will be activated in a PWM signal
// Modify the values according to your needs, however keep the number of elements on the array
float Porcen[] = {100, 100, 70, 100, 65, 100, 100, 40, 30, 30, 45, 25, 100, 100, 30};
//############### L0   L1   L2  L3   L4  L5   L6   L7  L8  L9  L10 L11 L12  L13  L14


// #######################################################################################

// ###################### Do not modify anything from here on ############################

// #######################################################################################

// The following vector has the names of the Arduino MEGA ports used for switching the LEDs
int Puerto[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50};

String strPuerto = ""; // This stores a text indicating the port
boolean strCompleto = false;
int LEDstate = LOW; // This indicates if the LED is activated or not
int LED = 0;
float inten = 100; // This indicates a value of intensity in PWM

// Para el PWM
float periodo = 200; // This is a value for the PWM period in microseconds
int tEnc = 0;
int tApa = 0;

void setup() {
  Serial.begin(9600);
  // This routine simply configures the different ports as outputs
  for (int i = 0; i < 22; i++) {
    pinMode(Puerto[i], OUTPUT);
  }
  //10 bytes will be reserved for the incomming string
  strPuerto.reserve(10);
}

void loop() {
  if (strCompleto == true && LEDstate == HIGH) {
    LED = strPuerto.toInt(); // The incomming string is converted to an int
    strPuerto = ""; // Reseting the variable
    strCompleto = false; // Reseting the variable
    Serial.print("LED");
    Serial.print(LED);
    Serial.print("ON");
    Serial.print("\r\n");
    if (LED == 100) apagarTodo(); // This function turns off all the LEDs
    pwmPin(Puerto[LED], Porcen[LED], periodo);
  }
  if (strCompleto == true && LEDstate == LOW) {
    LED = strPuerto.toInt(); // Converting the incomming data to int
    strPuerto = ""; // Reseting the variable
    strCompleto = false; // Reseting the variable
    Serial.println("ALL OFF");
    apagarTodo(); // This function turns off all the LEDs
  }
}

void serialEvent() {
  while (Serial.available()) {
    // Capturing the incomming character
    char inChar = (char)Serial.read();
    // This character is added to the port string
    strPuerto += inChar;
    // If the received character is a new line
    // it means that it finished. (Condition for Python).
    if (inChar == 'H' || inChar == 'h') { // If one of the incomming chars is H or h then the LED is on
      strCompleto = true;
      LEDstate = HIGH;
    } else if (inChar == 'L' || inChar == "l") { // If one of the chars is L or l then the LED is off
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
      // If the received character is a new line
      // it means that it finished. (Condition for Python).
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
