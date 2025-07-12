
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <allegro5/allegro_primitives.h>

#include "constantes_pc.h"
#include "entidades.h"
#include "backend.h"

int main(void){
    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    al_install_keyboard();
    al_init_image_addon();
    al_init_font_addon();
	al_init_ttf_addon();
	
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }

   
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_DISPLAY *display = al_create_display(DISPLAY_LENGTH, DISPLAY_HIGH);
    if (!display) {
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    // Variables de juego
    player_t player;
    ALLEGRO_BITMAP *image[9];
    alien_t aliens[ALIEN_ROWS][ALIEN_COLS];
    alienBlock_t aliensBlock;
    bullet_t playerBullet = {0};
    bullet_t alienBullet = {0};
    shield_t shields[NUM_SHIELDS];
    stats_t gameStats;
    char img[20];
    bool keys[ALLEGRO_KEY_MAX] = {false};
    bool tryShoot = false;
    //int frameCount = 0;
    ovni_t ovni = { .visible = false, .alive = true };  // Estado inicial
	double lastOvniDespawnTime;
	initOvni(&ovni, al_get_time(), &lastOvniDespawnTime);


    double timeLastMovAlien = 0;
    double timeLastRow = 0;
    int lastRowToPrint = -1;
    bool alienFrameFila[ALIEN_ROWS] = {false};

    for (int i = 0; i < 9; i++) {
        sprintf(img, "img%d.png", i);
        image[i] = al_load_bitmap(img);
        if (!image[i]) {
            fprintf(stderr, "Error: no se pudo cargar la imagen %s\n", img);
            return -1;
        }
    }
    ALLEGRO_FONT *font = al_load_ttf_font("PressStart2P-Regular.ttf", 14, 0);
	if (!font) {
		fprintf(stderr, "No se pudo cargar la fuente\n");
		return -1;
	}

    /*ALLEGRO_BITMAP *shieldImages[5];  // 5 niveles de daño

	for (int i = 0; i < 5; i++) {
		char path[20];
		sprintf(path, "shield%d.png", i);
		shieldImages[i] = al_load_bitmap(path);
		if (!shieldImages[i]) {
		    fprintf(stderr, "No se pudo cargar imagen %s\n", path);
		    return -1;
		}
	}*/


    initPlayer(&player);
    initAliensBlock(&aliensBlock);
    initAliensArray(aliens);
    initShieldsArray(shields);
    initGameStats(&gameStats);

    bool running = true;

    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        switch (ev.type) {//setea que pasa con las teclas
            case ALLEGRO_EVENT_KEY_DOWN:
                keys[ev.keyboard.keycode] = true;
                if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {
                    tryShoot = true;
                }
                break;

            case ALLEGRO_EVENT_KEY_UP:
                keys[ev.keyboard.keycode] = false;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE://termina el programa si cierro la ventana
                running = false;
                break;

            case ALLEGRO_EVENT_TIMER: {
		            // ======= ACTUALIZACIONES =======
		        if (keys[ALLEGRO_KEY_LEFT]) {
		            playerMove(-1, &player);
		        }
		        if (keys[ALLEGRO_KEY_RIGHT]) {
		            playerMove(1, &player);
		        }
		        playerShoot(&playerBullet, &player, &tryShoot);

		        // Movimiento aliens fila por fila
		        double actualTime = al_get_time();
		        if (lastRowToPrint == -1 && actualTime - timeLastMovAlien >= DELAY_START) {
		            lastRowToPrint = ALIEN_ROWS - 1;
		            timeLastRow = actualTime;
		            timeLastMovAlien = actualTime;
		        }

		        if (lastRowToPrint >= 0 && actualTime - timeLastRow >= DELAY_ROW) {
		            if (lastRowToPrint == 0) {
		                alienFrameFila[0] = !alienFrameFila[0];
		                blockMove(aliens, &aliensBlock);
		                lastRowToPrint = -1;
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
							al_draw_bitmap(image[index], shields[j].coord.coordX, shields[j].coord.coordY, 0);
						}
					}
				}

		        al_draw_bitmap(image[7], player.coord.coordX - PLAYER_SIZE_X / 2, player.coord.coordY, 0); //dibuja al jugador
				char scoreText[30];
				sprintf(scoreText, "Score: %d", gameStats.actualScore);
				al_draw_text(font, al_map_rgb(0, 255, 0), SCORE_POSITION_X, SCORE_POSITION_Y, 0, scoreText);//dibuja el score
					
					
				if (ovni.visible) {
				    al_draw_bitmap(image[6], ovni.coord.coordX, ovni.coord.coordY, 0);
				}
	
				for (int i = 0; i < player.health; i++) {
					int drawX = LIVES_POSITION_X + i * (PLAYER_SIZE_X + LIVES_SPACING);
					al_draw_bitmap(image[7], drawX, LIVES_POSITION_Y, 0);
				}
				if(player.health==0){
					
					
				}//poner condicion que se termine el juego, sino se castean las vidas


		        if (playerBullet.active){
		            al_draw_bitmap(image[8], playerBullet.coord.coordX, playerBullet.coord.coordY, 0);//dibuja la bala del jugador
				}
		        if (alienBullet.active){
		           al_draw_bitmap(image[8], alienBullet.coord.coordX, alienBullet.coord.coordY, 0);//dibuja la bala del alien
				}
		        for (int i = 0; i < ALIEN_ROWS; i++) {
		            for (int j = 0; j < ALIEN_COLS; j++) {//dibuja los aliens y su desfase
		                if (!aliens[i][j].alive){
		               		continue;
						}
		                int offsetX = aliensBlock.coord.coordX;
		                if (lastRowToPrint >= 0 && i > lastRowToPrint){
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

		            // ======= COLISIONES Y BALAS =======
		        collisionBA(&playerBullet, aliens, &aliensBlock, &gameStats, lastRowToPrint);
		        collisionBB(&playerBullet, &alienBullet);
		        collisionBP(&alienBullet, &player);
		        collisionAS (aliens, shields, &aliensBlock);
		        collisionAP (&player, aliens, &aliensBlock);
		        collisionBS(&playerBullet, &alienBullet, shields);
		        shieldsUpdate(shields);
		            
		    
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
		            alienShoot(&alienBullet, shooter, 7, &aliensBlock, lastRowToPrint, shooterRow);
		        }
		        
		        updateOvni(&ovni, al_get_time(), &lastOvniDespawnTime);


		        break;
        	}
        }
    }

    for (int i = 0; i < 9; i++) {
        al_destroy_bitmap(image[i]);
    }

    al_destroy_display(display);
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
    al_destroy_font(font);


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
*/
