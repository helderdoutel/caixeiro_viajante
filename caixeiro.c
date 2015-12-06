#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 300

// Escreve arquivo de saida
void escrevearquivo(FILE *f, char *nome, int dim, float md, int *vtc){
	int x=0;
	fprintf(f, "NAME: %s\n", nome);
	fprintf(f, "COMMENT: solucao obtida para %s\n", nome);
	fprintf(f, "TYPE: TSP\n");
	fprintf(f, "NAME: %d\n", dim);
	fprintf(f, "TOTAL_WEIGHT: %f\n", md);
	fprintf(f, "TOUR_SECTION\n");
	for(;x<dim;x++){
		fprintf(f, "%d\n", vtc[x]); // escreve vertices do ciclo mais curto
	}
	fprintf(f, "EOF");
}

// Calcula distancia da permutacao a partir de um arquivo temporario, grava distancia minima e retorna 
float menordistancia(FILE *f, int dim, float adj[][dim], int *menor){
	char ch[MAX] = "INICIO";
	int x, j, i;
	int ciclo[dim];
	float dist, menordist = -1.0;
	int conta = 0;
	// loop que so e interrompido ao achar o fim do arquivo
	do{
		dist = 0;
		fgets(ch, MAX, f);
		// adiciona valores da linha no vetor
		for(x=0;x<dim;x++){
			fscanf(f, "%d", &ciclo[x]);
			conta++;
		}
		// calcula distancia do ciclo a partir de uma partir de distancias de uma matriz vertices x vertices pre calculada
		// antes da permutacao
		for(x=0;x<dim;x++){
			if(x<dim-1){
				dist = dist + adj[(ciclo[x]-1)][(ciclo[x+1]-1)];
			}
			else{
				dist = dist + adj[(ciclo[x]-1)][(ciclo[0]-1)];
			}
		}
		//salva distancia do primeiro ciclo
		if(menordist == -1.0){
			menordist = dist;
			for(x=0;x<dim;x++){
				menor[x] = ciclo[x];
			}
		}
		// verifica se novo ciclo tem distancia menor
		if(menordist > dist){
			menordist = dist;
			for(x=0;x<dim;x++){
				menor[x] = ciclo[x];
			}
		}
	}while(strcmp(ch, "EOF"));

	// escreve na tela menor distancia encontrada
	printf("menor distancia = %f\n", menordist);

	// escreve caminho percorrido no ciclo
	for(x=0;x<dim;x++){
		printf("%d ", menor[x]);
	}
	
   	printf("\n");
   	return menordist;
}

// inverte valores no vetor que esta sendo permutado
void swap(int *x, int *y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}
 

// permuta os valores de forma recursiva
// checa tempo passado por parametro para parada
void permute(clock_t c1, int *a, int l, int r, FILE *f, int t)
{
	clock_t c2;
	float tempo;
	c2 = clock();
	tempo = (c2 - c1)/ CLOCKS_PER_SEC; //s
    int i, x=0;
    // condicao de parada tempo ou fim da permutacao
    if (l == r || tempo > t){
    	// escreve permutacao em um arquivo
   		for(x=0;x<=r;x++){
   			// printf("%d ", a[x]);
   			fprintf(f, "%d ", a[x]);
   		}
   		if(tempo > t){
	   		printf("tempo limite determinado estourado\n");
	   	}
	   	fprintf(f, "\n");
    }
    // recursao - inverte - permuta - inverte
    else
    {
       	for (i = l; i <= r; i++)
       	{
          	swap((a+l), (a+i));
          	permute(c1, a, l+1, r, f, t);
        	swap((a+l), (a+i));
        }
    }
}

// calcula distancia 2d
float distancia2d(int xi, int yi, int xj, int yj){
	float distemp, distancia;
	distemp = ((xi-xj)*(xi-xj)) + ((yi - yj)*(yi - yj));
	distancia = sqrt(distemp);
	return distancia;
}

// calcula distancia 3d
float distancia3d(int xi, int yi, int zi, int xj, int yj, int zj){
	float distemp, distancia;
	distemp = ((xi-xj)*(xi-xj)) + ((yi - yj)*(yi - yj)) + ((zi - zj)*(zi - zj));
	distancia = sqrt(distemp);
	return distancia;
}

// main
int main(int argc, char* argv[]){
	int i = 0, dim, j, k, x, y, z, id;
	char ch[MAX], aux[10], nome[MAX];
	clock_t c1, c2;
	float tempo;
	float md;
	int tempo_finaliza;

	// carrega arquivo e abre arquivo para escrever
	FILE *f = fopen(argv[2], "r");
	FILE *cp = fopen("cache", "w+");
	FILE *n = fopen(argv[3], "w+");

	// iniciar clock
	c1 = clock();
	//converte argumento para inteiro
	tempo_finaliza = atoi(argv[1]);

	// verifica se todos argumentos foram passados na inicializacao
	if(!argv[3]){
		printf("insira o nome do arquivo de saida\nexemplo: ./caixeiro 100 exemplo.tsp saida.tour.tsp");
		return 0;
	}

	// loop ate achar o fim do arquivo
	for(i=0;strcmp(ch, "EOF") != 0;i++){
		if(i == 3){
			fscanf(f, "%s", aux); // Consome "DIMENSION:"
			fscanf(f, "%d", &dim); // Pega dimensao do grafo
			printf("%s %d\n",aux, dim);
		}
		if(i > 3){
			fgets(ch, MAX, f); // consome \n
			fgets(ch, MAX, f); // consome tipo arquivo
			if(ch[22] == '2'){ // Verifica se arquivo e 2d
				fgets(ch, MAX, f); // iniciar variaveis em funcao dos parametros carregados
				int vtc[dim][3];
				float adj[dim][dim];
				int mp[dim];
				// salva coordenadas em uma matriz
				for(j=0;j<dim;j++){
					fscanf(f, "%d %d %d", &id, &x, &y);
					mp[j] = id;
					vtc[j][0] = id;
					vtc[j][1] = x;
					vtc[j][2] = y;
				}
				// calcula matriz de distancias
				for(i=0;i<dim;i++){
					for(j=0;j<dim;j++){
						adj[i][j] = distancia2d(vtc[i][1], vtc[i][2], vtc[j][1], vtc[j][2]);
					}
				}
				// permuta valores do vetor de referencia (ids)
				permute(c1, mp, 0, dim-1, cp, tempo_finaliza);
				// escreve fim do arquivo de permutacao
				fprintf(cp, "EOF");
				fclose(cp);
				// carrega arquivo de permutacao em modo de leitura
				FILE *dists = fopen("cache", "r");
				// iniciar vetor que guardara menor ciclo
				int menor[dim];
				// passa arquivo, dimensao, matriz de distancias e vetor onde sera guardado o resultado, funcao retorna tbm a distancia
				md = menordistancia(dists, dim, adj, menor);
				// finaliza tempo apos todos calculos
				c2 = clock();
				// calcula tempo
				tempo = (c2 - c1)/ CLOCKS_PER_SEC; //s
				printf("tempo = %f\n", tempo);
				// escreve arquivo de saida, passa arquivo de saida, nome, dimensao, tempo final e vetor do menor ciclo
				escrevearquivo(n, nome, dim, md, menor);
				fclose(n);	
			}
			// mesmo procedimento com 3 coordenadas
			if(ch[22] == '3'){ // Verifica se existe EUC_3D
				fgets(ch, MAX, f); // Pula linha
				int vtc[dim][4]; // inicia variaveis em funcao dos parematros carregados
				float adj[dim][dim];
				int mp[dim];
				// salva vetor de referencia de ids para permutar e matriz de coordenadas para calculo de distancia
				for(j=0;j<dim;j++){
					fscanf(f, "%d %d %d %d", &id, &x, &y, &z);
					mp[j] = id;
					vtc[j][0] = id;
					vtc[j][1] = x;
					vtc[j][2] = y;
					vtc[j][3] = z;
				}
				// calcula distancia
				for(i=0;i<dim;i++){
					for(j=0;j<dim;j++){
						adj[i][j] = distancia3d(vtc[i][1], vtc[i][2], vtc[i][3], vtc[j][1], vtc[j][2], vtc[j][3]);
					}
				}
				// permuta valores em arquivo temporario
				permute(c1, mp, 0, dim-1, cp, tempo_finaliza);
				fprintf(cp, "EOF");
				fclose(cp);
				// abre arquivo em mode de leitura
				FILE *dists = fopen("cache", "r");
				int menor[dim];
				// acha menor distancia e salva menor ciclo em um vetor
				md = menordistancia(dists, dim, adj, menor);
				// finaliza tempo
				c2 = clock();
				// calcula tempo
				tempo = (c2 - c1)/ CLOCKS_PER_SEC; //s
				printf("tempo = %f\n", tempo);
				// escreve no arquivo
				escrevearquivo(n, nome, dim, tempo, menor);
				fclose(n);	

			}
			// se achar fim do arquivo finaliza loop
			if(strcmp(ch, "EOF")){
				printf("acabou\n");
				printf("Arquivo de solucao %s gravado no diretorio\n", argv[3]);
				fclose(f);
				break;
			}
		}
		// salva nome do arquivo e consome linhas
		if(i < 3){
			if(i==0){
				fscanf(f, "%s", ch);
				fscanf(f, "%s", nome);
				fgets(ch, MAX, f);
			}
			else{
				fgets(ch, MAX, f); 
			}
		}
	}

	return 0;
}