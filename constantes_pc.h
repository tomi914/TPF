#define ALIEN_ROWS 5  // Cantidad de FILAS de ALIENS.
#define ALIEN_COLS 11  // Cantidad de COLUMNAS de ALIENS.
#define DISPLAY_HIGH 1000	// ALTURA del display en coordenadas (Como depende del display (front-end), lo definimos en el make)
#define DISPLAY_LENGTH 1000 // LARGO del display en coordenadas (Como depende del display (front-end), lo definimos en el make) 
#define ALIEN_A_SIZE_X 50 // Cuantos ocupa un ALIEN a lo LARGO en coordenadas (Como depende del display (front-end), lo definimos en el make)
#define ALIEN_A_SIZE_Y 51 // Cuantos ocupa un ALIEN a lo ALTO en coordenadas (Como depende del display (front-end), lo definimos en el make)
#define ALIEN_B_SIZE_X 56 // Cuantos ocupa un ALIEN a lo LARGO en coordenadas (Como depende del display (front-end), lo definimos en el make)
#define ALIEN_B_SIZE_Y 41 // Cuantos ocupa un ALIEN a lo ALTO en coordenadas (Como depende del display (front-end), lo definimos en el make)
#define ALIEN_C_SIZE_X 60 // Cuantos ocupa un ALIEN a lo LARGO en coordenadas (Como depende del display (front-end), lo definimos en el make)
#define ALIEN_C_SIZE_Y 40 // Cuantos ocupa un ALIEN a lo ALTO en coordenadas (Como depende del display (front-end), lo definimos en el make)
#define A_INIT_JUMP_SIZE_X 20	// Salto necesario para inicializar centrados los aliens
#define A_INIT_JUMP_SIZE_Y 10	// Salto necesario para inicializar centrados los aliens
#define B_INIT_JUMP_SIZE_X 14	// Salto necesario para inicializar centrados los aliens
#define B_INIT_JUMP_SIZE_Y 10	// Salto necesario para inicializar centrados los aliens
#define C_INIT_JUMP_SIZE_X 10	// Salto necesario para inicializar centrados los aliens
#define C_INIT_JUMP_SIZE_Y 10	// Salto necesario para inicializar centrados los aliens
#define INIT_ALIENS_MARGIN_X 60	//margen para donde arrancan los aliens
#define INIT_ALIENS_MARGIN_Y 40	//margen para donde arrancan los aliens
#define JUMP_SIZE_X	20			//salto para los movimientos
#define JUMP_SIZE_Y 20			//salto para los movimientos
#define DISPLAY_MARGIN_X 20	//margen en coordenadas que deseamos tener para no chocarnos con los bordes del display
#define DISPLAY_MARGIN_Y 20	//margen en coordenadas que deseamos tener para no chocarnos con los bordes del display
#define INIT_PLAYER_Y 1	//en que coordenada Y va  a estar
#define NUM_SHIELDS 4	//cantidad de escudos 
#define SHIELD_INIT_SIZE_X 60	//tamaño inicial de los escudos
#define SHIELD_INIT_SIZE_Y 30	//tamaño inicial de los escudos
#define SHIELD_INIT_Y 600 //en que coordenada en Y ubicamos los escudos
#define SHIELD_INIT_X_JUMP 50 //cuantas coordenadas hay de espaciado entre cada escudo
//agregar tmb a constantes_pi.h
#define PLAYER_SIZE_X 50//VER
#define PLAYER_SIZE_Y 56
#define SPEED_PLAYER 10
#define BULLET_SIZE_X 5
#define BULLET_SIZE_Y 15
#define SPEED_BULLET_PLAYER 5 // Velocidad de la bala del player.
#define SPEED_BULLET_ALIEN(level) (1 + (((level) - 1) / 3)) // Velocidad de la bala del alien. Aumenta cada 3 niveles: nivel 1,2,3: v1 ; nivel 4,5,6: v2 ...
