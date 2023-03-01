#define USE_SERIAL
#define USE_AUTO_BRIGHTNESS
#define CLOCK_MODE

#include <Adafruit_NeoPixel.h>
#include "clock.h"

#ifdef USE_AUTO_BRIGHTNESS
const int luxSensorPin = A0;
// float light;
int luxValue;
#endif  //USE_AUTO_BRIGHTNESS

#ifdef CLOCK_MODE
#define DIGITAL_CLOCK
//#define RING_CLOCK

#ifdef DIGITAL_CLOCK
#define DIGIT_CLOCK 4 // Choose 4 or 6
#define ledSegm     1
#define modeSegment 2
#define USE_COLON

uint8_t digitPos_0, digitPos_1, digitPos_2, digitPos_3;
#ifdef USE_COLON
uint8_t colonPos_0;
#endif
#if (DIGIT_CLOCK == 6)
uint16_t digitPos_4, digitPos_5;
#ifdef USE_COLON
uint8_t colonPos_1;
#endif
#endif

bool onOff = true;

/*  PINDAH DI CLOCK.H
uint8_t numbers[40][7] = {
  {1, 1, 1, 0, 1, 1, 1},      // 0
  {0, 0, 1, 0, 0, 0, 1},      // 1
  {0, 1, 1, 1, 1, 1, 0},      // 2
  {0, 1, 1, 1, 0, 1, 1},      // 3
  {1, 0, 1, 1, 0, 0, 1},      // 4
  {1, 1, 0, 1, 0, 1, 1},      // 5
  {1, 1, 0, 1, 1, 1, 1},      // 6
  {0, 1, 1, 0, 0, 0, 1},      // 7
  {1, 1, 1, 1, 1, 1, 1},      // 8
  {1, 1, 1, 1, 0, 1, 1},      // 9
  {0, 0, 0, 0, 0, 0, 0},      // off
  {1, 1, 1, 1, 0, 0, 0},      // degrees symbol
  {1, 1, 0, 0, 1, 1, 0},      // C(elcius)
  {1, 1, 0, 1, 1, 0, 0},      // F(ahrenheit)
  {0, 1, 1, 1, 1, 1, 1},      // a
  {1, 0, 0, 1, 1, 1, 1},      // b
  {0, 0, 0, 1, 1, 1, 0},      // c
  {0, 0, 1, 1, 1, 1, 1},      // d
  {1, 1, 0, 1, 1, 1, 0},      // e
  {1, 1, 0, 1, 1, 0, 0},      // f
  {1, 1, 0, 0, 1, 1, 1},      // g
  {1, 0, 1, 1, 1, 0, 1},      // h
  {0, 1, 0, 0, 1, 0, 0},      // i
  {0, 1, 0, 0, 0, 1, 1},      // j
  {1, 1, 0, 1, 1, 0, 1},      // k
  {1, 0, 0, 0, 1, 1, 0},      // l
  {0, 1, 0, 1, 1, 0, 1},      // m
  {0, 0, 0, 1, 1, 0, 1},      // n
  {0, 0, 0, 1, 1, 1, 1},      // o
  {1, 1, 1, 1, 1, 0, 0},      // p
  {1, 1, 1, 1, 0, 0, 1},      // q
  {0, 0, 0, 1, 1, 0, 0},      // r
  {1, 1, 0, 0, 0, 1, 1},      // s
  {1, 0, 0, 1, 1, 1, 0},      // t
  {0, 0, 0, 0, 1, 1, 1},      // u
  {1, 0, 1, 0, 0, 1, 0},      // v
  {1, 0, 1, 1, 0, 1, 0},      // w
  {0, 0, 0, 0, 1, 0, 1},      // x
  {1, 0, 1, 1, 0, 1, 1},      // y
  {0, 1, 1, 0, 1, 1, 0},      // z
}; */

//    MODE == 1                MODE == 2                MODE == 3                MODE == 4
//       B               //       B               //       F               //       A  
//      ----             //      ----             //      ----             //      ----  
// A  |      |  C        // A  |      |  C        // E  |      |  G        // F  |      |  B 
//    |      |           //    |      |           //    |      |           //    |      |  
//      ---- G           //      ---- D           //      ---- D           //      ---- G  
//    |      |           //    |      |           //    |      |           //    |      |  
// F  |      |  D        // E  |      |  G        // A  |      |  C        // E  |      |  C 
//      ----             //      ----             //      ----             //      ----  
//       E               //       F               //       B               //       D  
//                        F A B G E D C               F A B G E D C               F A B G E D C
//                    0 = 1 1 1 0 1 1 1           a = 0 1 1 1 1 1 1           n = 0 0 0 1 1 0 1
//       A            1 = 0 0 1 0 0 0 1           b = 1 0 0 1 1 1 1           o = 0 0 0 1 1 1 1
//      ----          2 = 0 1 1 1 1 1 0           c = 0 0 0 1 1 1 0           p = 1 1 1 1 1 0 0
// F  |      |  B     3 = 0 1 1 1 0 1 1           d = 0 0 1 1 1 1 1           q = 1 1 1 1 0 0 1
//    |      |        4 = 1 0 1 1 0 0 1           e = 1 1 0 1 1 1 0           r = 0 0 0 1 1 0 0
//      ---- G        5 = 1 1 0 1 0 1 1           f = 1 1 0 1 1 0 0           s = 1 1 0 0 0 1 1
//    |      |        6 = 1 1 0 1 1 1 1           g = 1 1 0 0 1 1 1           t = 1 0 0 1 1 1 0
// E  |      |  C     7 = 0 1 1 0 0 0 1           h = 1 0 1 1 1 0 1           u = 0 0 0 0 1 1 1
//      ----          8 = 1 1 1 1 1 1 1           i = 0 1 0 0 1 0 0           v = 1 0 1 0 0 1 0
//       D            9 = 1 1 1 1 0 1 1           j = 0 1 0 0 0 1 1           w = 1 0 1 1 0 1 0
//                  off = 0 0 0 0 0 0 0           k = 1 1 0 1 1 0 1           x = 0 0 0 0 1 0 1
//               degree = 1 1 1 1 0 0 0           l = 1 0 0 0 1 1 0           y = 1 0 1 1 0 1 1
//            C(elcius) = 1 1 0 0 1 1 0           m = 0 1 0 1 1 0 1           z = 0 1 1 0 1 1 0
//         F(ahrenheit) = 1 1 0 1 1 0 0
//    F A B G E D C
//0 = 11111111111111111111111100000000111111111111111111111111
//1 = 00000000000000001111111100000000000000000000000011111111
//2 = 00000000111111111111111111111111111111111111111100000000
//3 = 00000000111111111111111111111111000000001111111111111111
//4 = 11111111000000001111111111111111000000000000000011111111
//5 = 11111111000000000000000011111111000000001111111111111111
//6 = 11111111111111110000000011111111111111111111111111111111
//7 = 00000000111111111111111100000000000000000000000011111111
//8 = 11111111111111111111111111111111111111111111111111111111
//9 = 11111111111111111111111111111111000000001111111111111111


#endif //DIGITAL_CLOCK
#endif  //CLOCK_MODE

// Which pin on the Arduino is connected to the NeoPixels?
const int ledPin = 14;

// How many NeoPixels are attached to the Arduino?
#define ledCount    226

// NeoPixel brightness, 0 (min) to 255 (max)
uint8_t bBrightness = 100; // Set bBrightness to default (max = 255)

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(ledCount, ledPin, NEO_GRBW + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

uint8_t colors = 0;
uint8_t pixel = 0;
uint8_t digit = 0;

uint8_t colorWheel[][3] = {
//Red, Green, Blue
//  {255,   255,    255},   // 0 - White
//  {255,   0,    0},   // 1 - Red
//  {255,   127,    0},   // 2 - Orange
//  {255,   255,    0},   // 3 - Yellow
//  {127,   255,    0},   // 4 - 
//  {0,   255,    0},   // 4 - Green
//  {0,   255,    127},   // 4 - 
//  {0,   255,    255},   // 5 - Aqua
//  {0,   127,    255},   // 6 - 
//  {0,   0,    255},   // 6 - Blue
//  {64,   0,    128},   // 7 - Indigo 64,0,128
//  {127,   0,    255},   // 8 - Violet
//  {255,   0,    255},   // 9 - Fuchsia
//  {255,   0,    127},   // 9 - 
{0,   0,    0},
{0,   0,    63},  // oxford blue
{0,   0,    127},  // 
{0,   0,    191},
{0,   0,    255},
{0,   63,   0},  // forest green
{0,   63,   63},
{0,   63,   127},
{0,   63,   191},
{0,   63,   255},
{0,   127,  0},
{0,   127,  63},
{0,   127,  127},
{0,   127,  191},
{0,   127,  255},
{0,   191,  0},
{0,   191,  63},
{0,   191,  127},
{0,   191,  191},
{0,   191,  255},
{0,   255,  0},
{0,   255,  63},
{0,   255,  127},
{0,   255,  191},
{0,   255,  255},
{63,  0,    0},  // bulgarian rose
{63,  0,    63},  // tyrian purple
{63,  0,    127},
{63,  0,    191},
{63,  0,    255},
{63,  63,   0},  // olive drab
{63,  63,   63},  // Paynes Grey
{63,  63,   127},  // Dark Slate Blue
{63,  63,   191},
{63,  63,   255},
{63,  127,  0},
{63,  127,  63},
{63,  127,  127},
{63,  127,  191},
{63,  127,  255},
{63,  191,  0},
{63,  191,  63},
{63,  191,  127},
{63,  191,  191},
{63,  191,  255},
{63,  255,  0},
{63,  255,  63},
{63,  255,  127},
{63,  255,  191},
{63,  255,  255},
{127, 0,    0},
{127, 0,    63},
{127, 0,    127},
{127, 0,    191},
{127, 0,    255},
{127, 63,   0},
{127, 63,   63},
{127, 63,   127},
{127, 63,   191},
{127, 63,   255},
{127, 127,  0},
{127, 127,  63},
{127, 127,  127},
{127, 127,  191},
{127, 127,  255},
{127, 191,  0},
{127, 191,  63},
{127, 191,  127},
{127, 191,  191},
{127, 191,  255},
{127, 255,  0},
{127, 255,  63},
{127, 255,  127},
{127, 255,  191},
{127, 255,  255},
{191, 0,    0},
{191, 0,    63},
{191, 0,    127},
{191, 0,    191},
{191, 0,    255},
{191, 63,   0},
{191, 63,   63},
{191, 63,   127},
{191, 63,   191},
{191, 63,   255},
{191, 127,  0},
{191, 127,  63},
{191, 127,  127},
{191, 127,  191},
{191, 127,  255},
{191, 191,  0},
{191, 191,  63},
{191, 191,  127},
{191, 191,  191},
{191, 191,  255},
{191, 255,  0},
{191, 255,  63},
{191, 255,  127},
{191, 255,  191},
{191, 255,  255},
{255, 0,    0},
{255, 0,    63},
{255, 0,    127},
{255, 0,    191},
{255, 0,    255},
{255, 63,   0},
{255, 63,   63},
{255, 63,   127},
{255, 63,   191},
{255, 63,   255},
{255, 127,  0},
{255, 127,  63},
{255, 127,  127},
{255, 127,  191},
{255, 127,  255},
{255, 191,  0},
{255, 191,  63},
{255, 191,  127},
{255, 191,  191},
{255, 191,  255},
{255, 255,  0},
{255, 255,  63},
{255, 255,  127},
{255, 255,  191},
{255, 255,  255},
};

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void whiteOverRainbow(int whiteSpeed, int whiteLength) {

  if(whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 3;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;

  for(;;) { // Repeat forever (or until a 'break' or 'return')
    for(int i=0; i<strip.numPixels(); i++) {  // For each pixel in strip...
      if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
         ((tail > head) && ((i >= tail) || (i <= head)))) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, 255)); // Set white
      } else {                                             // else set rainbow
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      }
    }

    strip.show(); // Update strip with new contents
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.

    firstPixelHue += 40; // Advance just a little along the color wheel

    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
      if(++head >= strip.numPixels()) {      // Advance head, wrap around
        head = 0;
        if(++loopNum >= loops) return;
      }
      if(++tail >= strip.numPixels()) {      // Advance tail, wrap around
        tail = 0;
      }
      lastTime = millis();                   // Save time of last movement
    }
  }
}

void displayNumber(byte number, byte segment, byte color)
{
  uint16_t startIndex = 0;
  uint8_t pos = 0;
  switch(segment) {
    case 0:
      startIndex = digitPos_0;
      break;
    case 1:
      startIndex = digitPos_1;
      break;
    case 2:
      startIndex = digitPos_2;
      break;
    case 3:
      startIndex = digitPos_3;
      break;
#if (DIGIT_CLOCK == 6)
    case 4:
      startIndex = digitPos_4;
      break;
    case 5:
      startIndex = digitPos_5;
      break;
#endif
    default:
      startIndex = 0;
      break;
  }

  for(byte i=0; i<7; i++)
  {
    for(byte j=0; j<ledSegm; j++)
    {
      pos = startIndex + (i * ledSegm) + j;
  //Serial.print("i : ");
  //Serial.print(i);
  //Serial.print(", j : ");
  //Serial.print(j);
  //Serial.print(", pos : ");
  //Serial.print(pos);
  //Serial.print(", numb : ");
  //Serial.println(numbers[number][i]);
      if(numbers[number][i] == 1)
      {
      	strip.setPixelColor(pos, 0, 255, 255);
      }
      else
      {
        strip.setPixelColor(pos, 0, 0, 0);
      }
    }
  }
}

#ifdef USE_COLON
void displayColons(uint8_t color)
{
  uint8_t pos = 0;
#if (DIGIT_CLOCK == 6)
  uint8_t pos1 = 0;
  pos1 = colonPos_1;
#endif
  pos = colonPos_0;
  if(onOff)
  {
    strip.setPixelColor(pos, 255, 0, 0);
    strip.setPixelColor(pos+1, 255, 0, 0);
#if (DIGIT_CLOCK == 6)
    strip.setPixelColor(pos1, 255, 0, 0);
    strip.setPixelColor(pos1+1, 255, 0, 0);
#endif
  }
  else
  {
    strip.setPixelColor(pos, 0, 0, 0);
    strip.setPixelColor(pos+1, 0, 0, 0);
#if (DIGIT_CLOCK == 6)
    strip.setPixelColor(pos1, 0, 0, 0);
    strip.setPixelColor(pos1+1, 0, 0, 0);
#endif
  }

  onOff = !onOff;
}
#endif  //USE_COLON

void setup() {
    // put your setup code here, to run once:
#ifdef USE_SERIAL
    Serial.begin(9600);
    Serial.println("RGB test");
#endif  //USE_SERIAL

#ifdef USE_AUTO_BRIGHTNESS
    pinMode(luxSensorPin, INPUT); //data pin for ambientlight sensor
#endif  //USE_AUTO_BRIGHTNESS

#ifdef CLOCK_MODE
    digitPos_0 = 0;
    digitPos_1 = 7*ledSegm;
#ifdef USE_COLON
    colonPos_0 = 14*ledSegm;
    digitPos_2 = colonPos_0+2;
    digitPos_3 = (21*ledSegm)+2;
#else
    digitPos_2 = 14*ledSegm;
    digitPos_3 = 21*ledSegm;
#endif  //USE_COLON

#if (DIGIT_CLOCK == 6)
#ifdef USE_COLON
    colonPos_1 = (28*ledSegm)+2;
    digitPos_4 = colonPos_1+2;
    digitPos_5 = (35*ledSegm)+4;
#else
    digitPos_4 = 28*ledSegm;
    digitPos_5 = 35*ledSegm;
#endif  //USE_COLON
#endif  //DIGIT_CLOCK == 6
#ifdef USE_SERIAL
    Serial.print("digitPos_0: ");
    Serial.println(digitPos_0, DEC);
    Serial.print("digitPos_1: ");
    Serial.println(digitPos_1, DEC);
#ifdef USE_COLON
    Serial.print("colonPos_0: ");
    Serial.println(colonPos_0, DEC);
#endif
    Serial.print("digitPos_2: ");
    Serial.println(digitPos_2, DEC);
    Serial.print("digitPos_3: ");
    Serial.println(digitPos_3, DEC);
#if (DIGIT_CLOCK == 6)
#ifdef USE_COLON
    Serial.print("colonPos_1: ");
    Serial.println(colonPos_1, DEC);
#endif  //USE_COLON
    Serial.print("digitPos_4: ");
    Serial.println(digitPos_4, DEC);
    Serial.print("digitPos_5: ");
    Serial.println(digitPos_5, DEC);
#endif  //(DIGIT_CLOCK == 6)
#endif  //USE_SERIAL
#endif  //CLOCK_MODE

    strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();            // Turn OFF all pixels ASAP
    strip.setBrightness(bBrightness);
}

void loop() {
    // put your main code here, to run repeatedly:
#ifdef USE_AUTO_BRIGHTNESS
    luxValue = analogRead(luxSensorPin);
    bBrightness = luxValue * 0.249;// percentage calculation
#ifdef USE_SERIAL
    Serial.print("luxValue: ");
    Serial.println(luxValue, DEC);
    Serial.print("Brightness: ");
    Serial.println(bBrightness, DEC);
    strip.setBrightness(bBrightness);
#endif  //USE_SERIAL
#endif  //USE_AUTO_BRIGHTNESS
    //Fill along the length of the strip in various colors...
    // colorWipe(strip.Color(colorWheel[colors][0],  colorWheel[colors][1],  colorWheel[colors][2]), 50);
    // for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    //     strip.setPixelColor(i, colorWheel[colors][0], colorWheel[colors][1], colorWheel[colors][2]);         //  Set pixel's color (in RAM)
    //     strip.show();                          //  Update strip to match
    //     delay(50);                           //  Pause for a moment
    // }
#ifdef USE_SERIAL
      Serial.print("loops: ");
      Serial.println(colors, DEC);
      Serial.print("pixel: ");
      Serial.println(pixel, DEC);
#endif  //USE_SERIAL
    strip.setPixelColor(pixel, colorWheel[colors][0], colorWheel[colors][1], colorWheel[colors][2]);
//    displayNumber(digit, pixel, 69);
    colors++;
//    digit++;
//    if(digit>9) digit = 0;
    pixel++;
    if(pixel>7) pixel = 0;
    if(colors > 125) colors = 0;
    strip.show();                          //  Update strip to match
    delay(1000);
}
