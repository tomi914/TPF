//LIBRERIAS ESTANDAR
#include <stdio.h>
#include <time.h> //para el clock
#include <unistd.h> //para el usleep
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

//LIBRERIAS LOCALES
#include "./libs/joydisp/joydrv.h" 
#include "./libs/joydisp/disdrv.h"
#include "entidades.h"
#include "constantes_pi.h"
#include "inputOutputPi.h"
#include "backend.h"

int main(void){
    player_t player;
    bullet_t pBullet;
    bullet_t aBullet;
    aliensBlock_t block;
    alien_t alien [5][5];
    shield_t shield[4];
    stats_t stats;
    ovni_t ovni;
    alien_t * shooter;
    joyinfo_t info;
    joy_init();
    clock_t clkP = clock(); //clock pause
    clock_t clkS = clock(); //clock select
    clock_t clkO; //clock ovni
    int i, j;
    bool shoot;
    int shooterRow;
    int menu = 0;
    bool restart = false;
    bool quit = false;
    srand(time(NULL)); // Inicializa el generador con el tiempo actual
    dcoord_t myPoint;
    disp_init();	//inicializa el display
    int lastRowToPrint = -1;
    clock_t clkB = 0; //tiempos para el movimiento de los aliens
    clock_t clkA = 0;
    int offsetX;
    float interval; 

    GameState game = GAME_STATE_START; 

    while(!quit){
        switch(game){
            case GAME_STATE_START:
                disp_clear();
                menu += getDirY(&info, &clkS);
                if(menu==2){
                    menu = 0;
                } else if(menu==-1){
                    menu = 1;
                }
                if(getClick(&info) || restart){
                   switch(menu){
                    case 0:
                        restart = false;
                        initPlayer(&player);
                        initAliensBlock(&block);
                        initAliensArray(alien);
                        initGameStats(&stats);
                        initShieldsArray(shield);
                        initOvni(&ovni, clock(), &clkO);
                        pBullet.active = false;
                        aBullet.active = false;
                        game = GAME_STATE_PLAYING;
                        break;
                    case 1:
                        game = GAME_STATE_EXIT;
                        break;
                    }
                }   
                drawSymbol(menu);
                disp_update();
                break;

            case GAME_STATE_PLAYING:
                disp_clear(); //Limpia el display
                //Shields
                for(i=0; i<4; i++){
                    if(shield[i].health){
                    myPoint.x = shield[i].coord.coordX;
                    myPoint.y = shield[i].coord.coordY;
                    disp_write(myPoint, D_ON);
                    }
                }
                //Ovni
                updateOvni (&ovni, clock(), &clkO, rand() % 1001);
                if(ovni.alive && ovni.visible){
                    myPoint.x = ovni.coord.coordX;
                    myPoint.y = ovni.coord.coordY;
                    disp_write(myPoint, D_ON);
                }
                //Aliens
                updateAliensBlock(alien, &block);
                shooter = selectAlienShooter(alien, &block, &player);//elije alien a disparar
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
                disp_write(myPoint, D_ON);
                if(stats.level<17){
                    if((block.coord.coordY + block.lastRowAlive*2*ALIEN_B_SIZE_Y)<8){
                    interval = 0.09 - (stats.level -1) /200.;
                    } else if((block.coord.coordY + block.lastRowAlive*2)<10){
                        interval = 0.08 - (stats.level -1) /200.; 
                    } else if((block.coord.coordY + block.lastRowAlive*2)<13){
                        interval = 0.06 - (stats.level -1) /1000.;
                    } else{
                        interval = 0.03 - (stats.level -1) /1000.;
                    }
                } else{
                    interval = 0.02;
                }
                if((lastRowToPrint == -1 && ((double)(clock() - clkB) / CLOCKS_PER_SEC) >= interval)){
                        lastRowToPrint = ALIEN_ROWS - 1; // empezamos desde la fila de abajo
                        clkA = clock();
                        printf("1\n");
                }
                if((lastRowToPrint >= 0 && ((double)(clock() - clkA) / CLOCKS_PER_SEC) >= interval)){
                    if(lastRowToPrint == 0){
                        blockMove(alien, &block); 
                        lastRowToPrint = -1;
                        clkB = clock();
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
                        //collisionDetect(&pBullet, &aBullet, alien, &ovni, shield, &block, &player, &stats, lastRowToPrint, &clkO);
                        if (!alien[i][j].alive) {
                            continue;
                        }
                        // Offset del bloque horizontal si hay que aplicarlo (cuando baja una fila o si ya terminó de bajar)
                        offsetX = block.coord.coordX;
                        if (lastRowToPrint >= 0 && i >= lastRowToPrint) {
                            offsetX += block.direction;
                        }
                        // Coordenadas absolutas del alien actual
                        myPoint.x = offsetX + alien[i][j].coord.coordX;
                        myPoint.y = block.coord.coordY + alien[i][j].coord.coordY;
                        // Encender LED del alien en pantalla
                        disp_write(myPoint, D_ON);
                    }
                }
                //Colisiones
                collisionDetect(&pBullet, &aBullet, alien, &ovni, shield, &block, &player, &stats, lastRowToPrint, &clkO);
                //Player
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
                shoot = getClick(&info);
                playerShoot(&pBullet, &player, &shoot);
                if(pBullet.active){
                    myPoint.x = pBullet.coord.coordX;
                    myPoint.y = pBullet.coord.coordY;
                }
                disp_write(myPoint, D_ON);
                disp_update(); //Muestra en el display
                if(getPause(&info, &clkP)){
                    game = GAME_STATE_PAUSED;
                }
                if(!(player.health)){	//si se quedo sin vidas o si los aliens conquistaron la tierra
                    game = GAME_STATE_GAMEOVER;
			    }
                newLevelCheck(alien, &block, &player, shield, &stats);
                break;

            case GAME_STATE_PAUSED:
                disp_clear();
                menu += getDirY(&info, &clkS);
                if(menu==3){
                    menu = 0;
                } else if(menu==-1){
                    menu = 2;
                }
                if(getClick(&info)){
                   switch(menu){
                    case 0:
                        game = GAME_STATE_PLAYING;
                        break;
                    case 1:
                        game = GAME_STATE_EXIT;
                        break;
                    case 2:
                        game = GAME_STATE_START;
                        restart = true;
                        menu = 0;
                        break;
                    } 
                }
                drawSymbol(menu);
                disp_update();
                break;

            case GAME_STATE_GAMEOVER:
                disp_clear();
                drawNum(0, 6, stats.actualScore);
                disp_update();
                sleep(4);
                game = GAME_STATE_START;
                break;

            case GAME_STATE_EXIT:
                quit = true;
                break;
        }
    }
    disp_clear();
    disp_update();
    return 0;
}

    
