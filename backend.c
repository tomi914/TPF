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
void initAliensBlock(alienBlock_t aliensBlock){
	aliensBlock.coordX = MARGIN_X;
    aliensBlock.coordY = MARGIN_Y;
    aliensBlock.direction = 1;
    aliensBlock.firstColAlive = 0;
    aliensBlock.lastColAlive = ALIEN_COLS - 1;
    aliensBlock.lastRowAlive = ALIEN_ROWS - 1; 
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
		    	aliens[i][j].coord.coordX = j * A_INIT_JUMP_SIZE_X;	//chequear que este bien visualmente
		    	aliens[i][j].coord.coordY = i * A_INIT_JUMP_SIZE_Y;
		    }
		    else if(i < 3){
		    	aliens[i][j].type = 'B';
		    	aliens[i][j].coord.coordX = j * B_INIT_JUMP_SIZE_X;
		    	aliens[i][j].coord.coordY = i * B_INIT_JUMP_SIZE_Y;
		    }
		    else{
		    	aliens[i][j].type = 'C';
		    	aliens[i][j].coord.coordX = j * C_INIT_JUMP_SIZE_X;
		    	aliens[i][j].coord.coordY = i * C_INIT_JUMP_SIZE_Y;
		    }
		}
	}
}

void initShieldsArray(shield_t * shields[NUM_SHIELDS]){	//recibe un puntero al arreglo de escudos definido en el front

	int i; 
	
	shields[0]->coord.coordX = MARGIN_X + SHIELD_INIT_X_JUMP/2; //ubico el primer escudo, dejo el margen + la mitad de los saltos 
	
	for(i=0;i<NUM_SHIELDS;i++){
		shields[i]->health = 15; 	//le seteo la cantidad de vidas
		shields[i]->coord.coordY = SHIELD_INIT_Y; 	//lo ubico en el display
		shields[i]->coord.coordX += (SHIELD_INIT_X_JUMP * i); 
	}
}

//creo que esta bien, chequear con los muchachos. 
void initPlayer(player_t * player){		//OPCIONAL: JUGAR DE A DOS, como se podria hacer? 
	
	player->health = 3;
	player->coord.coordX = DISPLAY_LENGTH / 2;	//asi arranca al medio
	player->coord.coordY = INIT_PLAYER_Y;
	
}

void blockMove(alien_t * aliens[ALIEN_ROWS][ALIEN_COLS], alienBlock_t aliensBlock){ 

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
	//en borde derecho: comparo coordX del bloque + coordX de la primera fila viva + largo del bloque con el largo del display menos el margen
	//en borde izquierdo: comparo coordX del bloque + coordX de la primera fila viva con el margen 
	if(aliensBlock.direction==1 && ((aliens[0][aliensBlock.firstColAlive].coord.coordX + alienRowlength + aliensBlock.coordX) >= DISPLAY_LENGTH - DISPLAY_MARGIN_X)){	//verifico si llego al limite derecho
		aliensBlock.direction = -1; 		//cambio de direccion
		aliensBlock.coordY += JUMP_SIZE_Y;	//salto abajo
	}	
	else if(aliensBlock.direction==-1 && ((aliens[0][aliensBlock.firstColAlive].coord.coordX + aliensBlock.coordX )<= DISPLAY_MARGIN_X)){	//verifico si llego al limite izquierdo
		aliensBlock.direction = 1; 		//cambio de direccion
		aliensBlock.coordY += JUMP_SIZE_Y;	//salto abajo
	}
	else{
		aliensBlock.coordX += JUMP_SIZE_X * aliensBlock.direction;		//suma o resta dependiendo de hacia donde tiene que ir
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
void collisionBA(bullet_t * bullet , alien_t * aliens[ALIEN_ROWS][ALIEN_COLS], alienBlock_t aliensBlock){	

	int i, j; 
	
	if(bullet->active){	//verifico que la bala del jugador este activa
		for(i=(aliensBlock.lastRowAlive); i>=0 ; i--){	//recorro filas de aliens desde abajo hacia arriba
			for(j=aliensBlock.firstColAlive; j<=aliensBlock.lastColAlive ; j++){	//recorro columnas de aliens sin analizar las que ya murieron
				if(aliens[i][j].alive){	//verifico que el alien este vivo
				
					//creo variables intermedias por claridad
					uint16_t alienX = aliens[i][j].coord.coordX + aliensBlock.coordX;
					uint16_t alienY = aliens[i][j].coord.coordY + aliensBlock.coordY;
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
void collisionAS(alien_t * aliens[ALIEN_ROWS][ALIEN_COLS], shield_t * shields[NUM_SHIELDS], alienBlock_t aliensBlock){

	int j, k; 
	
	//solo analizamos la colision con la ultima fila viva, no con las otras
	for(j=aliensBlock.firstColAlive; j<=aliensBlock.lastColAlive ; j++){
		for(k=0; k<NUM_SHIELDS; k++){
			
			if(aliens[aliensBlock.lastRowAlive][j]. alive && shields[k].health){//verifico que el alien este vivo y que el escudo tambien
			
				//variables intermedias para mejor comprension
				uint16_t alienX = aliens[aliensBlock.lastRowAlive][j].coord.coordX + aliensBlock.coordX;
				uint16_t alienY = aliens[aliensBlock.lastRowAlive][j].coord.coordY + aliensBlock.coordY;
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

void shieldsUpdate(shield_t * shields[NUM_SHIELDS]){
	//esta funcion analiza la vida que le queda a cada escudo y actualiza su tamaño
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




