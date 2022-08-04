#include <IRremote.h>
const uint8_t IR_PIN = 12;
const uint8_t LEDRED_PIN = 11;
const uint8_t LEDGREEN_PIN = 10;


uint8_t ledRedState = LOW;
uint8_t ledGreenState = LOW;

IRrecv receiver(IR_PIN);
  
decode_results results;

void setup()
{
  pinMode(LEDRED_PIN, OUTPUT);
  pinMode(LEDGREEN_PIN, OUTPUT);
  
  receiver.enableIRIn();
  Serial.begin(9600);
}

void loop()
{
  
  if (receiver.decode(&results))
  {
    Serial.println(results.value, DEC);
    if (results.value == 16582903)
    {
      ledRedState = !ledRedState;
      digitalWrite(LEDRED_PIN, ledRedState);
      Serial.println("Red");
    }
    else if (results.value == 16615543)
    {
      ledGreenState = !ledGreenState;
      digitalWrite(LEDGREEN_PIN, ledGreenState);
      Serial.println("Green");
    }
    receiver.resume();
  }
  delay(100);
}