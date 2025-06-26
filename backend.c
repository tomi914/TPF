#include<stdbool.h>
#include<stdint.h>
#include<pthread.h>

#define ALIEN_ROWS 5  // Cantidad de FILAS de ALIENS.
#define ALIEN_COLS 5  // Cantidad de COLUMNAS de ALIENS.
#define DISPLAY_HIGH 16	// ALTURA del display (Como depende del display (front-end), lo definimos en el make)
#define DISPLAY_LENGTH 16 // LARGO del display (Como depende del display (front-end), lo definimos en el make) 
#define ALIEN_SIZE_X 1 // Cuantos PIXELES ocupa un ALIEN a lo LARGO (Como depende del display (front-end), lo definimos en el make)
#define ALIEN_SIZE_Y 1 // Cuantos PIXELES ocupa un ALIEN a lo ALTO (Como depende del display (front-end), lo definimos en el make)
#define JUMP_SIZE 1	// Define el salto de movimiento en cada "clock" (Como depende del display (front-end), lo definimos en el make)

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


int main(void){
	
//defino aliens para probar
	alien_t aliens[ALIEN_ROWS][ALIEN_COLS]; // Se define una matriz que contendrá la cantidad de aliens de largo y ancho que se desee.
	int i, j;
	
	for(i = 0; i < ALIEN_ROWS; i++) { // Itero sobre una fila en especifico
		for(j = 0; j < ALIEN_COLS; j++) { // Itero sobre una columna
			aliens[i][j].alive = true; // 
			aliens[i][j].coord.coordX = j * 2;  // base X
			aliens[i][j].coord.coordY = i;      // base Y
			if(i == 0){
		    	aliens[i][j].type = 'A';
		    }
		    else if(i < 3){
		    	aliens[i][j].type = 'B';
		    }
		    else{
		    	aliens[i][j].type = 'C';
		    }
		}
	}
	
	alienMove();
	
	return 0;	
}

void alienMove(void){ // ¡¡¡¡FALTA HACER QUE BAJEN LAS FILAS Y CHEQUEAR CORRECTA DETECCIÓN DE BORDES. SI ES POSIBLE OPTIMIZAR!!!!
	
	int i, j;
	char direction = 'R'; 	//	R:right - L:left
	int lengthRow = (ALIEN_COLS * ALIEN_SIZE_X) + (JUMP_SIZE * (ALIEN_COLS-1)); // largo de la fila en la definicion utilizada
	
	while(1){
		switch (direction){ // La idea es que varíe el movimiento según vaya hacia la derecha o hacia la izquierda
			case 'R': // caso DERECHA
				while((aliens[0][0].coord.coordX + lengthRow) < DISPLAY_LENGTH-1){	//para no chocar con el borde derecho*/
					for(i=ALIEN_ROWS-1;i>=0;i--){ 
						for(j=ALIEN_COLS-1;j>=0;j--){ 
							aliens[i][j].coord.coordX += JUMP_SIZE;	// Avanzamos cada columna de aliens lo que indica JUMP_SIZE hacia la DERECHA
						}
					}
				}
				direction = 'L'; // caso IZQUIERDA
			break;
			case 'L':
				while((aliens[ALIEN_ROWS-1][ALIEN_COLS-1].coord.coordX - lengthRow) > 1){	//para no chocar con el borde*/
					for(i=ALIEN_ROWS-1;i>=0;i--){
						for(j=0;j<ALIEN_ROWS;j++){
							aliens[i][j].coord.coordX -= JUMP_SIZE;	// Avanzamos cada columna de aliens lo que indica JUMP_SIZE hacia la IZQUIERDA
						}
					}
				}
				direction = 'R';
			break;
		}
	}
}

		X--X--X--X--X
		X--X--X--X--X
		X--X--X--X--X
		X--X--X--X--X
		X--X--X--X--X

				X--X--X--X--X
				X--X--X--X--X
				X--X--X--X--X
				X--X--X--X--X
				X--X--X--X--X

						X--X--X--X--X
						X--X--X--X--X
						X--X--X--X--X
						X--X--X--X--X
						X--X--X--X--X






