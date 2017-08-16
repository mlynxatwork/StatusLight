

#include <FastLED.h>
#include <avr/io.h>

#define NUM_LEDS 11
#define DATAPIN 0

CRGBArray<NUM_LEDS> leds;
int buttonOne = A1;
int buttonTwo = A2;
int buttonThree = A3;
bool onePressed = 0;
bool twoPressed = 0;
bool threePressed = 0;
volatile int oneState = 0;
volatile int twoState = 0;
volatile int threeState = 0;
CHSV color = CHSV(96, 255, 255);


static inline void timerInit(void){
  TCCR1 |= (1 << CTC1);  //clear the timer on match
  TCCR1 |= (1 << CS12) | (1 << CS11) | (1 << CS13); //set the prescaler
  OCR1C = 50; //set the timing, i think this is 200uS
  TIMSK |= (1 << OCIE1A);
  //PRR |= (1 << PRTIM0);
  sei();
}



ISR(TIMER1_COMPA_vect){
  //leds(0, NUM_LEDS-1) = CRGB::Red;
  oneState = analogRead(buttonOne);
  if(oneState <= 900){ //it's no longer High
    if(onePressed){ //this is the second time we've read the button
      if(oneState <= 100){ //reading zero go green
        color.hue = 96;
        onePressed = 0;
      } else {
        color.hue = 160;
        onePressed = 0;
      }
    } else {
      onePressed = 1;
    }
  }
  
  twoState = analogRead(buttonTwo);
  if(twoState <= 900){ //it's no longer High
    if(twoPressed){ //this is the second time we've read the button
      if(twoState <= 100){ 
        color.hue = 0;
        twoPressed = 0;
      } else {
        color.hue = 50;
        twoPressed = 0;
      }
    } else {
      twoPressed = 1;
    }
  }

  threeState = analogRead(buttonThree);
  if(threeState <= 900){ //it's no longer High
    if(threePressed){ //this is the second time we've read the button
      if(threeState <= 100){ 
        color.hue = 128;
        threePressed = 0;
      } else {
        color.hue = 192;
        threePressed = 0;
      }
    } else {
      threePressed = 1;
      twoPressed, onePressed = 0;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<NEOPIXEL, DATAPIN>(leds, NUM_LEDS);
  
  pinMode(buttonOne, INPUT_PULLUP);
  pinMode(buttonTwo, INPUT_PULLUP);
  pinMode(buttonThree, INPUT_PULLUP);
  timerInit();

  leds(0, NUM_LEDS-1) = color; //Start off in green
}

void loop(){
  
  //just do a nice fade in and out. 
  for(int i = 50; i < 255; i++){
    leds(0, NUM_LEDS-1) = CHSV(color.hue, 255, i);
    FastLED.show();
    FastLED.delay(10);
  }

  for(int i = 255; i > 50; i--){
    leds(0, NUM_LEDS-1) = CHSV(color.hue, 255, i);
    FastLED.show();
    FastLED.delay(20);
  }

}
