//LIBRERIAS ESTANDAR
#include <stdio.h>
#include <time.h> //para el clock
#include <unistd.h> //para el usleep

#include<stdbool.h>
#include<stdint.h>
#include<stdlib.h>

//LIBRERIAS LOCALES
#include "./libs/joydisp/joydrv.h" 
#include "./libs/joydisp/disdrv.h"
#include "entidades.h"
#include "constantes_pi.h"
#include "backend.h"

//PROTOTIPOS FUNCIONES
int getDirX(joyinfo_t* joy); //si se mueve a la izq o a la der
int getDirY(joyinfo_t* joy, clock_t* last); //si se mueve hacia arriba o hacia abajo (para elegir opciones en el menu)
bool getShoot(joyinfo_t* joy); //disparo
bool getPause(joyinfo_t* joy, clock_t* last); //pausa
void drawPlay(void);
void drawRestart(void);
void drawExit(void);

//VARIABLES GLOBALES
player_t player;
bullet_t pBullet;
alienBlock_t block;
alien_t alien [5][5];
stats_t stats;

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

int main(void){
	joyinfo_t info;
	joy_init();
    clock_t clkP = clock(); //clock pause
    clock_t clkS = clock(); //clock select
    //clock_t clkA; //clock aliens moving
    int i, j;
    bool shoot;
    //int lastRowToPrint = -1;
    
    initPlayer(&player);
    initAliensBlock(&block);
    initAliensArray(alien);
    initGameStats(&stats);
    

	dcoord_t myPoint;

	disp_init();	//inicializa el display

    while(1){
        disp_clear();
        if(getPause(&info, &clkP)){
            clkS = clock();
            while(!getPause(&info, &clkP)){
                drawExit();
                disp_update();
                printf("%d\n", getDirY(&info, &clkS));
            }
        }

        //updateAliensBlock(alien, &block);
        /*if(lastRowToPrint == 4){
            blockMove(alien, &block);
        }
        for(i=4; i>=0; i--){
            for(j=0; j<5; j++){
                if(alien[i][j].alive){
                    if(lastRowToPrint==-1){
                        myPoint.x = alien[i][j].coord.coordX;
                        myPoint.y = alien[i][j].coord.coordY;
                        disp_write(myPoint, D_ON);
                        lastRowToPrint = 4;
                        clkA = clock();
                    }
                    if(lastRowToPrint==i && ((double)(clock()-clkA)/CLOCKS_PER_SEC)>=0.5){
                        clkA = clock();
                        myPoint.x = alien[i][j].coord.coordX + block.coord.coordX;
                        myPoint.y = alien[i][j].coord.coordY + block.coord.coordY;
                        disp_write(myPoint, D_ON);
                        lastRowToPrint--;
                    } else{
                        myPoint.x = alien[i][j].coord.coordX + block.coord.coordX - 1 * block.direction;
                        myPoint.y = alien[i][j].coord.coordY + block.coord.coordY - 1 * block.direction;
                        disp_write(myPoint, D_ON);
                    }
                }
            }
        }*/

        blockMove(alien, &block);
        for(i=0; i<5; i++){
            for(j=0; j<5; j++){
                if(alien[i][j].alive){
                    myPoint.x = alien[i][j].coord.coordX + block.coord.coordX;
                    myPoint.y = alien[i][j].coord.coordY + block.coord.coordY;
                    disp_write(myPoint, D_ON);
                }
            }
        }
        playerMove(getDirX(&info), &player);
        for(i=0; i<3; i++){
            myPoint.x = player.coord.coordX + i;
            myPoint.y = player.coord.coordY;
            disp_write(myPoint, D_ON);
        }
        shoot = getShoot(&info);
        playerShoot(&pBullet, &player, &shoot);
        if(pBullet.active){
            myPoint.x = pBullet.coord.coordX;
            myPoint.y = pBullet.coord.coordY;
            collisionBA(&pBullet, alien, &block, &stats, 0);
        }
        disp_write(myPoint, D_ON);
        disp_update();
    }

	return 0;
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

int getDirY(joyinfo_t* joy, clock_t* last){
    static int state = 0;
    *joy = joy_read(); 
	if(joy->y>10){ //Retorna 1 si se mueve hacia arriba
        if(((double)(clock()-*last)/CLOCKS_PER_SEC)<=0.2 && !state){
            *last = clock();
            state = 1;
            return 1;
        }
	}else if(joy->y<-10){ //Retorna -1 si se mueve hacia abajo
		if(((double)(clock()-*last)/CLOCKS_PER_SEC)<=0.2 && !state){
            *last = clock();
            state = 1;
            return -1;
        }
	} else{
        state = 0;
    }
	return 0;
}

bool getShoot(joyinfo_t* joy){
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

bool getPause(joyinfo_t* joy, clock_t* last){
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
    if((((double)(clock()-*last)/CLOCKS_PER_SEC)>=0.2 && click) || click == 2){
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

void drawPlay(void){
    int i, j;
    dcoord_t point;
    for(i=0; i<16; i++){
        for(j=0; j<16; j++){
            if(playSymbol[i][j]){
                point.x = j;
                point.y = i;
                disp_write(point, D_ON);
            }
        }
    }
}

void drawRestart(void){
    int i, j;
    dcoord_t point;
    for(i=0; i<16; i++){
        for(j=0; j<16; j++){
            if(restartSymbol[i][j]){
                point.x = j;
                point.y = i;
                disp_write(point, D_ON);
            }
        }
    }
}

void drawExit(void){
    int i, j;
    dcoord_t point;
    for(i=0; i<16; i++){
        for(j=0; j<16; j++){
            if(exitSymbol[i][j]){
                point.x = j;
                point.y = i;
                disp_write(point, D_ON);
            }
        }
    }
}
    
