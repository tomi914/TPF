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
void drawNum(int xInicial, int yInicial, int arr[5][3]);
//FUNCION DESCOMPONER NUMERO PARA MOSTRAR EL SCORE EN DISPLAY

//VARIABLES GLOBALES
player_t player;
bullet_t pBullet;
bullet_t aBullet;
aliensBlock_t block;
alien_t alien [5][5];
shield_t shield[4];
stats_t stats;
ovni_t ovni;
alien_t * shooter;

//MOVIMIENTO ALIENS
int lastRowToPrint = -1;
clock_t clkB = 0; //tiempos para el movimiento de los aliens
clock_t clkA = 0;
int offsetX;
//int prevCoordX = 0;

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

int main(void){
	joyinfo_t info;
	joy_init();
    clock_t clkP = clock(); //clock pause
    clock_t clkS = clock(); //clock select
    clock_t clkO; //clock ovni
    //clock_t clkA = clock();; //clock aliens moving
    int i, j;
    bool shoot;
    int menu = 1;
    int shooterRow;
    srand(time(NULL)); // Inicializa el generador con el tiempo actual

    initPlayer(&player);
    initAliensBlock(&block);
    initAliensArray(alien);
    initGameStats(&stats);
    initShieldsArray(shield);
    initOvni(&ovni, clock(), &clkO);

	dcoord_t myPoint;

	disp_init();	//inicializa el display

    while(1){
        disp_clear();
        if(getPause(&info, &clkP)){
            clkS = clock();
            while(!getPause(&info, &clkP)){
                menu += getDirY(&info, &clkS);
                if(menu==4){
                    menu = 1;
                }
                if(menu==0){
                    menu = 3;
                }
                switch(menu){
                    case 1:
                        disp_clear();
                        drawPlay();
                        break;
                    case 2:
                        disp_clear();
                        drawExit();
                        break;
                    case 3:
                        disp_clear();
                        drawRestart();
                        break;
                }
                disp_update();
            }
        }

        shieldsUpdate(shield);

        for(i=0; i<4; i++){
            if(shield[i].health){
                myPoint.x = shield[i].coord.coordX;
                myPoint.y = shield[i].coord.coordY;
                disp_write(myPoint, D_ON);
            }
        }

        updateOvni (&ovni, clock(), &clkO, rand() % 1001);

        if(ovni.alive && ovni.visible){
            myPoint.x = ovni.coord.coordX;
            myPoint.y = ovni.coord.coordY;
            disp_write(myPoint, D_ON);
        }

        updateAliensBlock(alien, &block);
		    
        /*shooter = selectAlienShooter(alien, &block, &player);//elije alien a disparar
        if (shooter != NULL) {
            shooterRow = -1;
            for (int i = 0; i < ALIEN_ROWS; i++) {
                for (int j = 0; j < ALIEN_COLS; j++) {
                    if (&alien[i][j] == shooter) {
                        shooterRow = i;
                    }
                }
            }
            alienShoot(&aBullet, shooter, stats.level, &block, lastRowToPrint, shooterRow);
	    }

        if(aBullet.active){
            myPoint.x = aBullet.coord.coordX;
            myPoint.y = aBullet.coord.coordY;
        }

        disp_write(myPoint, D_ON);*/

        if(lastRowToPrint == -1 && ((double)(clock() - clkB) / CLOCKS_PER_SEC) >= 0.5){
				lastRowToPrint = ALIEN_ROWS - 1; // empezamos desde la fila de abajo
				clkA = clock();
				clkB = clock();
                printf("1\n");
		}
		if((lastRowToPrint >= 0 && ((double)(clock() - clkA) / CLOCKS_PER_SEC) >= 0.1)){
            if(lastRowToPrint == 0){
                blockMove(alien, &block); 
                lastRowToPrint = -1;
                printf("2, %d\n", block.coord.coordX);
            }
            else{
                lastRowToPrint--;
                clkA = clock();
                printf("3\n");
            }
		}
			
        for (i = 0; i < ALIEN_ROWS; i++) {
            for (j = 0; j < ALIEN_COLS; j++) {
                if (!alien[i][j].alive) {
                    continue;
                }
                // Offset del bloque horizontal si hay que aplicarlo (cuando baja una fila o si ya terminó de bajar)
                offsetX = block.coord.coordX;
                if (lastRowToPrint >= 0 && i > lastRowToPrint) {
                    offsetX += block.direction;
                }
                // Coordenadas absolutas del alien actual
                myPoint.x = offsetX + alien[i][j].coord.coordX;
                myPoint.y = block.coord.coordY + alien[i][j].coord.coordY;
                // Encender LED del alien en pantalla
                disp_write(myPoint, D_ON);
            }
        }

        /*blockMove(alien, &block);
        for(i=0; i<5; i++){
            for(j=0; j<5; j++){
                if(alien[i][j].alive){
                    myPoint.x = alien[i][j].coord.coordX + block.coord.coordX;
                    myPoint.y = alien[i][j].coord.coordY + block.coord.coordY;
                    disp_write(myPoint, D_ON);
                }
            }
        }*/

        playerMove(getDirX(&info), &player);

        for(i=0; i<3; i++){
            myPoint.x = player.coord.coordX + i;
            myPoint.y = player.coord.coordY;
            disp_write(myPoint, D_ON);
        }

        for(i=0; i<player.health-1; i++){
            myPoint.x = i;
            myPoint.y = 15;
            disp_write(myPoint, D_ON);
        }

        shoot = getShoot(&info);
        playerShoot(&pBullet, &player, &shoot);

        if(pBullet.active){
            myPoint.x = pBullet.coord.coordX;
            myPoint.y = pBullet.coord.coordY;
            /*collisionBA(&pBullet, alien, &block, &stats, lastRowToPrint);
            collisionBO(&pBullet, &ovni, &clkO, &stats);*/
        }
        //collisionDetect(bullet_t * bulletP, bullet_t * bulletA, alien_t aliens[ALIEN_ROWS][ALIEN_COLS], ovni_t * ovni, shield_t shields[NUM_SHIELDS], aliensBlock_t * aliensBlock, player_t * player, stats_t * gameStats, uint8_t printedRow, clock_t *lastOvniDespawnTime);
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

void drawNum(int xInicial, int yInicial, int arr[5][3]){ //VERIFICAR XINICIAL E YINICIAL(?
    int i, j;
    dcoord_t point;
    for(i=0; i<5; i++){
        for(j=0; j<3; j++){
            if(arr[i][j]){
                point.x = j + xInicial;
                point.y = i + yInicial;
                disp_write(point, D_ON);
            }
        }
    }
}
    
