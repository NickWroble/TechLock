/*---------------------------------------- RFID -----------------------------------------------------------------------
   --------------------------------------------------------------------------------------------------------------------
   Example sketch/program showing how to read data from a PICC to serial.
   --------------------------------------------------------------------------------------------------------------------
   This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid

   Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
   Reader on the Arduino SPI interface.

   When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
   then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
   you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
   will show the ID/UID, type and any data blocks it can read. Note: you may see "Timeout in communication" messages
   when removing the PICC from reading distance too early.

   If your reader supports it, this sketch/program will read all the PICCs presented (that is: multiple tag reading).
   So if you stack two or more PICCs on top of each other and present them to the reader, it will first output all
   details of the first and then the next PICC. Note that this may take some time as all data blocks are dumped, so
   keep the PICCs at reading distance until complete.

   @license Released into the public domain.

   Typical pin layout used:
   -----------------------------------------------------------------------------------------
               MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
               Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
   Signal      Pin          Pin           Pin       Pin        Pin              Pin
   -----------------------------------------------------------------------------------------
   RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
   SPI SS      SDA(SS)      10            53        D10        10               10
   SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
   SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
   SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
*/

#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
char Nick;
char Michael;


/*------------------------------------------ FINGERPRINT --------------------------------------------------------*/

// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// +5V connect  sensor (Red wire)
// GND connect sensor (Black wire)

#include <Dyrobot_Fingerprint.h>
#include <SoftwareSerial.h>
int p;
int getFingerprintIDez();
SoftwareSerial mySerial(2, 3);
Dyrobot_Fingerprint finger = Dyrobot_Fingerprint(&mySerial);


/*------------------------------------------- SERVO ------------------------------------------------------------*/


#include <Servo.h>
Servo myservo;


/*---------------------------------------- SETUP ---------------------------------------------------------*/

void setup() {
  Serial.begin(9600);		// Initialize serial communications with the PC
  while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  SPI.begin();			// Init SPI bus
  mfrc522.PCD_Init();		// Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  Serial.println(" ");

  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
  Serial.println("Waiting for valid finger...");

  c);   //Initializes the servo
  myservo.write(100);  //Sets the servo to the locking positiom
}


/*---------------------------------------- LOOP ------------------------------------------------------*/


void loop() {

  if (! mfrc522.PICC_IsNewCardPresent()) {   //Look for new cards
    return;
  }

  if (! mfrc522.PICC_ReadCardSerial()) {     // Select one of the cards
    return;
  }

  if ((cardTest() == 0   &&   (getFingerprintIDez() >= 0 && getFingerprintIDez() <= 10)) ||
      (cardTest() == 1   &&   (getFingerprintIDez() >= 11 && getFingerprintIDez() <= 20))) {
    myservo.write(0);
    delay(5000);
    while (analogRead(A0) > 400) {
    }
    delay(2000);
    myservo.write(100);
  }
}


/*----------------------------------------------- FUNCTIONS -------------------------------------------------*/



//checks each byte of each card for a matching reference card
int cardTest() {
  int bytes[10];
  if (
    //Nick's Card
    (mfrc522.uid.uidByte[0] == 4 ) &&
    (mfrc522.uid.uidByte[1] == 74) &&
    (mfrc522.uid.uidByte[2] == 24) &&
    (mfrc522.uid.uidByte[3] == 154) &&
    (mfrc522.uid.uidByte[4] == 99) &&
    (mfrc522.uid.uidByte[5] == 94) &&
    (mfrc522.uid.uidByte[6] == 128) &&
    (mfrc522.uid.uidByte[7] == 0) &&
    (mfrc522.uid.uidByte[8] == 0) &&
    (mfrc522.uid.uidByte[9] == 0)) {
    return 0;
  } else if ((mfrc522.uid.uidByte[0] == 4 ) &&
             (mfrc522.uid.uidByte[1] == 35) &&
             (mfrc522.uid.uidByte[2] == 86) &&
             (mfrc522.uid.uidByte[3] == 154) &&
             (mfrc522.uid.uidByte[4] == 99) &&
             (mfrc522.uid.uidByte[5] == 94) &&
             (mfrc522.uid.uidByte[6] == 128) &&
             (mfrc522.uid.uidByte[7] == 0) &&
             (mfrc522.uid.uidByte[8] == 0) &&
             (mfrc522.uid.uidByte[9] == 0)) {
    return 1;
  }
  else {
    return -1;
  }
}


//Checks for a matching fingerprint
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
