#include <MFRC522.h>

#define RST_PIN         9               // Configurable, see typical pin layout above
#define SS_PIN          10              // Configurable, see typical pin layout above
#define MISO_PIN        12              // Configurable, see typical pin layout above
#define MOSI_PIN        11              // Configurable, see typical pin layout above
#define SCK_PIN         13              // Configurable, see typical pin layout above
#define SDA_PIN         10              // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for serial to initialize
  Serial.println("RFID reader");
  mfrc522.PCD_Init(); // Initialize MFRC522
}

void loop() {

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Check if the card is a Mifare Classic card
  if (mfrc522.PICC_GetType(mfrc522.uid.sak) != MFRC522::PICC_TYPE_MIFARE_MINI &&  
      mfrc522.PICC_GetType(mfrc522.uid.sak) != MFRC522::PICC_TYPE_MIFARE_1K &&
      mfrc522.PICC_GetType(mfrc522.uid.sak) != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println("Card is not a Mifare Classic card");
    return;
  }

  // Read the block containing the credit card number and expiration date
  byte blockData[18];
  byte block = 4; // The block containing the credit card number and expiration date
MFRC522::StatusCode status = mfrc522.MIFARE_Read(block, blockData, sizeof(blockData));
  if (status != MFRC522::STATUS_OK) {
    Serial.println("Failed to read block");
    return;
  }

  // Extract the credit card number and expiration date from the block data
  String cardNumber = "";
  for (int i = 0; i < 10; i++) {
    cardNumber += (char)(blockData[i] / 16 + '0');
    cardNumber += (char)(blockData[i] % 16 + '0');
  }

  // Convert the expiration date bytes to a string in the format "MM/YY"
  String expirationDate = "";
  expirationDate += (char)(blockData[12] / 16 + '0');
  expirationDate += (char)(blockData[12] % 16 + '0');
  expirationDate += '/';
  expirationDate += (char)(blockData[13] / 16 + '0');
  expirationDate += (char)(blockData[13] % 16 + '0');

  // Print the credit card number and expiration date
  // Print the credit card number and expiration date
  Serial.println("Card number: " + cardNumber);
  Serial.println("Expiration date: " + expirationDate);
}

