#include<stdbool.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>

#include "entidades.h"
#include "backend.h"
#include "constantes_pi.h"

//inicializar en el front: 
//	- alienBlock_t aliensBlock
//	- alien_t * aliens[ALIEN_ROWS][ALIEN_COLS]
// 	- shield_t * shields[NUM_SHIELDS]
//	- bullet_t * bulletPlayer
//	- bullet_t * bulletAlien

//creo que esta bien, chequear con los muchachos. 
void initPlayer(player_t * player){		//OPCIONAL: JUGAR DE A DOS, como se podria hacer? 
	
	player->health = 3;
	player->coord.coordX = 0;	//asi arranca al costado izquierdo
	player->coord.coordY = INIT_PLAYER_Y;
}

//revisar logica y compatibilidad REVISADO
void playerMove(int dire, player_t * player){
    static float speed = 0.95;
	if(dire<0 && player->coord.coordX>0){
        if(!speed){
            speed=-0.95;
        }
        speed -= SPEED_PLAYER;
	} else if(dire>0 && player->coord.coordX<DISPLAY_LENGTH-PLAYER_SIZE_X){
        if(!speed){
            speed=0.95;
        }
        speed += SPEED_PLAYER;
	} else{
        speed = 0;
    }
    player->coord.coordX += (int)speed; 
    if(speed>=1 || speed<=-1){
        speed = 0.05;
    }
}

void playerShoot(bullet_t *playerBullet, player_t *player, bool *tryShoot){
    static float speed = 0;
    if (*tryShoot && !playerBullet->active){
       *tryShoot = false;
        playerBullet->active = true;
        playerBullet->coord.coordY = player->coord.coordY;
        playerBullet->coord.coordX = player->coord.coordX + 1 - BULLET_SIZE_X / 2;
    } else if (playerBullet->active){
        if (playerBullet->coord.coordY < 1) {
            playerBullet->active = false;
            *tryShoot = false;
        }
        if(playerBullet->coord.coordY){
            speed -= SPEED_BULLET_PLAYER;
            playerBullet->coord.coordY += (int) speed;
        }
    } else{
        speed = 0;
    }
    if(speed<=-1){
        speed = 0;
    }
}
