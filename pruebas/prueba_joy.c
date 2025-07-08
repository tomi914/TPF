#include <stdio.h>
#include <time.h>
#include <unistd.h> //para el usleep (en mili)
#include "./libs/joydisp/joydrv.h"

int getDir(joyinfo_t* joy); //si se mueve a la izq o a la der
int getBullet(joyinfo_t* joy); //si se presiona una vez es una bala
int getPause(joyinfo_t* joy); //si se presiona dos veces seguidas el pulsador y abre un menu para reanudar, restart o exit

int getPress(joyinfo_t* joy);

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

int getPress(joyinfo_t* joy, clock_t last){ //clock_t last = clock();
	double interval = 1.0; //en segundos
	if((*joy).sw == J_PRESS){
		clock_t now = clock();
		double elapsed = (double)(now - last) / CLOCKS_PER_SEC;
	}
}

int main() {
    clock_t last = clock();
    double interval = 1.0; // segundos
    while (1){
        clock_t now = clock();
        double elapsed = (double)(now - last) / CLOCKS_PER_SEC;
        if (elapsed >= interval) {
            printf("Timer! 1 segundo pasado.\n");
            last = now;
        }
        // resto del programa corre libremente aquí
    }
    return 0;
}