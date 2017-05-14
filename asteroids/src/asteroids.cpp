/* Asteroids
    Sample solution for assignment
    Semester 2 -- Small Embedded Systems
    Dr Alun Moon
*/

/* C libraries */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

/* hardware platform libraries */
#include <display.h>
#include <mbed.h>

/* Main game elements */
#include "model.h"
#include "view.h"
#include "controller.h"

/* Game state */
float elapsed_time; 
int   score;
int   lives;
struct ship player;
struct missile *shots = NULL;
struct rock *asteroids = NULL;

float Dt = 0.01f;

Ticker model, view, controller;
void init_game();
bool paused = true;
bool inPlay = false;

/* The single user button needs to have the PullUp resistor enabled */
DigitalIn userbutton(P2_10,PullUp);
int main()
{
  init_DBuffer();
	model.attach( physics, 0.01);
	view.attach( draw, 0.045);
  controller.attach( controls, 0.1);
	initialise_missiles();
	initialise_asteroids();
  init_game();
	
	while(true) {
		if (lives > 0){
    /* Pause to start */
			while( userbutton.read() ){ /* remember 1 is not pressed */
				paused=true;
				wait_ms(100);
			}//button pressed
			paused = false;
			inPlay = true;
			while(inPlay){
				//playing		
			}
			//life lost
			paused = true;
		}
		//life is 0 or less
		if(lives <= 0 && !userbutton.read() && paused){
			init_game();
		}
  }
}

void init_game(){
	player.p.x = 200;
	player.p.y = 136;
	player.v.x = 0;
	player.v.y = 0;
	player.speed = 150;//max speed in pixels per second
	player.drag = 1;//in 
	lives = 5;
	score = 0;
	elapsed_time = 0;
	paused = false;
}
