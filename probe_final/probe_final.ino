#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


int fsrPin = 0;     // the FSR and 10K pulldown are connected to a0
int fsrReading;     // the analog reading from the FSR resistor divider
int fsrVoltage;     // the analog reading converted to voltage
unsigned long fsrResistance;  // The voltage converted to resistance
unsigned long fsrConductance; 
long fsrForce;       // Finally, the resistance converted to force
 
void setup(void) {
  pinMode(6, OUTPUT);
  Serial.begin(9600);   // We'll send debugging information via the Serial monitor
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay(); // Clear the buffer
  display.setTextSize(2); // Set text size to normal
  display.setTextColor(SSD1306_WHITE); // Set color to white
  display.clearDisplay();
  // display.setCursor(1,0); // Set cursor position (x, y)
  display.setCursor(OLED_WIDTH - 1, OLED_HEIGHT - 1);
  display.println(F("DiOra Probe")); // Display initial message8
  display.display(); // Show initial message on the display
  delay(2000);
  display.clearDisplay();

}
 
void loop() {
  fsrReading = analogRead(fsrPin);  
  Serial.print("Analog reading = ");
  Serial.println(fsrReading);
  
 
  // analog voltage reading ranges from about 0 to 1023 which maps to 0V to 5V (= 5000mV)
  fsrVoltage = map(fsrReading, 0, 1023, 0, 5000);
  Serial.print("Voltage reading in mV = ");
  Serial.println(fsrVoltage);  

  if (fsrVoltage == 0) {
    Serial.println("No pressure"); 
    
    
     
     
    
    
  } else {
    // The voltage = Vcc * R / (R + FSR) where R = 10K and Vcc = 5V
    // so FSR = ((Vcc - V) * R) / V        yay math!
    fsrResistance = 5000 - fsrVoltage;     // fsrVoltage is in millivolts so 5V = 5000mV
    fsrResistance *= 10000;                // 10K resistor
    fsrResistance /= fsrVoltage;
    Serial.print("FSR resistance in ohms = ");
    Serial.print(fsrResistance);
 
    fsrConductance = 1000000;           // we measure in micromhos so 
    fsrConductance /= fsrResistance;
 
    // Use the two FSR guide graphs to approximate the force
    if (fsrConductance <= 1000) {
      fsrForce = fsrConductance / 80;
      Serial.print("Force in Newtons: ");
      Serial.println(fsrForce);
      
      
       
      
      
       
      
    } else {
      fsrForce = fsrConductance - 1000;
      fsrForce /= 30;
      Serial.print("Force in Newtons: ");
      Serial.println(fsrForce);            
    }
    if (fsrForce>0){
      digitalWrite(6, HIGH);
      display.clearDisplay(); // Clear the buffer
      display.setTextSize(2); // Set text size to normal
      display.setTextColor(SSD1306_WHITE); 
      display.setCursor(1,0); 
      // display.print(fsrForce);
      display.print("Pressure  Exceeded");  
      display.display();
      display.clearDisplay(); 
      // delay(1000);
      // digitalWrite(6, LOW);
    }
    else{
      digitalWrite(6, LOW);
      display.clearDisplay(); // Clear the buffer
      display.setTextSize(2); // Set text size to normal
      display.setTextColor(SSD1306_WHITE); 
      display.setCursor(1,0); 
      display.print(fsrForce);
      display.print(" N");  
      display.display();
    }

  }
  Serial.println("--------------------");
delay(1000);
}
