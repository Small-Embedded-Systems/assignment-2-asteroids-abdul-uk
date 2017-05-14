/* Asteroids view
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

#include "view.h"
#include "asteroids.h"
#include "model.h"
#include "utils.h"

Display *graphics = Display::theDisplay();
extern "C" uint8_t flip(uint8_t);

const colour_t background = rgb(0,51,102); /* Midnight Blue */
uint8_t flip(uint8_t); //assembly function for flipping image bit pattern
static void rock_flip();
static unsigned char flippedRock[200];

const coordinate_t shape[] = {
    {0,-10}, {-5,5}, {5,5}
};

/* double buffering functions */
void init_DBuffer(void)
{   /* initialise the LCD driver to use second frame in buffer */
    uint16_t *bufferbase = graphics->getFb();
    uint16_t *nextbuffer = bufferbase+ (480*272);
    LPC_LCD->UPBASE = (uint32_t)nextbuffer;
}

void swap_DBuffer(void)
{   /* swaps frames used by the LCD driver and the graphics object */
    uint16_t *buffer = graphics->getFb();
    graphics->setFb( (uint16_t*) LPC_LCD->UPBASE);
    LPC_LCD->UPBASE = (uint32_t)buffer;
}
void draw_sidebar(int score, float elapsed_time, int lives);
void draw_ship(struct ship);
void draw_missiles(struct missile *l);
void draw_rocks(struct rock *lst);
void pause();
void draw(void)
{
  graphics->fillScreen(background);

  draw_sidebar(score, elapsed_time, lives);
	draw_missiles(shots);
	draw_rocks(asteroids);
	draw_ship(player);
	if(paused && lives > 0){
		pause();
	}else if(paused && lives <= 0){
		gameover();
	}
  swap_DBuffer();
}


void draw_sidebar(int score, float elapsed_time, int lives){
	graphics->fillRect(400, 0,80, 272, BLACK);
	graphics->setTextColor(WHITE, BLACK);
	graphics->setTextSize(1);
	
	graphics->setCursor(403, 2);
	graphics->printf("CM0506");
	graphics->setCursor(403, 17);
	graphics->printf("Created by:");
	
	graphics->fillCircle(403, 36, 2, RED);
	graphics->setCursor(409, 32);
	graphics->printf("A. Alshehri");
	
	graphics->fillCircle(403, 49, 2, RED);
	graphics->setCursor(409, 45);
	graphics->printf("R. Alhassan");

	graphics->setCursor(403, 75);
	graphics->printf("Score: %d", score);
	graphics->setCursor(403, 90);
	graphics->printf("Time: %.0f", elapsed_time);
	graphics->setCursor(403, 105);
	graphics->printf("Lives: %d",lives);
	if(lives == 5){
		graphics->fillCircle(400 + 30, 125, 5, RED);
		graphics->fillCircle(400 + 30, 143, 5, RED);
		graphics->fillCircle(400 + 30, 161, 5, RED);
		graphics->fillCircle(400 + 30, 179, 5, RED);
		graphics->fillCircle(400 + 30, 197, 5, RED);
	}else	if(lives == 4){
		graphics->fillCircle(400 + 30, 125, 5, RED);
		graphics->fillCircle(400 + 30, 143, 5, RED);
		graphics->fillCircle(400 + 30, 161, 5, RED);
		graphics->fillCircle(400 + 30, 179, 5, RED);
	}else	if(lives == 3){
		graphics->fillCircle(400 + 30, 125, 5, RED);
		graphics->fillCircle(400 + 30, 143, 5, RED);
		graphics->fillCircle(400 + 30, 161, 5, RED);
	}else	if(lives == 2){
		graphics->fillCircle(400 + 30, 125, 5, RED);
		graphics->fillCircle(400 + 30, 143, 5, RED);
	}else if(lives == 1){
		graphics->fillCircle(400 + 30, 125, 5, RED);
	}
}

void draw_ship(struct ship){
	float tra0x = player.p.x + (shape[0].x*cos(radians(player.heading))) - (shape[0].y*sin(radians(player.heading)));
	float tra0y = player.p.y + (shape[0].x*sin(radians(player.heading))) + (shape[0].y*cos(radians(player.heading)));
	float tra1x = player.p.x + (shape[1].x*cos(radians(player.heading))) - (shape[1].y*sin(radians(player.heading))); 
	float tra1y = player.p.y + (shape[1].x*sin(radians(player.heading))) + (shape[1].y*cos(radians(player.heading)));
	float tra2x = player.p.x + (shape[2].x*cos(radians(player.heading))) - (shape[2].y*sin(radians(player.heading)));
	float tra2y = player.p.y + (shape[2].x*sin(radians(player.heading))) + (shape[2].y*cos(radians(player.heading)));

	graphics->drawTriangle(tra0x, tra0y, tra1x, tra1y,tra2x,tra2y, YELLOW);
}

void draw_missiles(missile *shots){
	while(shots) {	
		graphics->fillCircle(shots->p.x,shots->p.y,1,RED);
		shots = shots->next;
	}
}
void draw_rocks(struct rock *lst){
	rock_flip();
	while(lst){
		graphics->drawBitmap(lst->p.x-20, lst->p.y-20, flippedRock, asteroid_width, asteroid_height, BLACK);
		lst = lst->next;
	}	
}

void rock_flip(){
	int i;
	for (i=0; i < 200; i++){
		flippedRock[i] = flip(asteroid_bits[i]);
	}
}
void pause(){
	graphics->setCursor(182,150);
	graphics->printf("PAUSED"); 
}
void gameover(){
	graphics->setCursor(182,150);
	graphics->printf("GAME OVER"); 
	graphics->setCursor(172,165);
	graphics->printf("TOTAL SCORE : %d", score); 
}