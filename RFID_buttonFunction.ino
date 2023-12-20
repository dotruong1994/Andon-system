void check_RFID() {
  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      checkLEDVal = 1;
    }
  } else {
    checkLEDVal = 0;
  }
}


void call_tpm() {
  light_lock = 0;
  Serial.println("Got it");
  if (!mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("RFID read");
    return;
  }
  // Select a card
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  mfrc522.PICC_HaltA();       // halt PICC
  mfrc522.PCD_StopCrypto1();  // stop encryption on PCD
  Serial.println(content);
  digitalWrite(LED_PIN, HIGH);
  vTaskDelay(100);
  digitalWrite(LED_PIN, LOW);
  vTaskDelay(100);
  digitalWrite(LED_PIN, HIGH);
  vTaskDelay(100);
  digitalWrite(LED_PIN, LOW);
  vTaskDelay(100);
  digitalWrite(LED_PIN, HIGH);
  vTaskDelay(100);
  digitalWrite(LED_PIN, LOW);
  rfid = "http://172.21.143.74:8080/cmmsservice/repairCheckList/up/VY/0:" + ID + ":" + content + "";
  send_rfid_callTPM();
}


void receive_order() {
  light_lock = 0;
  Serial.println("Got it");
  if (!mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("RFID read");
    return;
  }
  // Select a card
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  mfrc522.PICC_HaltA();       // halt PICC
  mfrc522.PCD_StopCrypto1();  // stop encryption on PCD
  Serial.println(content);
  digitalWrite(LED_PIN, HIGH);
  vTaskDelay(100);
  digitalWrite(LED_PIN, LOW);
  vTaskDelay(100);
  digitalWrite(LED_PIN, HIGH);
  vTaskDelay(100);
  digitalWrite(LED_PIN, LOW);
  vTaskDelay(100);
  digitalWrite(LED_PIN, HIGH);
  vTaskDelay(100);
  digitalWrite(LED_PIN, LOW);
  rfid = "http://172.21.143.74:8080/cmmsservice/repairCheckList/up/VY/2:" + ID + ":" + content + "";
  send_rfid_Receive();
}


