#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAX 300

void menordistancia(FILE *f, int dim, int adj[][dim]){
	char ch[MAX] = "INICIO";
	int x, j, i;
	int ciclo[dim], menor[dim];
	int dist, menordist = -1;
	do{
		dist = 0;
		fgets(ch, MAX, f);
		for(x=0;x<dim;x++){
			fscanf(f, "%d", &ciclo[x]);
			printf("%d", ciclo[x]);
		}
		printf("\n", ciclo[x]);
		for(x=0;x<dim;x++){
			if(x<dim-1){
				printf("%d + %d\n", dist, adj[(ciclo[x]-1)][(ciclo[x+1]-1)]);
				dist = dist + adj[(ciclo[x]-1)][(ciclo[x+1]-1)];
			}
			else{
				printf("%d + %d\n", dist, adj[(ciclo[x]-1)][(ciclo[0]-1)]);
				printf("x %d y %d \n", ciclo[x]-1, ciclo[0]-1);
				dist = dist + adj[(ciclo[x]-1)][(ciclo[0]-1)];
			}
		}
		if(menordist == -1){
			menordist = dist;
			for(x=0;x<dim;x++){
				menor[x] = ciclo[x];
			}
		}
		if(menordist > dist){
			menordist = dist;
			for(x=0;x<dim;x++){
				menor[x] = ciclo[x];
			}
		}
	}while(strcmp(ch, "EOF"));

	printf("menor distancia = %d\n", menordist);

	for(x=0;x<dim;x++){
		printf("%d ", menor[x]);
	}
	printf("\n");
	for(i=0;i<dim;i++){
		for(j=0;j<dim;j++){
   			printf("%d ", adj[i][j]);
   		}
   		printf("\n");
   	}
   	printf("\n");
	printf("\n");
}
 
void swap(int *x, int *y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}
 
void permute(int *a, int l, int r, FILE *f)
{
   int i, x=0;
   if (l == r){
   		for(x=0;x<=r;x++){
   			printf("%d ", a[x]);
   			fprintf(f, "%d ", a[x]);
   		}
   		printf("\n");
   		fprintf(f, "\n");
   }	
   else
   {
       for (i = l; i <= r; i++)
       {
          swap((a+l), (a+i));
          permute(a, l+1, r, f);
          swap((a+l), (a+i));
       }
   }
}

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
	int i = 0, dim, j, x, y, z, id;
	char ch[MAX], aux[10];

	FILE *f = fopen("test.tsp", "r");
	FILE *cp = fopen("cache", "w+");

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
				int adj[dim][dim];
				int mp[dim];
				for(j=0;j<dim;j++){
					fscanf(f, "%d %d %d", &id, &x, &y);
					mp[j] = id;
					vtc[j][0] = id;
					vtc[j][1] = x;
					vtc[j][2] = y;
				}
				for(i=0;i<dim;i++){
					for(j=0;j<dim;j++){
						adj[i][j] = distancia2d(vtc[i][1], vtc[i][2], vtc[j][1], vtc[j][2]);
					}
				}
				for(j=0;j<dim;j++){
					printf("id=%d\n", vtc[j][0]);
					printf("x=%d ", vtc[j][1]);
					printf("y=%d\n", vtc[j][2]);
				}
				for(i=0;i<dim;i++){
					for(j=0;j<dim;j++){
						printf("%d ", adj[i][j]);
					}
					printf("\n");
				}
				permute(mp, 0, dim-1, cp);
				fprintf(cp, "EOF");
				fclose(cp);
				FILE *dists = fopen("cache", "r");
				menordistancia(dists, dim, adj);
			}

			if(ch[22] == '3'){ // Verifica se existe EUC_3D
				fgets(ch, MAX, f); // Pula linha
				int vtc[dim][4];
				for(j=0;j<dim;j++){
					fscanf(f, "%d %d %d %d", &id, &x, &y, &z);
					vtc[j][0] = id;
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
				fclose(f);
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