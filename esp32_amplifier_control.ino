// ESP32 Amplifier Control Code - Example Sketch
// Libraries needed:
// - Adafruit_GFX
// - Adafruit_SSD1306
// - RotaryEncoder
// - IRremoteESP32

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RotaryEncoder.h>
#include <IRremoteESP32.h>

// --- Pin Definitions ---
// I2C
#define I2C_SDA 21
#define I2C_SCL 22
// Amplifier Control
#define AMP_MUTE_PIN 18
#define AMP_SD_PIN 5
// Rotary Encoder
#define ENCODER_PIN_A 34
#define ENCODER_PIN_B 35
// IR Receiver
#define IR_RECEIVER_PIN 15
// Audio Mux
#define MUX_SELECT_PIN 19

// --- Objects ---
Adafruit_SSD1306 display(128, 64, &Wire, -1);
RotaryEncoder encoder(ENCODER_PIN_A, ENCODER_PIN_B, RotaryEncoder::LatchMode::FOUR3);
IRrecv irrecv(IR_RECEIVER_PIN);
decode_results results;

// --- Global Variables ---
int currentVolume = 50;
int currentInput = 0; // 0 = AUX, 1 = BT

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);

  // Initialize Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Amp Control v1.0");
  display.display();

  // Initialize Amplifier Control Pins
  pinMode(AMP_MUTE_PIN, OUTPUT);
  pinMode(AMP_SD_PIN, OUTPUT);
  digitalWrite(AMP_SD_PIN, HIGH); // Take amps out of shutdown
  digitalWrite(AMP_MUTE_PIN, LOW); // Unmute

  // Initialize IR Receiver
  irrecv.enableIRIn();

  // Initialize DSP (conceptual - requires specific library/commands)
  // dsp.begin();
  // dsp.setVolume(currentVolume);

  Serial.println("System Initialized.");
}

void loop() {
  // --- Check Rotary Encoder ---
  static int lastPos = 0;
  encoder.tick();
  int newPos = encoder.getPosition();
  if (newPos != lastPos) {
    currentVolume += (newPos - lastPos);
    if (currentVolume > 100) currentVolume = 100;
    if (currentVolume < 0) currentVolume = 0;
    updateVolume(currentVolume);
    lastPos = newPos;
  }

  // --- Check IR Remote ---
  if (irrecv.decode(&results)) {
    // Handle IR codes for volume up/down, mute, input select, etc.
    // e.g., if (results.value == 0xFFE01F) { updateVolume(++currentVolume); }
    irrecv.resume();
  }

  delay(10); // Small delay to prevent busy-waiting
}

void updateVolume(int volume) {
  Serial.print("Setting volume to: ");
  Serial.println(volume);

  // Send volume command to DSP
  // dsp.setVolume(volume);

  // Update display
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Volume: ");
  display.print(volume);
  display.display();
}

void setInput(int input) {
  currentInput = input;
  digitalWrite(MUX_SELECT_PIN, currentInput);
  Serial.print("Input set to: ");
  Serial.println(input == 0 ? "AUX" : "Bluetooth");

  // Update display
}
