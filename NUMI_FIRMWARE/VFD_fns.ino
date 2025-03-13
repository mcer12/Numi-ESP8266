
void initScreen() {

  bri = json["bri"].as<int>();
  crossFadeTime = json["fade"].as<int>();
  //setupCrossFade();
  //aw210xx.begin(ADDR_AW, I2C_SPEED, FREQ_2K, true); // Address, I2C speed in hz, pwm frequency, phase shift enable/disable
  aw210xx.begin();
  aw210xx.setFreq(FREQ_62K); // set frequency, see accepted values in the comment above setup()

  for (int ii = 0; ii < 36; ii++) {
    aw210xx.setCol(ii, 255);
    aw210xx.setBri(ii, 0);
  }
  aw210xx.setGlobalBri(255); // global brightness setting, accepts values 0-255
  aw210xx.update();

  uint8_t verValue = aw210xx.readRegister(REG_RESET);
  Serial.print("Chip id: ");
  Serial.println(verValue, HEX);

  //ITimer.attachInterruptInterval(TIMER_INTERVAL_uS, TimerHandler);
  // 30ms slow 240ms, 20 => medium 160ms, 15 => quick 120ms

  blankAllDigits();
}

void enableScreen() {
  //ITimer.attachInterruptInterval(TIMER_INTERVAL_uS, TimerHandler);
}
void disableScreen() {
  //ITimer.detachInterrupt();
}

/*
  void setupCrossFade() {
  if (crossFadeTime > 0) {
    fade_animation_ticker.attach_ms(crossFadeTime, handleFade); // handle dimming animation
  } else {
    fade_animation_ticker.attach_ms(20, handleFade); // handle dimming animation
  }
  }
  void handleFade() {
  for (int i = 0; i < registersCount; i++) {
    for (int ii = 0; ii < segmentCount; ii++) {
      if (targetBrightness[i][ii] > bri_vals_separate[bri][i]) targetBrightness[i][ii] = bri_vals_separate[bri][i];

      if (crossFadeTime > 0) {
        if (targetBrightness[i][ii] > segmentBrightness[i][ii]) {
          segmentBrightness[i][ii] += bri_vals_separate[bri][i] / dimmingSteps;
          if (segmentBrightness[i][ii] > bri_vals_separate[bri][i]) segmentBrightness[i][ii] = bri_vals_separate[bri][i];
        } else if (targetBrightness[i][ii] < segmentBrightness[i][ii]) {
          segmentBrightness[i][ii] -= bri_vals_separate[bri][i] / dimmingSteps;
          if (segmentBrightness[i][ii] < 0) segmentBrightness[i][ii] = 0;
        }
      } else {
        segmentBrightness[i][ii] = targetBrightness[i][ii];
      }
    }
  }
  aw210xx.update();

  }
*/

void handleColon() {
  // 0 = off, 1 = always on, 2 = ON/OFF each second, 3 = always on with gradient
  if (json["colon"].as<int>() == 0) {
    aw210xx.setBri(colonPins[0], 0);
    aw210xx.setBri(colonPins[1], 0);
    aw210xx.update();
    return;
  } else if (json["colon"].as<int>() == 1 || json["colon"].as<int>() == 3) {
    aw210xx.setBri(colonPins[0], bri_vals_colon[bri]);
    aw210xx.setBri(colonPins[1], bri_vals_colon[bri]);
    aw210xx.update();
    return;
  } else if (json["colon"].as<int>() == 2) {
    if (toggleSeconds) {
      aw210xx.setBri(colonPins[0], bri_vals_colon[bri]);
      aw210xx.setBri(colonPins[1], bri_vals_colon[bri]);
    }
    else {
      aw210xx.setBri(colonPins[0], 0);
      aw210xx.setBri(colonPins[1], 0);
    }
    toggleSeconds = !toggleSeconds;
    aw210xx.update();
    return;
  }
}

void setDigit(uint8_t digit, uint8_t value) {
  draw(digit, numbers[value]);
}

void setAllDigitsTo(uint16_t value) {
  for (int i = 0; i < registersCount; i++) {
    setDigit(i, value);
  }
}

void blankDigit(uint8_t digit) {
  for (int i = 0; i < segmentCount; i++) {
    aw210xx.setBri(digitPins[digit][i], 0);
  }
  aw210xx.update();
}

void blankAllDigits() {
  for (int i = 0; i < registersCount; i++) {
    blankDigit(i);
  }
}

void setDot(uint8_t digit, bool enable) {

}

void draw(uint8_t digit, uint8_t value[segmentCount]) {
  for (int i = 0; i < segmentCount; i++) {
    if (value[i] == 1) {
      //targetBrightness[digit][i] = bri_vals_separate[bri][digit];
      aw210xx.setBri(digitPins[digit][i], bri_vals_separate[bri][digit]);

    } else {
      //targetBrightness[digit][i] = 0;
      aw210xx.setBri(digitPins[digit][i], 0);
    }
  }
  aw210xx.update();

}

void showTime() {

  int hours = hour();
  if (hours > 12 && json["t_format"].as<int>() == 0) { // 12 / 24 h format
    hours -= 12;
  } else if (hours == 0 && json["t_format"].as<int>() == 0) {
    hours = 12;
  }

  int splitTime[6] = {
    (hours / 10) % 10,
    hours % 10,
    (minute() / 10) % 10,
    minute() % 10,
    (second() / 10) % 10,
    second() % 10,
  };

  for (int i = 0; i < registersCount ; i++) {
    if (i == 0 && splitTime[0] == 0 && json["zero"].as<int>() == 0) {
      blankDigit(i);
      continue;
    }
    setDigit(i, splitTime[i]);
  }

}

void cycleDigits() {
  for (int i = 0; i < 10; i++) {
    for (int ii = 0; ii < registersCount; ii++) {
      setDigit(ii, i);
    }
    delay(1000);
  }
}

void showIP() {
  IPAddress ip_addr = WiFi.localIP();

  if (registersCount < 6) {
    blankDigit(0);
    if ((ip_addr[3] / 100) % 10 == 0) {
      blankDigit(1);
    } else {
      setDigit(1, (ip_addr[3] / 100) % 10);
    }
    if ((ip_addr[3] / 10) % 10 == 0) {
      blankDigit(2);
    } else {
      setDigit(2, (ip_addr[3] / 10) % 10);
    }
    setDigit(3, (ip_addr[3]) % 10);
  } else {
    setDigit(0, 1);
    draw(1, letter_p);
    blankDigit(2);
    if ((ip_addr[3] / 100) % 10 == 0) {
      blankDigit(3);
    } else {
      setDigit(3, (ip_addr[3] / 100) % 10);
    }
    if ((ip_addr[3] / 10) % 10 == 0 && (ip_addr[3] / 100) % 10 == 0) {
      blankDigit(4);
    } else {
      setDigit(4, (ip_addr[3] / 10) % 10);
    }
    setDigit(5, (ip_addr[3]) % 10);
  }


}

void toggleNightMode() {
  if (json["nmode"].as<int>() == 0) return;
  if (hour() >= 22 || hour() <= 6) {
    bri = 0;
  } else {
    bri = json["bri"].as<int>();
  }
}
