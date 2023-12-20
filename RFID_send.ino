void send_rfid_callTPM() {
 
  if (wifiMulti.run() == WL_CONNECTED) {
    HTTPClient http;
    String rfid_Str = rfid;
    http.begin(rfid_Str.c_str());
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      state =3;
      EEPROM.write(0, state);
      EEPROM.commit();
      Serial.println("State saved in flash memory");
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void send_rfid_Receive() {
  //  if(countState>=2)
  //     {
  //       countState=1;
  //       EEPROM.write(1, countState);
  //       EEPROM.commit();
  //       Serial.println("reset countstate do bấm linh tinh");
  //     }
  if (wifiMulti.run() == WL_CONNECTED) {
    HTTPClient http;
    String rfid_Str = rfid;
    http.begin(rfid_Str.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      state =4;
      countState++;
      EEPROM.write(0, state);
      EEPROM.write(1, countState);
      EEPROM.commit();
      Serial.print("countState: ");
      Serial.println(countState);
      Serial.println("State saved in flash memory");
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
