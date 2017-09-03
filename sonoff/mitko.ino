void do_cmnd_identify()
{
  IPAddress ip = WiFi.localIP();
  int digit; // ones, tens, hundreds
  
  digitalWrite(LED_PIN, (LED_INVERTED) ? 1 : 0);
  delay(600); // just in case - make it more visible

  if (ip[3] > 100) {
    digit = ip[3] / 100;
    blinkCount(digit);
    delay(1600); // 1600 + 400 = 2000 = more than enough
  }

  if (ip[3] > 10) {
    digit = ip[3] % 100 / 10;
    blinkCount(digit);
    delay(1600); // 1600 + 400 = 2000 = more than enough
  }
  
  digit = ip[3] % 10;
  blinkCount(digit);
  delay(400);
  
  // now return it back (don't know if it necessary, but just in case)
  if (sysCfg.ledstate &0x01) {
      digitalWrite(LED_PIN, (LED_INVERTED) ? !power : power);
  }
}

void blinkCount(int count) {
  //Serial.println(count);
  int i;
  for (i = 1; i <= count; i++) {
    delay(400);
    digitalWrite(LED_PIN, (LED_INVERTED) ? 0 : 1);
    delay(200);
    digitalWrite(LED_PIN, (LED_INVERTED) ? 1 : 0);
  }
}

