#include <Wire.h>
#include <U8g2lib.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

// ────────── Display Setup ──────────
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// ────────── RFID Setup ──────────
MFRC522DriverPinSimple ss_pin(5);  // SS (SDA) connected to GPIO 5
MFRC522DriverSPI driver{ss_pin};  // No RST pin required
MFRC522 mfrc522{driver};

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println(F("--- RFID + OLED Test ---"));

  // Init RFID
  Serial.println(F("Initializing RFID reader..."));
  mfrc522.PCD_Init();
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);
  Serial.println(F("RFID ready."));

  // Init OLED
  Serial.println(F("Initializing OLED..."));
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.clearBuffer();
  u8g2.drawStr(0, 15, "RFID + OLED Ready");
  u8g2.sendBuffer();
  delay(1000);
}

void loop() {
  Serial.println(F("Polling for card..."));

  if (!mfrc522.PICC_IsNewCardPresent()) {
    Serial.println(F("  No card."));
    delay(200);
    return;
  }

  Serial.println(F("  Card detected!"));

  if (!mfrc522.PICC_ReadCardSerial()) {
    Serial.println(F("  ERROR: Could not read card serial."));
    delay(500);
    return;
  }

  Serial.println(F("  Card read OK."));

  // Construct UID string
  String uidString;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uidString += "0";
    uidString += String(mfrc522.uid.uidByte[i], HEX);
  }
  uidString.toUpperCase();

  Serial.print(F("Card UID: "));
  Serial.println(uidString);

  // Show on OLED
  u8g2.clearBuffer();
  u8g2.drawStr(0, 15, "RFID UID:");
  u8g2.drawStr(0, 35, uidString.c_str());
  u8g2.sendBuffer();

  delay(2000);
}
