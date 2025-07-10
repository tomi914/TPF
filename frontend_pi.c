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
int getDir(joyinfo_t* joy); //si se mueve a la izq o a la der
bool getShoot(joyinfo_t* joy); //disparo
bool getPause(joyinfo_t* joy, clock_t* last); //pausa

//VARIABLES GLOBALES
player_t player;
bullet_t pBullet;
alienBlock_t block;
alien_t alien [5][5];

int main(void){
	joyinfo_t info;
	joy_init();
    clock_t clk = clock();
    int i, j;
    bool shoot;
    
    initPlayer(&player);
    initAliensBlock(&block);
    initAliensArray(alien);
    

	dcoord_t myPoint;

	disp_init();	//inicializa el display

    while(1){
        disp_clear();
        if(getPause(&info, &clk)){
            while(!getPause(&info, &clk)){
                myPoint.x = 15;
                myPoint.y = 15;
                disp_write(myPoint, D_ON);
                disp_update();
            }
        }
        /*updateAliensBlock(&alien, &block);
        blockMove(alien, &block);*/
        for(i=0; i<5; i++){
            for(j=0; j<5; j++){
                if(alien[i][j].alive){
                    myPoint.x = alien[i][j].coord.coordX;
                    myPoint.y = alien[i][j].coord.coordY;
                    disp_write(myPoint, D_ON);
                }
            }
        }
        playerMove(getDir(&info), &player);
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
        }
        disp_write(myPoint, D_ON);
        disp_update();
    }

	return 0;
}

int getDir(joyinfo_t* joy){ //Determina la direccion del player
	*joy = joy_read();
	if(joy->x>10){ //Retorna 1 si se mueve a la derecha
		return 1;
	}
	if(joy->x<-10){ //Retorna -1 si se mueve a la izquierda
		return -1;
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
    if((((double)(clock()-*last)/CLOCKS_PER_SEC)>=0.5 && click) || click == 2){
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

/*int getPress(joyinfo_t* joy, clock_t* last){ //cuenta la cantidad de clicks en un intervalo de tiempo a partir de un click
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
    if(((double)(clock()-*last)/CLOCKS_PER_SEC)>=0.3 && click){
        int state = click;
        click = 0;
        pressing = 0;
        return state;
    }
    return 0;
}*/

    /*if(joy->sw==J_PRESS || pressed){ //funcion que si mantengo apretado mas de x tiempo devuelve otro numero
        pressed=1;
        if(((double)(clock()-*last)/CLOCKS_PER_SEC)>=1){
            if(joy->sw==J_NOPRESS){

            }
            
            pressed=0;
            return -1; //pausa
        } 
        if(joy->sw==J_PRESS){
            pressed=0;
            return 1; //bala
        }
        return 20;
	}else{
        pressed=0;
        *last=clock();
        return 0;
    }*/
    
