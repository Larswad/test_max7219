#include <max7219.h>

//#define TEST_PATTERN_MOVEMENT
//#define TEST_PERCENTAGE
#define TEST_SCROLLING

Max7219* pMax;

const byte PROGMEM INPIN = 11, LOADPIN = 13, CLOCKPIN = 12;

#ifdef TEST_SCROLLING
static byte PROGMEM myText[] = "   HEJSAN ALLA GLADA BARNEN, DETTA E LARS MED SIN NYA MAX7219 SCROLL...    ";
#endif

static void flashIntensity(byte speed = 1);

void setup ()
{
	pMax = new Max7219(INPIN, LOADPIN, CLOCKPIN);
#ifdef TEST_SCROLLING
	pMax->resetScrollText(myText);
#endif
} // setup


void loop ()
{

#ifdef TEST_PATTERN_MOVEMENT
	// just a funny pattern movement.
	while(1) {
		static byte PROGMEM arr[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };
		static boolean dir[8] = { false, false, false, false, false, false, false, false };
		for(byte i = 0; i < sizeof(arr); ++i) {
			pMax->maxSingle(i + 1, arr[i]);
			if(!dir[i]) {
				if(arr[i] bitand (1 << 7))
					dir[i] = true;
			}
			else {
				if(arr[i] bitand (1 << 0))
					dir[i] = false;
			}
			if(!dir[i])
				arr[i] <<= 1;
			else
				arr[i] >>= 1;
		}
		delay(100);
	}
#endif

#ifdef TEST_PERCENTAGE
	// test percantage increase, from 0 to 1000 in quarter steps.
	while(1) {
		pMax->resetPercentage(1000);
		pMax->showPercentage(0);
		delay(1000);
		pMax->showPercentage(250);
		delay(1000);
		pMax->showPercentage(500);
		delay(1000);
		pMax->showPercentage(750);
		delay(1000);
		pMax->showPercentage(1000);
		delay(1000);

		pMax->resetPercentage(1000);
		for(int i = 0; i <= 1000; ++i) {
			pMax->showPercentage(i);
			delay(10);
		}
		for(int i = 0; i < 1000; ++i) {
			delay(10);
			flashIntensity();
		}
		delay(1000);
	}
#endif

#ifdef TEST_SCROLLING
	pMax->doScrollLeft();
	//pMax->doScrollUp();
	delay(50);
#endif
//	delay(100);
} // loop


static void flashIntensity(byte speed)
{
	static byte changeIntensity = 0;
	static char intensity = 0;
	static bool intensityDirection = false;

	if(0 == changeIntensity) {
		changeIntensity = 0;
		if(!intensityDirection) {
			intensity -= 1;
			if(intensity <= 0) {
				intensityDirection = true;
				intensity = 0;
			}
		}
		else {
			intensity += 1;
			if(intensity >= 15) {
				intensityDirection = false;
				intensity = 15;
			}
		}
		pMax->setIntensity(intensity);
	}
	else
		changeIntensity++;
} // flashIntensity
