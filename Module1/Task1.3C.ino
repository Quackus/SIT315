const uint8_t PR_PIN = A0;
const uint8_t PIR_PIN = 2;
const uint8_t LEDRED_PIN = 13;
const uint8_t LEDGREEN_PIN = 12;


uint8_t ledRedState = LOW;
uint8_t ledGreenState = LOW;

uint8_t lightLevel = LOW;
uint8_t proximity = LOW;

const uint16_t t1_load = 0;
const uint16_t t1_comp = 31250;

void setup()
{
  pinMode(LEDRED_PIN, OUTPUT);
  pinMode(LEDGREEN_PIN, OUTPUT);
  pinMode(PR_PIN, INPUT);
  
  cli();
  
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), proxCheck, CHANGE);
  
  TCCR1A = 0;
  
  TCCR1B &= -(1 << WGM13);
  TCCR1B |= (1 << WGM12);
  
  TCCR1B |= (1 << CS12);
  TCCR1B &= -(1 << CS10);
  TCCR1B &= -(1 << CS11);
  
  TCNT1 = t1_load;
  OCR1A = t1_comp;
  
  TIMSK1 = (1 << OCIE1A);
  
  sei();
  
  Serial.begin(9600);
}

void loop()
{

  Serial.print("LIGHT: ");
  Serial.print(lightLevel);
  Serial.print("  PROX: ");
  Serial.println(proximity);
  
  if (proximity == HIGH)
    digitalWrite(LEDRED_PIN, HIGH);
  else
    digitalWrite(LEDRED_PIN, LOW);
  
  if (lightLevel > 100)
    digitalWrite(LEDGREEN_PIN, HIGH);
  else
    digitalWrite(LEDGREEN_PIN, LOW);
  
  delay(1000);
}

void proxCheck()
{
  proximity = digitalRead(PIR_PIN);
}
                  
                  
                  
ISR(TIMER1_COMPA_vect)	
{
  	lightLevel = analogRead(PR_PIN);
}