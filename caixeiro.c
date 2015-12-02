#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAX 300
int distancia2d(int xi, int yi, int xj, int yj){
	int distemp, distancia;
	distemp = ((xi-xj)*(xi-xj)) + ((yi - yj)*(yi - yj));
	distancia = sqrt(distemp);
	return distancia;
}

int distancia3d(int xi, int yi, int xj, int yj, int zi, int zj){
	int distemp, distancia;
	distemp = ((xi-xj)*(xi-xj)) + ((yi - yj)*(yi - yj)) + ((zi - zj)*(zi - zj));
	distancia = sqrt(distemp);
	return distancia;
}

int main(){
	int i = 0, dim, j, x, y, z, tmp;
	char ch[MAX], aux[10];

	FILE *f = fopen("test.tsp", "r");

	for(i=0;strcmp(ch, "EOF") != 0;i++){	
		if(i == 3){
			fscanf(f, "%s", aux); // Consome "DIMENSION:"
			fscanf(f, "%d", &dim); // Pega o número de cidades
			printf("%s %d\n",aux, dim);
		}
		if(i > 3){
			fgets(ch, MAX, f); // consome \n
			fgets(ch, MAX, f); // consome tipo arquivo
			if(ch[22] == '2'){ // Verifica se existe EUC_2D
				fgets(ch, MAX, f); // Pula linha
				int vtc[dim][3];
				for(j=0;j<dim;j++){
					fscanf(f, "%d %d %d", &tmp, &x, &y);
					vtc[j][0] = tmp;
					vtc[j][1] = x;
					vtc[j][2] = y;
				}
				for(j=0;j<dim;j++){
					printf("id=%d\n", vtc[j][0]);
					printf("x=%d ", vtc[j][1]);
					printf("y=%d\n", vtc[j][2]);
				}
			}

			if(ch[22] == '3'){ // Verifica se existe EUC_2D
				fgets(ch, MAX, f); // Pula linha
				int vtc[dim][4];
				for(j=0;j<dim;j++){
					fscanf(f, "%d %d %d %d", &tmp, &x, &y, &z);
					vtc[j][0] = tmp;
					vtc[j][1] = x;
					vtc[j][2] = y;
					vtc[j][3] = z;
				}
				for(j=0;j<dim;j++){
					printf("id=%d\n", vtc[j][0]);
					printf("x=%d ", vtc[j][1]);
					printf("y=%d ", vtc[j][2]);
					printf("z=%d\n", vtc[j][3]);
				}
			}

			if(strcmp(ch, "EOF")){ // Verifica se existe EXPLICIT
				printf("acabou\n");
				break;
			}
		}
		if(i < 3){
			fgets(ch, MAX, f); // Armazena cada linha em ch
			printf("%s", ch);
		}
	}

	return 0;
}	