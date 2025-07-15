//LIBRERIAS ESTANDAR
#include <time.h> //para el clock
#include <unistd.h> //para el usleep
#include<stdbool.h>

//LIBRERIAS LOCALES
#include "./libs/joydisp/joydrv.h" 
#include "./libs/joydisp/disdrv.h"
#include "inputOutputPi.h"

int playSymbol[16][16] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
    {0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
    {0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
    {0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
    {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
    {0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

int restartSymbol[16][16] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0},
    {0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0},
    {0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

int exitSymbol[16][16] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0},
    {0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0},
    {0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0},
    {0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
    {0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0},
    {0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0},
    {0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0},
    {0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

int cero[5][3] = {
    {1, 1, 1},
    {1, 0, 1},
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
};

int uno[5][3] = {
    {0, 0, 1},
    {0, 0, 1},
    {0, 0, 1},
    {0, 0, 1},
    {0, 0, 1},
};

int dos[5][3] = {
    {1, 1, 1},
    {0, 0, 1},
    {1, 1, 1},
    {1, 0, 0},
    {1, 1, 1},
};

int tres[5][3] = {
    {1, 1, 1},
    {0, 0, 1},
    {1, 1, 1},
    {0, 0, 1},
    {1, 1, 1},
};

int cuatro[5][3] = {
    {1, 0, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 0, 1},
    {0, 0, 1},
};

int cinco[5][3] = {
    {1, 1, 1},
    {1, 0, 0},
    {1, 1, 1},
    {0, 0, 1},
    {1, 1, 1},
};

int seis[5][3] = {
    {1, 1, 1},
    {1, 0, 0},
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
};

int siete[5][3] = {
    {1, 1, 1},
    {0, 0, 1},
    {0, 0, 1},
    {0, 0, 1},
    {0, 0, 1},
};

int ocho[5][3] = {
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
};

int nueve[5][3] = {
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
    {0, 0, 1},
    {1, 1, 1},
};

bool getClick(joyinfo_t* joy){ //Detecta el flanco de bajada del pulsador
    static bool pressed = false;
    *joy = joy_read();
    if(joy->sw == J_NOPRESS){
        pressed = false;
    }
    if(joy->sw == J_PRESS && !pressed){
        pressed = true;
        usleep(10000);
        return 1;
    }
    return 0;
}

bool getPause(joyinfo_t* joy, clock_t* last){ //Detecta el doble click en un periodo corto de  tiempo del pulsador
    static int pressing = 0;
    static int click = 0;
    *joy = joy_read();
    if(joy->sw == J_PRESS && !click && !pressing){
        pressing = 1;
        *last = clock();
        usleep(10000);
    }
    if(!pressing && joy->sw == J_PRESS){
        pressing = 1;
        usleep(10000);
    }
    if(pressing && joy->sw == J_NOPRESS){
        pressing = 0;
        click++;
        usleep(10000);
    }
    if((((double)(clock()-*last)/CLOCKS_PER_SEC)>=0.1 && click) || click == 2){
        pressing = 0;
        if(click == 2){
            click = 0;
            return true;
        } 
        click = 0;
        return false;
    }
    return false;
}

int getDirX(joyinfo_t* joy){ //Determina la direccion del player
	*joy = joy_read();
	if(joy->x>10){ //Retorna 1 si se mueve a la derecha
		return 1;
	}
	if(joy->x<-10){ //Retorna -1 si se mueve a la izquierda
		return -1;
	}
	return 0;
}

int getDirY(joyinfo_t* joy, clock_t* last){ //Si muevo para arriba o para abajo devuelve 1, si mantengo para arriba o hacia abajo durante un tiempo devuelve otro 1
    static int state = 0;
    *joy = joy_read(); 
	if(((double)(clock()-*last)/CLOCKS_PER_SEC)>=0.1){
        state = 0;
    }
    if(joy->y>10){ //Retorna 1 si se mueve hacia arriba
        if(!state){
            *last = clock();
            state = 1;
            return 1;
        }
	}else if(joy->y<-10){ //Retorna -1 si se mueve hacia abajo
		if(!state){
            *last = clock();
            state = 1;
            return -1;
        }
	} else{
        state = 0;
    }
	return 0;
}

void drawSymbol(int x) {
    int i, j;
    dcoord_t point;
    int (*arr)[16] = NULL; 

    switch (x) {
        case 0:
            arr = playSymbol;
            break;
        case 1:
            arr = exitSymbol;
            break;
        case 2:
            arr = restartSymbol;
            break;
    }

    for (i = 0; i < 16; i++) {
        for (j = 0; j < 16; j++) {
            if (arr[i][j]) {
                point.x = j;
                point.y = i;
                disp_write(point, D_ON);
            }
        }
    }
}

void drawDigit(int xInicial, int yInicial, int digit){ //dibujar un digito (queda en el buffer)
    int (*arr)[3] = NULL;  

    switch (digit) {
        case 0: arr = cero; break;
        case 1: arr = uno; break;
        case 2: arr = dos; break;
        case 3: arr = tres; break;
        case 4: arr = cuatro; break;
        case 5: arr = cinco; break;
        case 6: arr = seis; break;
        case 7: arr = siete; break;
        case 8: arr = ocho; break;
        case 9: arr = nueve; break;
    }

    dcoord_t point;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            if (arr[i][j]) {
                point.x = j + xInicial;
                point.y = i + yInicial;
                disp_write(point, D_ON);
            }
        }
    }
}

void drawNum(int xInicial, int yInicial, int num){ //dibujar un nunmero de cuatro digitos (queda en el buffer)
    int mil = num / 1000;
    int cen = (num % 1000) / 100;
    int dec = (num % 100) / 10;
    int uni = num % 10;

    drawDigit(xInicial, yInicial, mil);
    drawDigit(xInicial + 4, yInicial, cen);
    drawDigit(xInicial + 8, yInicial, dec);
    drawDigit(xInicial + 12, yInicial, uni);
}
