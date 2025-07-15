 # PARA CREAR EJECUTABLE PC:
# make pc=1

# PARA CREAR EJECUTABLE PI:
# make space_invaders_pi pi=1

# Compilador
CC = gcc

# Flags de compilación
FLAGSE = -Wall -o #Creación ejecutable
FLAGSO = -Wall -c #Creación código objeto

# Librerías
LIBSPI = ./libs/joydisp/joydrv.o ./libs/joydisp/disdrv.o
LIBSPC = -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec

space_invaders_pc: frontend_pc.o backend.o
	$(CC) $(FLAGSE) space_invaders_pc frontend_pc.o backend.o $(LIBSPC)

space_invaders_pi: frontend_pi.o backend.o inputOutputPi.o 
	$(CC) $(FLAGSE) space_invaders_pi frontend_pi.o backend.o inputOutputPi.o $(LIBSPI)

ifdef pi
backend.o: backend.c backend.h constantes_pi.h entidades.h
	$(CC) $(FLAGSO) backend.c -Dpi

inputOutputPi.o: inputOutputPi.c inputOutputPi.h
	$(CC) $(FLAGSO) inputOutputPi.c

frontend_pi.o: frontend_pi.c backend.h constantes_pi.h entidades.h inputOutputPi.h
	$(CC) $(FLAGSO) frontend_pi.c
endif

ifdef pc
backend.o: backend.c constantes_pc.h entidades.h backend.h
	$(CC) $(FLAGSO) backend.c -Dpc

frontend_pc.o: frontend_pc.c backend.h constantes_pc.h entidades.h
	$(CC) $(FLAGSO) frontend_pc.c 
endif

clean:
	rm -f *.o space_invaders_pc space_invaders_pi backend frontend_pc frontend_pi inputOutputPi #Borrar objetos y ejecutables
