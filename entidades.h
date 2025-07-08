typedef struct {
	int coordX;
	int coordY;
	int8_t direction;
	uint8_t firstColAlive;
	uint8_t lastColAlive;
}alienBlock_t;

typedef struct{ // Almacena las coordenadas 
	uint16_t coordX;
	uint16_t coordY;
}coord_t;

typedef struct{ //Almacena las estadisticas de este juego y los anteriores.
	uint32_t score;
	uint16_t level;
	//ranking?
}stats_t;

/*   VER SI ES NECESARIO
typedef struct{
	bool pause;
	bool restart;
	bool exit; 
}menu_t;
*/

//entidades y objetos
 
typedef struct{ // Almacena la información general de toodos los ALIENS.
	bool alive;
	char type;
	coord_t coord; 
	uint8_t vel;  //chequear porque se modifica con el nivel
}alien_t;

typedef struct{ // Almacena la información general de los ESCUDOS (los tratamos como escuditos)
	bool built; 
	coord_t coord; 
}shield_t;

typedef struct{ // Almacena la información general del JUGADOR (nave espacial)
	uint8_t health; 
	coord_t coord;
}player_t;

typedef struct{ // Almacena la información general de las BALAS
	char type; 
	coord_t coord;
	bool active; // Nos indica si la bala existe o no.
}bullet_t;
