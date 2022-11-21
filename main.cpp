#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

#include "consts.h"

//#define MODO_TESTE

int main(int agrc, char** argv) 
{
	srand(time(NULL));
	
	#ifdef MODO_TESTE

    #else
        ler_instancia("inst1.txt");
        testar_instancia("");
        ordenarObjetos();
        
        Solucao s;
        
        //heuConGul(s);
        //heuConAle(s);
        	
    #endif
    
    return 0;
}


/*
void heuConAle(Solucao &s)
{
	for(int j = 0; j < numNav; j++)
	{
		s.vetSol[j] = (rand()%(numBer));
		s.vetPos[j] = ;
		s.vetTemp[j] = ;
	}
}
*/

void ordenarObjetos()
{
    int flag, aux;
    for(int j = 0; j < numNav; j++)
        vetIndNavOrd[j] = j;
    flag = 1;
    while(flag)
    {
        flag = 0;
        for(int j = 0; j < numNav - 1; j++)
        {
            if(vetInNav[vetIndNavOrd[j]] > vetInNav[vetIndNavOrd[j+1]])
            {
                flag = 1;
                aux = vetIndNavOrd[j];
                vetIndNavOrd[j] = vetIndNavOrd[j+1];
                vetIndNavOrd[j+1] = aux;
            }
        }
    }
}

void ler_instancia(char *arq)
{
	FILE *f = fopen(arq, "r");
	//numero de navios e ber�os
	fscanf(f, "%d %d", &numNav, &numBer);
	//tamanho de cada navio
	for(int j = 0; j < numNav; j++)
		fscanf(f, "%d", &vetTamNav[j]);
	//entrada de cada navio
	for(int j = 0; j < numNav; j++)
		fscanf(f, "%d", &vetInNav[j]);
	//saida de cada navio
	for(int j = 0; j < numNav; j++)
		fscanf(f, "%d", &vetOutNav[j]);
	//abertura e fechamento de cada berco
	for(int i = 0; i < numBer; i++)
		fscanf(f, "%d %d", &vetOpBer[i], &vetClsBer[i]);
	//tamanho do vizinho esquerdo e direito de cada berco
	for(int i = 0; i < numBer; i++)
	{
		fscanf(f, "%d %d", &matBercos[i][0], &matBercos[i][1]);
	}
	//tempo de atendimento de cada navio por berco
	for(int i = 0; i < numBer; i++)
		for(int j = 0; j < numNav; j++)
			fscanf(f, "%d", &matTempNavBer[i][j]);
	fclose(f);
}

void testar_instancia(char *arq)
{
	FILE *f;
	if(arq == "")
		f = stdout;
	else
		f = fopen(arq, "w");
	fprintf(f, "%d %d\n", numNav, numBer);
	for(int j = 0; j < numNav; j++)
		fprintf(f, "%d ", vetTamNav[j]);
	fprintf(f, "\n");
	for(int j = 0; j < numNav; j++)
		fprintf(f, "%d ", vetInNav[j]);
	fprintf(f, "\n");
	for(int j = 0; j < numNav; j++)
		fprintf(f, "%d ", vetOutNav[j]);
	fprintf(f, "\n");
	for(int i = 0; i < numBer; i++)
		fprintf(f, "%d %d\n", vetOpBer[i], vetClsBer[i]);
	fprintf(f, "\n");
	for(int i = 0; i < numBer; i++)
		fprintf(f, "%d %d\n", matBercos[i][0], matBercos[i][1]);
	fprintf(f, "\n");
	for(int i = 0; i < numBer; i++)
	{
		for(int j = 0; j < numNav; j++)
			fprintf(f, "%d ", matTempNavBer[i][j]);
		fprintf(f, "\n");
	}
		
	if(arq != "")
		fclose(f);
}

/*
void heuConGul(Solucao &s)
{
	memset(&s.vetTemp, 0, sizeof(s.vetTemp));
	memset(&s.vetPos, 0, sizeof(s.vetPos));
	memset(&s.vetSol, -1, sizeof(s.vetSol));
	for(int j = 0; j < numNav; j++)
		for(int i = 0; i < numBer; i++)
			if(i == 0)
			{
				if(vetTamBer[i] + vetEsqBer[i+1] >= vetTamNav[vetIndNavOrd[i]])
				{
					s.vetSol[vetIndNavOrd[j]] = i;
					s.vetTemp[vetIndNavOrd[j]] = vetOpBer[i];
					s.vetPos[vetIndNavOrd[j]] = 0;
					break;
				}
			}
			else {
				if(i == (numBer - 1))
				{
					if(vetDirBer[i-1] + vetTamBer[i] + vetEsqBer[i+1] >= vetTamNav[vetIndNavOrd[i]])
					{
						break;
					}
				}
				else {
					if(vetDirBer[i-1] + vetTamBer[i] >= vetTamNav[vetIndNavOrd[i]])
					{
						break;
					}
				}
			}
			
			if(s.vetPesMoc[i] + vetPesObj[vetIndObjOrd[j]] <= vetCapMoc[i])
			{
				s.vetSol[vetIndObjOrd[j]] = i;
				s.vetPesMoc[i] += vetPesObj[vetIndObjOrd[j]];
				break;
			}
}
*/

void calcFO(Solucao &s) //Tem um erro vetTemp[j] nao esta definido
{
	s.funObj = 0;
	for(int j = 0; j < numNav; j++)
	{
		//s.funObj += vetTemp[j] - vetInNav[j] + matTempNavBer[s.vetSol[j]][j]; //Tempo de servi�o
	}
}

void escreverSolucao(Solucao &s, const bool flag)
{
	int tmpServ = 0;
	
	printf("FO: %d\n", s.funObj);
	for(int j = 0; j < numNav; j++)
	{
		tmpServ += matTempNavBer[s.vetSol[j]][j];
	}
	printf("Tempo de Srevi�o: %d\n", tmpServ);
	printf("-------------------------------\n");
	if(flag)
	{
		printf("%-10s%-10s%-10s%-10s\n", "Navio", "Ber�o", "Tempo", "Posi��o");
		for(int j = 0; j < numNav; j++)
		{
			printf("%-10d%-10d%-10d%-10d\n", j+1, s.vetSol[j], s.vetTemp[j], s.vetPos[j]);
		}
	}
}

void buildingSolution(Solucao s){
	int index;
	int sizeShip;
	int arriveShip;
	int aux=0;  //Vai servir para saber qual e o X
	int sobra;  //Saber quanto  sobrou do berco anterior
	int totalDoBerco;

	int matBercoAux[MAX_BER][2];
	memcpy(&matBercoAux, &matBercos, sizeof(matBercos));

	for(int i =0; i < numNav; i++){ //Percorrer o vetIndNavOrd
		//Pegando o navio em questao
		index = vetIndNavOrd[i];
		sizeShip = vetTamNav[index];
		arriveShip = vetInNav[index];

		/*
			TODO: 
				-Forma saber a posicao (X) do navio
				-Forma para saber o tempo (Y) do navio
		*/

		for(int indexBerco = 0; indexBerco < numBer; indexBerco++){ //Percorrer os bercos
			if(matTempNavBer[indexBerco][index]!=0){ //Neste caso o navio pode ser atendido nesse berco
				if(matBercoAux[indexBerco][0]==0){  //caso inicial
					if(matBercoAux[indexBerco][1]+matBercoAux[indexBerco+1][0]>=sizeShip){
						if(matBercoAux[indexBerco][1]>=sizeShip){
							matBercoAux[indexBerco][1] = matBercoAux[indexBerco][1]-sizeShip;
							/*
								TODO: colocar o navio na solucao
							*/
							break;
						}
						else{
							matBercoAux[indexBerco+1][0] = (matBercoAux[indexBerco][1]+matBercoAux[indexBerco+1][0])-sizeShip;
							matBercoAux[indexBerco][1] = 0;
							/*
								TODO: colocar o navio na solucao
							*/
							break;
						}
					}
				}
				else{
					if(matBercoAux[indexBerco][1]==0){  //Ultimo caso
						if(matBercoAux[indexBerco][0]+matBercoAux[indexBerco-1][1]>=sizeShip){
							/*
								TODO: colocar o navio na solucao
							*/
						}
						if(indexBerco == numBer-1){
							indexBerco = -1;
							memcpy(&matBercoAux, &matBercos, sizeof(matBercos));
						}
						break;
					}
					else{ // Caso normal
						if(matBercoAux[indexBerco-1][1]+matBercoAux[indexBerco][0]+matBercoAux[indexBerco][1]+matBercoAux[indexBerco+1][0]>=sizeShip){
							if(matBercoAux[indexBerco-1][1]>=sizeShip){
								matBercoAux[indexBerco-1][1] = (matBercoAux[indexBerco-1][1])-sizeShip;
								/*
									TODO: colocar o navio na solucao
								*/
								break;
							}
							else{
								if(matBercoAux[indexBerco-1][1]+matBercoAux[indexBerco][0]>=sizeShip){
									matBercoAux[indexBerco][0] = (matBercoAux[indexBerco][0]+matBercoAux[indexBerco-1][1])-sizeShip;
									matBercoAux[indexBerco-1][1] = 0;
									/*
										TODO: colocar o navio na solucao
									*/
									break;
								}
								else{
									if(matBercoAux[indexBerco-1][1]+matBercoAux[indexBerco][0]+matBercoAux[indexBerco][1]>=sizeShip){
										matBercoAux[indexBerco][1] = (matBercoAux[indexBerco-1][1]+matBercoAux[indexBerco][0]+matBercoAux[indexBerco][1])-sizeShip;
										matBercoAux[indexBerco-1][1] = 0;
										matBercoAux[indexBerco][0] = 0;
										/*
											TODO: colocar o navio na solucao
										*/
										break;
									}
									else{
										if(matBercoAux[indexBerco-1][1]+matBercoAux[indexBerco][0]+matBercoAux[indexBerco][1]+matBercoAux[indexBerco+1][0]>=sizeShip){
											matBercoAux[indexBerco+1][0] = (matBercoAux[indexBerco-1][1]+matBercoAux[indexBerco][0]+matBercoAux[indexBerco][1]+matBercoAux[indexBerco+1][0])-sizeShip;
											matBercoAux[indexBerco-1][1] = 0;
											matBercoAux[indexBerco][1] = 0;
											matBercoAux[indexBerco][0] = 0;
											/*
												TODO: colocar o navio na solucao
											*/
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}