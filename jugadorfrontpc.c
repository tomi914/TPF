#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include<stdbool.h>
#include<stdint.h>
#include<stdlib.h>
#include <allegro5/allegro_primitives.h>



int main() {
    if (!al_init()) {
    	fprintf(stderr, "failed to initialize allegro!\n");//verifico que se haya iniciado bien allegro
        return -1;
    }
    al_install_keyboard();
    al_init_image_addon();
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }
    ALLEGRO_DISPLAY *display = al_create_display(DISPLAY_LENGTH, DISPLAY_HIGH); //crea display
    if (!display) {
        fprintf(stderr, "failed to create display!\n");//verifico que se haya creado bien el display
        return -1;
    }

	player_t player;//creo todas las variables necesarias
	ALLEGRO_BITMAP *image[9];
	alien_t aliens[ALIEN_ROWS][ALIEN_COLS];
	aliensBlock_t aliensBlock;
	bullet_t playerBullet;
	bullet_t alienBullet;
	shield_t shields[NUM_SHIELDS];
	stats_t gameStats;
	char img[20];
	int i, j;
	
	bool alienFrameFila[ALIEN_ROWS] = {false};  // Un frame por cada fila

	bool tryShoot=false;
	int frameCount = 0;

	
	for (i = 0; i < 9; i++) {//subo todas las imagenes
		sprintf(img, "img%d.png", i);
		image[i] = al_load_bitmap(img);
		if (!image[i]) {
		    fprintf(stderr, "Error: no se pudo cargar la imagen %s\n", img);//verifico que se hayan subido bien las imgs
		    return -1;
    	}
	}
	initPlayer(&player);//inicializo al jugador
	initAliensBlock(&aliensBlock);//inicializo el bloque de aliens
	initAliensArray(aliens);//inicializo todos los aliens		    
	initShieldsArray(shields);//inicializo los 4 escudos
	initGameStats(&gameStats);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue(); //crea lista de eventos
    al_register_event_source(queue, al_get_keyboard_event_source()); //registra teclado y display como fuentes de eventos
    al_register_event_source(queue, al_get_display_event_source(display));
    bool running = true; //indica si el programa debe seguir corriendo
    bool keys[ALLEGRO_KEY_MAX] = {false}; //setea todas las teclas en false asi si aprieto cambia
    // Variables necesarias (declarar fuera del loop principal si no lo hiciste ya)
	double timeLastMovAlien = 0;//tiempos para el movimiento de los aliens
	double timeLastRow = 0;
	int lastRowToPrint = -1;                 // -1 si no estamos imprimiendo fila por fila

    
    while (running) {//programa
        ALLEGRO_EVENT ev;
        while (al_get_next_event(queue, &ev)) {
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN){ //setea que si se aprieta una tecla, es true
                keys[ev.keyboard.keycode] = true;
            }
            else if (ev.type == ALLEGRO_EVENT_KEY_UP){ //setea que si se suelta una tecla es false
                keys[ev.keyboard.keycode] = false;
            }
            else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){ //setea que si se cierra la ventana, para el programa
                running = false;
            }
        }

		if (keys[ALLEGRO_KEY_LEFT]){//llamo a funcion playerMove para ir a la izq
			playerMove(-1, &player);
		}
		if (keys[ALLEGRO_KEY_RIGHT]){//llamo a funcion playerMove para ir a la der
			playerMove(1, &player);
		}
      
		if (keys[ALLEGRO_KEY_UP]) {
			tryShoot = true;  // se permite volver a disparar
		}
			playerShoot(&playerBullet, &player, &tryShoot);//llamo a funcion de disparo
		
              
		            
	al_clear_to_color(al_map_rgb(0, 0, 0));
	for (j = 0; j < NUM_SHIELDS; j++) {
		al_draw_bitmap(image[0], shields[j].coord.coordX, shields[j].coord.coordY, 0);//imprime escudos
	}
	al_draw_bitmap(image[7], player.coord.coordX - PLAYER_SIZE_X / 2, player.coord.coordY, 0);//imprime la nave y sus movimientos 
	if (playerBullet.active) {
		al_draw_bitmap(image[8], playerBullet.coord.coordX, playerBullet.coord.coordY, 0);//imprime el disparo
	}
	double actualTime = al_get_time(); //hace un control del tiempo





	// 🚀 INICIAR MOVIMIENTO DE FILAS SI TOCA MOVER EL BLOQUE
	if (lastRowToPrint == -1 && actualTime - timeLastMovAlien >= DELAY_START) {
		lastRowToPrint = ALIEN_ROWS - 1; // empezamos desde la fila de abajo
		timeLastRow = actualTime;
		timeLastMovAlien = actualTime;
	}

	// ⏳ AVANZAMOS UNA FILA HACIA ARRIBA CADA DELAY_FILA
	if (lastRowToPrint >= 0 && actualTime - timeLastRow >= DELAY_ROW) {
	   if (lastRowToPrint == 0) {
		// Última fila que salta: no alteramos su frame visual
		alienFrameFila[0] = !alienFrameFila[0];
		blockMove(aliens, &aliensBlock);
		lastRowToPrint = -1;
	} else if (lastRowToPrint > 0 && lastRowToPrint < ALIEN_ROWS) {
		// Fila intermedia: alternamos frame normalmente
		alienFrameFila[lastRowToPrint] = !alienFrameFila[lastRowToPrint];
		lastRowToPrint--;
		timeLastRow = actualTime;
	}

	}

	// 🧟‍♂️ DIBUJAR ALIENS — todas las filas visibles SIEMPRE
	for (i = 0; i < ALIEN_ROWS; i++) {
		for (j = 0; j < ALIEN_COLS; j++) {
		    if (!aliens[i][j].alive) continue;

		    int offsetX = aliensBlock.coord.coordX;

		    // Si estamos en animación, y esta fila aún no se imprimió en este frame, simulamos que no se movió
		    if (lastRowToPrint >= 0 && i > lastRowToPrint) {
		        offsetX += JUMP_SIZE_X * aliensBlock.direction;
		    }

		    int drawX = offsetX + aliens[i][j].coord.coordX;
		    int drawY = aliensBlock.coord.coordY + aliens[i][j].coord.coordY;

			if (i > 2) {
				al_draw_bitmap(alienFrameFila[i] ? image[5] : image[4], drawX, drawY, 0); // Alien tipo C
			} else if (i > 0) {
				al_draw_bitmap(alienFrameFila[i] ? image[3] : image[2], drawX, drawY, 0); // Alien tipo B
			} else {
				al_draw_bitmap(alienFrameFila[i] ? image[1] : image[0], drawX, drawY, 0); // Alien tipo A
			}
		
		}
	}

	// 🔁 MOSTRAR TODO
	al_flip_display();



		//collisionBA(playerBullet, aliens[ALIEN_ROWS][ALIEN_COLS], aliensBlock, gameStats, uint8_t printedRow);
			
		
		al_rest(0.01);
	
    }
   	for (i=0; i<9; i++){
		al_destroy_bitmap(image[i]);
	}
    al_destroy_display(display);
    al_destroy_event_queue(queue);
    return 0;
}





       /*
        if (keys[ALLEGRO_KEY_UP]){
         
		     ALLEGRO_BITMAP *sprite_original2 = al_load_bitmap("imagen2.png"); //sube la img

			ALLEGRO_BITMAP *sprite_escalado2 = al_create_bitmap(shotW, shotH); //la reescala y printea
			al_set_target_bitmap(sprite_escalado2);
			al_draw_scaled_bitmap(sprite_original2,
				              0, 0, al_get_bitmap_width(sprite_original2), al_get_bitmap_height(sprite_original2),
				              0, 0, shotW, shotH,
				              0);
			al_set_target_backbuffer(al_get_current_display());
	//si tomy hace los png esto no es necesario

			const float speed = 5.0; //velocidad del disparo

			al_clear_to_color(al_map_rgb(0, 0, 0));
			while (shotY>0){
				al_draw_bitmap(sprite_escalado2, x-shotW/2-1, shotY-playerH/4, 0);
				al_flip_display();
				shotY-=speedShot;
				al_rest(0.01);  // pequeña pausa
				}
		    }
		    
     */

/*	const int playerW = 64; //tamano jugador
	const int playerH = 64;
    float x = screenW / 2.0; //donde arranca la nave (medio) en x
	float y = screenH - 100; //donde arranca la nave en y
    float speed = 5.0; //velocidad de la nave
    const int shotW = 10; //tamano de disparo
	const int shotH = 20;
	float shotY=y;
	float speedShot = 10.0; //velocidad de la nave
	bool shooting = false;
	float shotX;*/





/*ALLEGRO_BITMAP *imagen = al_load_bitmap("imagen2.png");
    if (!imagen) {
        return -1;
    }

    // Dibujar la imagen en la posición (x=0, y=0)
    al_clear_to_color(al_map_rgb(0, 0, 0));  // Limpiar pantalla a negro
    al_draw_bitmap(imagen, 0, 0, 0);         // Dibujar imagen en esquina superior izquierda
    al_flip_display();                      // Mostrar en pantalla
        
        
        
        }
        
      

        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(sprite_escalado, x, y, 0);
        al_flip_display();
        al_rest(0.01);  // pequeña pausa*/ //para subir imagenes
