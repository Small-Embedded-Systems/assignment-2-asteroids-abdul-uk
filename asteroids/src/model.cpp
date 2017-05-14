/* Asteroids model */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"
#include "view.h"
#include "utils.h"
#include "asteroids.h"

static const size_t MAXSize = 10;

static missile missile_data[MAXSize];
static missile *freeMissileNodes;
void new_missile(struct missile *r);

static rock rock_data[MAXSize];
static rock *freeRockNodes;
void new_asteroid(struct rock *r);

//functions
ship move_ship(ship, float Dt); 
void shoot_missile();
void move_missiles(struct missile *r);
static void move_missiles(struct missile *l);
static void update_missile_list(struct missile *l);

static void move_asteroids(struct rock *l, float x);
static void update_rock_list(struct rock *l);
static bool ship_hits(struct rock *l);
static void missile_hit_rocks(struct missile *l, struct rock *k);
struct rock * new_asteroids();



void physics(void)
{
  if(!paused){
		elapsed_time += Dt;
		player = move_ship(player, Dt);
		move_missiles(shots);
		asteroids = new_asteroids();
		move_asteroids(asteroids, Dt);
		
		missile_hit_rocks(shots, asteroids);
		if(ship_hits(asteroids)){
			inPlay = false;
			score += (int) elapsed_time;
			elapsed_time = 0;
			lives--;			
		}
		update_missile_list(shots);
		update_rock_list(asteroids);
	}
}
//moving the ship
ship move_ship(ship, float time){
  //ship's accelration
	player.p.x += player.v.x*time;
	player.p.y += player.v.y*time;
	//ship's drag
	player.v.x -= (player.v.x*player.drag)*0.01f;
	player.v.y -= (player.v.y*player.drag)*0.01f;
	//screen wrap-around
	if(player.p.x < 0){
		player.p.x += 390;
	}
	if(player.p.x >= 390){
		player.p.x -= 390;
	}
	if(player.p.y < 0){
 		player.p.y += 272;
	}
	if(player.p.y >= 272){
		player.p.y -= 272;
	}		
	return player;
}

void shoot_missile(){
	struct missile *newMissile = allocate_missile_node();
	if(newMissile){
		newMissile->next = shots;
		shots = newMissile;
		new_missile(newMissile);
	}
	update_missile_list(shots);
}
void new_missile(struct missile *r){
	r->heading = player.heading;
	
	r->p.x = player.p.x;
	r->p.y = player.p.y;
	
	//adjusting position of missiles
	r->v.x = sin(radians(r->heading));
	r->v.y = -cos(radians(r->heading));
	
	//adjusting position of missiles
	r->p.x += r->v.x * 10;
	r->p.y += r->v.y * 10;
	
	//setting up the speed
	r->v.x *=50;
	r->v.y *=50;
	
	//time to live
	r->ttl = 10;

}

void initialise_missiles(){
	int n;
	for(n=0; n<(MAXSize-1) ; n++){
		missile_data[n].next = &missile_data[n+1];
	}
	missile_data[n].next = NULL;
	freeMissileNodes = &missile_data[0];
}

missile *allocate_missile_node(void){
	missile *node = NULL;
	if (freeMissileNodes){
		node = freeMissileNodes;
		freeMissileNodes = freeMissileNodes->next;
	}
	return node;
}

void free_missile_node(missile *n){
	n->next = freeMissileNodes;
	freeMissileNodes = n;
}

void update_missile_list(struct missile *l){
	for ( ; l ; l = l->next){		
		if (l->next->ttl <= 0){
			struct missile *expiredM = l->next;
			l->next = l->next->next;
			free_missile_node(expiredM);
		}		
	}
}

void move_missiles(struct missile *l){
	for ( ; l ; l = l->next){
		l->p.x += l->v.x * Dt;
		l->p.y += l->v.y * Dt;
		
		if (l->p.y >= 272){
			l->ttl = 0;
		}
		if (l->p.y < 0){
			l->ttl = 0;
		}
		if (l->p.x >= 400){
			l->ttl = 0;
		}
		if (l->p.x < 0){
			l->ttl = 0;
		}
		l->ttl -= Dt;	
	}	
}
struct rock * new_asteroids(){
	int i;
	for(i = 0; i < MAXSize-1; i++){
		struct rock *newRock = allocate_rock_node();
		if(newRock){
			newRock->next = asteroids;
			asteroids = newRock;
			
			int initAstPos = randrange(1,5);
			switch (initAstPos){
				case 1 :
					newRock->p.x = randrange(0,100);
					newRock->p.y = randrange(0,272);
					newRock->v.x = 1;
					if(newRock->p.y > 136) newRock->v.y = -10;
					if(newRock->p.y <= 136) newRock->v.y = 10;
				break;
				case 2 :
					newRock->p.x = randrange(280,380);
					newRock->p.y = randrange(0,272);
					newRock->v.x = -1;
					if(newRock->p.y > 136) newRock->v.y = -10;
					if(newRock->p.y <= 136) newRock->v.y = 10;
				break;	
				case 3 :
					newRock->p.x = randrange(0,380);
					newRock->p.y = randrange(0,100);
					newRock->v.y = 1;
					if(newRock->p.x > 200) newRock->v.x = -10;
					if(newRock->p.y <= 200) newRock->v.x = 10;
				break;	
				case 4 :
					newRock->p.x = randrange(0,380);
					newRock->p.y = randrange(172,272);
					newRock->v.y = 1;
					if(newRock->p.x > 200) newRock->v.x = -10;
					if(newRock->p.y <= 200) newRock->v.x = 10;
				break;				
			}
			newRock->ttl = 2000;
		}
		update_rock_list(asteroids);
	}
	return asteroids;
}

void initialise_asteroids(){
	int n;
	for(n=0; n<(MAXSize-1) ; n++){
		rock_data[n].next = &rock_data[n+1];
	}
	rock_data[n].next = NULL;
	freeRockNodes = &rock_data[0];	
}

rock *allocate_rock_node(void){
	rock *node = NULL;
	if (freeRockNodes){
		node = freeRockNodes;
		freeRockNodes = freeRockNodes->next;
	}
	return node;
}

void update_rock_list(struct rock *l){
	for ( ; l ; l = l->next){
		if (l->next->ttl <= 0){
			struct rock *expiredR = l->next;
			l->next = l->next->next;
			free_rock_node(expiredR);
		}		
	}
}

void free_rock_node(rock *n){
	n->next = freeRockNodes;
	freeRockNodes = n;
}
	
void move_asteroids(struct rock *l, float x){
	for ( ; l ; l = l->next){
		l->p.x += l->v.x * x;
		l->p.y += l->v.y * x;
		
		if (l->p.y >= 272){
			l->p.y -= 272;
		}
		if (l->p.y < 0){
			l->p.y += 272;
		}
		if (l->p.x >= 380){
			l->p.x -= 380;
		}
		if (l->p.x < 0){
			l->p.x += 380;
		}		
		l->ttl -= Dt;	
	}	
}

void missile_hit_rocks(struct missile *l, struct rock *k){
	for ( ; l ; l = l->next){
		for( ; k ; k =k->next){
			float r1 = 1; 
			float r2 = 20;
			float dx = l->p.x - k->p.x; float dy = l->p.y - k->p.y;
			float distance = sqrt(dx*dx + dy*dy);
			if (distance < (r1 + r2)){
					l->ttl = 0;
					k->ttl = 0;
			}
		}
	}
}
	
bool ship_hits(struct rock *l){
		for( ; l ; l =l->next){
			float r1 = 7.9; 
			float r2 = 20; 
			float dx = l->p.x - player.p.x; float dy = l->p.y - player.p.y;
			float distance = sqrt(dx*dx + dy*dy);
			if (distance < (r1 + r2)){
					l->ttl = 0;
					return true;
			}
		}
		return false;
}
	

