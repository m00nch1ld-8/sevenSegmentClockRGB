#define USE_SERIAL

#include <Adafruit_NeoPixel.h>

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

void setup() {
    // put your setup code here, to run once:
#ifdef USE_SERIAL
    Serial.begin(9600);
    Serial.println("RGB test");
#endif
    strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();            // Turn OFF all pixels ASAP
    strip.setBrightness(bBrightness);
}

void loop() {
    // put your main code here, to run repeatedly:
    //Fill along the length of the strip in various colors...
    colorWipe(strip.Color(colorWheel[colors][0],  colorWheel[colors][1],  colorWheel[colors][2]), 50);
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
        strip.setPixelColor(i, colorWheel[colors][0], colorWheel[colors][1], colorWheel[colors][2]);         //  Set pixel's color (in RAM)
        strip.show();                          //  Update strip to match
        delay(50);                           //  Pause for a moment
    }
#ifdef USE_SERIAL
      Serial.print("loops: ");
      Serial.println(colors, DEC);
      Serial.print("pixel: ");
      Serial.println(pixel, DEC);
#endif
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
