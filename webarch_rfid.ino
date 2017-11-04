/*
* Arduino Door Lock Access Control Project
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: MFRC522, https://github.com/miguelbalboa/rfid
*/
#include <SPI.h>
#include <MFRC522.h>
//#include <LiquidCrystal.h>
#include <Servo.h>
#define RST_PIN   5
#define SS_PIN    53
int relay = 12;
byte readCard[4];
char* myTags[100] = {"147BA1EA","45291691","A373329F","CEDF9359","CE989459","B5AD7EB","2EA039E4",
"452CD1EB","FBED49CC","63F4EC1C","9526D1EB","8E993DE4","3DB7244C","95DFD6EB","634EEF1C",
"4D681D4C","C3B0F31C","102E5A39","9BB83ACC","432FDF1D", "5767C3DB"};

int tagsCount = 0;
String tagID = "";
boolean successRead = false;
boolean correctTag = false;
int proximitySensor;
boolean doorOpened = false;
// Create instances1
MFRC522 mfrc522(SS_PIN, RST_PIN);
//LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Parameters: (rs, enable, d4, d5, d6, d7)
Servo myServo; // Servo motor
void setup() {
  // Initiating
  SPI.begin();        // SPI bus
  Serial.begin(9600);
  pinMode(12 , OUTPUT);
  mfrc522.PCD_Init(); //  MFRC522
  //lcd.begin(16, 2);   // LCD screen
  myServo.attach(8);  // Servo motor
  myServo.write(10); // Initial lock position of the servo motor
  // Prints the initial message
  
  //Serial.println("-No Master Tag!-");
  Serial.println("    SCAN NOW");
  //lcd.print("-No Master Tag!-");
  //lcd.setCursor(0, 1);
  //lcd.print();
  // Waits until a master card is scanned
//  while (!successRead) {
//    successRead = getID();
//    if ( successRead == true) {
//      myTags[tagsCount] = strdup(tagID.c_str()); // Sets the master tag into position 0 in the array
//      Serial.println(myTags[tagsCount]);
//      //lcd.clear();
//      //lcd.setCursor(0, 0);
//      Serial.println("Master Tag Set!");
//      //lcd.print("Master Tag Set!");
//      tagsCount++;
//    }
//  }
//  successRead = false;
//  printNormalModeMessage();
}
void loop() {
  //int proximitySensor = analogRead(A0);
  // If door is closed...
  
    if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
      return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
      return;
    }
    tagID = "";
    // The MIFARE PICCs that we use have 4 byte UID
    for ( uint8_t i = 0; i < 4; i++) {  //
      readCard[i] = mfrc522.uid.uidByte[i];
      tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    tagID.toUpperCase();
    mfrc522.PICC_HaltA(); // Stop reading
    correctTag = false;
    // Checks whether the scanned tag is the master tag
//    if (tagID == myTags[0]) {
//      //lcd.clear();
//      Serial.println("Program mode:");
//      //lcd.print("Program mode:");
//      //lcd.setCursor(0, 1);
//      Serial.println("Add/Remove Tag");
//      //lcd.print("Add/Remove Tag");
//      while (!successRead) {
//        successRead = getID();
//        if ( successRead == true) {
//          for (int i = 0; i < 100; i++) {
//            if (tagID == myTags[i]) {
//              myTags[i] = "";
//              //lcd.clear();
//              //lcd.setCursor(0, 0);
//              Serial.println("  Tag Removed!");
//              //lcd.print("  Tag Removed!");
//              printNormalModeMessage();
//              return;
//            }
//          }
//          myTags[tagsCount] = strdup(tagID.c_str());
//          //lcd.clear();
//         // lcd.setCursor(0, 0);
//         Serial.println("   Tag Added!");
//          //lcd.print("   Tag Added!");
//          printNormalModeMessage();
//          tagsCount++;
//          return;
//        }
//      }
//    }
//    successRead = false;
    // Checks whether the scanned tag is authorized
    for (int i = 0; i < 100; i++) {
      if (tagID == myTags[i]) {
        //lcd.clear();
        //lcd.setCursor(0, 0);
        Serial.println(" Access Granted!");
        //lcd.print(" Access Granted!");
        //digitalWrite(12 , LOW);
        myServo.write(170); // Unlocks the door
        printNormalModeMessage();
        correctTag = true;
      }
    }
    if (correctTag == false) {
      //lcd.clear();
      //lcd.setCursor(0, 0);
      Serial.println(" Access Denied!");
      //lcd.print(" Access Denied!");
      printNormalModeMessage();
    }
  
  // If door is open...
  else {
    //lcd.clear();
    //lcd.setCursor(0, 0);
    Serial.println(" Door Opened!");
    //lcd.print(" Door Opened!");
    //while (!doorOpened) {
      //proximitySensor = analogRead(A0);
      //if (proximitySensor > 200) {
       // doorOpened = true;
      //}
    //}
    //doorOpened = false;
    delay(500);
    //digitalWrite(12 , HIGH);
    myServo.write(10); // Locks the door
    //printNormalModeMessage();
  }
}

uint8_t getID() {
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return 0;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) {  // The MIFARE PICCs that we use have 4 byte UID
    readCard[i] = mfrc522.uid.uidByte[i];
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return 1;
}
void printNormalModeMessage() {
  delay(1000);
  //lcd.clear();
  //Serial.println("-Access Control-");
  //lcd.print("-Access Control-");
  //lcd.setCursor(0, 1);
  Serial.println(" Scan Your Tag!");
  //lcd.print(" Scan Your Tag!");
}
