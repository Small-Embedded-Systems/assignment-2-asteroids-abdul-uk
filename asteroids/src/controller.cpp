/* Controller */

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* hardware platform libraries */
#include <mbed.h>

/* asteroids */
#include "model.h"
#include "asteroids.h"
#include "utils.h"

/* Joystick 5-way switch
*/
typedef enum {JLT = 0, JRT, JUP, JDN, JCR} btnId_t;
DigitalIn joystick[] =  {P5_4, P5_0, P5_2, P5_1, P5_3};
bool jsPrsd(btnId_t b);
float a = 0;

void controls(void)
{
	if(jsPrsd(JUP)){
		float xN = sin(radians(player.heading));
		float yN = cos(radians(player.heading));
		player.v.x += (xN * player.speed)*0.1f;
		player.v.y -= (yN * player.speed)*0.1f;
	}
	if(jsPrsd(JDN)){
		float xN = sin(radians(player.heading));
		float yN = cos(radians(player.heading));
		player.v.x -= (xN * player.speed)*0.1f;//controller refresh rate allows pixel per sec speed
		player.v.y += (yN * player.speed)*0.1f;
	}
	if(jsPrsd(JRT)){
		player.heading += 15;
	}
	if(jsPrsd(JLT)){
		player.heading -= 15;
	}
	if(jsPrsd(JCR)){
		shoot_missile();
	}
}

 bool jsPrsd(btnId_t b) {
 bool result = false;
 uint32_t state;
 state = joystick[b].read();
 if (state == 0) {
 result = true;
 }
 return result;
 }
