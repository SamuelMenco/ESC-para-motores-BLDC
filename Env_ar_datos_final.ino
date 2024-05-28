#include <util/atomic.h>
#include <SPI.h>
#include <LoRa.h> 
#include <SoftwareSerial.h> 
#include <TinyGPS.h>


volatile unsigned long mseg = 0; 
volatile unsigned long t_stamp = 0;

//Corriente
float I;
#define pinCorriente A1

//Voltaje
float V;
#define pinVoltaje A6

// Para el GPS
TinyGPS gps;
SoftwareSerial ss(3,4); //Rx Tx
float latitude = 0;
float longitude = 0;
float speed_kmh = 0;


//RPM motor
volatile int pulsos = 0;  
volatile unsigned long t_0;
volatile unsigned long periodo;
volatile unsigned long RPM_motor;
float Speed;
bool newdata_motor = false;


//LoRa
#define BAND 433E6 // Frecuencia LoRa
String message;


void setup() {
  ss.begin(9600); //Inicializo una comunicación serial para el GPS
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP); //Pin para el sensor Hall del motor
  pinMode(8,INPUT); //Pin para el sensor hall de la llanta
  attachInterrupt(0, interrupcion0, FALLING); //Inicializar las interrupciones por Hardware en el pin 2 (Hall motor)
  
  // Configuración para generar una interrupción cada milisegundo y poder contar el tiempo
  TCCR1A = 0; 
  TCCR1B = (1 << WGM12) | (1 << CS10); 
  OCR1A = 15999; 
  TIMSK1 |= (1 << OCIE1A);
  sei();

  // Iniciar LoRa
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa initialization failed.");
    while (1);
  }

  Serial.println("LoRa initialized."); 

}

void loop() {
  I = get_corriente(100);
  V = get_voltaje(100);
  getGPS();

  if(newdata_motor == true){
      Serial.print("RPM_motor: ");
      Serial.print(RPM_motor);
      Serial.print (", ");
      Serial.print("I: ");
      Serial.print(I);
      Serial.print (", ");
      Serial.print("V: ");
      Serial.print(V);
      Serial.print (", ");
      Serial.print("Long: ");
      Serial.print(longitude, 10);
      Serial.print (", ");
      Serial.print("Lat: ");
      Serial.print(latitude, 10);
      Serial.print (", ");
      Serial.print("Speed km/h: ");
      Serial.println(speed_kmh);
      LoRa.beginPacket();
      LoRa.print(RPM_motor);
      LoRa.print (",");
      LoRa.print(I);
      LoRa.print (",");
      LoRa.print(V);
      LoRa.print (",");
      LoRa.print(longitude, 10);
      LoRa.print (",");
      LoRa.print(latitude, 10);
      LoRa.print (",");
      LoRa.println(speed_kmh);
      LoRa.endPacket();
      newdata_motor = false;
  
  }
  else {
      Serial.print("RPM_motor: ");
      Serial.print(RPM_motor);
      Serial.print (", ");
      Serial.print("I: ");
      Serial.print(I);
      Serial.print (", ");
      Serial.print("V: ");
      Serial.print(V);
      Serial.print (", ");
      Serial.print("Long: ");
      Serial.print(longitude, 10);
      Serial.print (", ");
      Serial.print("Lat: ");
      Serial.print(latitude, 10);
      Serial.print (", ");
      Serial.print("Speed km/h: ");
      Serial.println(speed_kmh);
      LoRa.beginPacket();
      LoRa.print(RPM_motor);
      LoRa.print (",");
      LoRa.print(I);
      LoRa.print (",");
      LoRa.print(V);
      LoRa.print (",");
      LoRa.print(longitude, 10);
      LoRa.print (",");
      LoRa.print(latitude, 10);
      LoRa.print (",");
      LoRa.println(speed_kmh);
      LoRa.endPacket();
    
  }
 
 delay(100);
}


//Contador que aumenta cada milisegundo
ISR(TIMER1_COMPA_vect) {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    mseg++;
  }
}

//Aqui se miden las rpm del motor
void interrupcion0() {
  pulsos++;
  if (pulsos == 1) {
    t_stamp = mseg;
    RPM_motor = 60000 / (t_stamp - t_0);
    pulsos = 0;
    t_0 = t_stamp;
    newdata_motor = true;  
  }
}


//Aquí hago un promedio de n muestras del sensor de corriente
float get_corriente(int n_muestras)
{
  float voltajeSensor;
  float corriente=0;
  for(int i=0;i<n_muestras;i++)
  {
    voltajeSensor = analogRead(pinCorriente) * (5.0 / 1023.0);////lectura del sensor
    corriente=corriente+((voltajeSensor-2.481476)/0.107744); //Ecuación  para obtener la corriente
  }
  corriente=corriente/n_muestras;
  return(corriente);
}


//Aquí hago un promedio de n muestras del divisor de voltaje
float get_voltaje(int n_muestras)
{
  float voltaje=0;
  for(int i=0;i<n_muestras;i++)
  {
    voltaje = voltaje + ((analogRead(pinVoltaje) * (5.0 / 1023.0))*5.441931236);////lectura del sensor   
  }
  voltaje=voltaje/n_muestras;
  return(voltaje);
}

//Leer datos GPS
void getGPS(){
  
    while (ss.available() > 0) {
    char c = ss.read();
    if (gps.encode(c)) {
        float flat, flon;
        unsigned long age;
        gps.f_get_position(&flat, &flon, &age);
        if (age != TinyGPS::GPS_INVALID_AGE) {
            latitude = flat;
            longitude = flon;
            speed_kmh = gps.f_speed_kmph();
        }
        break; // Salir del bucle después de obtener datos válidos
    }
}
}
