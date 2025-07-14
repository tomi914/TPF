#ifndef CONSTANTES_PC_H
#define CONSTANTES_PC_H

//DISPLAY
#define DISPLAY_HIGH 800		// ALTURA del display en coordenadas
#define DISPLAY_LENGTH 1000 	// LARGO del display en coordenadas 
#define DISPLAY_MARGIN_X 50		// Margen en coordenadas entre los limites del display real y el display utilizable
#define DISPLAY_MARGIN_Y 100	// Margen en coordenadas entre los limites del display real y el display utilizable
//ALIENS
#define ALIEN_ROWS 5  			// Cantidad de FILAS de ALIENS.
#define ALIEN_COLS 11  			// Cantidad de COLUMNAS de ALIENS.
#define ALIEN_A_SIZE_X 50 		// LARGO en coordenadas que ocupa un ALIEN del tipo A
#define ALIEN_A_SIZE_Y 51 		// ALTO en coordenadas que ocupa un ALIEN del tipo A
#define ALIEN_B_SIZE_X 56 		// LARGO en coordenadas que ocupa un ALIEN del tipo B
#define ALIEN_B_SIZE_Y 41 		// ALTO en coordenadas que ocupa un ALIEN del tipo B
#define ALIEN_C_SIZE_X 60 		// LARGO en coordenadas que ocupa un ALIEN del tipo C
#define ALIEN_C_SIZE_Y 40 		// ALTO en coordenadas que ocupa un ALIEN del tipo C 
#define A_INIT_JUMP_SIZE_X 20	// Salto necesario para inicializar centrados los aliens del tipo A
#define A_INIT_JUMP_SIZE_Y 10	// Salto necesario para inicializar centrados los aliens del tipo A
#define B_INIT_JUMP_SIZE_X 14	// Salto necesario para inicializar centrados los aliens del tipo B
#define B_INIT_JUMP_SIZE_Y 10	// Salto necesario para inicializar centrados los aliens del tipo B
#define C_INIT_JUMP_SIZE_X 10	// Salto necesario para inicializar centrados los aliens del tipo C
#define C_INIT_JUMP_SIZE_Y 10	// Salto necesario para inicializar centrados los aliens del tipo C
#define INIT_ALIENS_MARGIN_Y (OVNI_SIZE_Y+JUMP_SIZE_Y)	// Coordenada Y de inicializacion del bloque de aliens
#define JUMP_SIZE_X	20			// Salto en coordenadas para cada movimiento del bloque de aliens
#define JUMP_SIZE_Y 20			// Salto en coordenadas para cada movimiento del bloque de aliens

//PLAYER
#define INIT_PLAYER_Y 672		// Coordenada Y de inicializacion del jugador
#define INIT_PLAYER_X 50	
#define PLAYER_SIZE_X 50		// Tamaño del jugador
#define PLAYER_SIZE_Y 56		// Tamaño del jugador

//SHIELDS
#define NUM_SHIELDS 4			// Cantidad de escudos
#define SHIELD_INIT_Y 600 		// Coordenada Y de inicializacion de los escudos
#define INIT_SHIELDS_MARGIN_X (DISPLAY_MARGIN_X +(SHIELD_INIT_X_JUMP - SHIELD_INIT_SIZE_X) / 2 + SHIELD_X)
#define SHIELD_INIT_X_JUMP (((DISPLAY_LENGTH)-(DISPLAY_MARGIN_X*2))/4) 	// Calculo de espaciado en X para la inicializacion de escudos
#define SHIELD_INIT_SIZE_X 80	// Tamaño inicial de los escudos
#define SHIELD_INIT_SIZE_Y 60	// Tamaño inicial de los escudos
#define SHIELD_1_SIZE_X 80		// tamaño estado 1 de los escudos (en la pc el tamaño de los escudos es cte)
#define SHIELD_1_SIZE_Y 60		// tamaño estado 1 de los escudos						|
#define SHIELD_2_SIZE_X 80		// tamaño estado 2 de los escudos						|
#define SHIELD_2_SIZE_Y 60		// tamaño estado 2 de los escudos						|
#define SHIELD_3_SIZE_X 80		// tamaño estado 3 de los escudos						|
#define SHIELD_3_SIZE_Y 60		// tamaño estado 3 de los escudos						|
#define SHIELD_4_SIZE_X 80		// tamaño estado 4 de los escudos						|
#define SHIELD_4_SIZE_Y 60		// tamaño estado 4 de los escudos						|
#define SHIELD_5_SIZE_X 80		// tamaño estado 4 de los escudos						|
#define SHIELD_5_SIZE_Y 60		// tamaño estado 4 de los escudos (en la pc el tamaño de los escudos es cte)
#define SHIELD_HEALTH 15		// Cuantas vidas tiene cada escudo

//BULLETS
#define BULLET_SIZE_X 4			// Tamaño de las balas
#define BULLET_SIZE_Y 15		// Tamaño de las balas
#define SPEED_BULLET_PLAYER 5 	// Velocidad de la bala del player.

//OVNI
#define OVNI_SIZE_X 99				// Tamaño del ovni
#define OVNI_SIZE_Y 43				// Tamaño del ovni
#define INIT_OVNI_MARGIN_X 0		//Coordenada de inicializacion del ovni
#define INIT_OVNI_MARGIN_Y 5		//Coordenada de inicializacion del ovni
#define SPEED_OVNI 2  				// Aumenta la velocidad con el nivel
#define OVNI_SPAWN_INTERVAL 2 		//Define el tiempo entre que desaparece un ovni y aparece el otro.

//GAMESTATS
#define MAX_TOP_SCORES 10		// Cantidad de scores almacenados
#define POINTS_ALIEN_A 30		// Puntaje obtenido por matar al alien A
#define POINTS_ALIEN_B 20		// Puntaje obtenido por matar al alien B
#define POINTS_ALIEN_C 10		// Puntaje obtenido por matar al alien C
#define LEVEL_POINTS(level) (100*(level))	// Puntaje obtenido por subir de nivel
#define SCORE_POSITION_X 10		// Coordenada para ubicar el letreto de score
#define SCORE_POSITION_Y 10		// Coordenada para ubicar el letreto de score
#define LIVES_POSITION_X 10		// Coordenada para ubicar las naves que indican la vida restante
#define LIVES_POSITION_Y (DISPLAY_HIGH - PLAYER_SIZE_Y - 10)	// Coordenada para ubicar las naves que indican la vida restante
#define LIVES_SPACING 10 		// Espaciado para ubicar las naves que indican la vida restante


//FALTA COMENTAR Y CLASIFICAR
#define DELAY_START 1
#define DELAY_ROW 0.2
#define FPS 100
#define FACTOR_CORRECTOR 0	
#define CARPETA "resizedImages/"
#define BULLET_SPEED_ADJUSTMENT 1
#define HITBOX PLAYER_SIZE_X/2 //que hace?
#define SHIELD_X 80 			//para q ande la pi tmbien      que hace?			
			


//QUE HACEMOS CON ESTO?

//#define SHIELD_INIT_POSITION_Y 500
//#define SCREEN_W 1000				//HAY QUE CAMBIAR EL FRONT Y PONER LAS OTRAS
//#define SCREEN_H 800				//HAY QUE CAMBIAR EL FRONT Y PONER LAS OTRAS
#define SHOOT_FREQ(level) (80-(level)*5)	//SE TERMINA HACIENDO NEGATIVO A PARTIR DEL NIVEL 16
#define SPEED_PLAYER 10			// Velocidad de desplazamiento lateral del jugador						NO MODIFICA NADA
#define PLAYER_LIMIT_X PLAYER_SIZE_X		//es lo mismo que porner PLAYER_SIZE_X
//#define BULLET_INDEX(n) (((n) - 1) % 16) // Devuelve un numero entre 0 y 15 de acuerdo a la bala que se disparó
//#define SPEED_BULLET_ALIEN(level) (5 + (((level) - 1) / 2)) // Velocidad de la bala del alien. Aumenta cada 3 niveles: nivel 1,2,3: v1 ; nivel 4,5,6: v2 ...
//#define MARGIN_X 50

#endif //CONSTANTES_PC_H
