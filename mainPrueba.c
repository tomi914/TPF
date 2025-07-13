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
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60);

    // Agregamos las fuentes de eventos a la cola
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    // Array que guarda qué teclas están presionadas actualmente. Las incializamos todas como inactivas.
    bool keys[ALLEGRO_KEY_MAX] = {false};

    // Controla si el juego sigue corriendo
    bool running = true;

    // Indica si tenemos que redibujar la pantalla.
    bool redraw = true;

    // Indica el estado actual del juego (tenemos 5 estados disponibles). Lo incializamos en INICIO.
    GameState gameState = GAME_STATE_START;

    // Opción seleccionada cuando estamos en los menús. La incializamos en START_OPTION_PLAY porque arrancamos en el menú de inicio y queremos que apunte directo a PLAY.
    int selectedOption = START_OPTION_PLAY;

    // Creamos un arreglo donde vamos a guardar las imágenes, para mayor practicidad.
    ALLEGRO_BITMAP * imagenes[IMG_TOTAL]; 

    const char *rutas[IMG_TOTAL] = { // (CARPETA es una macro definida que indica 'carpeta/')
        CARPETA "fondo.png",
        CARPETA "spaceInvaders.png",
        CARPETA "playG.png",
        CARPETA "playW.png",
        CARPETA "resumeG.png",
        CARPETA "resumeW.png",
        CARPETA "restartG.png",
        CARPETA "restartW.png",
        CARPETA "restartBigG.png",
        CARPETA "restartBigW.png",
        CARPETA "quitG.png",
        CARPETA "quitW.png"
    };

    // Cargamos cada imagen del array rutas al array imagenes.
    for (int i = 0; i < IMG_TOTAL; i++) {
        imagenes[i] = al_load_bitmap(rutas[i]);
        if (!imagenes[i]) {
            fprintf(stderr, "Error cargando imagen: %s\n", rutas[i]);
            return -1;
        }
    }

    // Iniciamos el temporizador (manda eventos cada 1/60 segundos)
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
              
            // Si se presiona ESC en cualquier momento, se sale del juego
            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                gameState = GAME_STATE_EXIT;
            }
        }
  

        // Si hay que dibujar algo nuevo, y no hay eventos pendientes
        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;  // Como ya vamos a redibujar, lo ponemos en falso para los proximos

            // Maneja lo que se debe mostrar en pantalla según el estado del juego
            handleGameState(keys, &selectedOption, &gameState, imagenes);

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


void handleGameState(bool keys[], int *selectedOption, GameState *gameState, ALLEGRO_BITMAP *imagenes[]) {
    switch (*gameState) {
        case GAME_STATE_START:
            handleMenuInput(keys, selectedOption, gameState, MENU_TYPE_START);
            drawMenu(imagenes, *selectedOption, MENU_TYPE_START);
            break;
            
        case GAME_STATE_PLAYING:
            // INTEGRAR LO DE LAS FUNCIONES ACTUALES
            break;

        case GAME_STATE_PAUSED:
            handleMenuInput(keys, selectedOption, gameState, MENU_TYPE_PAUSE);
            drawMenu(imagenes, *selectedOption, MENU_TYPE_PAUSE);
            break;

        case GAME_STATE_GAMEOVER:
            handleMenuInput(keys, selectedOption, gameState, MENU_TYPE_GAMEOVER);
            drawMenu(imagenes, *selectedOption, MENU_TYPE_GAMEOVER);
            break;
    }
}

