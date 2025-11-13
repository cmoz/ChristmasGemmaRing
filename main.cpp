#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Pin definitions for Gemma M0
#define NEOPIXEL_PIN A0    // Built-in NeoPixel on Gemma M0
#define TOUCH_PIN A2      // Touch sensor pin (A1/D2)
#define NUM_PIXELS 12      // Built-in NeoPixel count (can be changed if external strip is used)
#define BRIGHTNESS 20

// Create NeoPixel object
Adafruit_NeoPixel pixels(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Christmas sequence states
enum ChristmasSequence {
  IDLE,
  TWINKLE_RED_GREEN,
  RAINBOW_CHASE,
  BREATHING_GOLD,
  CANDY_CANE_FLASH,
  WARM_WHITE_FADE
};

ChristmasSequence currentSequence = IDLE;
unsigned long sequenceStartTime = 0;
unsigned long lastUpdate = 0;
int sequenceStep = 0;
bool touchDetected = false;
unsigned long touchTime = 0;
int touchThreshold = 200;  // Much higher threshold to prevent false triggers from electrical noise
int baselineReading = 0;   // Baseline analog reading when not touched

void checkTouch();
void nextChristmasSequence();
void runChristmasSequence(unsigned long currentTime);
void twinkleRedGreen(unsigned long currentTime);
void rainbowChase(unsigned long currentTime);
void breathingGold(unsigned long currentTime);
void candyCaneFlash(unsigned long currentTime);
void warmWhiteFade(unsigned long currentTime);
uint32_t wheel(byte WheelPos);
void rainbowCycle(unsigned long currentTime);
void rainbowChase(unsigned long currentTime);


void setup() {
  Serial.begin(115200);
  
  // Initialize NeoPixel
  
  pixels.begin();
  pixels.setBrightness(BRIGHTNESS); // Adjust brightness (0-255)
  pixels.show(); // Initialize all pixels to 'off'
  
  // Initialize touch pin and get baseline reading
  pinMode(TOUCH_PIN, INPUT);
  delay(100); // Allow pin to stabilize
  
  // Take baseline reading (average of several readings)
  long total = 0;
  for (int i = 0; i < 10; i++) {
    total += analogRead(TOUCH_PIN);
    delay(10);
  }
  baselineReading = total / 10;
  
  Serial.println("Christmas Brooch Ready! Touch to start sequences.");
  Serial.print("Baseline touch reading: ");
  Serial.println(baselineReading);
  
  // Show a brief startup indication for all 12 pixels
  for (int i = 0; i < NUM_PIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255)); // White flash all pixels
  }
  pixels.show();
  delay(500);
  pixels.clear();
  pixels.show();
}

void loop() {
  unsigned long currentTime = millis();
  
  // Check for touch input
  checkTouch();
  
  // Handle touch detection with longer debouncing
  if (touchDetected && (currentTime - touchTime > 1000)) {
    nextChristmasSequence();
    touchDetected = false;
  }
  
  // Run the current Christmas sequence
  runChristmasSequence(currentTime);
  
  delay(10); // Small delay to prevent overwhelming the processor
}

void checkTouch() {
  // Take multiple readings and average them to reduce noise
  int total = 0;
  for (int i = 0; i < 3; i++) {
    total += analogRead(TOUCH_PIN);
    delay(1);
  }
  int currentReading = total / 3;
  
  int touchDifference = abs(currentReading - baselineReading);
  
  // Only trigger if difference is significant AND we haven't detected recently
  if (touchDifference > touchThreshold && !touchDetected) {
    touchDetected = true;
    touchTime = millis();
    Serial.print("Touch detected! Difference: ");
    Serial.print(touchDifference);
    Serial.print(" (Current: ");
    Serial.print(currentReading);
    Serial.print(", Baseline: ");
    Serial.print(baselineReading);
    Serial.println(")");
  }
  
  // Debug: Print current readings occasionally (every 2 seconds) when idle
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 2000 && currentSequence == IDLE) {
    Serial.print("Debug - Current: ");
    Serial.print(currentReading);
    Serial.print(", Baseline: ");
    Serial.print(baselineReading);
    Serial.print(", Difference: ");
    Serial.println(touchDifference);
    lastDebug = millis();
  }
}

void nextChristmasSequence() {
  // Cycle through Christmas sequences
  switch (currentSequence) {
    case IDLE:
      currentSequence = TWINKLE_RED_GREEN;
      break;
    case TWINKLE_RED_GREEN:
      currentSequence = RAINBOW_CHASE;
      break;
    case RAINBOW_CHASE:
      currentSequence = BREATHING_GOLD;
      break;
    case BREATHING_GOLD:
      currentSequence = CANDY_CANE_FLASH;
      break;
    case CANDY_CANE_FLASH:
      currentSequence = WARM_WHITE_FADE;
      break;
    case WARM_WHITE_FADE:
      currentSequence = IDLE;
      break;
  }
  
  sequenceStartTime = millis();
  sequenceStep = 0;
  lastUpdate = 0;
  
  Serial.print("Starting sequence: ");
  Serial.println(currentSequence);
}

void runChristmasSequence(unsigned long currentTime) {
  switch (currentSequence) {
    case IDLE:
      pixels.clear();
      pixels.show();
      break;
      
    case TWINKLE_RED_GREEN:
      twinkleRedGreen(currentTime);
      break;
      
    case RAINBOW_CHASE:
      rainbowChase(currentTime);
      break;
      
    case BREATHING_GOLD:
      breathingGold(currentTime);
      break;
      
    case CANDY_CANE_FLASH:
      candyCaneFlash(currentTime);
      break;
      
    case WARM_WHITE_FADE:
      warmWhiteFade(currentTime);
      break;
  }
}

void twinkleRedGreen(unsigned long currentTime) {
  if (currentTime - lastUpdate > 500) { // Update every 500ms
    pixels.clear(); // Clear all pixels first
    
    // Alternate between red and green, with some random twinkling
    for (int i = 0; i < NUM_PIXELS; i++) {
      if ((sequenceStep + i) % 2 == 0) {
        pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // Red
      } else {
        pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // Green
      }
    }
    pixels.show();
    sequenceStep++;
    lastUpdate = currentTime;
    
    // Removed auto-advance - only touch controls sequences now
  }
}

void rainbowChase(unsigned long currentTime) {
  if (currentTime - lastUpdate > 100) { // Update every 100ms
    pixels.clear();
    
    // Create a chasing rainbow effect around the ring
    for (int i = 0; i < NUM_PIXELS; i++) {
      uint32_t color = wheel(((i * 256 / NUM_PIXELS) + sequenceStep * 5) & 255);
      pixels.setPixelColor(i, color);
    }
    pixels.show();
    sequenceStep++;
    lastUpdate = currentTime;
    
    // Removed auto-advance - only touch controls sequences now
  }
}

void breathingGold(unsigned long currentTime) {
  if (currentTime - lastUpdate > 50) { // Update every 50ms
    float breath = (sin(sequenceStep * 0.1) + 1.0) / 2.0; // Sine wave from 0 to 1
    int brightness = (int)(breath * 255);
    
    // Set all pixels to the same breathing gold color
    for (int i = 0; i < NUM_PIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(brightness, brightness/2, 0)); // Gold color
    }
    pixels.show();
    sequenceStep++;
    lastUpdate = currentTime;
    
    // Removed auto-advance - only touch controls sequences now
  }
}

void candyCaneFlash(unsigned long currentTime) {
  if (currentTime - lastUpdate > 200) { // Update every 200ms
    pixels.clear();
    
    // Create alternating red/white pattern around the ring
    for (int i = 0; i < NUM_PIXELS; i++) {
      if ((sequenceStep + i) % 4 < 2) {
        pixels.setPixelColor(i, pixels.Color(255, 255, 255)); // White
      } else {
        pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // Red
      }
    }
    pixels.show();
    sequenceStep++;
    lastUpdate = currentTime;
    
    // Removed auto-advance - only touch controls sequences now
  }
}

void warmWhiteFade(unsigned long currentTime) {
  if (currentTime - lastUpdate > 50) { // Update every 50ms
    float fade = (sin(sequenceStep * 0.05) + 1.0) / 2.0; // Slower sine wave
    int brightness = (int)(fade * 255);
    
    // Set all pixels to the same warm white fading color
    for (int i = 0; i < NUM_PIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(brightness, brightness * 0.8, brightness * 0.6)); // Warm white
    }
    pixels.show();
    sequenceStep++;
    lastUpdate = currentTime;
    
    // Removed auto-advance - only touch controls sequences now
  }
}

// Helper function to generate rainbow colors
uint32_t wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
