void leader() {
  light_lock = 0;
  if ((state == 3) || (state == 4)) {
    return;
  }
  if (wifiMulti.run() == WL_CONNECTED) {
    HTTPClient http;
    String callLeader_Str = callLeader;
    http.begin(callLeader_Str.c_str());
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      state = 1;
      callLeaderCountDown = millis();
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
    // line_report_gunstuck();
  } else {
    Serial.println("WiFi Disconnected");
  }
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
}

void material() {
  light_lock = 0;
  if ((state == 3) || (state == 4)) {
    return;
  }
  if (wifiMulti.run() == WL_CONNECTED) {
    HTTPClient http;
    String callMaterial_Str = callMaterial;
    http.begin(callMaterial_Str.c_str());
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      state = 2;
      callMaterialCountDown = millis();
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
    // line_report_overCement();
  } else {
    Serial.println("WiFi Disconnected");
  }
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
}
