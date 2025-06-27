#include <stdio.h>
#include <unistd.h> //para el usleep (en mili)
#include "./libs/joydisp/joydrv.h"

int getDir(joyinfo_t* joy); //si se mueve a la izq o a la der
int getBullet(joyinfo_t* joy); //si se presiona una vez es una bala
int getPause(joyinfo_t* joy); //si se presiona dos veces seguidas el pulsador
int getRestart(joyinfo_t* joy); //si se mantiene mas de x segundos para abajo
int getExit(joyinfo_t* joy); //si se mantiene mas de x segundos para arriba

int main(void){
	joyinfo_t info;
	joy_init();
	do{
		printf("%d\n", getDir(&info));	
		usleep(2000);
	} while (info.sw == J_NOPRESS);	//mientras no se presione el switch 
	return 0;
}

int getDir(joyinfo_t* joy){ 
	*joy = joy_read();
	if(joy->x>10){
		return 1;
	}
	if(joy->x<-10){
		return -1;
	}
	return 0;
}