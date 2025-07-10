typedef struct{ // Almacena las coordenadas 
	uint16_t coordX;
	uint16_t coordY;
}coord_t;

typedef struct{ // Almacena la información general de toodos los ALIENS.
	bool alive;
	char type;
	coord_t coord; 
	uint8_t vel;  //chequear porque se modifica con el nivel
}alien_t;

typedef struct{ //Almacena las estadisticas de este juego y los anteriores.
	uint32_t score;
	uint16_t level;
	//ranking?
}stats_t;

typedef struct{ // Almacena la información general de los ESCUDOS
	uint8_t health; 
	coord_t coord; 
}shield_t;

typedef struct{ // Almacena la información general del JUGADOR (nave espacial)
	uint8_t health; 
	coord_t coord;
}player_t;

typedef struct{ // Almacena la información general de las BALAS  
	coord_t coord;
	bool active; // Nos indica si la bala existe o no.
}bullet_t;


typedef struct {
	coord_t coord;
	int8_t direction;
	uint8_t firstColAlive;
	uint8_t lastColAlive;
	uint8_t lastRowAlive;
	uint16_t width;
}aliensBlock_t;