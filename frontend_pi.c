//LIBRERIAS ESTANDAR
#include <stdio.h>
#include <time.h> 			//Para manejo de tiempos (clock)
#include <unistd.h> 		//Para funciones como sleep y usleep
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

//LIBRERIAS LOCALES
#include "./libs/joydisp/joydrv.h" 		// Manejo del joystick
#include "./libs/joydisp/disdrv.h"		// Manejo del display de LEDs
#include "entidades.h"					// Estructuras del juego 
#include "constantes_pi.h"				// Constantes para Raspberry Pi
#include "inputOutputPi.h"				// Entrada/salida para Pi
#include "backend.h"					// Funciones lógicas del juego

int main(void){
	// Se crean todas las variables necesarias para el programa
	player_t player;		// Jugador
	bullet_t pBullet;		// Bala del jugador
	bullet_t aBullet;		// Bala del alien
	aliensBlock_t block;	// Bloque de aliens 
	alien_t alien [ALIEN_ROWS][ALIEN_COLS];	// Arreglo de aliens
	shield_t shield[NUM_SHIELDS];	// Arreglo de escudos
	stats_t stats;			// Estadistucas del juego 
	ovni_t ovni;			// Nave nodriza
	alien_t * shooter;		// Puntero al alien que dispara
	joyinfo_t info;			// Información del joystick
	joy_init();				// Inicializa el joystick
	clock_t clkP = clock(); //clock pause
	clock_t clkS = clock(); //clock select
	clock_t clkO; 			//clock ovni
	int i, j;
	bool shoot;
	int shooterRow;
	int menu = 0;
	bool restart = false;
	bool quit = false;
	srand(time(NULL)); 		// Inicializa el generador con el tiempo actual
	dcoord_t myPoint;		// Struct de coordenadas del LED a encender
	disp_init();			//inicializa el display
	int lastRowToPrint = -1;	// Última fila de aliens que debe imprimirse (para animación por filas)
	clock_t clkB = 0; 		//tiempos para el movimiento de los aliens
	clock_t clkA = 0;
	int offsetX;
	float interval; 

	GameState game = GAME_STATE_START; 

	while(!quit){		//Bucle principal del juego
		switch(game){
			case GAME_STATE_START:		//Cuando el estado del juego es menu de inicio
				disp_clear();
				//Navegacion por el menu con el joystick
				menu += getDirY(&info, &clkS);
				if(menu==2){
					menu = 0;
				} else if(menu==-1){
					menu = 1;
				}
				if(getClick(&info) || restart){
					switch(menu){
						case 0:		//Entra al juego
							restart = false;
							//Inicializa todo
							initPlayer(&player);
							initAliensBlock(&block);
							initAliensArray(alien);
							initGameStats(&stats);
							initShieldsArray(shield);
							initOvni(&ovni, clock(), &clkO);
							pBullet.active = false;
							aBullet.active = false;
							game = GAME_STATE_PLAYING;	//Setea el estado del juego
							break;
						case 1:		//Sale del juego
							game = GAME_STATE_EXIT;	//setea el estado del juego
							break;
					}
				}   
				drawSymbol(menu);
				disp_update();
				break;

			case GAME_STATE_PLAYING:	//Cuando el estado del juego es jugando
				disp_clear(); //Limpia el display
				
				// Muestra los shields en pantalla (si les queda vida)
				for(i=0; i<4; i++){
					if(shield[i].health){
					myPoint.x = shield[i].coord.coordX;
					myPoint.y = shield[i].coord.coordY;
					disp_write(myPoint, D_ON);
					}
                }
				
				//Actualiza el estado del ovni
				updateOvni (&ovni, clock(), &clkO, rand() % 1001);
				// Muestra el ovni en pantalla (si debe estar)
				if(ovni.alive && ovni.visible){
					myPoint.x = ovni.coord.coordX;
					myPoint.y = ovni.coord.coordY;
					disp_write(myPoint, D_ON);
				}
				
				// Actualiza el bloque de aliens
				updateAliensBlock(alien, &block);
				// Se selleciona un alien tirador
				shooter = selectAlienShooter(alien, &block, &player);//elije alien a disparar
				if (shooter != NULL) {	//Si hay tirador nuevo
					shooterRow = -1;
					for (int i = 0; i < ALIEN_ROWS; i++) {
						for (int j = 0; j < ALIEN_COLS; j++) {
							if (&alien[i][j] == shooter) {
								shooterRow = i;		
							}
						}
					}
					//Efectua el disparo del alien
					alienShoot(&aBullet, shooter, stats.level, &block, lastRowToPrint, shooterRow);
				}
				if(aBullet.active){ //si hay una bala de alien, se dibuja
					myPoint.x = aBullet.coord.coordX;
					myPoint.y = aBullet.coord.coordY;
				}
				disp_write(myPoint, D_ON);
				
				//Calcula el tiempo entre movimientos de los aliens segun nivel y posicion
				if(stats.level<17){
					if((block.coord.coordY + block.lastRowAlive*2*ALIEN_B_SIZE_Y)<8){
					interval = 0.09 - (stats.level -1) /200.;
					} else if((block.coord.coordY + block.lastRowAlive*2)<10){
						interval = 0.08 - (stats.level -1) /200.; 
					} else if((block.coord.coordY + block.lastRowAlive*2)<13){
						interval = 0.06 - (stats.level -1) /1000.;
					} else{
						interval = 0.03 - (stats.level -1) /1000.;
					}
				} else{
					interval = 0.02;
				}
				//Mueve visualmente las filas de los aliens una por una 
				if((lastRowToPrint == -1 && ((double)(clock() - clkB) / CLOCKS_PER_SEC) >= interval)){
						lastRowToPrint = ALIEN_ROWS - 1; // empieza desde la fila de abajo
						clkA = clock();
						printf("1\n");
				}
				if((lastRowToPrint >= 0 && ((double)(clock() - clkA) / CLOCKS_PER_SEC) >= interval)){
					if(lastRowToPrint == 0){
						blockMove(alien, &block); 
						lastRowToPrint = -1;
						clkB = clock();
						printf("2, %d\n", block.coord.coordX);
					}
					else{
						lastRowToPrint--;
						clkA = clock();
						printf("3\n");
					}
				}  
				//Dibuja cada alien vivo en su lugar correspondiente
				for (i = 0; i < ALIEN_ROWS; i++) {
					for (j = 0; j < ALIEN_COLS; j++) {
						if (!alien[i][j].alive) {
							continue;
						}
						// Offset del bloque horizontal si hay que aplicarlo (cuando baja una fila o si ya terminó de bajar)
						offsetX = block.coord.coordX;
						if (lastRowToPrint >= 0 && i >= lastRowToPrint) {
							offsetX += block.direction;
						}
						// Coordenadas absolutas del alien actual
						myPoint.x = offsetX + alien[i][j].coord.coordX;
						myPoint.y = block.coord.coordY + alien[i][j].coord.coordY;
						// Encender LED del alien en pantalla
						disp_write(myPoint, D_ON);
					}
				}
				//Verificacion de colisiones (todas)
				collisionDetect(&pBullet, &aBullet, alien, &ovni, shield, &block, &player, &stats, lastRowToPrint, &clkO);
				//Mueve el jugador segun el joystick
				playerMove(getDirX(&info), &player);
				for(i=0; i<3; i++){
					myPoint.x = player.coord.coordX + i;
					myPoint.y = player.coord.coordY;
					disp_write(myPoint, D_ON);
				}
				// Dibuja la cantidadad de vidas como naves del jugador
				for(i=0; i<player.health-1; i++){
					myPoint.x = i;
					myPoint.y = 15;
					disp_write(myPoint, D_ON);
				}
				//Verifica si se debe disparar y actualiza posición de la bala del jugador
				shoot = getClick(&info);
				playerShoot(&pBullet, &player, &shoot);
				if(pBullet.active){
					myPoint.x = pBullet.coord.coordX;
					myPoint.y = pBullet.coord.coordY;
				}
				disp_write(myPoint, D_ON);
				disp_update(); //Muestra en el display
				//Detecta si se solicito pausa
				if(getPause(&info, &clkP)){
					game = GAME_STATE_PAUSED;
				}
				//Detecta si el jugador perdio
				if(!(player.health)){	//si se quedo sin vidas o si los aliens conquistaron la tierra
					game = GAME_STATE_GAMEOVER;
				}
				//Verifica si se debe subir de nivel
				newLevelCheck(alien, &block, &player, shield, &stats);
				break;

			case GAME_STATE_PAUSED:	//menu de pausa
				disp_clear();
				//permite navegar por el menu
				menu += getDirY(&info, &clkS);
				if(menu==3){
					menu = 0;
				} else if(menu==-1){
					menu = 2;
				}
				if(getClick(&info)){
					switch(menu){
					case 0:		//Continuar
						game = GAME_STATE_PLAYING;
						break;	
					case 1:		//salir
						game = GAME_STATE_EXIT;
						break;
					case 2:		//reiniciar
						game = GAME_STATE_START;
						restart = true;
						menu = 0;
						break;
					} 
				}
				//Muestro en pantalla el simbolo de cada opcion
				drawSymbol(menu);
				disp_update();
				break;

			case GAME_STATE_GAMEOVER:	//perdieron el juego
				disp_clear();
				drawNum(0, 6, stats.actualScore);	//se muestra el score
				disp_update();
				sleep(4);
				game = GAME_STATE_START;	//vuelve al menu de inicio
				break;

			case GAME_STATE_EXIT:	//para salir del juego
				quit = true;
				break;

		case GAME_STATE_SCORE_SCREEN:
			break;
		}
	}
	disp_clear();
	disp_update();
	return 0;
}

    
