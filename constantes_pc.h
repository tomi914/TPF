#ifndef CONSTANTES_PC_H
#define CONSTANTES_PC_H

#define ALIEN_ROWS 5  // Cantidad de FILAS de ALIENS.
#define ALIEN_COLS 11  // Cantidad de COLUMNAS de ALIENS.
#define DISPLAY_HIGH 800	// ALTURA del display en coordenadas (Como depende del display (front-end), lo definimos en el make)
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
//#define INIT_ALIENS_MARGIN_X 60	//margen para donde arrancan los aliens
//#define INIT_ALIENS_MARGIN_Y 100	//margen para donde arrancan los aliens
#define JUMP_SIZE_X	20			//salto para los movimientos
#define JUMP_SIZE_Y 20			//salto para los movimientos
#define DISPLAY_MARGIN_X 50	//margen en coordenadas que deseamos tener para no chocarnos con los bordes del display
#define DISPLAY_MARGIN_Y 100	//margen en coordenadas que deseamos tener para no chocarnos con los bordes del display
#define INIT_PLAYER_Y 672	//en que coordenada Y va  a estar
#define NUM_SHIELDS 4	//cantidad de escudos 
#define SHIELD_X 80 //para q ande la pi tmbien
#define SHIELD_INIT_POSITION_1_X 100
#define SHIELD_INIT_POSITION_2_X 300
#define SHIELD_INIT_POSITION_3_X 600
#define SHIELD_INIT_POSITION_4_X 800
#define SHIELD_INIT_POSITION_Y 500
#define SHIELD_INIT_SIZE_X 80	//tamaño inicial de los escudos
#define SHIELD_INIT_SIZE_Y 60	//tamaño inicial de los escudos
#define SHIELD_1_SIZE_X 80	//tamaño estado 1 de los escudos
#define SHIELD_1_SIZE_Y 60	//tamaño estado 1 de los escudos
#define SHIELD_2_SIZE_X 80	//tamaño estado 2 de los escudos
#define SHIELD_2_SIZE_Y 60	//tamaño estado 2 de los escudos
#define SHIELD_3_SIZE_X 80	//tamaño estado 3 de los escudos
#define SHIELD_3_SIZE_Y 60	//tamaño estado 3 de los escudos
#define SHIELD_4_SIZE_X 80	//tamaño estado 4 de los escudos
#define SHIELD_4_SIZE_Y 60	//tamaño estado 4 de los escudos
#define SHIELD_5_SIZE_X 80	//tamaño estado 4 de los escudos
#define SHIELD_5_SIZE_Y 60	//tamaño estado 4 de los escudos
#define SHIELD_INIT_Y 600 //en que coordenada en Y ubicamos los escudos
#define SHIELD_INIT_X_JUMP (((DISPLAY_LENGTH)-(DISPLAY_MARGIN_X*2))/4) //cuantas coordenadas hay de espaciado entre cada escudo

//agregar tmb a constantes_pi.h
#define PLAYER_SIZE_X 50//VER
#define PLAYER_SIZE_Y 56
#define SPEED_PLAYER 10
#define BULLET_SIZE_X 4
#define BULLET_SIZE_Y 15
#define SPEED_BULLET_PLAYER 5 // Velocidad de la bala del player.
//#define SPEED_BULLET_ALIEN(level) (5 + (((level) - 1) / 2)) // Velocidad de la bala del alien. Aumenta cada 3 niveles: nivel 1,2,3: v1 ; nivel 4,5,6: v2 ...
#define MARGIN_X 50
#define SHOOT_FREQ(level) (80-(level)*5)
//Ovni
#define OVNI_SIZE_X 99
#define OVNI_SIZE_Y 43
#define INIT_OVNI_MARGIN_X 0
#define INIT_OVNI_MARGIN_Y 5
#define SPEED_OVNI 2  // Aumenta la velocidad con el nivel
#define OVNI_SPAWN_INTERVAL 2 //Define el tiempo entre que desaparece un ovni y aparece el otro.
#define BULLET_INDEX(n) (((n) - 1) % 16) // Devuelve un numero entre 0 y 15 de acuerdo a la bala que se disparó
//Puntajes 
#define POINTS_ALIEN_A 30
#define POINTS_ALIEN_B 20
#define POINTS_ALIEN_C 10
#define LEVEL_POINTS(level) (100*(level))


#define DELAY_START 1
#define DELAY_ROW 0.2

#define SCORE_POSITION_X 10
#define SCORE_POSITION_Y 10
#define LIVES_POSITION_X 10
#define LIVES_POSITION_Y (DISPLAY_HIGH - PLAYER_SIZE_Y - 10)
#define LIVES_SPACING 10  // espacio entre naves

#define FPS 100


#define INIT_ALIENS_MARGIN (OVNI_SIZE_Y+JUMP_SIZE_Y)			
#define INIT_SHIELDS_MARGIN_X (DISPLAY_MARGIN_X +(SHIELD_INIT_X_JUMP - SHIELD_INIT_SIZE_X) / 2 + SHIELD_X)			
#define FACTOR_CORRECTOR 0						
#define INIT_PLAYER_MARGIN_X 50				
#define PLAYER_LIMIT_X PLAYER_SIZE_X	


#define SCREEN_W 1000
#define SCREEN_H 800
#define CARPETA "resizedImages/"
#define BULLET_SPEED_ADJUSTMENT 1

#define SHIELD_HEALTH 15
#define HITBOX PLAYER_SIZE_X/2


#endif //CONSTANTES_PC_H