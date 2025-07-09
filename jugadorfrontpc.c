#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

int main() {
    if (!al_init()) {
    	fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }
    al_install_keyboard();
    al_init_image_addon();
    ALLEGRO_DISPLAY *display = al_create_display(DISPLAY_LENGTH, DISPLAY_HIGH); //crea display
    if (!display) {
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }

	player_t player;
	ALLEGRO_BITMAP *image[9];
	alien_t alien[55];
	bullet_t playerBullet;
	bullet_t alienBullet;
	char img[20];
	int i;
	int jump;
	int fil, col;
	bool tryShoot;
	

	//player.coord.coordY=DISPLAY_HIGH-DISPLAY_HIGH/8-PLAYER_SIZE_Y/2;
	//player.coord.coordX=DISPLAY_LENGTH/2-PLAYER_SIZE_X/2;
	

	
	initPlayer(&player);
	for (i=0; i<9; i++){
		sprintf(img, "img%d.png", i);
		image[i]= al_load_bitmap(img);
	}
	for (i = 0; i < 9; i++) {
		sprintf(img, "img%d.png", i);
		image[i] = al_load_bitmap(img);
		if (!image[i]) {
		    fprintf(stderr, "Error: no se pudo cargar la imagen %s\n", img);
		    return -1;
    }
}

	

       /* 	for (fil=0; fil<ALIEN_ROWS; fil++){//completar las coord de los aliens ??
        		if (fil<1){
        			for (col=0; col<ALIEN_COLS; col++){
		    		al_draw_bitmap(image[0], player->coord.coordX, player->coord.coordY, 0);
		    		}
		   		} else if (fil<3){
			   		for (col=0; col<ALIEN_COLS; col++){
						al_draw_bitmap(image[2], player->coord.coordX, player->coord.coordY, 0);
						}
		   		} else {
			   		for (col=0; col<ALIEN_COLS; col++){
						al_draw_bitmap(image[4], player->coord.coordX, player->coord.coordY, 0);
						}
		   		}
		   	}
		    jump=0;*/	    
		    
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue(); //crea lista de eventos
    al_register_event_source(queue, al_get_keyboard_event_source()); //registra teclado y display como fuentes de eventos
    al_register_event_source(queue, al_get_display_event_source(display));

    bool running = true; //indica si el programa debe seguir corriendo
    bool keys[ALLEGRO_KEY_MAX] = {false}; //setea todas las teclas en false asi si aprieto cambia
   

    while (running) {
        ALLEGRO_EVENT ev;
        while (al_get_next_event(queue, &ev)) {
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) //setea que si se aprieta una tecla, es true
                keys[ev.keyboard.keycode] = true;
            else if (ev.type == ALLEGRO_EVENT_KEY_UP) //setea que si se suelta una tecla es false
                keys[ev.keyboard.keycode] = false;
            else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //setea que si se cierra la ventana, para el programa
                running = false;
        }


		if (keys[ALLEGRO_KEY_LEFT]){//llamo a funcion playerMove para ir a la izq
			playerMove(-1, &player);
		}
		if (keys[ALLEGRO_KEY_RIGHT]){//llamo a funcion playerMove para ir a la der
			playerMove(1, &player);
		}
      

		

		
		// Detecta flanco de bajada (tecla recién soltada)
		if (keys[ALLEGRO_KEY_UP]) {
			tryShoot = true;  // se permite volver a disparar
		}

			playerShoot(&playerBullet, &player, &tryShoot);
		



        //playerShoot(&playerBullet, &player, &tryShoot);//meter en el backend el try y cambiar lo de bullet alive
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(image[7], player.coord.coordX-PLAYER_SIZE_X/2, player.coord.coordY, 0); //dibuja la nave en su posicion inicial y desp la actualiza con los mov
        
        
        if (playerBullet.active){// cambiar en el back lo de player con shooting
			al_draw_bitmap(image[8], playerBullet.coord.coordX, playerBullet.coord.coordY, 0);
			} //dibuja el disparo
        al_flip_display();
        al_rest(0.01);  // pequeña pausa
        
        
        //PONER TIEMPO
        
        /*
        if (jump){//imprimo la segunda variante de los aliens
        
       
        		for (fil=0; fil<ALIEN_ROWS; fil++){//completar con nro de imagen
        		if (fil<1){
        			for (col=0; col<ALIEN_COLS; col++){
		    		al_draw_bitmap(image[0], player->coord.coordX, player->coord.coordY, 0);
		    		}
		   		} else if (fil<3){
			   		for (col=0; col<ALIEN_COLS; col++){
						al_draw_bitmap(image[2], player->coord.coordX, player->coord.coordY, 0);
						}
		   		} else {
			   		for (col=0; col<ALIEN_COLS; col++){
						al_draw_bitmap(image[4], player->coord.coordX, player->coord.coordY, 0);
						}
		   		}
		   	}
		    jump=0;
		    al_flip_display();

        } else {
        
        	for (fil=0; fil<ALIEN_ROWS; fil++){//completar con nro de imagen
        		if (fil<1){
        			for (col=0; col<ALIEN_COLS; col++){
		    		al_draw_bitmap(image[1], player->coord.coordX, player->coord.coordY, 0);
		    		}
		   		} else if (fil<3){
			   		for (col=0; col<ALIEN_COLS; col++){
						al_draw_bitmap(image[3], player->coord.coordX, player->coord.coordY, 0);
						}
		   		} else {
			   		for (col=0; col<ALIEN_COLS; col++){
						al_draw_bitmap(image[5], player->coord.coordX, player->coord.coordY, 0);
						}
		   		}
		   	}
		    jump=1;
		    al_flip_display();
        }
    
    */
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
