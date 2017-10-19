# Esta es la target que se ejecuta por defecto si se escribe "make" en la consola
all: dev

# Compilación principal del ejecutable
dev:
	@echo 'Compilando'
	gcc coordinador.c funciones.c -o coordinador -lm
	@echo "Ejecute el programa haciendo: ./coordinador -i archivo -n Ni -m Ci -s salida -d"
	@echo "Donde  archivo: nombre del archivo junto a su extension. Ej: ejemplo1.txt"
	@echo "       Ni: cantidad de filas de la matriz."
	@echo "       Mi: cantidad de columnas de la matriz."
	@echo "		  salida: nombre del archivo de salida."
	@echo "       -d: (opcional) Si desea resultado por pantallas."
	