//el tipo de alien (A,B o C) no influye en las medidas. 

#define ALIEN_ROWS 4  // Cantidad de FILAS de ALIENS.
#define ALIEN_COLS 5  // Cantidad de COLUMNAS de ALIENS.
#define DISPLAY_HIGH 16	// ALTURA del display en coordenadas (Como depende del display (front-end), lo definimos en el make)
#define DISPLAY_LENGTH 16 // LARGO del display en coordenadas (Como depende del display (front-end), lo definimos en el make) 
#define ALIEN_A_SIZE_X 1 // Cuantos ocupa un ALIEN a lo LARGO en coordenadas (Como depende del display (front-end), lo definimos en el make)
#define ALIEN_A_SIZE_Y 1 // Cuantos ocupa un ALIEN a lo ALTO en coordenadas (Como depende del display (front-end), lo definimos en el make)
#define ALIEN_B_SIZE_X 1 // Cuantos ocupa un ALIEN a lo LARGO en coordenadas (Como depende del display (front-end), lo definimos en el make)
#define ALIEN_B_SIZE_Y 1 // Cuantos ocupa un ALIEN a lo ALTO en coordenadas (Como depende del display (front-end), lo definimos en el make)
#define ALIEN_C_SIZE_X 1 // Cuantos ocupa un ALIEN a lo LARGO en coordenadas (Como depende del display (front-end), lo definimos en el make)
#define A_INIT_JUMP_SIZE_X 1	// Salto necesario para inicializar centrados los aliens
#define A_INIT_JUMP_SIZE_Y 1	// Salto necesario para inicializar centrados los aliens
#define B_INIT_JUMP_SIZE_X 1	// Salto necesario para inicializar centrados los aliens
#define B_INIT_JUMP_SIZE_Y 1	// Salto necesario para inicializar centrados los aliens
#define C_INIT_JUMP_SIZE_X 1	// Salto necesario para inicializar centrados los aliens
#define C_INIT_JUMP_SIZE_Y 1	// Salto necesario para inicializar centrados los aliens
#define INIT_ALIENS_MARGIN_X 60	//margen para donde arrancan los aliens
#define INIT_ALIENS_MARGIN_Y 40	//margen para donde arrancan los aliens
#define JUMP_SIZE_X	1			//salto para los movimientos
#define JUMP_SIZE_Y 1			//salto para los movimientos
#define DISPLAY_MARGIN_X 1	//margen en coordenadas que deseamos tener para no chocarnos con los bordes del display
#define DISPLAY_MARGIN_Y 1 	//margen en coordenadas que deseamos tener para no chocarnos con los bordes del display
#define PLAYER_SIZE_X 3
#define PLAYER_SIZE_Y 2
#define INIT_PLAYER_Y 1	//en que coordenada Y va  a estar
#define BULLET_SIZE_X 1
#define BULLET_SIZE_Y 1
#define NUM_SHIELDS 4	//cantidad de escudos 
//ojo que en la raspberry no cambia el tamaño de los escudos
#define SHIELD_INIT_SIZE_X 3	//tamaño inicial de los escudos
#define SHIELD_INIT_SIZE_Y 2	//tamaño inicial de los escudos
#define SHIELD_INIT_Y 12 //en que coordenada en Y ubicamos los escudos
#define SHIELD_INIT_X_JUMP 3 //cuantas coordenadas hay de espaciado entre cada escudo