
#ifndef ENTIDADES_H
#define ENTIDADES_H

typedef struct{ // Almacena las coordenadas 
	int16_t coordX;
	int16_t coordY;
}coord_t;

typedef struct {
	coord_t coord;
	int8_t direction;
	uint8_t firstColAlive;
	uint8_t lastColAlive;
	uint8_t lastRowAlive;
	uint16_t width; 
}aliensBlock_t;

typedef struct{ // Almacena la información general de toodos los ALIENS.
	bool alive;
	char type;
	coord_t coord; 
	uint8_t vel;  //chequear porque se modifica con el nivel
}alien_t;

typedef struct{ //Almacena las estadisticas de este juego y los anteriores.
	uint32_t actualScore;
	uint16_t level;
	//ranking?
}stats_t;

typedef struct{ // Almacena la información general de los ESCUDOS
	uint8_t health; 
	coord_t coord; 
	uint16_t sizeX;		//tiene tamaño variable porque cuando se daña, se achica
	uint16_t sizeY;
}shield_t;

typedef struct{ // Almacena la información general del JUGADOR (nave espacial)
	uint8_t health; 
	coord_t coord;
	uint16_t bulletsFired;
}player_t;

typedef struct{ // Almacena la información general de las BALAS 
	coord_t coord;
	bool active; // Nos indica si la bala existe o no.
}bullet_t;

typedef struct{ // Almacena la información general del ovni.
	bool alive; //Indica si el ovni está vivo o no.
	bool visible; //Indica si el ovni está visible en pantalla o no.
	coord_t coord;
}ovni_t;

// Imágenes del MENÚ
enum {
    IMG_START_END_BACKGROUND,
    IMG_PAUSE_BACKGROUND,
    IMG_LOGO,
    IMG_PLAY_ON,
    IMG_PLAY_OFF,
    IMG_RESUME_ON,
    IMG_RESUME_OFF,
    IMG_RESTART_ON,
    IMG_RESTART_OFF,
    IMG_RESTART_ON_GAMEOVER,
    IMG_RESTART_OFF_GAMEOVER,
    IMG_QUIT_ON,
    IMG_QUIT_OFF,
    IMG_TOTAL_MENU
};

// Tenemos 3 TIPOS distintos de MENÚ
typedef enum{
    MENU_TYPE_START,
    MENU_TYPE_PAUSE,
    MENU_TYPE_GAMEOVER
} MenuType;


// Cada menu va a tener una cantidad específica de OPCIONES

// Menú de INICIO
typedef enum{
  START_OPTION_PLAY,
  START_OPTION_QUIT
} StartMenuOption;

// Menú de PAUSA
typedef enum{
  PAUSE_OPTION_RESUME,
  PAUSE_OPTION_RESTART,
  PAUSE_OPTION_QUIT
} PauseMenuOption;

// Menú de GAMEOVER
typedef enum{
  GAMEOVER_OPTION_RESTART,
  GAMEOVER_OPTION_QUIT
} GameoverMenuOption;

// Tenemos 5 ESTADOS de JUEGO
typedef enum {
    GAME_STATE_START,
    GAME_STATE_PLAYING,
    GAME_STATE_PAUSED,
    GAME_STATE_GAMEOVER,
    GAME_STATE_EXIT,
    GAME_STATE_SCORE_SCREEN
} GameState;

#endif // ENTIDADES_H
