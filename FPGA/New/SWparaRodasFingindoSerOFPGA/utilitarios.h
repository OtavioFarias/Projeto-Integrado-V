#ifndef UTILITARIOS_H
#define UTILITARIOS_H

#include <stdio.h>

//os bits são contados da esquerda para a direita começando em 0
//número para transformar
int separarBits(int n, int inicio, int fim);


//retorna valor a ser guardado na malha
//n número original, v = valor a ser escrito (ex.: 0b1010)
int escreverBits(int n, int inicio, int fim, int v);


void printBits29(int num);

#endif
