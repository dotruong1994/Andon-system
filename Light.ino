void dencanbo() {
  for (int i = 0; i < 7; i++) {
    leds[i] = CRGB(0, 0, 255);
    FastLED.show();
  }
}
void denbaotri() {
  for (int i = 0; i < 7; i++) {
    leds[i] = CRGB(0, 255, 20);
    FastLED.show();
  }
}
void denhoatdong() {
  for (int i = 0; i < 7; i++) {
    leds[i] = CRGB(236, 236, 236);
    FastLED.show();
  }
}
void den_nhan_don() {
  for (int i = 0; i < 7; i++) {
    leds[i] = CRGB(255, 0, 0);
    FastLED.show();
  }
}
void denhetlieu() {
  for (int i = 0; i < 7; i++) {
    leds[i] = CRGB(249, 244, 0);
    FastLED.show();
  } 
}

void den_trieu_hoi_thanh_dong() {
  for (int i = 0; i < 7; i++) {
    leds[i] = CRGB(0, 128, 128);
    FastLED.show();
  }
}