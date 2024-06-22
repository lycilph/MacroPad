#include <HID-Project.h>
#include <Keypad.h>
#include <EncoderButton.h>
#include <LowPower.h>

// Layer led handling
const int numberOfLayers = 3;
const int layerLeds[numberOfLayers] = {4, 3, 2}; // These are the pins for the leds corresponding to the layer
int currentLayer = 0;

// Rotary Encoder setup
const int buttonPin = 19;
const int encoderPin1 = 21;
const int encoderPin2 = 20;
EncoderButton eb1(encoderPin1, encoderPin2, buttonPin);

// Keypad stuff
const byte ROWS = 3; // 3 rows
const byte COLS = 3; // 3 columns
char keys[ROWS][COLS] = {
  {'1','4','7'},
  {'2','5','8'},
  {'3','6','9'},
};
byte rowPins[ROWS] = {15, 14, 16}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
unsigned long keyHeldTime[LIST_MAX]; // Keep a list of how long each key is held

bool hasSlept = false;

void setup() {
  //Serial.begin(9600); // Debug stuff

  // Setup for leds (ie. turn the all off)
  for (int i = 0; i < numberOfLayers; i++){
    pinMode(layerLeds[i], OUTPUT);
    digitalWrite(layerLeds[i], LOW);
  }

  // Start "animation" of leds
  StartAnimation();

  // Set start layer
  currentLayer = 0;
  digitalWrite(layerLeds[currentLayer], HIGH);

  // Setup for encoder button
  eb1.setEncoderHandler(onEb1Encoder);
  eb1.setClickHandler(onEb1Click);
  eb1.setLongPressHandler(onEb1LongClick);

  // Sends a clean report to the host. This is important on any Arduino type.
  Keyboard.begin();
}

void StartAnimation() {
  int waitTime = 250;

  for (int i = 0; i < numberOfLayers; i++)
  {
    digitalWrite(layerLeds[i], HIGH);
    delay(waitTime);
  }

  for (int i = 0; i < numberOfLayers; i++)
  {
    digitalWrite(layerLeds[i], LOW);
    delay(waitTime);
  }

  for (int i = 0; i < numberOfLayers; i++)
  {
    digitalWrite(layerLeds[i], HIGH);
    delay(waitTime);
    digitalWrite(layerLeds[i], LOW);
    delay(waitTime);
  }
}


void onEb1Click(EncoderButton& eb) {
  Consumer.write(MEDIA_PLAY_PAUSE);
}

void onEb1LongClick(EncoderButton& eb) {
  currentLayer += 1;
  if (currentLayer >= numberOfLayers)
    currentLayer = 0;

  UpdateLayerLeds();
}

void onEb1Encoder(EncoderButton& eb) {
  int incr = eb.increment();
  if (incr > 0)
  {
    Consumer.write(MEDIA_VOLUME_DOWN);
  }
  else if (incr < 0)
  {
    Consumer.write(MEDIA_VOLUME_UP);
  }
}

void UpdateLayerLeds()
{
  TurnLayerLedsOff();

  // Turn the led for the current layer on
  digitalWrite(layerLeds[currentLayer], HIGH);
}

void TurnLayerLedsOff()
{
  // Turn all leds off
  for (int i = 0; i < numberOfLayers; i++)
  {
    digitalWrite(layerLeds[i], LOW);
  }
}

void loop() {
  if (USBDevice.isSuspended())
  {
    //Serial.println("Sleeping Now");
    //delay(100);

    TurnLayerLedsOff();
    LowPower.idle(SLEEP_8S, ADC_OFF, TIMER4_OFF, TIMER3_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART1_OFF, TWI_OFF, USB_OFF);
    hasSlept = true;

    //Serial.println("Woke up again");
  }

  if (hasSlept && USBDevice.isSuspended() == false)
  {
    hasSlept = false;
    UpdateLayerLeds();
  }

  // Call 'update' for every EncoderButton
  eb1.update();

  // 'Update' the keypad (and check for pressed keys)
  if (keypad.getKeys())
  {
    for (int i=0; i<LIST_MAX; i++)
    {
      if (keypad.key[i].stateChanged && keypad.key[i].kstate == PRESSED)
      {
        //Serial.println(keypad.key[i].kchar);
        HandleSingleKey(keypad.key[i].kchar);
      }
    }
  }
  // Check for held keys
  for (int i=0; i<LIST_MAX; i++)
  {
    if (keypad.key[i].kstate == HOLD && (millis() - keyHeldTime[i]) > 100)
    {
      //Serial.print(keypad.key[i].kchar);
      //Serial.println(" - held");
      HandleSingleKey(keypad.key[i].kchar);
      keyHeldTime[i] = millis();
    }
  }
}

void HandleSingleKey(char key)
{
  switch (currentLayer) {
    case 0:
      HandlerLayer0(key);
      break;
    case 1:
      HandlerLayer1(key);
      break;
    case 2:
      HandlerLayer2(key);
      break;
  }
}

void HandlerLayer0(char key)
{
  switch (key) {
    case '1':
      Keyboard.write(KEY_F13);
      break;
    case '2':
      Keyboard.write(KEY_F14);
      break;
    case '3':
      Keyboard.write(KEY_F15);
      break;
    case '4':
      Keyboard.write(KEY_F16);
      break;
    case '5':
      Keyboard.write(KEY_F17);
      break;
    case '6':
      Keyboard.write(KEY_F18);
      break;
    case '7':
      Keyboard.write(KEY_F19);
      break;
    case '8':
      Keyboard.write(KEY_F20);
      break;
    case '9':
      Keyboard.write(KEY_F21);
      break;
  }
}

void HandlerLayer1(char key) {
  switch (key) {
    case '1':
      Keyboard.write('q');
      break;
    case '2':
      Keyboard.write('w');
      break;
    case '3':
      Keyboard.write('e');
      break;
    case '4':
      Keyboard.write('a');
      break;
    case '5':
      Keyboard.write('s');
      break;
    case '6':
      Keyboard.write('d');
      break;
    case '7':
      Keyboard.write('z');
      break;
    case '8':
      Keyboard.write('x');
      break;
    case '9':
      Keyboard.write('x');
      break;
  }
}

void HandlerLayer2(char key) {
  switch (key) {
    case '1':
      Keyboard.press(KEY_LEFT_CTRL);  // press and hold crtl
      Keyboard.press('c');            // press and hold c
      Keyboard.releaseAll();          // release both
      break;
    case '2':
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('v');
      Keyboard.releaseAll();
      break;
    case '3':
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('a');
      Keyboard.releaseAll();
      break;
    case '4':
      Keyboard.write(KEY_PAGE_UP);
      break;
    case '5':
      Keyboard.write(KEY_UP_ARROW);
      break;
    case '6':
      Keyboard.write(KEY_PAGE_DOWN);
      break;
    case '7':
      Keyboard.write(KEY_LEFT_ARROW);
      break;
    case '8':
      Keyboard.write(KEY_DOWN_ARROW);
      break;
    case '9':
      Keyboard.write(KEY_RIGHT_ARROW);
      break;
  }
}