#ifndef CONSTANTES_PI_H
#define CONSTANTES_PI_H

//DISPLAY
#define DISPLAY_HIGH 16		// ALTURA del display en coordenadas
#define DISPLAY_LENGTH 16 	// LARGO del display en coordenadas 
#define DISPLAY_MARGIN_X 1	// Margen en coordenadas entre los limites del display real y el display utilizable
#define DISPLAY_MARGIN_Y 1	// Margen en coordenadas entre los limites del display real y el display utilizable
//ALIENS
#define ALIEN_ROWS 4  			// Cantidad de FILAS de ALIENS.
#define ALIEN_COLS 5  			// Cantidad de COLUMNAS de ALIENS.
#define ALIEN_A_SIZE_X 1 		// LARGO en coordenadas que ocupa un ALIEN del tipo A
#define ALIEN_A_SIZE_Y 1 		// ALTO en coordenadas que ocupa un ALIEN del tipo A
#define ALIEN_B_SIZE_X 1		// LARGO en coordenadas que ocupa un ALIEN del tipo B
#define ALIEN_B_SIZE_Y 1 		// ALTO en coordenadas que ocupa un ALIEN del tipo B
#define ALIEN_C_SIZE_X 1 		// LARGO en coordenadas que ocupa un ALIEN del tipo C
#define ALIEN_C_SIZE_Y 1 		// ALTO en coordenadas que ocupa un ALIEN del tipo C 
#define ALIEN_EXTREME 2			// Margen para movimiento aliens con el desfase
#define A_INIT_JUMP_SIZE_X 1	// Salto necesario para inicializar centrados los aliens del tipo A
#define A_INIT_JUMP_SIZE_Y 1	// Salto necesario para inicializar centrados los aliens del tipo A
#define B_INIT_JUMP_SIZE_X 1	// Salto necesario para inicializar centrados los aliens del tipo B
#define B_INIT_JUMP_SIZE_Y 1	// Salto necesario para inicializar centrados los aliens del tipo B
#define C_INIT_JUMP_SIZE_X 1	// Salto necesario para inicializar centrados los aliens del tipo C
#define C_INIT_JUMP_SIZE_Y 1	// Salto necesario para inicializar centrados los aliens del tipo C
#define INIT_ALIENS_MARGIN 0	// No se usa en esta interfaz
#define JUMP_SIZE_X	1			// Salto en coordenadas para cada movimiento del bloque de aliens
#define JUMP_SIZE_Y 1			// Salto en coordenadas para cada movimiento del bloque de aliens
#define SHOOT_FREQ(level) (80-(level)*5)	//SE TERMINA HACIENDO NEGATIVO A PARTIR DEL NIVEL 16

//PLAYER
#define INIT_PLAYER_Y 14		// Coordenada Y de inicializacion del jugador
#define INIT_PLAYER_MARGIN_X 0	// Coordenada X de inicializacion del jugador   CAMBIAR NOMBRE PARA QUE COINCIDA 
#define PLAYER_SIZE_X 3			// Tamaño del jugador
#define PLAYER_SIZE_Y 1			// Tamaño del jugador
#define PLAYER_LIMIT_X 0		// No se usa en esta interfaz
#define SPEED_PLAYER 0.05		// Velocidad de desplazamiento lateral del jugador	
#define HITBOX 0 				//Corrige coordenadas

//SHIELDS
#define NUM_SHIELDS 4			// Cantidad de escudos
#define SHIELD_INIT_Y 12 		// Coordenada Y de inicializacion de los escudos
#define INIT_SHIELDS_MARGIN_X 0	// No se usa en esta interfaz
#define SHIELD_INIT_X_JUMP 3 	// Calculo de espaciado en X para la inicializacion de escudos
#define SHIELD_INIT_SIZE_X 1	// Tamaño inicial de los escudos
#define SHIELD_INIT_SIZE_Y 1	// Tamaño inicial de los escudos
#define SHIELD_HEALTH 6			// Cuantas vidas tiene cada escudo
#define CORRECTION_FACTOR 1		// Factor de correcion para inicializar los esudos
#define SHIELD_X 0 				// No se usa en esta interfaz

//BULLETS
#define BULLET_SIZE_X 1				// Tamaño de las balas
#define BULLET_SIZE_Y 1				// Tamaño de las balas
#define SPEED_BULLET_PLAYER 0.09 	// Velocidad de la bala del player
#define BULLET_SPEED_ADJUSTMENT 300 // Al modificarla cambia la velocidad de la bala disparada por los aliens, mientras mas chica mas rapido va

//OVNI
#define OVNI_SIZE_X 1				// Tamaño del ovni
#define OVNI_SIZE_Y 1				// Tamaño del ovni
#define INIT_OVNI_MARGIN_X 1		// Coordenada de inicializacion del ovni
#define INIT_OVNI_MARGIN_Y -1		// Coordenada de inicializacion del ovni
#define SPEED_OVNI 0.009  			// Aumenta la velocidad con el nivel
#define OVNI_SPAWN_INTERVAL 2 		// Define el tiempo entre que desaparece un ovni y aparece el otro.

//GAMESTATS
#define POINTS_ALIEN_A 30		// Puntaje obtenido por matar al alien A
#define POINTS_ALIEN_B 20		// Puntaje obtenido por matar al alien B
#define POINTS_ALIEN_C 10		// Puntaje obtenido por matar al alien C
#define LEVEL_POINTS(level) (100*(level))	// Puntaje obtenido por subir de nivel

#endif //CONSTANTES_PC_H
