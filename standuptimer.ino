#include <SimpleTimer.h>
#include <FastLED.h>

#define COUNTDOWN_SECONDS 60


#define LEDS_COUNT 30
#define TURNOFF_LONGPRESS_MILLIS 2000
#define AUTO_OFF_AFTER_TIMEOUT 30000

#define BUTTON_PIN 8
#define DATA_PIN 14
#define CLOCK_PIN 15

#define OFF_STATE 0
#define COUNTDOWN_STATE 1
#define TIMEOUT_STATE 2
byte state = OFF_STATE;

CRGB leds[LEDS_COUNT];
SimpleTimer timer;
int countdownTimerId = -1;
int countdownValue = COUNTDOWN_SECONDS;

void countdownTick() {
  countdownValue--;
  if (countdownValue < 0) {
    startTimeout();
    timer.deleteTimer(countdownTimerId);
  }
}

void turnOff() {
  state = OFF_STATE;
  Serial.println("Turned off");
}

void startTimeout() {
  state = TIMEOUT_STATE;
  timer.setTimeout(AUTO_OFF_AFTER_TIMEOUT, turnOff);
  Serial.println("Timeouted");
}

void startCountdown() {
  state = COUNTDOWN_STATE;
  timer.deleteTimer(countdownTimerId);
  countdownTimerId = timer.setInterval(1000, countdownTick);
  countdownValue = COUNTDOWN_SECONDS;
  Serial.println("Countdown started");
}

void offLoop() {
  for(int dot = 0; dot < LEDS_COUNT; dot++) { 
    leds[dot] = CRGB::Black;    
  }
  FastLED.show();
}

void countdownLoop() {
  fill_gradient_RGB(leds, 0, CRGB::Green, LEDS_COUNT-1, CRGB::Red);
  int progress = round((countdownValue / (double)COUNTDOWN_SECONDS) * (LEDS_COUNT));
  for(int dot = 0; dot < LEDS_COUNT - progress; dot++) { 
    leds[dot] = CRGB::Black;
  }
  FastLED.show();
}

void timeoutLoop() {
}

bool lastBlink = false;
void timeoutBlink() {
  if (state != TIMEOUT_STATE) {
    return;
  }
  
  CRGB color = CRGB::Red;
  if (lastBlink) {
    color = CRGB::Black;
  }
  
  for(int dot = 0; dot < LEDS_COUNT; dot++) { 
    leds[dot] = color;
  }
  FastLED.show();
  lastBlink = !lastBlink;
}

unsigned long lastPressed = 0;
void handleButton() {
  bool buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == LOW) {
    buttonFeedback();   
    if (lastPressed == 0) {
      lastPressed = millis();  
    }
  }

  if (buttonState == HIGH && lastPressed > 0) {
    unsigned long delta = millis() - lastPressed;
    if (delta > TURNOFF_LONGPRESS_MILLIS) {
      turnOff();
    } else {
      startCountdown();
    }
    lastPressed = 0;
  }
}

void buttonFeedback() {
  for(int dot = 0; dot < LEDS_COUNT; dot++) { 
    leds[dot] = (dot % 2 == 0 ? CRGB::OrangeRed : CRGB::Black);
  }
  FastLED.show();
}

void setup() { 
  Serial.begin(9600);
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, LEDS_COUNT);
  timer.setInterval(300, timeoutBlink);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  timer.run();
  switch(state) {
    case OFF_STATE:
      offLoop(); 
      break; 
    case COUNTDOWN_STATE: 
      countdownLoop();
      break;
    case TIMEOUT_STATE: 
      timeoutLoop();
      break;
  }
  handleButton(); 
}
