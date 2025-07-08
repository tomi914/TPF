#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

int main(){
    if (!al_init()){
    	fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }
    al_install_keyboard();
    ALLEGRO_DISPLAY *display = al_create_display(DISPLAY_LENGHT, DISPLAY_HIGH); //crea display
    if (!display) {
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue(); //crea lista de eventos
    al_register_event_source(queue, al_get_keyboard_event_source()); //registra teclado y display como fuentes de eventos
    al_register_event_source(queue, al_get_display_event_source(display));

    bool running = true; //indica si el programa debe seguir corriendo
    bool keys[ALLEGRO_KEY_MAX] = {false}; //setea todas las teclas en false asi si aprieto cambia

    while (running) {
        ALLEGRO_EVENT ev;
        while(al_get_next_event(queue, &ev)){
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) //setea que si se aprieta una tecla, es true
                keys[ev.keyboard.keycode] = true;
            else if (ev.type == ALLEGRO_EVENT_KEY_UP) //setea que si se suelta una tecla es false
                keys[ev.keyboard.keycode] = false;
            else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) //setea que si se cierra la ventana, para el programa
                running = false;
        }
        //PARTE TOMI
        if(keys[ALLEGRO_KEY_ESCAPE]){	//MENU EN CASO DE QUERER PAUSAR
        	while(keys[ALLEGRO_KEY_ESCAPE]){ //PAUSO EL JUEGO HASTA PROXIMO ESC
        		draw_menu();
        	}
        }
        //FIN PARTE TOMI
        
        al_flip_display();
        al_rest(0.01);  // pequeña pausa

    al_destroy_display(display);
    al_destroy_event_queue(queue);
    return 0;
}

void draw_menu() {
    ALLEGRO_FONT *font = al_load_font("arial.ttf", 24, 0);
    al_draw_filled_rectangle(100, 100, 540, 380, al_map_rgba(0, 0, 0, 200)); // semi-transparente
    al_draw_text(font, al_map_rgb(255, 255, 255), 320, 200, ALLEGRO_ALIGN_CENTER, "PAUSADO");
    al_draw_text(font, al_map_rgb(255, 255, 255), 320, 240, ALLEGRO_ALIGN_CENTER, "Presiona ESC para continuar");
    al_destroy_font(font);
}
