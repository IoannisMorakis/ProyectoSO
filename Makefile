#
# Proyecto
#

all: Proyecto.c
	gcc Proyecto.c -o Proyecto

clean:
	rm -rf Proyecto




#Proyecto:   Proyecto.o
#	gcc -o Proyecto Proyecto.o

#Proyecto.o: Proyecto.c
#	gcc -c Proyecto.c

