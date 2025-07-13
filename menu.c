#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_W 800
#define SCREEN_H 800
#define CARPETA "resizedImages/"

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

// Tenemos 5 ESTADOS de JUEGO
typedef enum {
    GAME_STATE_START = 0,
    GAME_STATE_PLAYING = 1,
    GAME_STATE_PAUSED = 2,
    GAME_STATE_GAMEOVER = 3,
    GAME_STATE_EXIT = 5
} GameState;

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
  GAMEOVER_OPTION_PLAY = 0,
  GAMEOVER_OPTION_QUIT = 1
} GameoverMenuOption;


/* Esta función se va a encargar de seleccionar las opciones, de acuerdo a que teclas presione el usuario y en que tipo de menú se encuentre.*/
void handleMenuInput(bool keys[], int *selectedOption, GameState *currentGameState, MenuType type) {
    
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
                    // InitVariables
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
                    // InitVariables
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
    
    al_draw_bitmap(imagenes[IMG_FONDO], 0, 0, 0); // Agregamos el marco para el menú, junto a los aliens y las naves de decoración.
    
    al_draw_bitmap(imagenes[IMG_LOGO], 200, 50, 0); // Agregamos el cartel de Space Invaders.

    if (type == MENU_TYPE_START) { // Si estamos en el MENÚ de INICIO
            al_draw_bitmap(selectedOption == START_OPTION_PLAY ? imagenes[IMG_PLAY_ON] : imagenes[IMG_PLAY_OFF], 211, 412, 0);
            al_draw_bitmap(selectedOption == START_OPTION_QUIT ? imagenes[IMG_QUIT_ON] : imagenes[IMG_QUIT_OFF], 267, 625, 0);
    }
    
    else if (type == MENU_TYPE_PAUSE) { // Si estamos en el MENÚ de PAUSA
        al_draw_bitmap(selectedOption == PAUSE_OPTION_RESUME ? imagenes[IMG_RESUME_ON] : imagenes[IMG_RESUME_OFF], 164, 325, 0);
        al_draw_bitmap(selectedOption == PAUSE_OPTION_RESTART ? imagenes[IMG_RESTART_ON] : imagenes[IMG_RESTART_OFF], 152, 475, 0);
        al_draw_bitmap(selectedOption == PAUSE_OPTION_QUIT ? imagenes[IMG_QUIT_ON] : imagenes[IMG_QUIT_OFF], 267, 625, 0);
    }
    
    else if (type == MENU_TYPE_GAMEOVER) { // Si estamos en el MENÚ de GAMEOVER
        al_draw_bitmap(selectedOption == GAMEOVER_OPTION_RESTART ? imagenes[IMG_RESTART_ON_GAMEOVER] : imagenes[IMG_RESTART_OFF_GAMEOVER], 94, 427, 0);
        al_draw_bitmap(selectedOption == GAMEOVER_OPTION_QUIT ? imagenes[IMG_QUIT_ON] : imagenes[IMG_QUIT_OFF], 267, 625, 0);
    }
    
}
