/******************************************************** INCLUDE ************************************************************/

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



/*******************************************************  PROTOTIPOS  *******************************************************************/


void handleScoreScreenState (ALLEGRO_FONT *fontScoreSmall, ALLEGRO_FONT *fontScoreMedium, ALLEGRO_FONT *fontScoreBig, stats_t *gameStats, GameState *gameState, double *timeScoreScreenStart, int *selectedOption, int *playerRankFlag);
                                        
void drawMenu(ALLEGRO_BITMAP *menuImage[IMG_TOTAL_MENU], int selectedOption, MenuType type);

void handleMenuInput (bool keys[], int *selectedOption, GameState *currentGameState, MenuType type, player_t * player, aliensBlock_t * aliensBlock, alien_t aliens[ALIEN_ROWS][ALIEN_COLS], shield_t shields[NUM_SHIELDS], 
                    stats_t * gameStats, int * playerRankFlag);
                    
int compararScores(const void *a, const void *b);

int top10Scores(int score);



/**********************************************************  MAIN  *****************************************************************/

int main() {

    //========================   Inicializamos la biblioteca Allegro  ====================================
    if (!al_init()) {
        fprintf(stderr, "Error al inicializar Allegro\n");
        return -1;
    }

    al_install_keyboard(); // Habilitamos el uso del teclado
    al_init_image_addon();// Habilitamos el uso de imágenes
    al_init_font_addon(); // Inicializamos fuentes
    al_init_ttf_addon(); // Inicializamos tipos de fuentes 

    // Creamos la ventana de tamaño DISPLAY_LENGTH x DISPLAY_HIGH, que va a ser el display
    ALLEGRO_DISPLAY *display = al_create_display(DISPLAY_LENGTH, DISPLAY_HIGH);
    
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

 
    bool keys[ALLEGRO_KEY_MAX] = {false}; // Array que guarda qué teclas están presionadas actualmente. Las incializamos todas como inactivas.
    bool redraw = true; // Indica si tenemos que redibujar la pantalla.

    
    // ============================================ VARIABLES DEL JUEGO =================================================
    
    // Información general del juego 
    GameState gameState = GAME_STATE_START; // Indica el estado actual del juego (tenemos 5 estados disponibles). Lo incializamos en INICIO.
    srand(time(NULL));
    stats_t gameStats;
    clock_t clock();
    clock_t clkO;
    double timeScoreScreenStart = 0;
    
    // Menú
    int selectedOption = START_OPTION_PLAY;// Opción seleccionada cuando estamos en los menús. La incializamos en START_OPTION_PLAY porque arrancamos en el menú de inicio y queremos que apunte directo a PLAY.
    
    // Jugabilidad
    player_t player;
    alien_t aliens[ALIEN_ROWS][ALIEN_COLS];
    aliensBlock_t aliensBlock;
    bullet_t playerBullet = {0};
    bullet_t alienBullet = {0};
    shield_t shields[NUM_SHIELDS];
    bool tryShoot = false;
    float interval;
    ovni_t ovni = { .visible = false, .alive = true };  // Estado inicial
    initOvni(&ovni, clock(), &clkO);
    double timeLastMovAlien = 0;
    double timeLastRow = 0;
    int lastRowToPrint = -1;
    bool alienFrameFila[ALIEN_ROWS] = {false};
    
    //Score y Ranking
    int playerRankFlag = 0; 
    
    
    // ==================== CARGAMOS LAS IMÁGENES ==========================
    
    // Imágenes del MENÚ
    ALLEGRO_BITMAP * menuImage[IMG_TOTAL_MENU]; // Arreglo donde se cargarán las imágenes del MENÚ
    
    int i;
    
    for (i = 0; i < IMG_TOTAL_MENU; i++) {
    
        // Cargamos el nombre y el acceso a cada imagen en un arreglo (path)
        char pathMenu[50];
        sprintf(pathMenu, "imagenes/menuImage%d.png", i);
        
        // Con sus paths, cargamos las IMAGENES del MENÚ en el arreglo 'menuImage'
        menuImage[i] = al_load_bitmap(pathMenu);
        
        // Si hubo un error al cargarlas
        if (!menuImage[i]) {
            fprintf(stderr, "Error cargando imagen de menú: %s\n", pathMenu);
            return -1;
        }
    }
    
    
    // Imágenes de ALIENS, OVNI, BALAS y PLAYER
    ALLEGRO_BITMAP *image[9];
    ALLEGRO_BITMAP *shield[7];
    ALLEGRO_BITMAP *bulletImg;
    
    char img[50];
    for (int i = 0; i < 9; i++) { // Utilizando su 
	    sprintf(img, "imagenes/img%d.png", i);
	    image[i] = al_load_bitmap(img);
	    if (!image[i]) {
	        fprintf(stderr, "Error: no se pudo cargar la imagen %s\n", img);
	        return -1;
	    }
	}
	
    for (int i = 1; i < 6; i++) {
	    sprintf(img, "imagenes/Shield%d.png", i);
       shield[i-1] = al_load_bitmap(img);
	    if (!shield[i-1]) {
	        fprintf(stderr, "Error: no se pudo cargar la imagen %s\n", img);
	        return -1;
	    }
    }
    
    bulletImg = al_load_bitmap("imagenes/bullet.png");
    if (!bulletImg) {
        fprintf(stderr, "Error: no se pudo cargar la imagen %s\n", img);
        return -1;
    }
    

    
    //==================================  FUENTES  ===========================================
    
    // Tenemos 3 fuentes para los scores:
    ALLEGRO_FONT *fontScoreSmall = al_load_ttf_font("PressStart2P-Regular.ttf", 20, 0);  // fuente CHICA
    ALLEGRO_FONT *fontScoreMedium = al_load_ttf_font("PressStart2P-Regular.ttf", 40, 0); // fuente MEDIANA
    ALLEGRO_FONT *fontScoreBig = al_load_ttf_font("PressStart2P-Regular.ttf", 60, 0);    // fuente GRANDE

    // Verificamos que se hayan cargado correctamente:
    if (!fontScoreSmall || !fontScoreMedium || !fontScoreBig) {
	    fprintf(stderr, "No se pudo cargar la fuente\n");
	    return -1;
    }
    
    
    //===============================  FUNCIONES DE INICIALIZACIÓN  ===========================
    
    // LLamamos a las funcionees que dejan todo inicializado para el inicio del juego
    initPlayer(&player);
    initAliensBlock(&aliensBlock);
    initAliensArray(aliens);
    initShieldsArray(shields);
    initGameStats(&gameStats);
    

    //============================  BÚCLE PRINCIPAL DEL JUEGO  ==============================

    al_start_timer(timer); // Iniciamos el temporizador (manda eventos cada 1/60 segundos)
    
    bool running = true; 
    
    while (running) {
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
                handleMenuInput(keys, &selectedOption, &gameState, MENU_TYPE_START, &player, &aliensBlock, aliens, shields, &gameStats, &playerRankFlag);
                drawMenu(menuImage, selectedOption, MENU_TYPE_START);
                break;
                
            case GAME_STATE_PLAYING:
                switch (ev.type) {

                    case ALLEGRO_EVENT_TIMER: {
                    
                        if (keys[ALLEGRO_KEY_LEFT])   // [<]: Para mover el jugador hacia la IZQUIERDA
                            playerMove(-1, &player);
                        
                        if (keys[ALLEGRO_KEY_RIGHT])  // [>]: Para mover el jugador hacia la DERECHA
                            playerMove(1, &player);
                        
                        if (keys[ALLEGRO_KEY_UP])     // [^]: Para activar DISPARO del jugador
                            tryShoot = true;
                        
                        if (keys[ALLEGRO_KEY_SPACE])  // [   ]: Para PAUSAR el juego
                            gameState= GAME_STATE_PAUSED;
                    
                        if (keys[ALLEGRO_KEY_ESCAPE]) // [esc] Para SALIR del juego
                            gameState = GAME_STATE_EXIT; 
                        
                        playerShoot(&playerBullet, &player, &tryShoot); // Dispara cuando corresponde

                        // Movimiento aliens de acuerdo a su posición y nivel actual
                       if (gameStats.level<17){
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
	                    
                        
                        double actualTime = al_get_time(); // Tomamos el tiempo actual para determinar si ya pasó suficiente tiempo para mover los aliens
                        
                        if (lastRowToPrint == -1 && actualTime - timeLastMovAlien >= interval) { // Si no se está imprimiendo una fila y pasó el intervalo, comenzamos a imprimir desde la última fila
                            lastRowToPrint = ALIEN_ROWS - 1;
                            timeLastRow = actualTime;
                        }
                        
                        if (lastRowToPrint >= 0 && actualTime - timeLastRow >= interval) { // Imprimimos fila por fila con cierto intervalo
                        
                            if (lastRowToPrint == 0) {
                                alienFrameFila[0] = !alienFrameFila[0];
                                blockMove(aliens, &aliensBlock);
                                lastRowToPrint = -1;
                                timeLastMovAlien = actualTime;
                            } 
                            
                            else { // Si no se está imprimiendo una fila y pasó el intervalo, comenzamos a imprimir desde la última fila
                            alienFrameFila[lastRowToPrint] = !alienFrameFila[lastRowToPrint];
                                lastRowToPrint--;
                                timeLastRow = actualTime;
                            }
                        }

                        //======================== DIBUJO  DE ELEMENTOS ===========================
                        
                        al_clear_to_color(al_map_rgb(0, 0, 0)); // Pantalla en negro.

                        for (int j = 0; j < NUM_SHIELDS; j++) { // Dibuja los escudos dependiendo su vida 
                        
			                if (shields[j].health > 0) {

				            int index;
				            
			                    if (shields[j].health > 12)
				                index = 0;
		                    
			                    else if (shields[j].health > 9)
				                index = 1;
				                    
			                    else if (shields[j].health > 6)
				                index = 2;
			                    
			                    else if (shields[j].health > 3)
				                index = 3;
			                    
			                    else if (shields[j].health > 0)
				                index = 4;
				                
			                    else
				                index=5;
		                
			                    if (index<5)
				                al_draw_bitmap(shield[index], shields[j].coord.coordX, shields[j].coord.coordY, 0);
			                }
			            }

                        al_draw_bitmap (image[7], player.coord.coordX - PLAYER_SIZE_X / 2, player.coord.coordY, 0); // Dibuja al JUGADOR
                        
                        char scoreText[40];
                        sprintf(scoreText, "Score: %d     Level: %d", gameStats.actualScore, gameStats.level);
                        al_draw_text(fontScoreSmall, al_map_rgb(0, 255, 0), SCORE_POSITION_X, SCORE_POSITION_Y, 0, scoreText); // Dibuja el SCORE
                            
                            
                        if (ovni.visible) 
                            al_draw_bitmap(image[6], ovni.coord.coordX, ovni.coord.coordY, 0); // Dibuja el OVNI
                            
                        for (int i = 0; i < player.health; i++) { // Dibuja las vidas restantes como naves
                            int drawX = LIVES_POSITION_X + i * (PLAYER_SIZE_X + LIVES_SPACING);
                            al_draw_bitmap(image[7], drawX, LIVES_POSITION_Y, 0);
                        }

                        if (player.health==0){ // Si se queda sin vida, se muestra el score y se comienza a contar el tiempo
                            gameState = GAME_STATE_SCORE_SCREEN;
                            timeScoreScreenStart = al_get_time(); 
                        }


                        if (playerBullet.active)
                            al_draw_bitmap(bulletImg, playerBullet.coord.coordX, playerBullet.coord.coordY, 0); // Dibuja la bala del jugador

                        if (alienBullet.active)
                            al_draw_bitmap(bulletImg, alienBullet.coord.coordX, alienBullet.coord.coordY, 0); // Dibuja la bala del alien
                           
                        for (int i = 0; i < ALIEN_ROWS; i++) {
                            for (int j = 0; j < ALIEN_COLS; j++) { // Dibuja los aliens y su desfase
                                if (!aliens[i][j].alive){
                               	    continue;
		                }
                                int offsetX = aliensBlock.coord.coordX;
                                if (lastRowToPrint >= 0 && i >= lastRowToPrint){
                                    offsetX += JUMP_SIZE_X * aliensBlock.direction;
                                }
                                
                                int drawX = offsetX + aliens[i][j].coord.coordX;
                                int drawY = aliensBlock.coord.coordY + aliens[i][j].coord.coordY;

                                if (i > 2)
                                    al_draw_bitmap(alienFrameFila[i] ? image[5] : image[4], drawX, drawY, 0);
                                    
                                else if (i > 0)
                                    al_draw_bitmap(alienFrameFila[i] ? image[3] : image[2], drawX, drawY, 0);
                                    
                                else
                                    al_draw_bitmap(alienFrameFila[i] ? image[1] : image[0], drawX, drawY, 0);
                            }
                       
                        }
                     
                        al_flip_display();

	               	collisionDetect (&playerBullet, &alienBullet, aliens, &ovni, shields, &aliensBlock, &player, &gameStats, lastRowToPrint, &clkO); // Detecta colisiones

                        updateAliensBlock (aliens, &aliensBlock); // Actualiza el estado del bloque de aliens (sus limites y dirección)

                        alien_t *shooter = selectAlienShooter(aliens, &aliensBlock, &player); // Selecciona un alien para disparar
                        
                        if (shooter != NULL) {
                            int shooterRow = -1;
                            for (int i = 0; i < ALIEN_ROWS; i++) { // Determina la fila del alien seleccionado
                                for (int j = 0; j < ALIEN_COLS; j++) {
                                    if (&aliens[i][j] == shooter)
                                        shooterRow = i;
                                }
                            }
                            
                            alienShoot(&alienBullet, shooter, gameStats.level, &aliensBlock, lastRowToPrint, shooterRow); // Realiza el disparo del alien
                        }
                        
	                updateOvni (&ovni, clock(), &clkO, rand() % 1001); // Actualiza estado del ovni

                        newLevelCheck (aliens, &aliensBlock, &player, shields, &gameStats);  // Verifica si se paso de nivel y reinicia entidades de ser necesario
                    }
                    break;
                } 
                break;

            case GAME_STATE_PAUSED:
                handleMenuInput(keys, &selectedOption, &gameState, MENU_TYPE_PAUSE, &player, &aliensBlock, aliens, shields, &gameStats, &playerRankFlag);
                drawMenu(menuImage, selectedOption, MENU_TYPE_PAUSE);
                break;

            case GAME_STATE_GAMEOVER:
                handleMenuInput(keys, &selectedOption, &gameState, MENU_TYPE_GAMEOVER, &player, &aliensBlock, aliens, shields, &gameStats, &playerRankFlag);
                drawMenu(menuImage, selectedOption, MENU_TYPE_GAMEOVER);
                break;
                
            case GAME_STATE_SCORE_SCREEN:
                handleScoreScreenState (fontScoreSmall, fontScoreMedium, fontScoreBig, &gameStats, &gameState, &timeScoreScreenStart, &selectedOption, &playerRankFlag);
                        break;
                        
            case GAME_STATE_EXIT:
                running = false; //No ingresará de nuevo al 
            }
			
	al_flip_display();
    }
}

    // Destruimos los recursos de Allegro
    for (int i = 0; i < IMG_TOTAL_MENU; i++) {
        if (menuImage[i])
            al_destroy_bitmap(menuImage[i]);
    }
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}



/**********************************************  FUNCIONES MENÚ  ************************************************************/

/* 1) HandleMenuInput (...)
    Esta función se va a encargar de seleccionar las opciones, de acuerdo a que teclas presione el usuario y en que tipo de menú se encuentre.*/
    
void handleMenuInput(bool keys[], int *selectedOption, GameState *currentGameState, MenuType type, player_t * player, aliensBlock_t * aliensBlock, alien_t aliens[ALIEN_ROWS][ALIEN_COLS], shield_t shields[NUM_SHIELDS], stats_t * gameStats, int * playerRankFlag) {
    
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
                    *playerRankFlag = 0;
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
                    *playerRankFlag = 0;
                } 
                
                else if (*selectedOption == GAMEOVER_OPTION_QUIT) { // Se presiona la tecla QUIT
                    *currentGameState = GAME_STATE_EXIT; // Se cambia el estado del juego a EXIT
                }
                break;
        }

        keys[ALLEGRO_KEY_ENTER] = false; // Desactivamos la tecla (ya se usó)
    }
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    
/* 2) drawMenu (...)
    Esta función se va a encargar de dibujar en pantalla las opciones del menú disponibles de acuerdo al TIPO de MENÚ en el que nos encontremos. Hay dos imágenes para cada opción: 
    -Imagen 1: Palabra en VERDE (ON), que se va a activar cundo estemos seleccionando la opción.
    -Imagen 2: Palabra en BLANCO (OFF), que se va a activar cuando NO estemos seleccionando la opción. */
    
void drawMenu(ALLEGRO_BITMAP *menuImage[IMG_TOTAL_MENU], int selectedOption, MenuType type) { 

    al_clear_to_color(al_map_rgb(0, 0, 0)); // Se pone el fondo en negro.

    if (type == MENU_TYPE_START) { // Si estamos en el MENÚ de INICIO
            al_draw_bitmap (menuImage [IMG_START_END_BACKGROUND], 000, 0, 0);
            al_draw_bitmap (selectedOption == START_OPTION_PLAY ? menuImage[IMG_PLAY_ON] : menuImage[IMG_PLAY_OFF], 311, 412, 0);
            al_draw_bitmap (selectedOption == START_OPTION_QUIT ? menuImage[IMG_QUIT_ON] : menuImage[IMG_QUIT_OFF], 367, 625, 0);
    }
    
    else if (type == MENU_TYPE_PAUSE) { // Si estamos en el MENÚ de PAUSA
        al_draw_bitmap (menuImage[IMG_PAUSE_BACKGROUND], 100, 0, 0);
        al_draw_bitmap (selectedOption == PAUSE_OPTION_RESUME ? menuImage[IMG_RESUME_ON] : menuImage[IMG_RESUME_OFF], 264, 325, 0);
        al_draw_bitmap (selectedOption == PAUSE_OPTION_RESTART ? menuImage[IMG_RESTART_ON] : menuImage[IMG_RESTART_OFF], 252, 475, 0);
        al_draw_bitmap (selectedOption == PAUSE_OPTION_QUIT ? menuImage[IMG_QUIT_ON] : menuImage[IMG_QUIT_OFF], 367, 625, 0);
    }
    
    else if (type == MENU_TYPE_GAMEOVER) { // Si estamos en el MENÚ de GAMEOVER
        al_draw_bitmap (menuImage[IMG_START_END_BACKGROUND], 000, 0, 0);
        al_draw_bitmap (selectedOption == GAMEOVER_OPTION_RESTART ? menuImage[IMG_RESTART_ON_GAMEOVER] : menuImage[IMG_RESTART_OFF_GAMEOVER], 194, 427, 0);
        al_draw_bitmap (selectedOption == GAMEOVER_OPTION_QUIT ? menuImage[IMG_QUIT_ON] : menuImage[IMG_QUIT_OFF], 367, 625, 0);
    }
    
    al_draw_bitmap(menuImage[IMG_LOGO], 300, 50, 0); // Agregamos el cartel de Space Invaders.
}




/***************************************************  FUNCIONES SCORE  *****************************************************************/

/* 1) handleScoreScreenState (...)
    Función que se llama cuando se pierde la partida, previo a que aparezca el menú de GAMEOVER. Se encarga de presentar en pantalla el score y, en caso de estar en el TOP 10, su posicionamiento. */

void handleScoreScreenState (ALLEGRO_FONT *fontScoreSmall, ALLEGRO_FONT *fontScoreMedium, ALLEGRO_FONT *fontScoreBig, stats_t *gameStats, GameState *gameState, double *timeScoreScreenStart, 
                            int *selectedOption, int *playerRankFlag){
		    
    al_clear_to_color (al_map_rgb(0, 0, 0)); //Seteamos el fondo en NEGRO
      
    char scoreText[50]; 
    sprintf (scoreText, "SCORE: %d", gameStats->actualScore); // Guardo el último SCORE del jugador en el top dentro de un string.

    static int playerRank; // Se va a almacenar el ranking del jugador.
      
    
    if (!(*playerRankFlag)){ // Flag para indicar que el score ya se escribío en el archivo y evitar así que se escriba muchas veces. (Se RESETEA con cada REINICIO de partida)
        playerRank = top10Scores (gameStats->actualScore);
        (*playerRankFlag)++;
    }

    
    if (1 <= playerRank && playerRank <= 10) { //SI SE POSICIONA DENTRO DEL RANKING
        
        /******* Texto de FELICITACIONES por haber alcanzado el TOP 10 *******/
        char congrats [] = "Congratulations! You ranked in the Top 10!";

        // ¿Donde y cómo lo ubico?
        int congratsTextWidth = al_get_text_width (fontScoreSmall, congrats); // Tomo el ANCHO del texto
        int congratsTextHeight = al_get_font_line_height (fontScoreSmall);    // Tomo el ALTO del texto
        al_draw_text (fontScoreSmall, al_map_rgb(255, 255, 255), (DISPLAY_LENGTH - congratsTextWidth) / 2, 2 * congratsTextHeight, 0, congrats); //Imprimo el string de felicitaciones en blanco, chico, centrado horizontalmente y en la parte de arriba de la pantalla
        
        
        /************* Texto de PLACEMENT **************/
        char placementText[20];
        sprintf (placementText, "You placed #%d", playerRank); // Guardo la posición del jugador en el top dentro de un string.
        
        // ¿Donde y cómo lo ubico?
        int placementTextWidth = al_get_text_width(fontScoreBig, placementText); // Tomo el ANCHO del texto
        int placementTextHeight = al_get_font_line_height(fontScoreBig);         // Tomo el ALTO del texto
        al_draw_text(fontScoreBig, al_map_rgb(0, 255, 0), (DISPLAY_LENGTH - placementTextWidth) / 2, (DISPLAY_HIGH - placementTextHeight) / 2, 0, placementText); // Imprimo "#<playerRank>" en verde, grande, centrado horizontalmente y verticalmente en la pantalla
        
        
        /*************** Texto de SCORE ******************/
        // ¿Donde y cómo lo ubico?
        int scoreTextWidth = al_get_text_width(fontScoreMedium, scoreText); // Tomo el ANCHO del texto
        int scoreTextHeight = al_get_font_line_height(fontScoreMedium);     // Tomo el ALTO del texto
        al_draw_text(fontScoreMedium, al_map_rgb(255, 255, 255), (DISPLAY_LENGTH - scoreTextWidth) / 2, (DISPLAY_HIGH - 2 * scoreTextHeight), 0, scoreText); // Imprimo el score mediano, en blanco, centrado horizontalmente y en la parte de abajo de la pantalla
    }

    else { // SI NO SE POSICIONA DENTRO DEL RANKING

        /*************** Solo imprimo texto de SCORE ******************/
        // ¿Donde y cómo lo ubico?
        int scoreTextWidth = al_get_text_width(fontScoreBig, scoreText); // Tomo el ANCHO del texto
        int scoreTextHeight = al_get_font_line_height(fontScoreBig);     // Tomo el ALTO del texto
        al_draw_text(fontScoreBig, al_map_rgb(255, 255, 255),(DISPLAY_LENGTH - scoreTextWidth) / 2,(DISPLAY_HIGH - scoreTextHeight) / 2, 0, scoreText); // Imprimo el score en verde, grande, centrado horizontalmente y verticalmente en la pantalla
    }


    // Mostrar por 4 SEGUNDOS
    if (al_get_time() - *timeScoreScreenStart >= 4.0) {
        *gameState = GAME_STATE_GAMEOVER;           // Se va al menú de GAMEOVER
        *selectedOption = GAMEOVER_OPTION_RESTART;  // Se coloca en el menú sobre la opción RESTART
    }
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/* 2) top10Scores (...)
    Función que es llamada por 'handleScoreScreenState (...)' cuando se pierde. 
    - Devuelve en que ranking del top 10 quedo, o un -1 si NO esta en el top 10.
    - Recibe un puntero al archivo donde estan almacenados los scores */
    
int top10Scores(int score){
	
	//agrego mi score al final del archivo
	FILE *scoresFile_a = fopen("scoresFile", "a+");
	if(scoresFile_a){								//Si se abrio correctamente
		fprintf(scoresFile_a, "%d\n", score);		
	}
	fflush(scoresFile_a);		
	fclose(scoresFile_a);
	
	//reservo memoria en el heap para hacer un arreglo de scores y guardo todos los scores en el arreglo
	int * scoresArray = (int*)calloc((MAX_TOP_SCORES+1),sizeof(int));
	FILE *scoresFile_r = fopen("scoresFile", "r");
	fseek(scoresFile_r, 0, SEEK_SET);			//me vuelvo a ubicar en el incio del archivo 
	if(scoresArray){						//Si se reservo correctamente
		int i; 
		for(i=0; i<MAX_TOP_SCORES+1; i++){
			fscanf(scoresFile_r, "%d", &scoresArray[i]);	//alamceno los scores en el arreglo
		}
	} 
	fflush(scoresFile_r);		
	fclose(scoresFile_r);
	
	//ordeno el arreglo de mayor a menor con qsort
	qsort((void*)scoresArray, MAX_TOP_SCORES+1, sizeof(int), compararScores);
	
	//guardo el arreglo nuevamente en el archivo (solo los mejores 10) 
	FILE *scoresFile_w = fopen("scoresFile", "w");
	if(scoresFile_w){								//Si se abrio correctamente
		int i; 
		for(i=0; i<MAX_TOP_SCORES; i++){
			fprintf(scoresFile_w, "%d\n", scoresArray[i]);
		}		
	}
	fflush(scoresFile_w);		
	fclose(scoresFile_w);
	
	//reviso si mi score esta dentro del top 10, si es asi, devuelvo en que top
	int ranking; 
	for(ranking=1; ranking<=MAX_TOP_SCORES; ranking++){
		if(score == scoresArray[ranking-1]){
			free(scoresArray);
			return ranking; 
		}
	}
	free(scoresArray);
	return -1; 
}

/*---------------------------------------------------------------------------------------------------------------------------------------------------------*/

/* 3) compararScores (...)
    Función de comparación para usar qsort en 'int top10Scores(int score);' */

int compararScores(const void *a, const void *b) {
	return (*(int *)b - *(int *)a);  // Orden descendente
}

