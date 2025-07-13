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
#define ALIEN_C_SIZE_Y 1 // Cuantos ocupa un ALIEN a lo LARGO en coordenadas (Como depende del display (front-end), lo definimos en el make)
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
#define INIT_PLAYER_Y 14	//en que coordenada Y va  a estar
#define BULLET_SIZE_X 1
#define BULLET_SIZE_Y 1
#define NUM_SHIELDS 4	//cantidad de escudos 
//ojo que en la raspberry no cambia el tamaño de los escudos
#define SHIELD_INIT_SIZE_X 1	//tamaño inicial de los escudos
#define SHIELD_INIT_SIZE_Y 1	//tamaño inicial de los escudos
#define SHIELD_INIT_Y 12 //en que coordenada en Y ubicamos los escudos
#define SHIELD_INIT_X_JUMP 3 //cuantas coordenadas hay de espaciado entre cada escudo
#define SHIELD_X 0
#define SPEED_PLAYER 0.05 
#define SPEED_BULLET_PLAYER 0.05
#define SHIELD_1_SIZE_X 1
#define SHIELD_2_SIZE_X 1
#define SHIELD_3_SIZE_X 1
#define SHIELD_4_SIZE_X 1
#define SHIELD_5_SIZE_X 1

#define POINTS_ALIEN_A 10
#define POINTS_ALIEN_B 20
#define POINTS_ALIEN_C 30
#define LEVEL_POINTS(level) (100*(level))

#define OVNI_SIZE_X 1
#define OVNI_SIZE_Y 1
#define INIT_OVNI_MARGIN_X 1
#define INIT_OVNI_MARGIN_Y -1
#define SPEED_OVNI  0.009// Aumenta la velocidad con el nivel
#define OVNI_SPAWN_INTERVAL 2 //Define el tiempo entre que desaparece un ovni y aparece el otro.
#define BULLET_INDEX(n) (((n) - 1) % 16) // Devuelve un numero entre 0 y 15 de acuerdo a la bala que se disparó

#define INIT_ALIENS_MARGIN 0
#define INIT_SHIELDS_MARGIN_X 0
#define FACTOR_CORRECTOR 1
#define INIT_PLAYER_MARGIN_X 0
#define PLAYER_LIMIT_X 0
#define BULLET_SPEED_ADJUSTMENT 1000
#define SHIELD_HEALTH 6
#define HITBOX 0
#define SHOOT_FREQ(level) (80-(level)*5)
