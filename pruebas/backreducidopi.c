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

void updateAliensBlock(alien_t aliens[ALIEN_ROWS][ALIEN_COLS], alienBlock_t * aliensBlock){
	
	uint8_t alienColAlive = 0;
	uint8_t alienRowAlive = 0; 
	uint8_t i, j; 
	
	//ACTUALIZO FIRSTCOLALIVE
	for(i = 0; i < ALIEN_ROWS; i++) { // recorro cada fila
		if(aliens[i][aliensBlock->firstColAlive].alive){
			alienColAlive++; 
		}
	}
	if(!alienColAlive){			//si esa columna ya no tiene aliens vivos
		aliensBlock->firstColAlive++; 		//actualizo la primera columna que tenga aliens vivos
	}
	
	alienColAlive = 0; 
	
	//ACTUALIZO LASTCOLALIVE
	for(i = 0; i < ALIEN_ROWS; i++) { // recorro cada fila
		if(aliens[i][aliensBlock->lastColAlive].alive){	//si nunca entra al if, alienColAlive queda en 0
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
		if(aliens[aliensBlock->lastRowAlive][j].alive){
			alienRowAlive++; 
		}
	}
	if(!alienRowAlive){			//si esa fila ya no tiene aliens vivos
		aliensBlock->lastRowAlive--; 		
	}
}

void blockMove(alien_t aliens[ALIEN_ROWS][ALIEN_COLS], alienBlock_t * aliensBlock){ 
	static float jumpx = 0;	
	static float jumpy = 0;	
	if(aliensBlock->direction==1 && ((aliens[0][aliensBlock->firstColAlive].coord.coordX + aliensBlock->width + aliensBlock->coord.coordX) >= DISPLAY_LENGTH - DISPLAY_MARGIN_X)){	//verifico si llego al limite derecho
		jumpy += JUMP_SIZE_Y;
		if(jumpy>=1){
			aliensBlock->direction = -1; 		//cambio de direccion
			aliensBlock->coord.coordY += (int)jumpy;	//salto abajo
			jumpy = 0;
		}
		
	}	
	else if(aliensBlock->direction==-1 && ((aliens[0][aliensBlock->firstColAlive].coord.coordX + aliensBlock->coord.coordX )<= DISPLAY_MARGIN_X)){	//verifico si llego al limite izquierdo
		jumpy += JUMP_SIZE_Y;
		if(jumpy>=1){
			aliensBlock->direction = 1; 		//cambio de direccion
			aliensBlock->coord.coordY += (int)jumpy;	//salto abajo
			jumpy = 0;
		}
	}
	else{
		jumpx += JUMP_SIZE_X;
		aliensBlock->coord.coordX += (int)jumpx * aliensBlock->direction;		//suma o resta dependiendo de hacia donde tiene que ir
		if(jumpx>=1){
			jumpx = 0;
		}
	}
}

//Inicializo los escudos
void initShieldsArray(shield_t shields[NUM_SHIELDS]){	//recibe un puntero al arreglo de escudos definido en el front

	int i; //probado en allegro
	
	for(i=0;i<NUM_SHIELDS;i++){
		shields[i].health = 15; 	//le seteo la cantidad de vidas
		shields[i].coord.coordY = SHIELD_INIT_Y; 	//lo ubico en el display
		shields[i].coord.coordX = DISPLAY_MARGIN_X + SHIELD_INIT_X_JUMP/2 + (SHIELD_INIT_X_JUMP * i); 
		shields[i].sizeX = SHIELD_INIT_SIZE_X;
		shields[i].sizeY = SHIELD_INIT_SIZE_Y;
		
	}
}

char rectangleOverlap(uint16_t AX, uint16_t AW, uint16_t BX, uint16_t BW, uint16_t AY, uint16_t AH, uint16_t BY, uint16_t BH){
	if(	AX <= BX + BW
	&&	BX <= AX + AW
	&&	AY <= BY + BH
	&&	BY <= AY + AH){
		return 1;
	}
	else{
		return 0; 
	}
}

//analiza todas las colisiones
/*void collisionDetect(bullet_t * bulletP, bullet_t * bulletA, alien_t * aliens[ALIEN_ROWS][ALIEN_COLS], ovni_t * ovni, shield_t * shields[NUM_SHIELDS], alienBlock_t * aliensBlock, player_t * player, stats_t * gameStats, uint8_t printedRow){
	if(bulletP->active){
		collisionBA(bulletP, aliens, aliensBlock, gameStats, printedRow); //bullet vs aliens
		collisionBB(bulletP, bulletA); //bullet vs bullet
		collisionBO(bulletP, ovni, gameStats); //bullet vs ovni
	}
	collisionAS(aliens, shields, aliensBlock); //aliens vs shields
	collisionBP(bulletA, player); //bullet vs player
	collisionAP(player, aliens, aliensBlock); //aliens vs player
}*/

//chequea bala del jugador con todos los aliens
//ver si en necesario analzar si por separado en Y cuando baja (baja de a una fila)
void collisionBA(bullet_t * bullet, alien_t aliens[ALIEN_ROWS][ALIEN_COLS], alienBlock_t * aliensBlock, stats_t * gameStats, int printedRow) {	

	if(bullet->coord.coordY < DISPLAY_MARGIN_Y){
		bullet->active = false; 
	}

	for (int row = aliensBlock->lastRowAlive; row >= 0; row--) { // Recorro filas desde abajo hacia arriba
		for (int col = aliensBlock->firstColAlive; col <= aliensBlock->lastColAlive; col++) { // Recorro columnas activas
			if (aliens[row][col].alive && bullet->active) { // Verifico que el alien esté vivo y la bala activa
				
				// Variables intermedias por claridad
				uint16_t alienX; 
				uint16_t alienY = aliens[row][col].coord.coordY + aliensBlock->coord.coordY;
				uint16_t bulletX = bullet->coord.coordX;
				uint16_t bulletY = bullet->coord.coordY;
				
				//distingo si esa fila ya se movio o no
				if(row >= printedRow){	
					alienX = aliens[row][col].coord.coordX + aliensBlock->coord.coordX;
				}
				else if(aliensBlock->direction == 1){	//si todavia no se imprimio la fila desplazada, comparo con las coordenadas anteriores
					alienX = aliens[row][col].coord.coordX + aliensBlock->coord.coordX - JUMP_SIZE_X;
				}
				else if(aliensBlock->direction == -1){	//si todavia no se imprimio la fila desplazada, comparo con las coordenadas anteriores
					alienX = aliens[row][col].coord.coordX + aliensBlock->coord.coordX + JUMP_SIZE_X;
				}
				
				// Chequeo superposición de rectángulos
				if (rectangleOverlap(alienX, ALIEN_B_SIZE_X, bulletX, BULLET_SIZE_X, alienY, ALIEN_B_SIZE_Y, bulletY, BULLET_SIZE_Y)){
					
					aliens[row][col].alive = false; 
					bullet->active = false;
					
					// Asigno puntos
					if (gameStats != NULL){
				        switch (aliens[row][col].type){
			                case 'A': gameStats->actualScore += POINTS_ALIEN_A; break;
			                case 'B': gameStats->actualScore += POINTS_ALIEN_B; break;
			                case 'C': gameStats->actualScore += POINTS_ALIEN_C; break;
			                default: break;
				        }
					}
					return;
				}
			}
		}
	}	
}
//chequea bala del jugador con bala de alien
void collisionBB(bullet_t * bulletP , bullet_t * bulletA){	

	if(bulletA->active){	//veo si la bala del alien tambien esta activa
		//creo variables intermedias por claridad
		uint16_t bulletPX = bulletP->coord.coordX;
		uint16_t bulletPY = bulletP->coord.coordY;
		uint16_t bulletAX = bulletA->coord.coordX;
		uint16_t bulletAY = bulletA->coord.coordY;
		
		//chequeo superposicion de rectangulos, si hay, desactivo las balas
		if(rectangleOverlap(bulletAX, BULLET_SIZE_X,bulletPX, BULLET_SIZE_X, bulletAY, BULLET_SIZE_Y, bulletPY, BULLET_SIZE_Y)){
			bulletA->active = false; 
			bulletP->active = false; 
			return;
		}
	}
}

// Chequea colisión entre aliens y escudos
/*void collisionAS (alien_t * aliens[ALIEN_ROWS][ALIEN_COLS], shield_t * shields[NUM_SHIELDS], alienBlock_t * aliensBlock){ 
	
	// Solo analizamos la colision con la ultima fila viva
	for(int col = aliensBlock->firstColAlive; col <= aliensBlock->lastColAlive ; col++){
		for(int s=0; s < NUM_SHIELDS; s++){
			
			if (aliens[aliensBlock->lastRowAlive][col].alive && shields[s].health){ // Verifico que el alien este vivo y que el escudo también
			
				// Posiciones
				uint16_t alienX = aliens[aliensBlock->lastRowAlive][col].coord.coordX + aliensBlock->coord.coordX;
				uint16_t alienY = aliens[aliensBlock->lastRowAlive][col].coord.coordY + aliensBlock->coord.coordY;
				uint16_t shieldX = shields[s].coord.coordX;
				uint16_t shieldW = shields[s].sizeX;
				
				// Dimensiones
				uint16_t shieldY = shields[s].coord.coordY;
				uint16_t shieldH = shields[s].sizeY;
				
				// Detección de colisión
				if(rectangleOverlap(alienX, ALIEN_B_SIZE_X, shieldX, shieldW, alienY, ALIEN_B_SIZE_Y, shieldY, shieldH)){
					shields[s].health -= 2; //ver cuanto conviene decrementar las vidas luego 
				}	
			}		
		}
	}	
}*/

// Recibe la bala del alien y el jugador 
void collisionBP (bullet_t * bullet, player_t * player){

	if (bullet->active){	//me fijo que la bala del alien este activa
		
		//variables intermedias para mejor comprension
		uint16_t bulletX = bullet->coord.coordX;
		uint16_t bulletY = bullet->coord.coordY;
		uint16_t playerX = player->coord.coordX;
		uint16_t playerY = player->coord.coordY;
		
		if(rectangleOverlap(playerX, PLAYER_SIZE_X, bulletX, BULLET_SIZE_X, playerY, PLAYER_SIZE_Y, bulletY, BULLET_SIZE_Y)){
			
			player->health--; //le saco una vida
			bullet->active = false; 
			
			if(!(player->health)){	//si se quedo sin vidas
				//lamo a funcion de gameover
			}
		}
	}
}

//recibe el jugador y los aliens
/*void collisionAP (player_t * player, alien_t * aliens[ALIEN_ROWS][ALIEN_COLS], alienBlock_t * aliensBlock){
	
	int j;
	
	//solo comparo con la ultima fila viva
	for(j=aliensBlock->firstColAlive; j<=aliensBlock->lastColAlive; j++){
		if(aliens[aliensBlock->lastRowAlive][j].alive){	//si el alien esta vivo 
		
			//variables intermedias para mejor comprension
			uint16_t playerX = player->coord.coordX;
			uint16_t playerY = player->coord.coordY;
			uint16_t alienX = aliens[aliensBlock->lastRowAlive][j].coord.coordX + aliensBlock->coord.coordX;
			uint16_t alienY = aliens[aliensBlock->lastRowAlive][j].coord.coordY + aliensBlock->coord.coordY;
			
			if((alienY + ALIEN_B_SIZE_Y >= playerY) || rectangleOverlap(playerX, PLAYER_SIZE_X, alienX, ALIEN_B_SIZE_X, playerY, PLAYER_SIZE_Y, alienY, ALIEN_B_SIZE_Y)){
				//lamo a funcion de gameover
			}
		}
	}
	
}*/

// Colisión entre bala y ovni

#define BULLET_INDEX(n) (((n) - 1) % 16) // Devuelve un numero entre 0 y 15 de acuerdo a la bala que se disparó

const int puntosOvni[16] = {100, 50, 50, 100, 150, 100, 100, 50, 300, 100, 100, 100, 50, 150, 100, 50}; //Son los valores que se usan en el juego real.

/*void collisionBO (bullet_t * bullet, ovni_t * ovni, player_t * player, stats_t * gameStats){

    if (bullet->active && ovni->visible && ovni->alive){
    
        // Variables intermedias por claridad
        uint16_t ovniX = ovni->coord.coordX;
		uint16_t ovniY = ovni->coord.coordY;
		uint16_t bulletX = bullet->coord.coordX;
		uint16_t bulletY = bullet->coord.coordY;
			
        // Chequeo superposición de rectangulos
        if (rectangleOverlap(ovniX, OVNI_SIZE_X, bulletX, BULLET_SIZE_X, ovniY, OVNI_SIZE_Y, bulletY, BULLET_SIZE_Y)){
            
            bullet->active = false;
            ovni->visible = false;
            ovni->alive = false;
            
            if (gameStats != NULL && player != NULL){
                    gameStats->actualScore += puntosOvni[BULLET_INDEX(player->bulletsFired)];
            }
        }
    }
}*/

//Inicializo las estadisticas del juego
void initGameStats(stats_t * gameStats){
	gameStats->actualScore = 0;
	gameStats->level = 1; 
}
