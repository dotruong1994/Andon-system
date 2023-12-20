void dencanbo() {
  light_lock = 1;
  for (int i = 0; i < 7; i++) {
    leds[i] = CRGB(0, 0, 255);
    FastLED.show();
  }
  Serial.println("den can bo");
}
void denbaotri() {
  light_lock = 1;
  for (int i = 0; i < 7; i++) {
    leds[i] = CRGB(0, 255, 20);
    FastLED.show();
  }
  Serial.println("den bao tri");
}
void denhoatdong() {
  light_lock = 1;
  for (int i = 0; i < 7; i++) {
    leds[i] = CRGB(236, 236, 236);
    FastLED.show();
  }
  Serial.println("den hoat dong");
}
void den_nhan_don() {
  light_lock = 1;
  for (int i = 0; i < 7; i++) {
    leds[i] = CRGB(255, 0, 0);
    FastLED.show();
  }
  Serial.println("den nhan don");
}
void denhetlieu() {
  light_lock = 1;
  for (int i = 0; i < 7; i++) {
    leds[i] = CRGB(249, 244, 0);
    FastLED.show();
  } 
  Serial.println("den het lieu");
}

void den_trieu_hoi_thanh_dong() {
  light_lock = 1;
  for (int i = 0; i < 7; i++) {
    leds[i] = CRGB(0, 128, 128);
    FastLED.show();
  }
  Serial.println("den wifi");
}
