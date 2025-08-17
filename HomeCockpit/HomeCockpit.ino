#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <LiquidCrystal.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR   0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

LiquidCrystal lcd(D7, D8, D4, D5, D6, D3);

String inputString = "";

void setup() {
  Serial.begin(115200);   // must match COM6 settings in Windows
  Serial.println("Arduino ready...");

  Wire.begin(D2, D1);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Halt
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Waiting for data...");
  display.display();

  lcd.clear();
  lcd.begin(16, 2);
  lcd.noDisplay();
}

void loop() {
  while (Serial.available()) {
    lcd.display();
    char c = Serial.read();
    if (c == '\n') {
      String dataref;
      String value;
      int delimiterPos = inputString.indexOf('=');

      if (delimiterPos != -1) {
        dataref = inputString.substring(0, delimiterPos);
        value = inputString.substring(delimiterPos + 1);
      }
      processLine(dataref, value);
      inputString = "";
    } else if (c != '\r') {
      inputString += c;
    }
  }
}

String lastLine = "";

void processLine(String dataref, String value)
{
  if(dataref == "IAS")
  {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.println("IAS:");
    display.setTextSize(3);
    display.println(value);
    display.display();
  }
  else if(dataref == "ALT")
  {
    lcd.setCursor(4,0);
    lcd.print("           ");
    lcd.setCursor(0,0);
    lcd.print(dataref + ":" + value);
  }
  else if(dataref == "HDG")
  {
    lcd.setCursor(0,1);
    lcd.print(dataref + ":" + value);
  }
}
