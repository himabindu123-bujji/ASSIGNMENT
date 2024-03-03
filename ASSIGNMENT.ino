 const int lm35Pin = A0;  // LM35 temperature sensor is connected to analog pin A0
const int ledPin = 13;   // Onboard LED is connected to digital pin 13

volatile float temperature = 0;  // Volatile variable for temperature readings

void setup() {
  pinMode(ledPin, OUTPUT);  // Set the LED pin as output

  // Configure Timer1 for 1 Hz (1000 ms) interval
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);  // CTC mode, prescaler 1024
  OCR1A = 15624;  // 16 MHz / (1024 * 1 Hz) - 1

  // Enable Timer1 compare match A interrupt
  TIMSK1 = (1 << OCIE1A);

  // Enable global interrupts
  sei();

  Serial.begin(9600);
}

void loop() {
  // Your main non-blocking code can go here
}

ISR(TIMER1_COMPA_vect) {
  // Timer1 compare match A interrupt service routine

  temperature = readTemperature();  // Read temperature from LM35 sensor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  if (temperature < 30) {
    blinkLED(250);  // Blink LED every 250 milliseconds
  } else {
    blinkLED(500);  // Blink LED every 500 milliseconds
  }
}

float readTemperature() {
  int sensorValue = analogRead(lm35Pin);
  float voltage = (sensorValue / 1023.0) * 5.0;  // Convert analog reading to voltage
  float temperature = voltage * 100.0;           // Convert voltage to temperature in degrees Celsius
  return temperature;
}

void blinkLED(unsigned long interval) {
  digitalWrite(ledPin, HIGH);  // Turn on the LED
  delay(interval);              // Wait for the specified interval
  digitalWrite(ledPin, LOW);   // Turn off the LED
  delay(interval);              // Wait for the specified interval
}
