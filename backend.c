#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h> //para el clock
#include "entidades.h"

#ifdef pi
#include "constantes_pi.h"
#endif
#ifdef pc
#include "constantes_pc.h"
#endif

#include "backend.h"

//inicializar en el front: 
//	- alienBlock_t aliensBlock
//	- alien_t aliens[ALIEN_ROWS][ALIEN_COLS]
// 	- shield_t shields[NUM_SHIELDS]
//	- bullet_t bulletPlayer
//	- bullet_t bulletAlien
//	- stats_t gameStats


//******************************************************************||FUNCIONES DE USO GENERAL||**************************************************************************************
int getAlienWidthByRow(int row){
	if (row >= 4) return ALIEN_C_SIZE_X;
	if (row >= 2) return ALIEN_B_SIZE_X;
	return ALIEN_A_SIZE_X;
}

int getAlienHeightByRow(int row){
	if (row >= 4) return ALIEN_C_SIZE_Y;
	if (row >= 2) return ALIEN_B_SIZE_Y;
	return ALIEN_A_SIZE_Y;
}

float getAlienBulletSpeed(int level){
	return ((4 + (((level) - 1) / 2))/(float)BULLET_SPEED_ADJUSTMENT); 
}
//******************************************************************||FUNCIONES DE INICIALIZACION||**************************************************************************************

//Inicializo el bloque de aliens
void initAliensBlock(aliensBlock_t * aliensBlock){ 
	aliensBlock->coord.coordX = DISPLAY_MARGIN_X;
    aliensBlock->coord.coordY = DISPLAY_MARGIN_Y+INIT_ALIENS_MARGIN;
    aliensBlock->direction = 1;
    aliensBlock->firstColAlive = 0;
    aliensBlock->lastColAlive = ALIEN_COLS - 1;
    aliensBlock->lastRowAlive = ALIEN_ROWS - 1; 
    aliensBlock->width = (aliensBlock->lastColAlive - aliensBlock->firstColAlive) * (ALIEN_B_SIZE_X + B_INIT_JUMP_SIZE_X) + ALIEN_B_SIZE_X;
}


//Inicializo el array de aliens
void initAliensArray(alien_t aliens[ALIEN_ROWS][ALIEN_COLS]){	//recibe un puntero al arreglo de aliens(xq en el stack? porque es mas rapido y no tenemos cant. variable)
	int i, j;

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
		shields[i].health = SHIELD_HEALTH; 	//le seteo la cantidad de vidas
		shields[i].coord.coordY = SHIELD_INIT_Y; 	//lo ubico en el display
		shields[i].coord.coordX = INIT_SHIELDS_MARGIN_X + (SHIELD_INIT_X_JUMP * (i+FACTOR_CORRECTOR)) - SHIELD_X; 
		shields[i].sizeX = SHIELD_INIT_SIZE_X;
		shields[i].sizeY = SHIELD_INIT_SIZE_Y;
	}
}

//Inicializo el jugador
void initPlayer(player_t * player){		
	player->health = 3;
	player->coord.coordX = INIT_PLAYER_MARGIN_X;	
	player->coord.coordY = INIT_PLAYER_Y;
	player->bulletsFired = 0;
}

//Inicializo las estadisticas del juego
void initGameStats(stats_t * gameStats){
	gameStats->actualScore = 0;
	gameStats->level = 1; 
}

/* Se incializa el OVNI como no visible al inicio, y se prepara el temporizador para su primera aparición. */
void initOvni(ovni_t * ovni, clock_t currentTime, clock_t * LastOvniDespawnTime){
    ovni->visible = false; // El OVNI arranca invisible.
    ovni->alive = true;
    *LastOvniDespawnTime = currentTime; //Se comienza a contar desde el instante actual, para que luego aparezca el ovni.
}

//******************************************************************||FUNCIONES DE ACTUALIZACION||**************************************************************************************

//Actualizo los datos del bloque de aliens 
void updateAliensBlock(alien_t aliens[ALIEN_ROWS][ALIEN_COLS], aliensBlock_t * aliensBlock){
	uint8_t alienColAlive = 0;
	uint8_t alienRowAlive = 0; 
	uint8_t i, j; 

	//ACTUALIZO FIRSTCOLALIVE
	for(i = 0; i < ALIEN_ROWS; i++) {  // recorro cada fila		
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
	for(j = 0; j < ALIEN_COLS; j++) { // recorro cada fila
		if(aliens[aliensBlock->lastRowAlive][j].alive){
			alienRowAlive++; 
		}
	}
	if(!alienRowAlive){			//si esa fila ya no tiene aliens vivos
		aliensBlock->lastRowAlive--; 		
	}
	//printf("fca: %d, lca: %d, lra: %d, abw: %d \n", aliensBlock->firstColAlive, aliensBlock->lastColAlive, aliensBlock->lastRowAlive, aliensBlock->width);
}

//solo se usa en la pc, no en la raspberry
void shieldsUpdate(shield_t shields[NUM_SHIELDS]){
	//esta funcion analiza la vida que le queda a cada escudo y actualiza su tamaño
	int k; 
	
	for(k=0; k<NUM_SHIELDS; k++){
		if(shields[k].health > 12){
			shields[k].sizeX = SHIELD_1_SIZE_X;
			shields[k].sizeY = SHIELD_1_SIZE_X;
		}
		else if(shields[k].health > 9){
			shields[k].sizeX = SHIELD_2_SIZE_X;
			shields[k].sizeY = SHIELD_2_SIZE_X;
		}
		else if(shields[k].health > 6){
			shields[k].sizeX = SHIELD_3_SIZE_X;
			shields[k].sizeY = SHIELD_3_SIZE_X;
		}
		else if(shields[k].health > 3){
			shields[k].sizeX = SHIELD_4_SIZE_X;
			shields[k].sizeY = SHIELD_4_SIZE_X;
		}
		else{
			shields[k].sizeX = SHIELD_5_SIZE_X;
			shields[k].sizeY = SHIELD_5_SIZE_X;
		}
	}
}

//Verifico si tengo que pasar al siguiente nivel
//creo que no hace falta pasarle las balas, ya deberian estar desactivadas
void newLevelCheck(alien_t aliens[ALIEN_ROWS][ALIEN_COLS], aliensBlock_t * aliensBlock, player_t * player, shield_t shields[NUM_SHIELDS], stats_t * gameStats){
	//verifico si murieron todos para volver a inicializar todo
	bool allDead = true;

					for (int i = 0; i < ALIEN_ROWS; i++) {
						for (int j = 0; j < ALIEN_COLS; j++) {
							if (aliens[i][j].alive) {
								allDead = false;
								break;  // ya con uno vivo, no hace falta seguir
							}
						}
						if (!allDead) break;
					}

					if (allDead) {
						// SUBIR DE NIVEL
						initPlayer(player);
						initAliensBlock(aliensBlock);
						initAliensArray(aliens);
						initShieldsArray(shields);
						gameStats->actualScore += LEVEL_POINTS(gameStats->level);
						gameStats->level++;
					}
}

/* Gestiona el movimiento y la reaparición del OVNI, fijandose si paso el tiempo de espera para la aparición (OVNI_SPAWN_INTERVAL -es constante-).
  -'lastOvniDespawnTime' marca el momento en que el OVNI dejó de ser visible (desapareció o fue destruido). Este valor es el punto de inicio para el "cooldown" o tiempo de espera antes de la próxima aparición del OVNI.
*/
void updateOvni (ovni_t * ovni, clock_t currentTime, clock_t * clkO, int random){
    static int direction = -1;
    static float speed = 0;

    if (ovni->visible == false){
        if (((double)(currentTime - *clkO)/CLOCKS_PER_SEC) >= OVNI_SPAWN_INTERVAL && random == 500){
            ovni->visible = true;
            ovni->alive = true;
            ovni->coord.coordY = DISPLAY_MARGIN_Y + INIT_OVNI_MARGIN_Y;
            // Alternar dirección en el momento de aparición
            direction *= -1;
            if(direction == -1){
                ovni->coord.coordX = DISPLAY_LENGTH - DISPLAY_MARGIN_Y - INIT_OVNI_MARGIN_X;
            } else {
                ovni->coord.coordX = INIT_OVNI_MARGIN_X;
            }
        }
    }
    if (ovni->visible == true){
        speed += SPEED_OVNI;
        ovni->coord.coordX += direction * (int)speed;
        if (speed >= 1){
            speed = 0;
        }
        // Si se va del display, desaparece
		if ((direction == -1 && ovni->coord.coordX + OVNI_SIZE_X <= DISPLAY_MARGIN_X) ||
			(direction == 1 && ovni->coord.coordX + OVNI_SIZE_X >= DISPLAY_LENGTH)) {
			ovni->visible = false;
			*clkO = currentTime;
		}
    }
}

//******************************************************************||FUNCIONES DE COLISIONES||**************************************************************************************

/*	verifica superposicion de rectangulos
	A y B entidades a analizar
	X e Y coordenadas reales de las entidades
	W y H Width y High tamaño de las entidades
	devuelve 1 si hay superposicion, sino 0 
*/
char rectangleOverlap(uint16_t AX, uint16_t AW, uint16_t BX, uint16_t BW, uint16_t AY, uint16_t AH, uint16_t BY, uint16_t BH){
	if(	AX < BX + BW
	&&	BX < AX + AW
	&&	AY < BY + BH
	&&	BY < AY + AH){
		return 1;
	}
	else{
		return 0; 
	}
}

//chequea bala del jugador con todos los aliens
void collisionBA(bullet_t * bullet, alien_t aliens[ALIEN_ROWS][ALIEN_COLS], aliensBlock_t * aliensBlock, stats_t * gameStats, int printedRow) {	

	for (int row = aliensBlock->lastRowAlive; row >= 0; row--) { // Recorro filas desde abajo hacia arriba
		for (int col = aliensBlock->firstColAlive; col <= aliensBlock->lastColAlive; col++) { // Recorro columnas activas
			if (aliens[row][col].alive) { // Verifico que el alien esté vivo
				
				// Variables intermedias por claridad
				uint16_t alienX; 
				uint16_t alienY = aliens[row][col].coord.coordY + aliensBlock->coord.coordY;
				uint16_t bulletX = bullet->coord.coordX;
				uint16_t bulletY = bullet->coord.coordY;
				
				//distingo si esa fila ya se movio o no
				if(row >= printedRow){	
					alienX = aliens[row][col].coord.coordX + aliensBlock->coord.coordX + JUMP_SIZE_X * aliensBlock->direction;
				}else{	//si todavia no se imprimio la fila desplazada, comparo con las coordenadas anteriores
					alienX = aliens[row][col].coord.coordX + aliensBlock->coord.coordX;
				}
				// Chequeo superposición de rectángulos
				if (rectangleOverlap(alienX, getAlienWidthByRow(row), bulletX, BULLET_SIZE_X, alienY, getAlienHeightByRow(row), bulletY, BULLET_SIZE_Y)){
					
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
void collisionAS (alien_t aliens[ALIEN_ROWS][ALIEN_COLS], shield_t shields[NUM_SHIELDS], aliensBlock_t * aliensBlock){ 
	
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
					shields[s].health -= 5; //ver cuanto conviene decrementar las vidas luego 
				}	
			}		
		}
	}	
}

//recibe bala del allien y escudos
void collisionBS(bullet_t * bulletP, bullet_t * bulletA, shield_t shields[NUM_SHIELDS]){
	
	int s;
	
	if(bulletP->active){
		for(s=0; s<NUM_SHIELDS; s++){
			if(shields[s].health){		//si el escudo todavia tiene vida
			
				//variables intermedias para mas claridad
				uint16_t bulletX = bulletP->coord.coordX;
				uint16_t bulletY = bulletP->coord.coordY;
				uint16_t shieldX = shields[s].coord.coordX;
				uint16_t shieldY = shields[s].coord.coordY;
				uint16_t shieldSizeX = shields[s].sizeX;
				uint16_t shieldSizeY = shields[s].sizeY;
				
				if(rectangleOverlap(shieldX, shieldSizeX, bulletX, BULLET_SIZE_X, shieldY, shieldSizeY, bulletY, BULLET_SIZE_Y)){
					shields[s].health--; 
					bulletP->active = false;
				}
			}
		}
	}	
	if(bulletA->active){
		for(s=0; s<NUM_SHIELDS; s++){
			if(shields[s].health){		//si el escudo todavia tiene vida
			
				//variables intermedias para mas claridad
				uint16_t bulletX = bulletA->coord.coordX;
				uint16_t bulletY = bulletA->coord.coordY;
				uint16_t shieldX = shields[s].coord.coordX;
				uint16_t shieldY = shields[s].coord.coordY;
				uint16_t shieldSizeX = shields[s].sizeX;
				uint16_t shieldSizeY = shields[s].sizeY;
				
				if(rectangleOverlap(shieldX, shieldSizeX, bulletX, BULLET_SIZE_X, shieldY, shieldSizeY, bulletY, BULLET_SIZE_Y)){
					shields[s].health--; 
					bulletA->active = false;
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
		uint16_t playerX = player->coord.coordX - HITBOX;
		uint16_t playerY = player->coord.coordY;
		
		if(rectangleOverlap(playerX, PLAYER_SIZE_X, bulletX, BULLET_SIZE_X, playerY, PLAYER_SIZE_Y, bulletY, BULLET_SIZE_Y)){
			
			player->health--; //le saco una vida
			bullet->active = false; 
		}
	}
}

//recibe el jugador y los aliens
void collisionAP (player_t * player, alien_t aliens[ALIEN_ROWS][ALIEN_COLS], aliensBlock_t * aliensBlock){
	
	int j;
	
	//solo comparo con la ultima fila viva
	for(j=aliensBlock->firstColAlive; j<=aliensBlock->lastColAlive; j++){
		if(aliens[aliensBlock->lastRowAlive][j].alive){	//si el alien esta vivo 
		
			//variables intermedias para mejor comprension
			uint16_t playerX = player->coord.coordX-PLAYER_SIZE_X/2;
			uint16_t playerY = player->coord.coordY;
			uint16_t alienX = aliens[aliensBlock->lastRowAlive][j].coord.coordX + aliensBlock->coord.coordX;
			uint16_t alienY = aliens[aliensBlock->lastRowAlive][j].coord.coordY + aliensBlock->coord.coordY;
			
			if(rectangleOverlap(playerX, PLAYER_SIZE_X, alienX, ALIEN_B_SIZE_X, playerY, PLAYER_SIZE_Y, alienY, ALIEN_B_SIZE_Y)){
				player->health = 0;
			}
		}
	}
	
}

// Colisión entre bala y ovni
void collisionBO(bullet_t * bullet, ovni_t * ovni, clock_t *lastOvniDespawnTime, stats_t * gameStats){
    const int puntosOvni[16] = {
        100, 50, 50, 100, 150, 100, 100, 50,
        300, 100, 100, 50, 150, 100, 100, 50
    }; // Puntajes por cada ovni destruido

    static uint8_t numOvniKilled = 0;

    if (bullet->active && ovni->visible && ovni->alive) {

        // Variables intermedias por claridad
        uint16_t ovniX = ovni->coord.coordX;
        uint16_t ovniY = ovni->coord.coordY;
        uint16_t bulletX = bullet->coord.coordX;
        uint16_t bulletY = bullet->coord.coordY;

        // Chequeo de superposición
        if (rectangleOverlap(ovniX, OVNI_SIZE_X, bulletX, BULLET_SIZE_X,
                             ovniY, OVNI_SIZE_Y, bulletY, BULLET_SIZE_Y)) {

            bullet->active = false;
            ovni->visible = false;
            ovni->alive = false;
            *lastOvniDespawnTime = clock();  // Marcar momento de desaparición

            // Sumar puntaje si corresponde
            if (gameStats != NULL) {
                uint8_t idx = (numOvniKilled < 16) ? numOvniKilled : 15;
                gameStats->actualScore += puntosOvni[idx];
            }

            numOvniKilled++;  // Para que el próximo ovni dé otro puntaje
        }
    }
}


//analiza todas las colisiones
void collisionDetect(bullet_t * bulletP, bullet_t * bulletA, alien_t aliens[ALIEN_ROWS][ALIEN_COLS], ovni_t * ovni, shield_t shields[NUM_SHIELDS], aliensBlock_t * aliensBlock, player_t * player, stats_t * gameStats, uint8_t printedRow, clock_t *clkO){
	if(bulletP->active){
		collisionBA(bulletP, aliens, aliensBlock, gameStats, printedRow); //bullet vs aliens
		collisionBB(bulletP, bulletA); //bullet vs bullet
		collisionBO(bulletP, ovni, clkO, gameStats); //bullet vs ovni
	}
	collisionBS(bulletP, bulletA, shields);//shield vs bullet
	collisionBP(bulletA, player);//player vs bullet
	collisionAS (aliens, shields, aliensBlock);//alien vs shield
	collisionAP (player, aliens, aliensBlock);//alien vs player
}

//******************************************************************||FUNCIONES DE DISPAROS||**************************************************************************************

/* 
Función que elige que alien va a ser el que va a disparar.
 - Elije el que tenga su coordenada en X mas cerca del jugador.
 - Elije el que esta más abajo dentro de una columna.
*/

alien_t * selectAlienShooter(alien_t alien[ALIEN_ROWS][ALIEN_COLS], aliensBlock_t * aliensBlock, player_t * player){
    alien_t * bestCandidate = NULL;
    int minDistX = DISPLAY_LENGTH;

    for (int col = aliensBlock->firstColAlive; col <= aliensBlock->lastColAlive; col++) {
        for (int row = ALIEN_ROWS - 1; row >= 0; row--) {
            if (alien[row][col].alive) {
                int alienX = alien[row][col].coord.coordX + aliensBlock->coord.coordX;
                int playerX = player->coord.coordX;
                int distX = abs(alienX - playerX);

                if (distX < minDistX) {
                    minDistX = distX;
                    bestCandidate = &alien[row][col];
                }
                break;
            }
        }
    }
    return bestCandidate;
}

void alienShoot(bullet_t * bullet, alien_t * alien, int level, aliensBlock_t * aliensBlock, int lastRowToPrint, int alienRowIndex){
	static uint8_t frameCounter = 0; 
	static float speed;

	// Calcular desplazamiento lateral simulado si está en animación
	int offsetX = aliensBlock->coord.coordX;
	if (lastRowToPrint >= 0 && alienRowIndex > lastRowToPrint) {
		offsetX += JUMP_SIZE_X * aliensBlock->direction;
	}
	// Obtenemos medidas del sprite correspondiente a ese alien
	int alienWidth = getAlienWidthByRow(alienRowIndex);
	int alienHeight = getAlienHeightByRow(alienRowIndex);

	if (!bullet->active) {	//mientas mas alto el nivel, mas balas tiran los aliens
		if(frameCounter >= SHOOT_FREQ(level)){
			bullet->active = true;
			// Coordenadas reales ajustadas para disparo centrado y desde la base
			bullet->coord.coordX = alien->coord.coordX + offsetX + alienWidth / 2;
			bullet->coord.coordY = alien->coord.coordY + aliensBlock->coord.coordY + alienHeight;
			frameCounter = 0; 
		}
		else{
			frameCounter++; 
		}
	}

	if (bullet->active) {
		speed += getAlienBulletSpeed(level);
		bullet->coord.coordY += (int)speed;
		if(speed>=1){
			speed = 0;
		}

		if (bullet->coord.coordY > DISPLAY_HIGH - DISPLAY_MARGIN_Y) {
			bullet->active = false;
		}
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
//******************************************************************||FUNCIONES DE MOVIMIENTO||**************************************************************************************

//Muevo el bloque de aliens
//HACER QUE LA VELOCIDAD DEL MOVIMIENTO DEPENDA DEL NIVEL
void blockMove(alien_t aliens[ALIEN_ROWS][ALIEN_COLS], aliensBlock_t * aliensBlock){ 
	if(aliensBlock->direction==1 && ((aliens[0][aliensBlock->firstColAlive].coord.coordX + aliensBlock->width + aliensBlock->coord.coordX) >= DISPLAY_LENGTH - ALIEN_EXTREME)){	//verifico si llego al limite derecho
		aliensBlock->direction = -1; 		//cambio de direccion
		aliensBlock->coord.coordY += JUMP_SIZE_Y;	//salto abajo
		aliensBlock->coord.coordX += JUMP_SIZE_X;
	}	
	else if(aliensBlock->direction==-1 && ((aliens[0][aliensBlock->firstColAlive].coord.coordX + aliensBlock->coord.coordX )<= ALIEN_EXTREME)){	//verifico si llego al limite izquierdo
		aliensBlock->direction = 1; 		//cambio de direccion
		aliensBlock->coord.coordY += JUMP_SIZE_Y;	//salto abajo
		aliensBlock->coord.coordX -= JUMP_SIZE_X;
		
		// Previene que se pase del margen izquierdo
		int minX = -(aliens[0][aliensBlock->firstColAlive].coord.coordX);
		if (aliensBlock->coord.coordX < minX){
			aliensBlock->coord.coordX = minX;
		}
	}
	else{
		aliensBlock->coord.coordX += JUMP_SIZE_X * aliensBlock->direction;		//suma o resta dependiendo de hacia donde tiene que ir
	}
}

void playerMove(int dire, player_t * player){
    static float speed = 0.95;
	
	if(dire<0 && player->coord.coordX>PLAYER_LIMIT_X){
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


