#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>


int main() {
    if (!al_init()) {
        return -1;
    }
    al_install_keyboard();
    al_init_image_addon();
    const int screenW = 850;
    const int screenH = 850;
    ALLEGRO_DISPLAY *display = al_create_display(screenW, screenH); //crea display
    if (!display) {
       
        return -1;
    }


	const int playerW = 64; //tamano jugador
	const int playerH = 64;
    float x = screenW / 2.0; //donde arranca la nave (medio) en x
    float y = screenH - 100; //donde arranca la nave en y
    float speed = 5.0; //velocidad de la nave
    const int shotW = 10; //tamano de disparo
	const int shotH = 20;
	float shotY=y;
	float speedShot = 10.0; //velocidad de la nave
	bool shooting = false;
	float shotX;

	ALLEGRO_BITMAP *sprite_original = al_load_bitmap("imagen.png"); //subo imagen nave
	ALLEGRO_BITMAP *sprite_escalado = al_create_bitmap(playerW, playerH); //la reescala y printea
	al_set_target_bitmap(sprite_escalado);
	al_draw_scaled_bitmap(sprite_original,
		                  0, 0, al_get_bitmap_width(sprite_original), al_get_bitmap_height(sprite_original),
		                  0, 0, playerW, playerH,
		                  0);
	al_set_target_backbuffer(al_get_current_display());
	
	 ALLEGRO_BITMAP *sprite_original2 = al_load_bitmap("imagen2.png"); //sube la img de disparo

			ALLEGRO_BITMAP *sprite_shot_escalado = al_create_bitmap(shotW, shotH); //la reescala y printea
			al_set_target_bitmap(sprite_shot_escalado);
			al_draw_scaled_bitmap(sprite_original2,
				              0, 0, al_get_bitmap_width(sprite_original2), al_get_bitmap_height(sprite_original2),
				              0, 0, shotW, shotH,
				              0);
			al_set_target_backbuffer(al_get_current_display());
	//si tomy hace los png esto no es necesario


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


		if (keys[ALLEGRO_KEY_LEFT]){
			playermove(-1, player);
		}//establezco limite de movimiento de nave y velocidad
		if (keys[ALLEGRO_KEY_RIGHT]){
			playermove(1, player);
		}
        
        if(keys[ALLEGRO_KEY_UP]){
        	playerShot(shooting, bullet, player);
        }
        
        
        
    
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(sprite_escalado, x-playerW/2, y, 0); //dibuja la nave
        if (shooting)
			al_draw_bitmap(sprite_shot_escalado, shotX, shotY, 0); //dibuja el disparo
        al_flip_display();
        al_rest(0.01);  // pequeña pausa
    }

    al_destroy_bitmap(sprite_escalado);
    al_destroy_bitmap(sprite_shot_escalado);
    al_destroy_bitmap(sprite_original);
    al_destroy_bitmap(sprite_shot_escalado);
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
