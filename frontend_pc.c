#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <time.h>
#include "entidades.h"
#include "constantes_pc.h"
#include "backend.h"


// Identifico cada imagen con un número para poder cargarlas en el arreglo
enum {
    IMG_FONDO = 0,
    
    IMG_LOGO = 1,
    
    IMG_PLAY_ON = 2,
    IMG_PLAY_OFF = 3,
    
    IMG_RESUME_ON = 4,
    IMG_RESUME_OFF = 5,
    
    IMG_RESTART_ON = 6,
    IMG_RESTART_OFF = 7,
    
    IMG_RESTART_ON_GAMEOVER = 8,
    IMG_RESTART_OFF_GAMEOVER = 9,
    
    IMG_QUIT_ON = 10,
    IMG_QUIT_OFF = 11,
    
    IMG_TOTAL = 12
};

// Tenemos 3 TIPOS distintos de MENÚ
typedef enum{
    MENU_TYPE_START = 0,
    MENU_TYPE_PAUSE = 1,
    MENU_TYPE_GAMEOVER = 2
} MenuType;


// Cada menu va a tener una cantidad específica de OPCIONES

// Menú de INICIO
typedef enum{
  START_OPTION_PLAY = 0,
  START_OPTION_QUIT = 1
} StartMenuOption;

// Menú de PAUSA
typedef enum{
  PAUSE_OPTION_RESUME = 0,
  PAUSE_OPTION_RESTART = 1,
  PAUSE_OPTION_QUIT = 2
} PauseMenuOption;

// Menú de GAMEOVER
typedef enum{
  GAMEOVER_OPTION_RESTART = 0,
  GAMEOVER_OPTION_QUIT = 1
} GameoverMenuOption;

// Tenemos 5 ESTADOS de JUEGO
typedef enum {
    GAME_STATE_START = 0,
    GAME_STATE_PLAYING = 1,
    GAME_STATE_PAUSED = 2,
    GAME_STATE_GAMEOVER = 3,
    GAME_STATE_EXIT = 4,
    GAME_STATE_SCORE_SCREEN = 5
} GameState;

// PROTOTIPOS
//void handleGameState(bool keys[], int *selectedOption, GameState *gameState, ALLEGRO_BITMAP *imagenes[]);
void drawMenu(ALLEGRO_BITMAP *imagenes[IMG_TOTAL], int selectedOption, MenuType type);
void handleMenuInput(bool keys[], int *selectedOption, GameState *currentGameState, MenuType type, player_t * player, aliensBlock_t * aliensBlock, alien_t aliens[ALIEN_ROWS][ALIEN_COLS], shield_t shields[NUM_SHIELDS], stats_t * gameStats);

int main() {

    // Inicializamos la biblioteca Allegro.
    if (!al_init()) {
        fprintf(stderr, "Error al inicializar Allegro\n");
        return -1;
    }

    al_install_keyboard(); // Habilitamos el uso del teclado
    al_init_image_addon();// Habilitamos el uso de imágenes
    al_init_font_addon(); // Inicializamos fuentes (creo que no vamos a usar)
    al_init_ttf_addon(); // Inicializamos tipos de fuentes 

    // Creamos la ventana de tamaño SCREEN_W x SCREEN_H, que va a ser el display
    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        fprintf(stderr, "Error al crear display\n");
        return -1;
    }

    // Creamos la cola de eventos, donde van a llegar los eventos del teclado, display, timer, etc.
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();

    // Creamos un temporizador para tener una tasa de refresco de 60fps.
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);

    // Agregamos las fuentes de eventos a la cola
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    // Array que guarda qué teclas están presionadas actualmente. Las incializamos todas como inactivas.
    bool keys[ALLEGRO_KEY_MAX] = {false};

    // Indica si tenemos que redibujar la pantalla.
    bool redraw = true;

    // Indica el estado actual del juego (tenemos 5 estados disponibles). Lo incializamos en INICIO.
    GameState gameState = GAME_STATE_START;

    // Opción seleccionada cuando estamos en los menús. La incializamos en START_OPTION_PLAY porque arrancamos en el menú de inicio y queremos que apunte directo a PLAY.
    int selectedOption = START_OPTION_PLAY;
    
    
    
    // Variables del juego
    player_t player;
    ALLEGRO_BITMAP *image[9];
    ALLEGRO_BITMAP *shield[7];
    ALLEGRO_BITMAP *bulletImg;
    alien_t aliens[ALIEN_ROWS][ALIEN_COLS];
    aliensBlock_t aliensBlock;
    bullet_t playerBullet = {0};
    bullet_t alienBullet = {0};
    shield_t shields[NUM_SHIELDS];
    stats_t gameStats;
    srand(time(NULL));
    clock_t clock();
    clock_t clkO;
    double timeScoreScreenStart = 0;
    bool tryShoot = false;
    float interval;
    ovni_t ovni = { .visible = false, .alive = true };  // Estado inicial
	//double lastOvniDespawnTime;
	initOvni(&ovni, clock(), &clkO);
    double timeLastMovAlien = 0;
    double timeLastRow = 0;
    int lastRowToPrint = -1;
    bool alienFrameFila[ALIEN_ROWS] = {false};
	// IMAGENES 
	char img[50];
	for (int i = 0; i < 9; i++) {
		sprintf(img, "imagenes/img%d.png", i);
		image[i] = al_load_bitmap(img);
		if (!image[i]) {
		    fprintf(stderr, "Error: no se pudo cargar la imagen %s\n", img);
		    return -1;
		}
	}
	for (int i = 1; i < 6; i++) {
		sprintf(img, "imagenes/imagenesNuevas/Shield%d.png", i);
	   shield[i-1] = al_load_bitmap(img);
		if (!shield[i-1]) {
		    fprintf(stderr, "Error: no se pudo cargar la imagen %s\n", img);
		    return -1;
		}
	}
	bulletImg = al_load_bitmap("imagenes/imagenesNuevas/bullet.png");
		if (!bulletImg) {
		    fprintf(stderr, "Error: no se pudo cargar la imagen %s\n", img);
		    return -1;
		}
		
	ALLEGRO_FONT *font = al_load_ttf_font("PressStart2P-Regular.ttf", 14, 0);
	if (!font) {
		fprintf(stderr, "No se pudo cargar la fuente\n");
		return -1;
	}

	// Creamos un arreglo donde vamos a guardar las imágenes, para mayor practicidad.
	ALLEGRO_BITMAP * imagenes[IMG_TOTAL]; 

	const char *rutas[IMG_TOTAL] = { // (CARPETA es una macro definida que indica 'carpeta/')
         "imagenes/fondo.png",
         "imagenes/spaceInvaders.png",
         "imagenes/playG.png",
         "imagenes/playW.png",
         "imagenes/resumeG.png",
         "imagenes/resumeW.png",
         "imagenes/restartG.png",
         "imagenes/restartW.png",
         "imagenes/restartBigG.png",
         "imagenes/restartBigW.png",
         "imagenes/quitG.png",
         "imagenes/quitW.png"
    };

    // Cargamos cada imagen del array rutas al array imagenes.
    for (int i = 0; i < IMG_TOTAL; i++) {
        imagenes[i] = al_load_bitmap(rutas[i]);
        if (!imagenes[i]) {
            fprintf(stderr, "Error cargando imagen: %s\n", rutas[i]);
            return -1;
        }
    }
	
	
    initPlayer(&player);
    initAliensBlock(&aliensBlock);
    initAliensArray(aliens);
    initShieldsArray(shields);
    initGameStats(&gameStats);
    
    
    //Iniciamos el temporizador (manda eventos cada 1/60 segundos)
    al_start_timer(timer);

    // BUCLE PRINCIPAL DEL JUEGO
    while (gameState != GAME_STATE_EXIT) {
        ALLEGRO_EVENT ev;
        
        // Espera (bloquea) hasta que llegue un evento a la cola
        al_wait_for_event(event_queue, &ev);


        // Si cerramos la ventana desde la (x) 
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            gameState = GAME_STATE_EXIT; // Se va del loop principal.
        }

        // Si pasa el tiempo en que configuramos el reloj (1/60 de segundo)
        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;  // Indicamos que se tiene que redibujar la pantalla
        }

        // Si se presionó una tecla
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            keys[ev.keyboard.keycode] = true;  // Guardamos que esa tecla está presionada
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			keys[ev.keyboard.keycode] = false;

			if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {
				tryShoot = false;
			}
		}
        // Si hay que dibujar algo nuevo, y no hay eventos pendientes
        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;  // Como ya vamos a redibujar, lo ponemos en falso para los proximos

            // Maneja lo que se debe mostrar en pantalla según el estado del juego
            
    	switch (gameState) {
		    case GAME_STATE_START:
		        handleMenuInput(keys, &selectedOption, &gameState, MENU_TYPE_START, &player, &aliensBlock, aliens, shields, &gameStats);
		        drawMenu(imagenes, selectedOption, MENU_TYPE_START);
		        break;
		        
		    case GAME_STATE_PLAYING:
		        
		        switch (ev.type) {

		        case ALLEGRO_EVENT_TIMER: {
				        // ======= ACTUALIZACIONES =======
				    if (keys[ALLEGRO_KEY_LEFT]) {
				        playerMove(-1, &player);
				    }
				    if (keys[ALLEGRO_KEY_RIGHT]) {
				        playerMove(1, &player);
				    }
				    if (keys[ALLEGRO_KEY_UP]) {
				        tryShoot = true;
				    }
				    if(keys[ALLEGRO_KEY_SPACE]) {
				        gameState= GAME_STATE_PAUSED;
				    }
				    if (keys[ALLEGRO_KEY_ESCAPE]) { // Se presiona QUIT
		                gameState = GAME_STATE_EXIT; // Se cambia el estado del juego a EXIT
		            }
				    
				    playerShoot(&playerBullet, &player, &tryShoot);

				    // Movimiento aliens fila por fila
				   if(gameStats.level<17){
						if((aliensBlock.coord.coordY)<240){
						interval = 0.1 - (gameStats.level -1) /200.;
						} else if((aliensBlock.coord.coordY)<300){
						    interval = 0.07 - (gameStats.level -1) /200.; 
						} else if((aliensBlock.coord.coordY)<350){
						    interval = 0.05 - (gameStats.level -1) /1000.;
						} else{
						    interval = 0.03 - (gameStats.level -1) /1000.;
						}
					} else{
						interval = 0.02;
					}
					
				    // Movimiento aliens fila por fila
				    double actualTime = al_get_time();
				    if (lastRowToPrint == -1 && actualTime - timeLastMovAlien >= interval) {
				        lastRowToPrint = ALIEN_ROWS - 1;
				        timeLastRow = actualTime;
				        
				    }

				    if (lastRowToPrint >= 0 && actualTime - timeLastRow >= interval) {
				        if (lastRowToPrint == 0) {
				            alienFrameFila[0] = !alienFrameFila[0];
				            blockMove(aliens, &aliensBlock);
				            lastRowToPrint = -1;
				            timeLastMovAlien = actualTime;
				        } else {
				            alienFrameFila[lastRowToPrint] = !alienFrameFila[lastRowToPrint];
				            lastRowToPrint--;
				            timeLastRow = actualTime;
				        }
				    }

				    // ======= DIBUJO =======
				    al_clear_to_color(al_map_rgb(0, 0, 0));

				    for (int j = 0; j < NUM_SHIELDS; j++) {//dibuja los escudos dependiendo su vida VER IMAGENES, TAMANO, Y POSICION
						if (shields[j].health > 0) {
							int index;
							if (shields[j].health > 12){
								index = 0;
							}
							else if (shields[j].health > 9){
								index = 1;
							}
							else if (shields[j].health > 6){
								index = 2;
							}
							else if (shields[j].health > 3){
								index = 3;
							}
							else if (shields[j].health > 0){
								index = 4;
							}
							else{
								index=5;
							}
							if(index<5){
								al_draw_bitmap(shield[index], shields[j].coord.coordX, shields[j].coord.coordY, 0);
							}
						}
					}

				    al_draw_bitmap(image[7], player.coord.coordX - PLAYER_SIZE_X / 2, player.coord.coordY, 0); //dibuja al jugador
					char scoreText[30];
					sprintf(scoreText, "Score: %d", gameStats.actualScore);
					al_draw_text(font, al_map_rgb(0, 255, 0), SCORE_POSITION_X, SCORE_POSITION_Y, 0, scoreText);//dibuja el score
						
						
					if (ovni.visible) {
						al_draw_bitmap(image[6], ovni.coord.coordX, ovni.coord.coordY, 0);//dibuja el ovni
					}
		
					for (int i = 0; i < player.health; i++) {
						int drawX = LIVES_POSITION_X + i * (PLAYER_SIZE_X + LIVES_SPACING);
						al_draw_bitmap(image[7], drawX, LIVES_POSITION_Y, 0);
					}
					if(player.health==0){
						gameState = GAME_STATE_SCORE_SCREEN;
    					timeScoreScreenStart = al_get_time();  // Guardás el tiempo de inicio
					}//poner condicion que se termine el juego, sino se castean las vidas


					


				    if (playerBullet.active){
				        al_draw_bitmap(bulletImg, playerBullet.coord.coordX, playerBullet.coord.coordY, 0);//dibuja la bala del jugador
					}
				    if (alienBullet.active){
				       al_draw_bitmap(bulletImg, alienBullet.coord.coordX, alienBullet.coord.coordY, 0);//dibuja la bala del alien
					}
				    for (int i = 0; i < ALIEN_ROWS; i++) {
				        for (int j = 0; j < ALIEN_COLS; j++) {//dibuja los aliens y su desfase
				            if (!aliens[i][j].alive){
				           		continue;
							}
				            int offsetX = aliensBlock.coord.coordX;
				            if (lastRowToPrint >= 0 && i >= lastRowToPrint){
				                offsetX += JUMP_SIZE_X * aliensBlock.direction;
				            }
				            

				            int drawX = offsetX + aliens[i][j].coord.coordX;
				            int drawY = aliensBlock.coord.coordY + aliens[i][j].coord.coordY;

				            if (i > 2){
				                al_draw_bitmap(alienFrameFila[i] ? image[5] : image[4], drawX, drawY, 0);
				            }
				            else if (i > 0){
				                al_draw_bitmap(alienFrameFila[i] ? image[3] : image[2], drawX, drawY, 0);
				            }
				            else{
				                al_draw_bitmap(alienFrameFila[i] ? image[1] : image[0], drawX, drawY, 0);
				            }
				        }
				   
				    }
				 
		

				    al_flip_display();

					collisionDetect(&playerBullet, &alienBullet, aliens, &ovni, shields, &aliensBlock, &player, &gameStats, lastRowToPrint, &clkO);

				    updateAliensBlock(aliens, &aliensBlock);
				
				    alien_t *shooter = selectAlienShooter(aliens, &aliensBlock, &player);//elije alien a disparar
				    if (shooter != NULL) {
				        int shooterRow = -1;
				        for (int i = 0; i < ALIEN_ROWS; i++) {
				           for (int j = 0; j < ALIEN_COLS; j++) {
				                if (&aliens[i][j] == shooter) {
				                    shooterRow = i;
				                }
				            }
				        }
				        alienShoot(&alienBullet, shooter, gameStats.level, &aliensBlock, lastRowToPrint, shooterRow);
				    }
				    
				    updateOvni (&ovni, clock(), &clkO, rand() % 1001);



					bool allDead = true;

					for (int i = 0; i < ALIEN_ROWS; i++) {
						for (int j = 0; j < ALIEN_COLS; j++) {
							if (aliens[i][j].alive) {
								allDead = false;
								break;  // ya con uno vivo, no hace falta seguir
							}
						}
						if (!allDead) break;
					}

					if (allDead) {
						// SUBIR DE NIVEL
						gameStats.level++;
						initPlayer(&player);
						initAliensBlock(&aliensBlock);
						initAliensArray(aliens);
						initShieldsArray(shields);
					}


					

					
				    break;
		    	}
		        }
		        break;

		    case GAME_STATE_PAUSED:
		        handleMenuInput(keys, &selectedOption, &gameState, MENU_TYPE_PAUSE, &player, &aliensBlock, aliens, shields, &gameStats);
		        drawMenu(imagenes, selectedOption, MENU_TYPE_PAUSE);
		        break;

		    case GAME_STATE_GAMEOVER:
		        handleMenuInput(keys, &selectedOption, &gameState, MENU_TYPE_GAMEOVER, &player, &aliensBlock, aliens, shields, &gameStats);
		        drawMenu(imagenes, selectedOption, MENU_TYPE_GAMEOVER);
		        break;
		        
		        
		    case GAME_STATE_SCORE_SCREEN:
				al_clear_to_color(al_map_rgb(0, 0, 0));
				
				char scoreText[50];
				sprintf(scoreText, "SCORE: %d", gameStats.actualScore);

				// Centrar horizontal y verticalmente
				int textWidth = al_get_text_width(font, scoreText);
				int textHeight = al_get_font_line_height(font);

				al_draw_text(font, al_map_rgb(255, 255, 255),
						     (SCREEN_W - textWidth) / 2,
						     (SCREEN_H - textHeight) / 2,
						     0, scoreText);

				// Mostrar por 3 segundos
				if (al_get_time() - timeScoreScreenStart >= 3.0) {
					gameState = GAME_STATE_GAMEOVER;
					selectedOption = GAMEOVER_OPTION_RESTART; // reinicia la selección
				}

    		break;

			}
			
		        // Muestra en pantalla todo lo que se dibujó desde la última vez
		        al_flip_display();
		    }
		}

    // Destruimos los recursos de Allegro
    for (int i = 0; i < IMG_TOTAL; i++) {
        if (imagenes[i])
            al_destroy_bitmap(imagenes[i]);
    }
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}


// Funciones MENÚ
/* Esta función se va a encargar de seleccionar las opciones, de acuerdo a que teclas presione el usuario y en que tipo de menú se encuentre.*/
void handleMenuInput(bool keys[], int *selectedOption, GameState *currentGameState, MenuType type, player_t * player, aliensBlock_t * aliensBlock, alien_t aliens[ALIEN_ROWS][ALIEN_COLS], shield_t shields[NUM_SHIELDS], stats_t * gameStats) {
    
    int numOptions; // Cada menú tiene una cantidad distinta de opciones
    
    switch (type) { // De acuerdo en que menú nos encontremos, cargamos la cantidad de opciones en 'numOptions'.
      case MENU_TYPE_START: numOptions = 2; break;
      case MENU_TYPE_PAUSE: numOptions = 3; break;
      case MENU_TYPE_GAMEOVER: numOptions = 2; break;
      default: numOptions = 0; break;
    }
    
    
    // Acá vamos moviendo el cursor.
    if (keys[ALLEGRO_KEY_UP]) { // Si se presiona la tecla hacia arriba, nos movemos una opción hacia arriba.
        (*selectedOption)--;
        
        if (*selectedOption < 0) // Si es el caso particular de que estabamos parados en la primera, saltamos a la última.
            *selectedOption = numOptions - 1;
            
        keys[ALLEGRO_KEY_UP] = false; // Desactivamos la tecla (ya se usó)
    }
    
    if (keys[ALLEGRO_KEY_DOWN]) { // Si se presiona la tecla hacia abajo, nos movemos una opción hacia abajo
        (*selectedOption)++;
        
        if (*selectedOption > numOptions - 1) // Si es el caso particular de que estabamos parados en la última fila, saltamos a la primera
          *selectedOption = 0;
          
        keys[ALLEGRO_KEY_DOWN] = false; // Desactivamos la tecla (ya se usó)
    }
    
    
    // Acá seleccionamos la opción sobre la que estamos parados y, de acuerdo a eso, cambiamos el estado del juego.
    if (keys[ALLEGRO_KEY_ENTER]) { // Si se presiona la tecla ENTER
        
        switch (type) { // De acuerdo al tipo de menú en el que nos encontramos, se interpretara la selected option de distinta manera
        
            case MENU_TYPE_PAUSE: // Si nos encontramos en el MENÚ de pausa
            
                if (*selectedOption == PAUSE_OPTION_RESUME) { // Se presiona RESUME
                    *currentGameState = GAME_STATE_PLAYING; // Se cambia el estado del juego a PLAYING
                } 
                
                else if (*selectedOption == PAUSE_OPTION_RESTART) { // Se presiona RESTART
                    *currentGameState = GAME_STATE_PLAYING; // Se cambia el estado del juego a PLAYING
                    initPlayer(player);
                    initAliensBlock(aliensBlock);
                    initAliensArray(aliens);
                    initShieldsArray(shields);
                    initGameStats(gameStats);
                } 
                
                else if (*selectedOption == PAUSE_OPTION_QUIT) { // Se presiona QUIT
                    *currentGameState = GAME_STATE_EXIT; // Se cambia el estado del juego a EXIT
                }
                break;

            case MENU_TYPE_START: // Si nos encontramos en el MENÚ de INICIO
            
                if (*selectedOption == START_OPTION_PLAY) { // Se presiona PLAY
                    *currentGameState = GAME_STATE_PLAYING; // Se cambia el estado del juego a PLAYING
                } 
                
              else if (*selectedOption == START_OPTION_QUIT) { // Se presiona QUIT
                    *currentGameState = GAME_STATE_EXIT; // Se cambia el estado del juego a EXIT
                }
                break;

            case MENU_TYPE_GAMEOVER: // Si nos encontramos en el MENÚ de GAMEOVER (cuando perdemos)
            
                if (*selectedOption == GAMEOVER_OPTION_RESTART) { // Se presiona RESTART
                    *currentGameState = GAME_STATE_PLAYING; // Se cambia el estado a PLAYING
                    initPlayer(player);
                    initAliensBlock(aliensBlock);
                    initAliensArray(aliens);
                    initShieldsArray(shields);
                    initGameStats(gameStats);
                } 
                
                else if (*selectedOption == GAMEOVER_OPTION_QUIT) { // Se presiona la tecla QUIT
                    *currentGameState = GAME_STATE_EXIT; // Se cambia el estado del juego a EXIT
                }
                break;
        }

        keys[ALLEGRO_KEY_ENTER] = false; // Desactivamos la tecla (ya se usó)
    }
}
    
/* Esta función se va a encargar de dibujar en pantalla las opciones del menú disponibles de acuerdo al TIPO de MENÚ en el que nos encontremos. Hay dos imágenes para cada opción: 
-Imagen 1: Palabra en VERDE (ON), que se va a activar cundo estemos seleccionando la opción.
-Imagen 2: Palabra en BLANCO (OFF), que se va a activar cuando NO estemos seleccionando la opción. */
void drawMenu(ALLEGRO_BITMAP *imagenes[IMG_TOTAL], int selectedOption, MenuType type) { 

    al_clear_to_color(al_map_rgb(0, 0, 0)); // Se pone el fondo en negro.
    
    al_draw_bitmap(imagenes[IMG_FONDO], 100, 0, 0); // Agregamos el marco para el menú, junto a los aliens y las naves de decoración.
    
    al_draw_bitmap(imagenes[IMG_LOGO], 300, 50, 0); // Agregamos el cartel de Space Invaders.

    if (type == MENU_TYPE_START) { // Si estamos en el MENÚ de INICIO
            al_draw_bitmap(selectedOption == START_OPTION_PLAY ? imagenes[IMG_PLAY_ON] : imagenes[IMG_PLAY_OFF], 311, 412, 0);
            al_draw_bitmap(selectedOption == START_OPTION_QUIT ? imagenes[IMG_QUIT_ON] : imagenes[IMG_QUIT_OFF], 367, 625, 0);
    }
    
    else if (type == MENU_TYPE_PAUSE) { // Si estamos en el MENÚ de PAUSA
        al_draw_bitmap(selectedOption == PAUSE_OPTION_RESUME ? imagenes[IMG_RESUME_ON] : imagenes[IMG_RESUME_OFF], 264, 325, 0);
        al_draw_bitmap(selectedOption == PAUSE_OPTION_RESTART ? imagenes[IMG_RESTART_ON] : imagenes[IMG_RESTART_OFF], 252, 475, 0);
        al_draw_bitmap(selectedOption == PAUSE_OPTION_QUIT ? imagenes[IMG_QUIT_ON] : imagenes[IMG_QUIT_OFF], 367, 625, 0);
    }
    
    else if (type == MENU_TYPE_GAMEOVER) { // Si estamos en el MENÚ de GAMEOVER
        al_draw_bitmap(selectedOption == GAMEOVER_OPTION_RESTART ? imagenes[IMG_RESTART_ON_GAMEOVER] : imagenes[IMG_RESTART_OFF_GAMEOVER], 194, 427, 0);
        al_draw_bitmap(selectedOption == GAMEOVER_OPTION_QUIT ? imagenes[IMG_QUIT_ON] : imagenes[IMG_QUIT_OFF], 367, 625, 0);
    }
    
}
