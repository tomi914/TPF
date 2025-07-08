//#include <stdlib.h>
#include <stdio.h>

#define img0 squid1.png
#define img1 squid1.png
#define img2 crab1.png
#define img3 crab2.png
#define img4 octopus1.png
#define img5 octopus2.png
#define img6 mistery_ship.png
#define img7 ship.png
#define img8 bullet.png

char img[20];
unsigned int imgSelect = 1;

int main(void){
	sprintf(img, "img%d.png", imgSelect);
	printf("%s", img);
}

//ALLEGRO_BITMAP *sprite_original2 = al_load_bitmap(img);

