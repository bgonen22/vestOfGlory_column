// This file are based on code from shenberg/VestOfGlory
// Source: https://github.com/shenberg/VestOfGlory
#include "FastLED.h"

// How many leds in your strip?
// #define LEDS_PER_STRIP 100
#define LEDS_PER_STRIP 179
#define LEFT_FRONT_PIN 13
#define RIGHT_FRONT_PIN 14
#define LEFT_REAR_PIN 11
#define RIGHT_REAR_PIN 12

// Define the array of leds
CRGB leds[LEDS_PER_STRIP * 4];
int ledX[LEDS_PER_STRIP * 4];
int ledY[LEDS_PER_STRIP * 4];

// static const int MAX_LEDS_PER_ROW_FRONT = 7;
// static const int ledsPerRow[] = {3, 7, 7, 7, 7, 7, 7, 7, 7, 6, 5, 5, 4, 4, 5, 4, 4, 4};
// static const int skipPerRow[] = {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, -1, 1, 0, 1};

static const int MAX_LEDS_PER_ROW_FRONT = 21;
static const int ledsPerColumn[] = {16, 18, 19, 21, 21, 21, 21, 21, 11, 10};
static const int skipPerColumn[] = {0,  0,  0,  0,  0,  0,  0,  0,  0,  0};

static const int MAX_LEDS_PER_ROW_BACK = 6;
static const int ledsPerRowBackLeft[] = {4, 4, 4, 4, 4, 5, 5, 5, 6, 6, 5, 5, 5, 5, 5, 6, 6}; // total 84 LEDs
static const int skipPerRowBackLeft[] = {0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0};
static const int ledsPerRowBackRight[] = {4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6};
static const int skipPerRowBackRight[] = {0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0};

static const int columnCountFront = sizeof(ledsPerColumn)/sizeof(ledsPerColumn[0]);
static const int rowCountBack = sizeof(ledsPerRowBackLeft)/sizeof(ledsPerRowBackLeft[0]);

static const int LEDS_BACK_LEFT = 84;
static const int LEDS_BACK_RIGHT = 81;

long startTime;

template<typename T>
class LedDraw {
public:
	static void drawFrontPart(T& instance, int startLed, int startX, int startY, bool mirror = false, int t = 0) {
		int led = startLed;
		int x, y;
		x = startX;
		y = startY;
		int direction = 1;

		if (mirror) {
			direction = -1;
			x += MAX_LEDS_PER_ROW_FRONT - 1;
		}

		for (int col = 0; col < columnCountFront; col++) {
			y += direction*skipPerColumn[col];
			for (int i = 0; i < ledsPerColumn[col]; i++) {
				leds[led] = instance.pixel(x,y,t);
				led++;
				// y += direction;
				y -= direction;
			}
			x --;
			direction = -1 * direction;
		}	
	}

	static void drawBackPart(T& instance, const int *ledCounts, const int *skipCounts, int startLed, int startX, int startY, bool mirror = false, int t = 0) {
		int led = startLed;
		int x, y;
		x = startX;
		y = startY;
		int direction = 1;

		if (mirror) {
			direction = -1;
			x += MAX_LEDS_PER_ROW_BACK - 1;
		}

		for (int row = 0; row < rowCountBack; row++) {
			x += direction*skipCounts[row];
			for (int i = 0; i < ledCounts[row]; i++) {
				leds[led] = instance.pixel(x,y,t);
				led++;
				x += direction;
			}
			y++;
			direction = -1 * direction;
		}	
	}



	static void drawFront(T& instance) {
		long time = (millis() - startTime);
		drawFrontPart(instance, 0, MAX_LEDS_PER_ROW_FRONT+1, 0, false, time);
		drawFrontPart(instance, LEDS_PER_STRIP, MAX_LEDS_PER_ROW_FRONT+1, 0, true, time);
	}
	static void drawBack(T& instance) {
		long time = (millis() - startTime);
		drawBackPart(instance, ledsPerRowBackLeft, skipPerRowBackLeft, LEDS_PER_STRIP*2, 0, 0, true, time);
		drawBackPart(instance, ledsPerRowBackRight, skipPerRowBackRight, LEDS_PER_STRIP*2 + LEDS_BACK_LEFT, MAX_LEDS_PER_ROW_BACK, 0, false, time);
	}
	static void draw(T& instance) {
		drawFront(instance);
		drawBack(instance);
	}
};



void setup() { 
	Serial.begin(57600);
	Serial.println("resetting");
	FastLED.addLeds<NEOPIXEL,LEFT_FRONT_PIN>(leds, LEDS_PER_STRIP);
	FastLED.addLeds<NEOPIXEL,RIGHT_FRONT_PIN>(&leds[LEDS_PER_STRIP], LEDS_PER_STRIP);
	FastLED.addLeds<NEOPIXEL, LEFT_REAR_PIN>(&leds[LEDS_PER_STRIP*2], LEDS_BACK_LEFT);
	FastLED.addLeds<NEOPIXEL, RIGHT_REAR_PIN>(&leds[LEDS_PER_STRIP*2 + LEDS_BACK_LEFT], LEDS_BACK_RIGHT);
	FastLED.setBrightness(0);
	startTime = millis();
}

void outlineRandomAmplitude(uint16_t delta) {
	// elapsed time
	static uint16_t time = 0;
	time += delta;
	//for(int i = 0; i < )
}


class PalettedPlasma {
private:
	CRGBPalette16 current;
	CRGBPalette16 target;
	CRGBPalette16 last;
	long startTime;
	long lastTime;
public:
	CRGB pixel(int x, int y, int t) {
		byte index = (sin16(sin16(x*139)+sin16(y*371) +32767*2 + t*7) + 32767) >> 8 ;
		byte brightness = (sin16(sin16(x*191)+sin16(y*197) +32767*2 + t*5) + 32767) >> 8;
		return ColorFromPalette(current, index, qsub8(brightness, 40));
	}

	PalettedPlasma() {
		lastTime = startTime = millis();
		last = RainbowColors_p;
		target = PartyColors_p;
	}

	void update() {
		int seconds = (millis() - lastTime);

		if (seconds > 5000) {
			last = target;
			lastTime = millis();
			switch(random8() % 4) {
			case 0:
				target = RainbowColors_p;
				break;
			case 1:
				target = PartyColors_p;
				break;
			case 2:
				target = LavaColors_p;
				break;
			case 3:
				target = ForestColors_p;
				break;
			}
			//current = target;
		} else {
			blend(last.entries, target.entries, current.entries, 16, seconds*255/5000);
		}


	}

	void draw() {
		LedDraw<PalettedPlasma>::draw(*this);
	}

};

class Animation {
public:
	virtual void update() = 0;
	virtual void draw() = 0;
};



static PalettedPlasma palPlasma;

void loop() { 
	Serial.print("x");
	long time = millis() - startTime;
	FastLED.setBrightness(time < 15000 ? time / 500.f : 60);
	palPlasma.update();
	palPlasma.draw();

	FastLED.show();
	//FastLED.delay(5);
	delay(10);
	
}