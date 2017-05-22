/** ==================== BACKUP RADAR ====================
 *
 * @author Pilvi Rajala
 * May 2017 
 */

/* ==================== PROGRAM DESCRIPTION ====================
 *  
 *  - When the sensor detects something over a meter away, LED stays off.
 *  - When the something is detected meter or less away from sensor
 *    LED starts blinking being on for 0.5 s and off for 0.5 s.
 *  - Every 10 cm the signal length reduces until it's only 0.1 s
 *  - When distance is less than 10 cm or sensor can't read,
 *    the LED will be on continuously
 */

// Define required pins
#define TRIGGER_PIN 9
#define ECHO_PIN 8


void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize LED_BUILTIN pin as an output pin with the line
  pinMode(LED_BUILTIN, OUTPUT);

  // Set TRIGGER_PIN as OUTPUT that is used to activate the sensor
  // and use ECHO_PIN to receive data from the sensor
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
}

void loop() {

  // Initialize variables 
  long pulseDuration;   // Duration of the echo pulse
  long distanceCm;
  long blinkInterval;

   // First give a short LOW pulse to ensure a clean HIGH pulse
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);

  // High pulse to the sensor to initate reading 
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Listen for the returning pulse
  pulseDuration = pulseIn(ECHO_PIN, HIGH);

  // Calculate distance to the detected object
  distanceCm = pulseToCentimeters(pulseDuration);

  // If distance to object is between 100 and 10 cm, blink the LED
  if(distanceCm > 10 && distanceCm <= 100){
    blinkInterval = getBlinkInterval(distanceCm);
    
    digitalWrite(LED_BUILTIN, HIGH);
    delay(blinkInterval);
    digitalWrite(LED_BUILTIN, LOW);
    delay(blinkInterval);

    // Output the distance to the console
    Serial.println(distanceCm);
     
  }
  // Distance to the object is 10 cm or less, or the sensor can't read data,
  // keep the LED on.
  else if(distanceCm <= 10 || distanceCm == NULL){
    digitalWrite(LED_BUILTIN, HIGH);

    // Output the distance to the console
    Serial.println(distanceCm);
  }
  // If the object is further than 100 cm away, the LED stays off.
  else if(distanceCm > 100) {
    digitalWrite(LED_BUILTIN, LOW);
  }

  // Add a bit delay
  delay(250);
  
}

/**
 * Convert the length of the return pulse (measured in microseconds) 
 * to centimeters.
 */
long pulseToCentimeters(long microseconds){
  // Speed of sound is 343 m/s (in dry air at 20°C)
  // which equals to 34 300 cm / 100 000 microseconds => 29.15 microseconds / centimeter
  // The signal travels back and forth, so the distance to the object is half of
  // the distance the signal travels.

  return microseconds / 29.15 / 2.0;
}

/**
 * When distance is...
 * >= 100 cm => 0.50 s => 500 ms
 * >=  90 cm => 0.45 s => 450 ms
 * >=  80 cm => 0.40 s => 400 ms
 * >=  70 cm => 0.35 s => 350 ms
 * >=  60 cm => 0.30 s => 300 ms
 * >=  50 cm => 0.25 s => 250 ms
 * >=  40 cm => 0.20 s => 200 ms
 * >=  30 cm => 0.15 s => 150 ms
 * >=  20 cm => 0.10 s => 100 ms
 */
 long getBlinkInterval(long distance){
    long roundedDistance = distance - (distance % 10);
    return (roundedDistance / 2) * 10;
 }


