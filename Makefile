final:compile run clear

compile:
	@gcc main.c matrixLib.c -o Main
run:
	@./Main
clear:
	@rm Main

