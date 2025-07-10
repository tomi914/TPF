#include<stdbool.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>

#include "entidades.h"
#include "constantes_pi.h"
#include "backend.h"

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

void initAliensBlock(alienBlock_t * aliensBlock){//probado en allegro
	aliensBlock->coord.coordX = DISPLAY_MARGIN_X;
    aliensBlock->coord.coordY = DISPLAY_MARGIN_Y;
    aliensBlock->direction = 1;
    aliensBlock->firstColAlive = 0;
    aliensBlock->lastColAlive = ALIEN_COLS - 1;
    aliensBlock->lastRowAlive = ALIEN_ROWS - 1; 
    aliensBlock->width = (aliensBlock->lastColAlive - aliensBlock->firstColAlive) * (ALIEN_B_SIZE_X + B_INIT_JUMP_SIZE_X) + ALIEN_B_SIZE_X;
}

//Inicializo el array de aliens
void initAliensArray(alien_t aliens[ALIEN_ROWS][ALIEN_COLS]){	//recibe un puntero al arreglo de aliens(xq en el stack? porque es mas rapido y no tenemos cant. variable)
	int i, j;//probado en allegro
	for(i = 0; i < ALIEN_ROWS; i++) { // Itero sobre una fila 
		for(j = 0; j < ALIEN_COLS; j++) { // Itero sobre una columna 
			aliens[i][j].alive = true; // todos arrancan vivos
			
			if(i == 0){
		    	aliens[i][j].type = 'A';		//en allegro cada tipo corresponde a una imagen diferente
		    	aliens[i][j].coord.coordX = A_INIT_JUMP_SIZE_X/2 + j * (ALIEN_A_SIZE_X + A_INIT_JUMP_SIZE_X);	//chequear que este bien visualmente y calcular bien los saltos
		    	aliens[i][j].coord.coordY = i * (ALIEN_A_SIZE_Y + A_INIT_JUMP_SIZE_Y);
		    }
		    else if(i < 3){
		    	aliens[i][j].type = 'B';
		    	aliens[i][j].coord.coordX = B_INIT_JUMP_SIZE_X/2 + j * (ALIEN_B_SIZE_X + B_INIT_JUMP_SIZE_X);
		    	aliens[i][j].coord.coordY = i * (ALIEN_B_SIZE_Y + B_INIT_JUMP_SIZE_Y);
		    }
		    else{
		    	aliens[i][j].type = 'C';
		    	aliens[i][j].coord.coordX = C_INIT_JUMP_SIZE_X/2 + j * (ALIEN_C_SIZE_X + C_INIT_JUMP_SIZE_X);
		    	aliens[i][j].coord.coordY = i * (ALIEN_C_SIZE_Y + C_INIT_JUMP_SIZE_Y);
		    }
		}
	}
}

void updateAliensBlock(alien_t * aliens[ALIEN_ROWS][ALIEN_COLS], alienBlock_t * aliensBlock){
	
	uint8_t alienColAlive = 0;
	uint8_t alienRowAlive = 0; 
	uint8_t i, j; 
	
	//ACTUALIZO FIRSTCOLALIVE
	for(i = 0; i < ALIEN_ROWS; i++) { // recorro cada fila
		if(aliens[i][aliensBlock->firstColAlive]->alive){
			alienColAlive++; 
		}
	}
	if(!alienColAlive){			//si esa columna ya no tiene aliens vivos
		aliensBlock->firstColAlive++; 		//actualizo la primera columna que tenga aliens vivos
	}
	
	alienColAlive = 0; 
	
	//ACTUALIZO LASTCOLALIVE
	for(i = 0; i < ALIEN_ROWS; i++) { // recorro cada fila
		if(aliens[i][aliensBlock->lastColAlive]->alive){	//si nunca entra al if, alienColAlive queda en 0
			alienColAlive++; 
		}
	}
	if(!alienColAlive){			//si esa columna ya no tiene aliens vivos
		aliensBlock->lastColAlive--; 		//actualizo la primera columna que tenga aliens vivos
	}

	//ACTUALIZO WIDTH
	aliensBlock->width = (aliensBlock->lastColAlive - aliensBlock->firstColAlive) * (ALIEN_B_SIZE_X + B_INIT_JUMP_SIZE_X) + ALIEN_B_SIZE_X;
	
	//ACTUALIZO LASTROWALIVE
	for(j = 0; j < ALIEN_COLS; i++) { // recorro cada fila
		if(aliens[aliensBlock->lastRowAlive][j]->alive){
			alienRowAlive++; 
		}
	}
	if(!alienRowAlive){			//si esa fila ya no tiene aliens vivos
		aliensBlock->lastRowAlive--; 		
	}
}

void blockMove(alien_t aliens[ALIEN_ROWS][ALIEN_COLS], alienBlock_t * aliensBlock){ 
		
	if(aliensBlock->direction==1 && ((aliens[0][aliensBlock->firstColAlive].coord.coordX + aliensBlock->width + aliensBlock->coord.coordX) >= DISPLAY_LENGTH - DISPLAY_MARGIN_X)){	//verifico si llego al limite derecho
		aliensBlock->direction = -1; 		//cambio de direccion
		aliensBlock->coord.coordY += JUMP_SIZE_Y;	//salto abajo
	}	
	else if(aliensBlock->direction==-1 && ((aliens[0][aliensBlock->firstColAlive].coord.coordX + aliensBlock->coord.coordX )<= DISPLAY_MARGIN_X)){	//verifico si llego al limite izquierdo
		aliensBlock->direction = 1; 		//cambio de direccion
		aliensBlock->coord.coordY += JUMP_SIZE_Y;	//salto abajo
	}
	else{
		aliensBlock->coord.coordX += JUMP_SIZE_X * aliensBlock->direction;		//suma o resta dependiendo de hacia donde tiene que ir
	}
}
