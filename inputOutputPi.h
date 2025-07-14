#ifndef INPUTOUTPUTPI_H
#define INPUTOUTPUTPI_H

//PROTOTIPOS FUNCIONES
int getDirX(joyinfo_t* joy); //si se mueve a la izq o a la der
int getDirY(joyinfo_t* joy, clock_t* last); //si se mueve hacia arriba o hacia abajo (para elegir opciones en el menu)
bool getClick(joyinfo_t* joy); 
bool getPause(joyinfo_t* joy, clock_t* last); //pausa
void drawSymbol(int x); 
void drawNum(int xInicial, int yInicial, int num);
//FUNCION DESCOMPONER NUMERO PARA MOSTRAR EL SCORE EN DISPLAY

#endif