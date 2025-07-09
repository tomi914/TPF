#include<stdbool.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>

//inicializar en el front: 
//	- alienBlock_t aliensBlock
//	- alien_t * aliens[ALIEN_ROWS][ALIEN_COLS]
// 	- shield_t * shields[NUM_SHIELDS]
//	- bullet_t * bulletPlayer
//	- bullet_t * bulletAlien

//creo que esta bien, chequear con los muchachos. 
void initAliensBlock(alienBlock_t * aliensBlock){
	aliensBlock->coordX = MARGIN_X;
    aliensBlock->coordY = MARGIN_Y;
    aliensBlock->direction = 1;
    aliensBlock->firstColAlive = 0;
    aliensBlock->lastColAlive = ALIEN_COLS - 1;
    aliensBlock->lastRowAlive = ALIEN_ROWS - 1; 
}

//al inicializar las coordenadas hay que tomar en cuenta el tamaño que ocupa cada alien y no solo los saltos
//creo que sería = (i*INIT_JUMP_SIZE)+((i-1)*ALIEN_SIZE)+DISPLAY_MARGIN 
//HAY QUE CORREGIRLA
void initAliensArray(alien_t * aliens[ALIEN_ROWS][ALIEN_COLS]){	//recibe un puntero al arreglo de aliens(xq en el stack? porque es mas rapido y no tenemos cant. variable)

	int i, j;
	
	for(i = 0; i < ALIEN_ROWS; i++) { // Itero sobre una fila 
		for(j = 0; j < ALIEN_COLS; j++) { // Itero sobre una columna 
			aliens[i][j].alive = true; // todos arrancan vivos
			
			if(i == 0){
		    	aliens[i][j].type = 'A';		//en allegro cada tipo corresponde a una imagen diferente
		    	aliens[i][j].coord.coordX = j * (ALIEN_A_SIZE_X + A_INIT_JUMP_SIZE_X);	//chequear que este bien visualmente
		    	aliens[i][j].coord.coordY = i * (ALIEN_A_SIZE_Y + A_INIT_JUMP_SIZE_Y);
		    }
		    else if(i < 3){
		    	aliens[i][j].type = 'B';
		    	aliens[i][j].coord.coordX = j * (ALIEN_B_SIZE_X + B_INIT_JUMP_SIZE_X);
		    	aliens[i][j].coord.coordY = i * (ALIEN_B_SIZE_Y + B_INIT_JUMP_SIZE_Y);
		    }
		    else{
		    	aliens[i][j].type = 'C';
		    	aliens[i][j].coord.coordX = j * (ALIEN_C_SIZE_X + C_INIT_JUMP_SIZE_X);
		    	aliens[i][j].coord.coordY = i * (ALIEN_C_SIZE_Y + C_INIT_JUMP_SIZE_Y);
		    }
		}
	}
}

void initShieldsArray(shield_t shields[NUM_SHIELDS]){	//recibe un puntero al arreglo de escudos definido en el front

	int i; 
	
	for(i=0;i<NUM_SHIELDS;i++){
		shields[i].health = 15; 	//le seteo la cantidad de vidas
		shields[i].coord.coordY = SHIELD_INIT_Y; 	//lo ubico en el display
		shields[i].coord.coordX = MARGIN_X + SHIELD_INIT_X_JUMP/2 + (SHIELD_INIT_X_JUMP * i); 
		shields[i].sizeX = SHIELD_INIT_SIZE_X;
		shields[i].sizeY = SHIELD_INIT_SIZE_Y;
		
	}
}

//creo que esta bien, chequear con los muchachos. 
void initPlayer(player_t * player){		//OPCIONAL: JUGAR DE A DOS, como se podria hacer? 
	
	player->health = 3;
	player->coord.coordX = DISPLAY_LENGTH / 2;	//asi arranca al medio
	player->coord.coordY = INIT_PLAYER_Y;
	
}

void blockMove(alien_t * aliens[ALIEN_ROWS][ALIEN_COLS], alienBlock_t * aliensBlock){ 

	uint8_t alienColAlive = 0; 
	uint8_t i; 
	
	//analizo unicamente la columna del extremo izquierdo
	for(i = 0; i < ALIEN_ROWS; i++) { // recorro cada fila
		if(aliens[i][aliensBlock->firstColAlive].alive){
			alienColAlive++; 
		}
	}
	if(!alienColAlive){			//si esa columna ya no tiene aliens vivos
		aliensBlock->firstColAlive++; 		//actualizo la primera columna que tenga aliens vivos
	}
	
	alienColAlive = 0; 
	
	//analizo unicamente la columna del extremo derecho
	for(i = 0; i < ALIEN_ROWS; i++) { // recorro cada fila
		if(aliens[i][aliensBlock->lastColAlive].alive){	//si nunca entra al if, alienColAlive queda en 0
			alienColAlive++; 
		}
	}
	if(!alienColAlive){			//si esa columna ya no tiene aliens vivos
		aliensBlock->lastColAlive--; 		//actualizo la primera columna que tenga aliens vivos
	}
	
	//chequeo de cambio de nivel
	if(aliensBlock->firstColAlive>aliensBlock->lastColAlive){
		//se sube de nivel porque todos los aliens estan muertos
	}
	
	//hay que ver con que ALIEN_..._SIZE_X y que ..._INIT_SIZE_X definimos esta variable
	//(creo que puede ser con cualquiera y DISPLAY_MARGIN_X absorbe el error)
	int alienRowLength = (aliensBlock->lastColAlive - aliensBlock->firstColAlive) * (ALIEN_B_SIZE_X + B_INIT_JUMP_SIZE_X) + ALIEN_B_SIZE_X;	//se almacena cuantas coord de largo tiene c/fila
	
	//hago movimientos
	//en borde derecho: comparo coordX del bloque + coordX de la primera fila viva + largo del bloque con el largo del display menos el margen
	//en borde izquierdo: comparo coordX del bloque + coordX de la primera fila viva con el margen 
	if(aliensBlock->direction==1 && ((aliens[0][aliensBlock->firstColAlive].coord.coordX + alienRowlength + aliensBlock->coordX) >= DISPLAY_LENGTH - DISPLAY_MARGIN_X)){	//verifico si llego al limite derecho
		aliensBlock->direction = -1; 		//cambio de direccion
		aliensBlock->coordY += JUMP_SIZE_Y;	//salto abajo
	}	
	else if(aliensBlock->direction==-1 && ((aliens[0][aliensBlock->firstColAlive].coord.coordX + aliensBlock->coordX )<= DISPLAY_MARGIN_X)){	//verifico si llego al limite izquierdo
		aliensBlock->direction = 1; 		//cambio de direccion
		aliensBlock->coordY += JUMP_SIZE_Y;	//salto abajo
	}
	else{
		aliensBlock->coordX += JUMP_SIZE_X * aliensBlock->direction;		//suma o resta dependiendo de hacia donde tiene que ir
	}
}

/*	verifica superposicion de rectangulos
	A y B entidades a analizar
	X e Y coordenadas reales de las entidades
	W y H Width y High tamaño de las entidades
	devuelve 1 si hay superposicion, sino 0 
*/
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

void collisionDetect(void){
	//aca van las funciones que chequean todas las colisiones posibles
}

//chequea bala del jugador con todos los aliens
void collisionBA(bullet_t * bullet , alien_t * aliens[ALIEN_ROWS][ALIEN_COLS], alienBlock_t * aliensBlock){	

	int i, j; 
	
	if(bullet->active){	//verifico que la bala del jugador este activa
		for(i=(aliensBlock->lastRowAlive); i>=0 ; i--){	//recorro filas de aliens desde abajo hacia arriba
			for(j=aliensBlock->firstColAlive; j<=aliensBlock->lastColAlive ; j++){	//recorro columnas de aliens sin analizar las que ya murieron
				if(aliens[i][j].alive){	//verifico que el alien este vivo
				
					//creo variables intermedias por claridad
					uint16_t alienX = aliens[i][j].coord.coordX + aliensBlock->coordX;
					uint16_t alienY = aliens[i][j].coord.coordY + aliensBlock->coordY;
					uint16_t bulletX = bullet->coord.coordX;
					uint16_t bulletY = bullet->coord.coordY;
					
					//chequeo superposicion de rectangulos, si hay, desactivo la bala y mato al alien
					if(rectangleOverlap(alienX, ALIEN_B_SIZE_X, bulletX, BULLET_SIZE_X,
                     					alienY, ALIEN_B_SIZE_Y, bulletY, BULLET_SIZE_Y)){
						aliens[i][j].alive = false; 
						bullet->active = false; 
						return;
						
					}
				}
			}
		}
	}	
}

//chequea bala del jugador con bala de alien
void collisionBB(bullet_t * bulletP , bullet_t * bulletA){	

	if(bulletP->active && bulletA->active){	//veo si las dos balas estan activas
		//creo variables intermedias por claridad
		uint16_t bulletPX = bulletP->coord.coordX;
		uint16_t bulletPY = bulletP->coord.coordY;
		uint16_t bulletAX = bulletA->coord.coordX;
		uint16_t bulletAY = bulletA->coord.coordY;
		
		//chequeo superposicion de rectangulos, si hay, desactivo las balas
		if(rectangleOverlap(bulletAX, BULLET_SIZE_X,bulletPX, BULLET_SIZE_X,
         					bulletAY, BULLET_SIZE_Y, bulletPY, BULLET_SIZE_Y)){
			bulletA->active = false; 
			bulletP->active = false; 
			return;
		}
	}
}

//chequea colision entre aliens y escudos
void collisionAS(alien_t * aliens[ALIEN_ROWS][ALIEN_COLS], shield_t * shields[NUM_SHIELDS], alienBlock_t * aliensBlock){

	int j, k; 
	
	//solo analizamos la colision con la ultima fila viva, no con las otras
	for(j=aliensBlock->firstColAlive; j<=aliensBlock->lastColAlive ; j++){
		for(k=0; k<NUM_SHIELDS; k++){
			
			if(aliens[aliensBlock->lastRowAlive][j]. alive && shields[k].health){//verifico que el alien este vivo y que el escudo tambien
			
				//variables intermedias para mejor comprension
				uint16_t alienX = aliens[aliensBlock->lastRowAlive][j].coord.coordX + aliensBlock->coordX;
				uint16_t alienY = aliens[aliensBlock->lastRowAlive][j].coord.coordY + aliensBlock->coordY;
				uint16_t shieldX = shields[k].coord.coordX;
				uint16_t shieldW = shields[k].sizeX;
				uint16_t shieldY = shields[k].coord.coordY;
				uint16_t shieldH = shields[k].sizeY;
				
				if(rectangleOverlap(alienX, ALIEN_B_SIZE_X, shieldX, shieldW, alienY, ALIEN_B_SIZE_Y, shieldY, shieldH)){
					shields[k].health -= 2; //ver cuanto conviene decrementar las vidas luego 
				}	
			}		
		}
	}	
}

//solo se usa en la pc, no en la raspberry
void shieldsUpdate(shield_t * shields[NUM_SHIELDS]){
	//esta funcion analiza la vida que le queda a cada escudo y actualiza su tamaño
	int k; 
	
	for(k=0; k<NUM_SHIELDS; k++){
		if(shields[k].health > 12){
			shields[k].sizeX = /*nuevo tamaño*/;
			shields[k].sizeY = /*nuevo tamaño*/;
		}
		else if(shields[k].health > 9){
			shields[k].sizeX = /*nuevo tamaño*/;
			shields[k].sizeY = /*nuevo tamaño*/;
		}
		else if(shields[k].health > 6){
			shields[k].sizeX = /*nuevo tamaño*/;
			shields[k].sizeY = /*nuevo tamaño*/;
		}
		else if(shields[k].health > 3){
			shields[k].sizeX = /*nuevo tamaño*/;
			shields[k].sizeY = /*nuevo tamaño*/;
		}
		else{
			shields[k].sizeX = /*nuevo tamaño*/;
			shields[k].sizeY = /*nuevo tamaño*/;
		}
	}
}

/* 
Función que elige que alien va a ser el que va a disparar.
 - Elije el que tenga su coordenada en X mas cerca del jugador.
 - Elije el que esta más abajo dentro de una columna.
*/
alien_t * selectAlienShooter (alien_t * alien[ALIEN_ROWS][ALIEN_COLS], alienBlock_t * alienBlock, player_t * player){
	
	alien_t * bestCandidate == NULL; //Almacena la dirección de memoria del alien que es el mejor candidato a disparar.
	int minDistX = DISPLAY_LENGTH; //Almacena la mínima distancia en X entre un alien y el player. Se incializa con una distancia más grande que cualquier otra posible.

	for (int col = alienBlock->firstColAlive; col <= alienBlock->lastColAlive; col++){
		bool found = false; //Flag para dejar de buscar en una columna una vez que el alien se encontró.

		for (int row = alienBlock->lastRowAlive; row >= 0 && found == false; row--){
			if (alien[row][col] != NULL && alien[row][col]->alive == true){
				//Se almacenan las coordenadas en X desde el alien y del player.
				int alienX = alien[row][col]->coord.coordX;
				int playerX = player->coord.coordX;

				//Se almacena la distancia entre el alien y el player.
				int distX = abs(alienX-playerX); //abs() pertenece a <stdlib.h>

				//Se analiza si el actual alien es la mejor opción.
				if (distX < minDistX){
					minDistX = distX; //Nueva mínima distancia.
					bestCandidate = alien[row][col]; //Nuevo mejor candidato para disparar.
				}

				found = true; //Se encontró el alien vivo de más abajo en esta columna.
			}

		}
	}
	return bestCandidate; //En caso de que no queden más aliens, se devuelve NULL.
}

/* 
Función que define cómo va a a disparar el alien 
*/
void alienShoot (bullet_t * bullet, alien_t * alien, int level){ 
	
	if (bullet->active == false){ //Si NO hay bala activa, crear una nueva.
		bullet->active = true;
		bullet->coord.coordY = alien->coord.coordY; // Dispara desde la parte mas alta del alien (lo atraviesa).
		bullet->coord.coordX = alien->coord.coordX + ALIEN_B_SIZE_X/2; // Dispara desde el centro en X del alien (le sumo la mitad del largo del alien B, que es el de tamaño intermedio).
	}
  
	// Si está activa la muevo en x;
	if (bullet->active == true){
		bullet->coord.coordY += SPEED_BULLET_ALIEN(level);

		// Si se sale del display, desactivo la bala.
		if (bullet->coord.coordY > DISPLAY_HIGH - DISPLAY_MARGIN_Y){
			bullet->active = false;
		}
	}
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




