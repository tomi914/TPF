#ifndef INPUTOUTPUTPI_H
#define INPUTOUTPUTPI_H

//JOYSTICK
int getDirX(joyinfo_t* joy); //si se mueve a la izq o a la der
int getDirY(joyinfo_t* joy, clock_t* last); //si se mueve hacia arriba o hacia abajo (para elegir opciones en el menu)
bool getClick(joyinfo_t* joy); // si se presiona el joystick
bool getPause(joyinfo_t* joy, clock_t* last); //pausa

//DISPLAY
void drawSymbol(int x); //Dibujar play, pause y restart
void drawDigit(int xInicial, int yInicial, int digit); //Dibujar un numero de un digito
void drawNum(int xInicial, int yInicial, int num); //Dibujar un numero de cuatro digitos

#endif
