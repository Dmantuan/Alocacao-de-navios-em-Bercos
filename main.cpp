#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <memory.h>
#include <stdio.h>

#include "consts.h"

//#define MODO_TESTE

int main(int agrc, char** argv) 
{	
	#ifdef MODO_TESTE
		int k = 0;
		clock_t h;
		double tempo;
		ler_instancia("inst3.txt");
        //testar_instancia("");
        ordenarObjetos();
        
        Solucao s;
        
        h = clock();
        heuConGul(s);
        h = clock() - h;
    	tempo = (double)h/CLOCKS_PER_SEC;
    	printf("Construtiva Gulosa 1 vez: %lf segundos\n", tempo);
    	
    	h = clock();
    	while(k < 100000)
    	{
    		heuConGul(s);
    		k++;
		}
        h = clock() - h;
    	tempo = (double)h/CLOCKS_PER_SEC;
    	printf("Construtiva Gulosa 100000 vezes: %lf segundos\n", tempo);
    	
    	k = 0;
		
		h = clock();
        calcFO(s);
        h = clock() - h;
    	tempo = (double)h/CLOCKS_PER_SEC;
    	printf("Calculo de FO 1 vez: %lf segundos\n", tempo);
    	
    	h = clock();
    	while(k < 100000)
    	{
    		calcFO(s);
    		k++;
		}
        h = clock() - h;
    	tempo = (double)h/CLOCKS_PER_SEC;
    	printf("Calculo de FO 100000 vez: %lf segundos\n", tempo);
    	
        escreverSolucao(s, 0);
    #else
        ler_instancia("inst3.txt");
        ordenarObjetos();
        
        Solucao s;
        
        heuConGul(s);
		calcFO(s);
		escreverSolucao(s,0);
		
    #endif
    
    return 0;
}

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
	//Tamanho de cada navio
	for(int j = 0; j < numNav; j++)
		fprintf(f, "%d ", vetTamNav[j]);
	fprintf(f, "\n");
	//Tempo de chegada de cada navio
	for(int j = 0; j < numNav; j++)
		fprintf(f, "%d ", vetInNav[j]);
	fprintf(f, "\n");
	//Tempo limite para saida de cada navio
	for(int j = 0; j < numNav; j++)
		fprintf(f, "%d ", vetOutNav[j]);
	fprintf(f, "\n");
	//Abertura e fechamento dos bercos
	for(int i = 0; i < numBer; i++)
		fprintf(f, "%d %d\n", vetOpBer[i], vetClsBer[i]);
	fprintf(f, "\n");
	//Tamanho de berco
	for(int i = 0; i < numBer; i++)
		fprintf(f, "%d %d\n", matBercos[i][0], matBercos[i][1]);
	fprintf(f, "\n");
	//Tempo de atendimento de cada navio em cada berco
	for(int i = 0; i < numBer; i++)
	{
		for(int j = 0; j < numNav; j++)
			fprintf(f, "%d ", matTempNavBer[i][j]);
		fprintf(f, "\n");
	}
		
	if(arq != "")
		fclose(f);
}

void clonarSolucao(Solucao &s, Solucao &clone)
{
	memcpy(&clone, &s, sizeof(s));
}

void calcFO(Solucao &s)
{
	s.funObj = 0;
	for(int j = 0; j < numNav; j++) //calculo de FO
	{
		s.funObj += s.vetTemp[j] - vetInNav[j] + matTempNavBer[s.vetSol[j]][j];
		s.tempServ = s.funObj;
	}
	
	for(int i = 0; i < numBer; i++) //estourar tempo máximo do berço
	{
		if(vetTempAux[i] > vetClsBer[i])
			s.funObj += 1000;
	}
	
	for(int j = 0; j < numNav; j++)  
	{
		for(int i = 0; i < numBer; i++)
		{
			if(s.vetTemp[j] + matTempNavBer[j][i] > vetOutNav[j]) //estourar horario do navio sair do porto
				s.funObj += 1000;
				
			if(s.vetTemp[j] < vetInNav[j]) //navio ser atendido antes de chegar
				s.funObj += 1000;
		}
	}
}

void escreverSolucao(Solucao &s, const bool flag)
{
	FILE *f;
		
	if(flag)
		f = stdout;
	else
		f = fopen("Solucao.sol", "w");
	
	fprintf(f, "FO: %d\n", s.funObj);

	fprintf(f, "Tempo de servico: %d\n", s.tempServ);
	
	fprintf(f, "--------------------------\n");
	fprintf(f, "%-10s%-10s%-10s%-10s\n", "Navio", "Berco", "Tempo", "Posicao");
	for(int j = 0; j < numNav; j++)
	{
		fprintf(f, "%-10d%-10d%-10d%-10d\n", j+1, s.vetSol[j]+1, s.vetTemp[j], s.vetPos[j]);
	}
	
	if(!flag)
		fclose(f);
}

void heuConGul(Solucao &s){
	int index;
	int sizeShip;
	int arriveShip;
	int indexBerco = 0;
	int X=0;  //Vai servir para saber qual e o X
	int h=0; //Altura referente ao tempo no grafico
	int hProx=0; //Para guardar qualsera a proxima altura
	int alocado;
	int matBercoAux[MAX_BER][2];
	memcpy(&matBercoAux, &matBercos, sizeof(matBercos));

	/*
		-Forma saber a posicao (X) do navio
			A cada um dos casos de iteracao de colocar o navio no berco ir incrementando o X
		-Forma para saber o tempo (Y) do navio
			Buscar o mais o mais alto e colocar todo mundo nessa 'linha' se possivel
	*/

	//Pegando o tempo do berco que abre por ultimo
	for(int j = 0; j < numBer; j++){
		if(vetOpBer[j]>h){
			h=vetOpBer[j];
		}
	}
	hProx=h;

	for(int i =0; i < numNav; i++){ //Percorrer o vetIndNavOrd
		//Pegando o navio em questao
		index = vetIndNavOrd[i];
		sizeShip = vetTamNav[index];
		arriveShip = vetInNav[index];
		alocado=0;

		for(indexBerco; indexBerco < numBer; indexBerco++){ //Percorrer os bercos
			if(matTempNavBer[indexBerco][index]!=0){ //Neste caso o navio pode ser atendido nesse berco
				if(matBercoAux[indexBerco][0]==0){  //caso inicial
					if(matBercoAux[indexBerco][1]+matBercoAux[indexBerco+1][0]>=sizeShip){
						if(matBercoAux[indexBerco][1]>=sizeShip){
							matBercoAux[indexBerco][1] = matBercoAux[indexBerco][1]-sizeShip;
							
							/*Colocando o navio na solucao*/
							s.vetPos[index]=X;
							s.vetTemp[index]=h;
							s.vetSol[index]=indexBerco;
							
							//Atualizando as variaves de controle X e h
							X = X + sizeShip+ matBercoAux[indexBerco][0];
							if(hProx < h+matTempNavBer[indexBerco][index]){
								hProx = h+matTempNavBer[indexBerco][index];
							}
							indexBerco++;
							break;
						}
						else{
							matBercoAux[indexBerco+1][0] = (matBercoAux[indexBerco][1]+matBercoAux[indexBerco+1][0])-sizeShip;
							matBercoAux[indexBerco][1] = 0;
							
							/*Colocando o navio na solucao*/
							s.vetPos[index]=X;
							s.vetTemp[index]=h;
							s.vetSol[index]=indexBerco;
							
							//Atualizando as variaves de controle X e h
							X = X + sizeShip+ matBercoAux[indexBerco][0];
							if(hProx < h+matTempNavBer[indexBerco][index]){
								hProx = h+matTempNavBer[indexBerco][index];
							}
							indexBerco++;
							break;
						}
					}
				}
				else{
					if(matBercoAux[indexBerco][1]==0){  //Ultimo caso
						if(matBercoAux[indexBerco][0]+matBercoAux[indexBerco-1][1]>=sizeShip){
							matBercoAux[indexBerco][0]= matBercoAux[indexBerco][0]-sizeShip;
							/*Colocando o navio na solucao*/
							s.vetPos[index]=X;
							s.vetTemp[index]=h;
							s.vetSol[index]=indexBerco;
							
							//Atualizando as variaves de controle X e h
							if(hProx < h+matTempNavBer[indexBerco][index]){
								hProx = h+matTempNavBer[indexBerco][index];
							}
							X = X + sizeShip+ matBercoAux[indexBerco][0];
							alocado=1;
						}
						else{
							i--;
						}
						if(indexBerco == numBer-1){ //Siginifica que e o oultimo berco
							indexBerco = 0;
							X = 0;
							h=hProx;
							memcpy(&matBercoAux, &matBercos, sizeof(matBercos));
						}
						else{
							indexBerco++;
							X = X + matBercoAux[indexBerco-1][0] +matBercoAux[indexBerco-2][1];
						}

						break;
					}
					else{ // Caso normal
						if(matBercoAux[indexBerco-1][1]+matBercoAux[indexBerco][0]+matBercoAux[indexBerco][1]+matBercoAux[indexBerco+1][0]>=sizeShip){
							if(matBercoAux[indexBerco-1][1]>=sizeShip){
								matBercoAux[indexBerco-1][1] = (matBercoAux[indexBerco-1][1])-sizeShip;
								
								/*Colocando o navio na solucao*/
								s.vetPos[index]=X;
								s.vetTemp[index]=h;
								s.vetSol[index]=indexBerco;
								
								//Atualizando as variaves de controle X e h
								if(hProx < h+matTempNavBer[indexBerco][index]){
									hProx = h+matTempNavBer[indexBerco][index];
								}
								X = X + sizeShip+ matBercoAux[indexBerco][0];
								indexBerco++;
								break;
							}
							else{
								if(matBercoAux[indexBerco-1][1]+matBercoAux[indexBerco][0]>=sizeShip){
									matBercoAux[indexBerco][0] = (matBercoAux[indexBerco][0]+matBercoAux[indexBerco-1][1])-sizeShip;
									matBercoAux[indexBerco-1][1] = 0;
									
									/*Colocando o navio na solucao*/
									s.vetPos[index]=X;
									s.vetTemp[index]=h;
									s.vetSol[index]=indexBerco;
									
									//Atualizando as variaves de controle X e h
									if(hProx < h+matTempNavBer[indexBerco][index]){
										hProx = h+matTempNavBer[indexBerco][index];
									}
									X = X + sizeShip+ matBercoAux[indexBerco][0];
									indexBerco++;
									break;
								}
								else{
									if(matBercoAux[indexBerco-1][1]+matBercoAux[indexBerco][0]+matBercoAux[indexBerco][1]>=sizeShip){
										matBercoAux[indexBerco][1] = (matBercoAux[indexBerco-1][1]+matBercoAux[indexBerco][0]+matBercoAux[indexBerco][1])-sizeShip;
										matBercoAux[indexBerco-1][1] = 0;
										matBercoAux[indexBerco][0] = 0;
										
										/*Colocando o navio na solucao*/
										s.vetPos[index]=X;
										s.vetTemp[index]=h;
										s.vetSol[index]=indexBerco;

										//Atualizando as variaves de controle X e h
										X = X + sizeShip+ matBercoAux[indexBerco][0];
										if(hProx < h+matTempNavBer[indexBerco][index]){
											hProx = h+matTempNavBer[indexBerco][index];
										}
										indexBerco++;
										break;
									}
									else{
										if(matBercoAux[indexBerco-1][1]+matBercoAux[indexBerco][0]+matBercoAux[indexBerco][1]+matBercoAux[indexBerco+1][0]>=sizeShip){
											matBercoAux[indexBerco+1][0] = (matBercoAux[indexBerco-1][1]+matBercoAux[indexBerco][0]+matBercoAux[indexBerco][1]+matBercoAux[indexBerco+1][0])-sizeShip;
											matBercoAux[indexBerco-1][1] = 0;
											matBercoAux[indexBerco][1] = 0;
											matBercoAux[indexBerco][0] = 0;
											
											/*Colocando o navio na solucao*/
											s.vetPos[index]=X;
											s.vetTemp[index]=h;
											s.vetSol[index]=indexBerco;
										
											//Atualizando as variaves de controle X e h
											if(hProx < h+matTempNavBer[indexBerco][index]){
												hProx = h+matTempNavBer[indexBerco][index];
											}
											X = X + sizeShip+ matBercoAux[indexBerco][0];
											indexBerco++;
											break;
										}
									}
								}
							}
						}
					}
				}
			}
			// Para os casos onde o navio nao pode ser atendido
			if(matBercoAux[indexBerco][0]!=0){ 
				if(indexBerco == numBer-1){ //Caso final
					X = 0;
					indexBerco = -1;
					h=hProx;
					memcpy(&matBercoAux, &matBercos, sizeof(matBercos));
				}
				else{
					X = X + matBercoAux[indexBerco-1][1] + matBercoAux[indexBerco][0];
				}
			}
		}
	}
}

void lerSolucao(char *arq, Solucao &s)
{
	int auxNav, auxBer, aux = 0;
	
	FILE *f = fopen(arq, "r");
	fscanf(f, "FO: %d\n", &s.funObj); //FO
	fscanf(f, "Tempo de servico: %d\n", &s.tempServ); //Tempo de servi�o
	fscanf(f, "--------------------------\n");
	fscanf(f, "Navio Berco Tempo Posicao\n");
	while(!feof(f)) //tabela de informa��es
	{
		fscanf(f, "%d ", &auxNav);
		fscanf(f, "%d ", &auxBer);
		fscanf(f, "%d ", &s.vetTemp[aux]);
		fscanf(f, "%d ", &s.vetPos[aux]);
		aux++;
		s.vetSol[auxNav - 1] = auxBer - 1;
	}
	fclose(f);
}
