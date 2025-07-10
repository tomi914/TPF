#include<stdbool.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>

//inicializar en el front: 
//	- alienBlock_t aliensBlock
//	- alien_t aliens[ALIEN_ROWS][ALIEN_COLS]
// 	- shield_t shields[NUM_SHIELDS]
//	- bullet_t bulletPlayer
//	- bullet_t bulletAlien
//	- stats_t gameStats

//Inicializo el bloque de aliens
void initAliensBlock(aliensBlock_t * aliensBlock){//probado en allegro
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

//Inicializo los escudos
void initShieldsArray(shield_t shields[NUM_SHIELDS]){	//recibe un puntero al arreglo de escudos definido en el front

	int i; //probado en allegro
	
	for(i=0;i<NUM_SHIELDS;i++){
		shields[i].health = 15; 	//le seteo la cantidad de vidas
		shields[i].coord.coordY = SHIELD_INIT_Y; 	//lo ubico en el display
		shields[i].coord.coordX = MARGIN_X + SHIELD_INIT_X_JUMP/2 + (SHIELD_INIT_X_JUMP * i); 
		shields[i].sizeX = SHIELD_INIT_SIZE_X;
		shields[i].sizeY = SHIELD_INIT_SIZE_Y;
		
	}
}

//Inicializo el jugador
void initPlayer(player_t * player){		//probado en allegro
	
	player->health = 3;
	player->coord.coordX = DISPLAY_LENGTH / 2;	//asi arranca al medio
	player->coord.coordY = DISPLAY_HIGH-DISPLAY_HIGH/8-PLAYER_SIZE_Y/2;
	
}

//Inicializo las estadisticas del juego
void initGameStats(stats_t * gameStats){
	gameStats->actualScore = 0;
	gameStats->level = 1; 
}

//Actualizo los datos del bloque de aliens 
void updateAliensBlock(alien_t * aliens[ALIEN_ROWS][ALIEN_COLS], aliensBlock_t * aliensBlock){
	
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

//Verifico si tengo que pasar al siguiente nivel
//creo que no hace falta pasarle las balas, ya deberian estar desactivadas
void newLevelCheck(alien_t aliens[ALIEN_ROWS][ALIEN_COLS], alienBlock_t * aliensBlock, player_t * player, shield_t shields[NUM_SHIELDS], stats_t * gameStats){
	
	//verifico si murieron todos para volver a inicializar todo, creo que con esto alcanza
	if(aliensBlock->firstColAlive > aliensBlock->lastColAlive){
		initAliensBlock(aliensBlock);
		initAliensArray(aliens);
		initPlayer(player);
		initShieldsArray(shields);
		
		gameStats->actualScore += LEVEL_POINTS(gameStats->level);
		gameStats->level++;
	}
	
}

//Muevo el bloque de aliens
//HACER QUE LA VELOCIDAD DEL MOVIMIENTO DEPENDA DEL NIVEL
void blockMove(alien_t aliens[ALIEN_ROWS][ALIEN_COLS], aliensBlock_t * aliensBlock){ 
		
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

//analiza todas las colisiones
void collisionDetect(bullet_t * bulletP, bullet_t * bulletA, alien_t * aliens[ALIEN_ROWS][ALIEN_COLS], ovni_t * ovni, shield_t * shields[NUM_SHIELDS], alienBlock_t * aliensBlock, player_t * player, stats_t * gameStats){
	if(bulletP->active){
		collisionBA(bulletP, aliens, aliensBlock, gameStats); //bullet vs aliens
		collisionBB(bulletP, bulletA); //bullet vs bullet
		collisionBO(bulletP, ovni, gameStats); //bullet vs ovni
	}
	collisionAS(aliens, shields, aliensBlock); //aliens vs shields
	collisionBP(bulletA, player); //bullet vs player
	collisionAP(player, aliens, aliensBlock); //aliens vs player
}

//chequea bala del jugador con todos los aliens
void collisionBA(bullet_t * bullet, alien_t * aliens[ALIEN_ROWS][ALIEN_COLS], alienBlock_t * aliensBlock, stats_t * gameStats) {	

	for (int row = aliensBlock->lastRowAlive; row >= 0; row--) { // Recorro filas desde abajo hacia arriba
		for (int col = aliensBlock->firstColAlive; col <= aliensBlock->lastColAlive; col++) { // Recorro columnas activas
			if (aliens[row][col].alive) { // Verifico que el alien esté vivo
				
				// Variables intermedias por claridad
				uint16_t alienX = aliens[row][col].coord.coordX + aliensBlock->coord.coordX;
				uint16_t alienY = aliens[row][col].coord.coordY + aliensBlock->coord.coordY;
				uint16_t bulletX = bullet->coord.coordX;
				uint16_t bulletY = bullet->coord.coordY;
				
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
void collisionAS (alien_t * aliens[ALIEN_ROWS][ALIEN_COLS], shield_t * shields[NUM_SHIELDS], alienBlock_t * aliensBlock){ 
	
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
}

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
void collisionAP (player_t * player, alien_t * aliens[ALIEN_ROWS][ALIEN_COLS], alienBlock_t * aliensBlock){
	
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
	
}

// Colisión entre bala y ovni

#define BULLET_INDEX(n) (((n) - 1) % 16) // Devuelve un numero entre 0 y 15 de acuerdo a la bala que se disparó

const int puntosOvni[16] = {100, 50, 50, 100, 150, 100, 100, 50, 300, 100, 100, 100, 50, 150, 100, 50}; //Son los valores que se usan en el juego real.

void collisionBO (bullet_t * bullet, ovni_t * ovni, player_t * player, stats_t * gameStats){

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
alien_t * selectAlienShooter (alien_t * alien[ALIEN_ROWS][ALIEN_COLS], alienBlock_t * aliensBlock, player_t * player){
	
	alien_t * bestCandidate == NULL; //Almacena la dirección de memoria del alien que es el mejor candidato a disparar.
	int minDistX = DISPLAY_LENGTH; //Almacena la mínima distancia en X entre un alien y el player. Se incializa con una distancia más grande que cualquier otra posible.

	for (int col = aliensBlock->firstColAlive; col <= aliensBlock->lastColAlive; col++){
		bool found = false; //Flag para dejar de buscar en una columna una vez que el alien se encontró.

		for (int row = aliensBlock->lastRowAlive; row >= 0 && found == false; row--){
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

//revisar logica y compatibilidad REVISADO

void playerMove(int dire, player_t * player){//probado en allegro
	if(dire<0 && player->coord.coordX>PLAYER_SIZE_X/2+SPEED_PLAYER/*ver bien los limites*/){
		player->coord.coordX -= SPEED_PLAYER; 
	}
	if(dire>0 && player->coord.coordX<(DISPLAY_LENGTH-PLAYER_SIZE_X/2-SPEED_PLAYER)/*ver bien los lim*/){
		player->coord.coordX += SPEED_PLAYER;
	}
}

void playerShoot(bullet_t * playerBullet, player_t * player, bool * tryShoot) {//probado en allegro
    if (*tryShoot && !playerBullet->active) {
       *tryShoot = false;
        playerBullet->active = true;
        playerBullet->coord.coordY = player->coord.coordY;
        playerBullet->coord.coordX = player->coord.coordX - BULLET_SIZE_X / 2;
        player->bulletsFired++; 
    }

    if (playerBullet->active) {
        playerBullet->coord.coordY -= SPEED_BULLET_PLAYER;
        if (playerBullet->coord.coordY < 10) {
            playerBullet->active = false;
            *tryShoot = false;
        }
    }
}

/* Se incializa el OVNI como no visible al inicio, y se prepara el temporizador para su primera aparición. */
void initOvni(ovni_t * ovni, double currentTime, double * LastOvniDespawnTime){
    ovni->visible = false; // El OVNI arranca invisible.
    ovni->alive = true;
    *LastOvniDespawnTime = currentTime; //Se comienza a contar desde el instante actual, para que luego aparezca el ovni.
}

/* Gestiona el movimiento y la reaparición del OVNI, fijandose si paso el tiempo de espera para la aparición (OVNI_SPAWN_INTERVAL -es constante-).
  -'lastOvniDespawnTime' marca el momento en que el OVNI dejó de ser visible (desapareció o fue destruido). Este valor es el punto de inicio para el "cooldown" o tiempo de espera antes de la próxima aparición del OVNI.
*/
void updateOvni (ovni_t * ovni, double currentTime, double * LastOvniDespawnTime){
  if (ovni->visible == false){ //Si el ovni no esta visible en pantalla
    if (currentTime - *LastOvniDespawnTime >= OVNI_SPAWN_INTERVAL) { //Si paso un tiempo definido luego de la ultima desaparición, se lo hace aparecer.
      ovni->visible = true;
      ovni->coord.coordY = DISPLAY_MARGIN_Y + INIT_OVNI_MARGIN_Y; //Le asigno una coordenada en Y un poco por debajo del márgen de arriba del display.
      ovni->coord.coordX = DISPLAY_LENGTH - DISPLAY_MARGIN_Y - INIT_OVNI_MARGIN_X; //Le asigno una coordenada en X un poco más a la izquierda del márgen de a la derecha del display.
    }
  }
  
  // Si está visible en pantalla lo muevo en x;
  if (ovni->visible == true){
    ovni->coord.coordX -= SPEED_OVNI;
  
    // Si se sale del display, lo pongo como invisible.
    if (ovni->coord.coordX + OVNI_SIZE_X < DISPLAY_MARGIN_X){
      ovni->visible = false;
      *LastOvniDespawnTime = currentTime; //Se almacena el tiempo en el que el ovni desaparece
    }
  }
}



