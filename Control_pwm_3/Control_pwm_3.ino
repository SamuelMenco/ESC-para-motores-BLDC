volatile int step = 0;
volatile int Next_step = 0;
volatile int Prev_step = 0;
int hall1 = 0;
int hall2 = 0;
int hall3 = 0;

#define Hall_1 5
#define Hall_2 6
#define Hall_3 7

#define HIN_faseA 3
#define HIN_faseB 10
#define HIN_faseC 9
#define LIN_faseA 12
#define LIN_faseB 11
#define LIN_faseC 8

// Para generar el PWM
#define potPin A0
volatile uint16_t adc_value;
volatile uint16_t pwm_value;
const byte PRESCALER2 = 0b001;

//Startup 
bool motorParado = false;

void setup() {
  //Cofniguro los pines de los Hall como input pullup (5v siempre a menos que el sensor mande tierra)
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  
  // Configurar los pines del 3, 8, 9, 10, 11 y 12 como salidas
  DDRD |= B00001000;
  DDRB |= B00011111;

  // Habilitar la interrupción de cambio de pin para los pines 5, 6 y 7
  PCICR |= B00000100;
  PCMSK2 |= B11100000;

  Serial.begin(9600);
  gethalls();
  DecideState();

  TCCR1B = (TCCR1B & 0b11111000) | PRESCALER2;
  TCCR2B = (TCCR2B & 0b11111000) | PRESCALER2;

  // Lectura de ADC por manipulación de puertos
  ADMUX = (1 << REFS0); // Referencia de voltaje: AVCC con conexión a VCC
  ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Habilitar ADC, habilitar interrupción ADC, prescaler de 128
  ADCSRB = 0; // Modo de conversión libre
  DIDR0 = 0; // Deshabilitar el apagado digital en los pines analógicos
  ADMUX |= 0; // Seleccionar el pin A0 como entrada analógica
  ADCSRA |= (1 << ADSC); // Iniciar la primera conversión

  Serial.println(step);
}

ISR(ADC_vect) {
  adc_value = ADC;
  pwm_value = ADC / 4;
  gethalls();
  DecideState();
  ADCSRA |= (1 << ADSC); // Iniciar la siguiente conversión
}

// Manejar la interrupción de cambio de pin
ISR(PCINT2_vect) {

}

void loop() {
  
}

void gethalls() {

  if (((PIND & B11100000) == B10100000 && (step == 6 || step == 0))) {
    step = 1;
    Next_step = 2;
    Prev_step = 6;
  }
  else if (((PIND & B11100000) == B10000000 && (step == 1 || step == 0))) {
    step = 2;
    Next_step = 3;
    Prev_step = 1;
  }
  else if (((PIND & B11100000) == B11000000 && (step == 2 || step == 0))) {
    step = 3;
    Next_step = 4;
    Prev_step = 2;
  }
  else if (((PIND & B11100000) == B01000000 && (step == 3 || step == 0))) {
    step = 4;
    Next_step = 5;
    Prev_step = 3;
  }
  else if (((PIND & B11100000) == B01100000 && (step == 4 || step == 0))) {
    step = 5;
    Next_step = 6;
    Prev_step = 4;
  }
  else if (((PIND & B11100000) == B00100000 && (step == 5 || step == 0))) {
    step = 6;
    Next_step = 1;
    Prev_step = 5;
  }
}

void DecideState() {
  switch (step) {
    case 1:
      writePhases(pwm_value, 0, 0, 1, 0, 0);
      break;
    case 2:
      writePhases(0, 0, 0, 1, pwm_value, 0);
      break;
    case 3:
      writePhases(0, 1, 0, 0, pwm_value, 0);
      break;
    case 4:
      writePhases(0, 1, pwm_value, 0, 0, 0);
      break;
    case 5:
      writePhases(0, 0, pwm_value, 0, 0, 1);
      break;
    case 6:
      writePhases(pwm_value, 0, 0, 0, 0, 1);
      break;
  }
}

void writePhases(uint8_t inA, uint8_t sdA, uint8_t inB, uint8_t sdB, uint8_t inC, uint8_t sdC) {
  analogWrite(HIN_faseA, inA);
  analogWrite(HIN_faseB, inB);
  analogWrite(HIN_faseC, inC);
  digitalWrite(LIN_faseA, sdA);
  digitalWrite(LIN_faseB, sdB);
  digitalWrite(LIN_faseC, sdC);
}
