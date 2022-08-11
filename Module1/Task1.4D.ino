const uint8_t PR_PIN = A0;
const uint8_t PIR_PIN = 2;
const uint8_t LEDRED_PIN = 13;
const uint8_t LEDGREEN_PIN = 12;
const uint8_t LEDBLUE_PIN = 11;
const uint8_t BUTTON_PIN = 7;


uint8_t ledRedState = LOW;
uint8_t ledGreenState = LOW;
uint8_t ledBlueState = LOW;

uint8_t lightLevel = LOW;
uint8_t proximity = LOW;

const uint16_t t1_load = 0;
// comparison value to tick timer interrupt
const uint16_t t1_comp = 31250;

void setup()
{
  initPins();
  
  // stop interrupts
  cli();
  
  initPCINT();
  
  // interrupt when pin 2 changes in value
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), proxCheck, CHANGE);
  
  
  initTimer();
  
  
  sei();
  
  Serial.begin(9600);
}

void initPins()
{
	// initialise pin modes
  pinMode(LEDRED_PIN, OUTPUT);
  pinMode(LEDGREEN_PIN, OUTPUT);
  pinMode(LEDBLUE_PIN,OUTPUT);
  pinMode(PR_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
}

void initPCINT()
{
  // enable interrupts for PCIE2 group
  PCICR |= B00000100;
  // enable interrupt for pin 7
  PCMSK2 |= B10000000;
}

void initTimer()
{
  TCCR1A = 0;
  
  TCCR1B &= -(1 << WGM13);
  TCCR1B |= (1 << WGM12);
  
  TCCR1B |= (1 << CS12);
  TCCR1B &= -(1 << CS10);
  TCCR1B &= -(1 << CS11);
  
  TCNT1 = t1_load;
  OCR1A = t1_comp;
  
  TIMSK1 = (1 << OCIE1A);
}

void loop()
{

  // outputs values to serial port
  Serial.print("LIGHT: ");
  Serial.print(lightLevel);
  Serial.print("  PROX: ");
  Serial.print(proximity);
  Serial.print("  BUTTON: ");
  Serial.println(ledBlueState);
  
  // light diode check,  if value is above a certain value, it will turn on the LED
  // left this here because it needs a comparison, due to not being HIGH/LOW
  if (lightLevel > 100)
    digitalWrite(LEDGREEN_PIN, HIGH);
  else
    digitalWrite(LEDGREEN_PIN, LOW);
  
  // prints values and updates light diode LED 1Hz
  delay(1000);
}

// function utilized for interrupt when IR proximity sensor changes in value
void proxCheck()
{
  proximity = digitalRead(PIR_PIN);
  digitalWrite(LEDRED_PIN, proximity);
}

// interrupt function for the PCIE2 pin group (pin 7, from button)
ISR(PCINT2_vect)
{
	ledBlueState = !(PIND & B10000000);
  	digitalWrite(LEDBLUE_PIN, ledBlueState);
}

// interrupt function for timer, reads light level from diode                  
ISR(TIMER1_COMPA_vect)	
{
  	lightLevel = analogRead(PR_PIN);
}
