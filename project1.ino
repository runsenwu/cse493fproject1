/*
    By Runsen Wu

    With code and help from resources such as:
      TheGeekPub.com
      https://www.thegeekpub.com/arduino-tutorials/
      Prof. Jon Froehlich
      @jonfroehlich
      http://makeabilitylab.io

      For a walkthrough and circuit diagram, see:
      https://makeabilitylab.github.io/physcomp/arduino/rgb-led-fade

      Based, in part, on:
      - https://learn.adafruit.com/adafruit-arduino-lesson-3-rgb-leds?view=all
      - https://gist.github.com/jamesotron/766994 (link no longer available)
*/

const int BUTTON_PIN = 8;  // Connect the Button to pin 7 or change here
const int RED_LED_PIN    = 6;       // Connect the LED to pin 3 or change here
const int BLUE_LED_PIN   = 5;
const int GREEN_LED_PIN  = 3;
const int INPUT_PHOTORESISTOR_PIN = A0;
const int INPUT_RESISTOR = A1;

const int MAX_COLOR_VALUE = 255;
const int DELAY_MS = 20; // delay in ms between changing colors

  // variables will change:
int ledState = LOW;        // tracks the current state of LED
int lastButtonState;        // the previous state of button
int currentButtonState; // the current state of button
int mode;
int _rgbLedValues[] = {255, 0, 0}; // Red, Green, Blue

enum RGB{
  RED,
  GREEN,
  BLUE,
  NUM_COLORS
};
enum RGB _curFadingUpColor = GREEN;
enum RGB _curFadingDownColor = RED;
const int FADE_STEP = 5;

void setup() {
  Serial.begin(9600);                         // initialize serial
  pinMode(BUTTON_PIN, INPUT);  // set arduino pin to input mode
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  mode = 0;
  currentButtonState = digitalRead(BUTTON_PIN);
}

void loop() {
  lastButtonState    = currentButtonState;                // save the last state
  currentButtonState = digitalRead(BUTTON_PIN); // read new state

  int inputresistor = analogRead(INPUT_RESISTOR);

  int photoresistorVal = analogRead(INPUT_PHOTORESISTOR_PIN); // read in photoresistor val
  int constraint = constrain(photoresistorVal, 200, 900);
  int ledVal = map(photoresistorVal, 225, 900, 0, 255); // inverse relationship

  if(lastButtonState == HIGH && currentButtonState == LOW) {
    Serial.println("The button is pressed: ");

    mode = (mode + 1) % 3;
    Serial.print("mode: ");
    Serial.println(mode);
  }

  // analogWrite(OUTPUT_LED_PIN, ledVal);

    // toggle state of LED
  if(mode == 0) {
    ledState = 0;
    setColor(ledState, ledState, ledState);
    Serial.println("Turning LED Off in mode 0");
  }
  else if (mode == 1) {
    Serial.println("mode 1");
    _rgbLedValues[_curFadingUpColor] += FADE_STEP;
    _rgbLedValues[_curFadingDownColor] -= FADE_STEP;

    // Check to see if we've reached our maximum color value for fading up
    // If so, go to the next fade up color (we go from RED to GREEN to BLUE
    // as specified by the RGB enum)
    // This fade code partially based on: https://gist.github.com/jamesotron/766994
    if(_rgbLedValues[_curFadingUpColor] > MAX_COLOR_VALUE){
      _rgbLedValues[_curFadingUpColor] = MAX_COLOR_VALUE;
      _curFadingUpColor = (RGB)((int)_curFadingUpColor + 1);

      if(_curFadingUpColor > (int)BLUE){
        _curFadingUpColor = RED;
      }
    }

    if(_rgbLedValues[_curFadingDownColor] < 0){
      _rgbLedValues[_curFadingDownColor] = 0;
      _curFadingDownColor = (RGB)((int)_curFadingDownColor + 1);

      if(_curFadingDownColor > (int)BLUE){
        _curFadingDownColor = RED;
      }
    }

    // Set the color and then delay
    double proportion = (double) ledVal / 255.0;

    setColor(max((proportion * _rgbLedValues[RED]), 0), max((proportion * _rgbLedValues[GREEN]), 0),
            max((proportion * _rgbLedValues[BLUE]), 0));
    delay(DELAY_MS);
    // rgc blinking with photo resister state
  } else {
    Serial.println(inputresistor);
    // delay(1000);
    if (inputresistor < 250) {
      setColor(0, 255, 0);
    } else if (inputresistor < 1010) {
      setColor(0, 0, 255);
    } else {
      setColor(255, 0, 0);
    }
    Serial.println("Turrning half the LED on in mode 2");
  }
}

void setColor(int red, int green, int blue)
{
  analogWrite(RED_LED_PIN, red);
  analogWrite(GREEN_LED_PIN, green);
  analogWrite(BLUE_LED_PIN, blue);
}
