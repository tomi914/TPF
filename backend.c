#include<stdbool.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>

#define ALIEN_ROWS 5  // Cantidad de FILAS de ALIENS.
#define ALIEN_COLS 5  // Cantidad de COLUMNAS de ALIENS.
#define DISPLAY_HIGH 16	// ALTURA del display en coordenadas (Como depende del display (front-end), lo definimos en el make)
#define DISPLAY_LENGTH 16 // LARGO del display en coordenadas (Como depende del display (front-end), lo definimos en el make) 
#define ALIEN_SIZE_X 1 // Cuantos ocupa un ALIEN a lo LARGO en coordenadas (Como depende del display (front-end), lo definimos en el make)
#define ALIEN_SIZE_Y 1 // Cuantos ocupa un ALIEN a lo ALTO en coordenadas (Como depende del display (front-end), lo definimos en el make)
#define JUMP_SIZE_X 1	// Define en coordenadas el salto de movimiento en cada "clock" (Como depende del display (front-end), lo definimos en el make)
#define JUMP_SIZE_Y 1	// Define en coordenadas el salto de movimiento en cada "clock" (Como depende del display (front-end), lo definimos en el make)
#define DISPLAY_MARGIN_X 1	//margen en coordenadas que deseamos tener para no chocarnos con los bordes del display
#define DISPLAY_MARGIN_Y 1 	//margen en coordenadas que deseamos tener para no chocarnos con los bordes del display
#define ALIEN_ROW_LENGTH ((ALIEN_COLS * ALIEN_SIZE_X) + (JUMP_SIZE * (ALIEN_COLS-1)))	//Ancho de la fila de aliens (en coordenadas)

typedef struct{ // Almacena las coordenadas 
	uint16_t coordX;
	uint16_t coordY;
}coord_t;

typedef struct{ //Almacena las estadisticas de este juego y los anteriores.
	uint32_t score;
	uint16_t level;
	//ranking?
}stats_t;

/*   VER SI ES NECESARIO
typedef struct{
	bool pause;
	bool restart;
	bool exit; 
}menu_t;
*/

//entidades y objetos
 
typedef struct{ // Almacena la información general de toodos los ALIENS.
	bool alive;
	char type;
	coord_t coord; 
}alien_t;

typedef struct{ // Almacena la información general de los ESCUDOS (los tratamos como escuditos)
	bool built; 
	coord_t coord; 
}shield_t;

typedef struct{ // Almacena la información general del JUGADOR (nave espacial)
	uint8_t health; 
	coord_t coord;
}player_t;

typedef struct{ // Almacena la información general de las BALAS
	char type; 
	coord_t coord; 
}bullet_t;

void playerShoot(void); //Función que 
void playerMove(void);  //recibe para donde ir
void alienShoot(void);  //
void alienMove(void);   //
void colisionCheck(void); 
void scoreTracker(void); 	//10-20-30-random 


//creo que esta bien, chequear con los muchachos. 
void initAliensArray(alien_t * aliens[ALIEN_ROWS][ALIEN_COLS]){	//recibe un puntero al arreglo de aliens(xq en el stack? porque es mas rapido y no tenemos cant. variable)

	int i, j;
	
	for(i = 0; i < ALIEN_ROWS; i++) { // Itero sobre una fila 
		for(j = 0; j < ALIEN_COLS; j++) { // Itero sobre una columna 
			aliens[i][j].alive = true; // todos arrancan vivos
			aliens[i][j].coord.coordX = j * JUMP_SIZE + DISPLAY_MARGIN_X;  	// seteo coordenadas en X con el espaciado definido en JUMP_SIZE
			aliens[i][j].coord.coordY = i * JUMP_SIZE + DISPLAY_MARGIN_Y; 	 // seteo coordenadas en Y con el espaciado definido en JUMP_SIZE
			if(i == 0){
		    	aliens[i][j].type = 'A';		//en allegro cada tipo corresponde a una imagen diferente
		    }
		    else if(i < 3){
		    	aliens[i][j].type = 'B';
		    }
		    else{
		    	aliens[i][j].type = 'C';
		    }
		}
	}
}

//creo que esta bien, chequear con los muchachos. 
void alienMove(void){ 
	
	//HABRIA QUE VER SI NO CONVIENE QUE SEAN GLOBALES PARA CUANDO SE REINICIA LA PARTIDA --- CREO QUE SI
	static char direction = 1; 	//	1:right // -1:left
	//estas dos variables son la referencia para ubicar las entidades 
	static int offsetX = 0;
	static int offsetY = 0;
	//como accedo a las coordenadas reales de un alien?
	//coordenadas reales = alien[i][j].coord.coordX + offsetX
	
	if(direction==1 && ((offsetX + ALIEN_ROW_LENGTH) >= DISPLAY_LENGTH - DISPLAY_MARGIN_X)){	//verifico si llego al limite derecho
		direction = -1; 		//cambio de direccion
		offsetY += JUMP_SIZE_Y;	//salto abajo
	}	
	else if(direction==-1 && (offsetX <= DISPLAY_MARGIN_X)){	//verifico si llego al limite izquierdo
		direction = 1; 		//cambio de direccion
		offsetY += JUMP_SIZE_Y;	//salto abajo
	}
	else{
		offsetX += JUMP_SIZE_X * direction;		//suma o resta dependiendo de hacia donde tiene que ir
	}
}

//creo que esta bien, chequear con los muchachos. 
void initPlayer(player_t * player){		//OPCIONAL: JUGAR DE A DOS, como se podria hacer? 
	
	player->health = 3;
	player->coord.coordX = DISPLAY_LENGTH / 2;	//asi arranca al medio
	player->coord.coordY = /*depende la interfaz*/;
	
}

//TERMINAR
void playerMove(/*algo que indique direccion*/, player_t * player){		//revisar que seria necesario que ingrese (coordIn serian las coord leidas en el teclado o joystick)
	
	player->coord.coordX += JUMP_SIZE_X;	//ver si es para la izquierda o derecha
	
}



