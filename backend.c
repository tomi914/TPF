#include<stdbool.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>

alienBlock_t aliensBlock = {0, 0, 1, 0, ALIEN_COLS - 1};	//inicializo los datos del bloque de aliens chequear coordX y coordY

//las imagenes tienen distinto tamaño por tipo de alien, corregir la función
//creo que esta bien, chequear con los muchachos. 
void initAliensArray(alien_t * aliens[ALIEN_ROWS][ALIEN_COLS]){	//recibe un puntero al arreglo de aliens(xq en el stack? porque es mas rapido y no tenemos cant. variable)

	int i, j;
	
	for(i = 0; i < ALIEN_ROWS; i++) { // Itero sobre una fila 
		for(j = 0; j < ALIEN_COLS; j++) { // Itero sobre una columna 
			aliens[i][j].alive = true; // todos arrancan vivos
			//aliens[i][j].coord.coordX = j * JUMP_SIZE + DISPLAY_MARGIN_X;  	// seteo coordenadas en X con el espaciado definido en JUMP_SIZE
			//aliens[i][j].coord.coordY = i * JUMP_SIZE + DISPLAY_MARGIN_Y; 	 // seteo coordenadas en Y con el espaciado definido en JUMP_SIZE
			if(i == 0){
		    	aliens[i][j].type = 'A';		//en allegro cada tipo corresponde a una imagen diferente
		    	aliens[i][j].coord.coordX = j * A_INIT_JUMP_SIZE_X + DISPLAY_MARGIN_X;	//chequear que este bien visualmente
		    	aliens[i][j].coord.coordY = i * A_INIT_JUMP_SIZE_Y + DISPLAY_MARGIN_Y;
		    }
		    else if(i < 3){
		    	aliens[i][j].type = 'B';
		    	aliens[i][j].coord.coordX = j * B_INIT_JUMP_SIZE_X + DISPLAY_MARGIN_X;
		    	aliens[i][j].coord.coordY = i * B_INIT_JUMP_SIZE_Y + DISPLAY_MARGIN_Y;
		    }
		    else{
		    	aliens[i][j].type = 'C';
		    	aliens[i][j].coord.coordX = j * C_INIT_JUMP_SIZE_X + DISPLAY_MARGIN_X;
		    	aliens[i][j].coord.coordY = i * C_INIT_JUMP_SIZE_Y + DISPLAY_MARGIN_Y;
		    }
		}
	}
}

void alienMove(alien_t * aliens[ALIEN_ROWS][ALIEN_COLS]){ 

	uint8_t alienColAlive = 0; 
	uint8_t i; 
	
	//analizo unicamente la columna del extremo izquierdo
	for(i = 0; i < ALIEN_ROWS; i++) { // recorro cada fila
		if(aliens[i][aliensBlock.firstColAlive].alive){
			alienColAlive++; 
		}
	}
	if(!alienColAlive){			//si esa columna ya no tiene aliens vivos
		aliensBlock.firstColAlive++; 		//actualizo la primera columna que tenga aliens vivos
	}
	
	alienColAlive = 0; 
	
	//analizo unicamente la columna del extremo derecho
	for(i = 0; i < ALIEN_ROWS; i++) { // recorro cada fila
		if(aliens[i][aliensBlock.lastColAlive].alive){	//si nunca entra al if, alienColAlive queda en 0
			alienColAlive++; 
		}
	}
	if(!alienColAlive){			//si esa columna ya no tiene aliens vivos
		aliensBlock.lastColAlive--; 		//actualizo la primera columna que tenga aliens vivos
	}
	
	//chequeo de cambio de nivel
	if(aliensBlock.firstColAlive>aliensBlock.lastColAlive){
		//se sube de nivel porque todos los aliens estan muertos
	}
	
	//hay que ver con que ALIEN_..._SIZE_X y que ..._INIT_SIZE_X definimos esta variable
	//(creo que puede ser con cualquiera y DISPLAY_MARGIN_X absorbe el error)
	int alienRowLength = (aliensBlock.lastColAlive - aliensBlock.firstColAlive) * (ALIEN_B_SIZE_X + B_INIT_JUMP_SIZE_X) + ALIEN_B_SIZE_X;	//se almacena cuantas coord de largo tiene c/fila
	
	//hago movimientos
	if(aliensBlock.direction==1 && ((aliensBlock.coordX + alienRowLength) >= DISPLAY_LENGTH - DISPLAY_MARGIN_X)){	//verifico si llego al limite derecho
		aliensBlock.direction = -1; 		//cambio de direccion
		aliensBlock.coordY += JUMP_SIZE_Y;	//salto abajo
	}	
	else if(aliensBlock.direction==-1 && (aliensBlock.coordX <= DISPLAY_MARGIN_X)){	//verifico si llego al limite izquierdo
		aliensBlock.direction = 1; 		//cambio de direccion
		aliensBlock.coordY += JUMP_SIZE_Y;	//salto abajo
	}
	else{
		aliensBlock.coordX += JUMP_SIZE_X * aliensBlock.direction;		//suma o resta dependiendo de hacia donde tiene que ir
	}
}

//creo que esta bien, chequear con los muchachos. 
void resetAliensBlock(void){
	aliensBlock.coordX = 0;
    aliensBlock.coordY = 0;
    aliensBlock.direction = 1;
    aliensBlock.firstColAlive = 0;
    aliensBlock.lastColAlive = ALIEN_COLS - 1;
}

//creo que esta bien, chequear con los muchachos. 
void initPlayer(player_t * player){		//OPCIONAL: JUGAR DE A DOS, como se podria hacer? 
	
	player->health = 3;
	player->coord.coordX = DISPLAY_LENGTH / 2;	//asi arranca al medio
	player->coord.coordY = /*depende la interfaz*/;
	
}

//revisar logica y compatibilidad


void playerMove(int dire, player_t * player){
	if(dire<0 && player->coord.coordX>=0/*ver bien los limites*/){
		player->coord.coordX -= JUMP_SIZE_X; //definir si el jump va a ser el mismo q los aliens
	}
	if(dire>0 && player.coord.coordX<(DISPLAY_LENGTH-PLAYER_SIZE_W)/*ver bien los lim*/){
		player->coord.coordX += JUMP_SIZE_X;
	}
}

void playerShoot(bool * shooting, bullet_t * bullet, player_t * player){
	static bool tryShoot = false;
	if (tryShoot && !(*shooting)) {
			*shooting = true;
			tryShoot = false;
			bullet->coord.coordY = player->coord.coordY;
			bullet->coord.coordX = player->coord.coordX - SHOT_SIZE_W / 2;  // fijar X en el momento del disparo
		}
	if (*shooting) {
			bullet->coord.coordY -= speedShot;//definir speed
			if (bullet->coord.coordY < 0) {
				*shooting = false;
				}
		}
}




