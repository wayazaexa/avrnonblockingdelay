#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "millis.h"

#define BIT_SET(a, b) (a |= (1U << b))
#define BIT_CLEAR(a, b) (a &= ~(1U << b))
#define BIT_FLIP(a, b) (a ^= (1U << b))
#define BIT_CHECK(a, b) (a & (1U << b))

// NON BLOCKING DELAYS
// Delays är INTE förbjudet. 
// när ska man inte använda delay? När vi har input  från sensorer etc 
// som inte får vänta "för länge"

// två sätt att hantera - "händelsestyrd programmering"
//      när status på denna pin ändras -> kör min funktion som heter onChange
//      INTERRUPT
// timer timer.h timer.c 
// http://blog.zakkemble.co.uk/millisecond-tracking-library-for-avr/
//   vår while-loop ska gå så FORT som möjligt

// https://wokwi.com/projects/363955195435615233

// B (digital pin 8 to 13)
// C (analog input pins)
// D (digital pins 0 to 7)
#define LED_PINLEFT 2
#define LED_PINRIGHT 1
#define LED_SWICTCHCLICKER 3
#define SWITCH_PINLEFT 7
#define SWITCH_PINRIGHT 6

int main(void) {
    
    // DATA DIRECTION REGISTER B
    BIT_SET(DDRB, LED_PINLEFT); //Sätt led_pin till output mode
    BIT_SET(DDRB, LED_PINRIGHT); //Sätt led_pin till output mode
    BIT_SET(DDRB, LED_SWICTCHCLICKER);
    BIT_CLEAR(DDRD, SWITCH_PINLEFT); 
    BIT_CLEAR(DDRD, SWITCH_PINRIGHT); 
    BIT_SET(PORTD, SWITCH_PINLEFT); 
    BIT_SET(PORTD, SWITCH_PINRIGHT); 
 //This means INPUT_PULLUP
	// https://forum.arduino.cc/t/using-avr-internal-pull-up-for-push-button-controlling/327729/4
	// https://www.hackster.io/Hack-star-Arduino/push-buttons-and-arduino-a-simple-guide-wokwi-simulator-c2281f    

    millis_init();
    sei();
    
    millis_t antalMilliSekunderSenasteBytet = 0;
    millis_t current_millis = 0;

    BIT_SET(PORTB, LED_PINLEFT);
    BIT_CLEAR(PORTB, LED_PINRIGHT); 

    //antalSekunder = 0;
    while (1) {
        if (BIT_CHECK(PIND, SWITCH_PINLEFT)) {
            BIT_SET(PORTB, LED_SWICTCHCLICKER);
        }
        else {
            BIT_CLEAR(PORTB, LED_SWICTCHCLICKER);
        }

        current_millis = millis_get();
        if(current_millis - antalMilliSekunderSenasteBytet >= 3000) {

            // EXEKVERAS      VAR TREDJE SEKUND
            BIT_FLIP(PORTB, LED_PINLEFT);
            BIT_FLIP(PORTB, LED_PINRIGHT); 

            antalMilliSekunderSenasteBytet = current_millis; // 13122
        }

    }
	return 0;
}