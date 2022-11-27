#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <memory.h>

#include "pmm.h"

#define MAX(X,Y) ((X > Y) ? X : Y)


#define MODO_TESTE  // executa apenas testes de desempenho

const int PESO = 100;

int main(int argc, char** argv)
{
	//srand(time(NULL));


    #ifdef MODO_TESTE
        //testar_alocacao();
        //testar_estruturas();
        //testar_heuConstrutivas();
        testar_buscaLocal();
    #else
        ler_instancia("pmm1.txt");
        testar_instancia("");




    #endif

	return 0;
}

void heuBLMM(Solucao &s)
{
    int mocOri, melObj, melMoc, flag;
    int melFO = s.funObj;
    while(true)
    {
        flag = 0;
        for(int j = 0; j < numObj; j++)
        {
            mocOri = s.vetSol[j];
            for(int i = -1; i < numMoc; i++)
            {
                if(i != mocOri)
                {
                    s.vetSol[j] = i;
                    calcFO(s);
                    if(s.funObj > melFO)
                    {
                        melFO = s.funObj;
                        melObj = j;
                        melMoc = i;
                        flag = 1;
                    }
                }
            }
            s.vetSol[j] = mocOri;
        }
        if(flag)
        {
            s.vetSol[melObj] = melMoc;
            s.funObj = melFO;
        }
        else
            break;
    }
    calcFO(s);
}

void heuBLPM(Solucao &s)
{
	int vetObjAux[MAX_OBJ]; // usado para evitar determinismo na ordem de teste dos objetos
    int mocOri, foOri, indice, aux;
    int melFO = s.funObj;
    for(int j = 0; j < numObj; j++)
        vetObjAux[j] = j;
    INICIO : ;
    foOri = s.funObj;
    for(int j = 0; j < numObj; j++)
    {
        indice = j + rand()%(numObj - j);
        mocOri = s.vetSol[vetObjAux[indice]];
        for(int i = -1; i < numMoc; i++)
        {
            if(i != mocOri)
            {
                s.vetSol[vetObjAux[indice]] = i;
                calcFO(s);
                if(s.funObj > melFO)
                {
                    melFO = s.funObj;
                    goto INICIO;
                }
                else
                {
                    s.vetSol[vetObjAux[indice]] = mocOri;
                    s.funObj = foOri;
                }
            }
        }
        aux = vetObjAux[j];
        vetObjAux[j] = vetObjAux[indice];
        vetObjAux[indice] = aux;
    }
    calcFO(s);
}

void heuBLRA(Solucao &s, const int iteracoes)
{
	int obj, moc, mocOri, foOri, flag;
    int melFO = s.funObj;
    while(true)
    {
        flag = 1;
        for(int t = 0; t < iteracoes; t++)
        {
            foOri = s.funObj;
            obj = rand()%numObj;
            do
                moc = rand()%(numMoc + 1) - 1;
            while(moc == s.vetSol[obj]);
            mocOri = s.vetSol[obj];
            s.vetSol[obj] = moc;
            calcFO(s);
            if(s.funObj > melFO)
            {
                melFO = s.funObj;
                flag = 0;
            }
            else
            {
                s.vetSol[obj] = mocOri;
                s.funObj = foOri;
            }
        }
        if(flag)
            break;
    }
    calcFO(s);
}

void heuConAleGul(Solucao &s, const int percentual)
{
	int qtd, pos, aux;
	int vetAux[MAX_OBJ];
	memcpy(&vetAux, &vetIndObjOrd, sizeof(vetIndObjOrd));
	qtd = MAX(1, (percentual / 100.0) * numObj);
	for(int j = 0; j < qtd; j++)
	{
		pos = j + rand()%(numObj-j);
		aux = vetAux[pos];
		vetAux[pos] = vetAux[j];
		vetAux[j] = aux;
	}
	memset(&s.vetPesMoc, 0, sizeof(s.vetPesMoc));
	memset(&s.vetSol, -1, sizeof(s.vetSol));
	for(int j = 0; j < numObj; j++)
		for(int i = 0; i < numMoc; i++)
			if(s.vetPesMoc[i] + vetPesObj[vetAux[j]] <= vetCapMoc[i])
			{
				s.vetSol[vetAux[j]] = i;
				s.vetPesMoc[i] += vetPesObj[vetAux[j]];
				break;
			}
}

void heuConGul(Solucao &s)
{
	memset(&s.vetPesMoc, 0, sizeof(s.vetPesMoc));
	memset(&s.vetSol, -1, sizeof(s.vetSol));
	for(int j = 0; j < numObj; j++)
		for(int i = 0; i < numMoc; i++)
			if(s.vetPesMoc[i] + vetPesObj[vetIndObjOrd[j]] <= vetCapMoc[i])
			{
				s.vetSol[vetIndObjOrd[j]] = i;
				s.vetPesMoc[i] += vetPesObj[vetIndObjOrd[j]];
				break;
			}
}

void heuConAle(Solucao &s)
{
	for(int j = 0; j < numObj; j++)
		s.vetSol[j] = (rand()%(numMoc+1)) - 1;
}

void calcFO(Solucao &s)
{
	memset(&s.vetPesMoc, 0, sizeof(s.vetPesMoc));
	s.funObj = 0;
	for(int j = 0; j < numObj; j++)
		if(s.vetSol[j] != -1)
		{
			s.funObj += vetValObj[j];
			s.vetPesMoc[s.vetSol[j]] += vetPesObj[j];
		}
	for(int i = 0; i < numMoc; i++)
		s.funObj -= PESO * MAX(0, s.vetPesMoc[i] - vetCapMoc[i]);
}

void escreverSolucao(Solucao &s, const bool flag)
{
	printf("FO: %d\n", s.funObj);
	if(flag)
	{
		printf("PES:\n");
		for(int i = 0; i < numMoc; i++)
			printf("%d  ", s.vetPesMoc[i]);
		printf("\nVETSOL:\n");
		for(int j = 0; j < numObj; j++)
			printf("%d  ", s.vetSol[j]);
	}
}

void ordenarObjetos()
{
    int flag, aux;
    for(int j = 0; j < numObj; j++)
        vetIndObjOrd[j] = j;
    flag = 1;
    while(flag)
    {
        flag = 0;
        for(int j = 0; j < numObj - 1; j++)
        {
            if((double)vetValObj[vetIndObjOrd[j]]/vetPesObj[vetIndObjOrd[j]] <
               (double)vetValObj[vetIndObjOrd[j+1]]/vetPesObj[vetIndObjOrd[j+1]])
            {
                flag = 1;
                aux = vetIndObjOrd[j];
                vetIndObjOrd[j] = vetIndObjOrd[j+1];
                vetIndObjOrd[j+1] = aux;
            }
        }
    }
}

void ler_instancia(char *arq)
{
	FILE *f = fopen(arq, "r");
	fscanf(f, "%d %d", &numObj, &numMoc);
	for(int j = 0; j < numObj; j++)
		fscanf(f, "%d", &vetValObj[j]);
	for(int j = 0; j < numObj; j++)
		fscanf(f, "%d", &vetPesObj[j]);
	for(int i = 0; i < numMoc; i++)
		fscanf(f, "%d", &vetCapMoc[i]);
	fclose(f);
}

void testar_instancia(char *arq)
{
	FILE *f;
	if(arq == "")
		f = stdout;
	else
		f = fopen(arq, "w");
	fprintf(f, "%d %d\n", numObj, numMoc);
	for(int j = 0; j < numObj; j++)
		fprintf(f, "%d ", vetValObj[j]);
	fprintf(f, "\n");
	for(int j = 0; j < numObj; j++)
		fprintf(f, "%d ", vetPesObj[j]);
	fprintf(f, "\n");
	for(int i = 0; i < numMoc; i++)
		fprintf(f, "%d ", vetCapMoc[i]);
	if(arq != "")
		fclose(f);
}


//======== ESTRUTURAS E METODOS AUXILIARES ========

const int PESO2 = 50; // peso usado para objetos levados em mais de uma mochila

void testar_buscaLocal()
{
    Solucao sol;
    clock_t h;
    double tempo;

    ler_instancia("pmm3.txt");
    ordenarObjetos();

    printf("\n\n>>> TESTE - BUSCA LOCAL - PMM3\n\n");

    heuConAle(sol);
    calcFO(sol);
    printf("Solucao inicial:");
    escreverSolucao(sol, 0);

    Solucao sol2;
    memcpy(&sol2, &sol, sizeof(sol));

    Solucao sol3;
    memcpy(&sol3, &sol, sizeof(sol));

    h = clock();
    heuBLMM(sol2);
    h = clock() - h;
    tempo = (double)h/CLOCKS_PER_SEC;
    escreverSolucao(sol2, 0);
    printf("BL MM...: %.5f seg.\n",tempo);

    h = clock();
    heuBLPM(sol);
    h = clock() - h;
    tempo = (double)h/CLOCKS_PER_SEC;
    escreverSolucao(sol, 0);
    printf("BL PM...: %.5f seg.\n",tempo);
    
    h = clock();
    heuBLRA(sol3, (numObj * (numMoc + 1)));
    h = clock() - h;
    tempo = (double)h/CLOCKS_PER_SEC;
    escreverSolucao(sol3, 0);
    printf("BL RA...: %.5f seg.\n",tempo);
}

void testar_heuConstrutivas()
{
    Solucao sol;
    clock_t h;
    double tempo;
    const int repeticoes = 10000;

    ler_instancia("pmm3.txt");
    ordenarObjetos();

    printf("\n\n>>> TESTE - HEURISTICAS CONSTRUTIVAS - PMM3 - %d REPETICOES\n", repeticoes);

    //---
    h = clock();
    for(int r = 0; r < repeticoes; r++)
        heuConAle(sol);
    calcFO(sol);
    h = clock() - h;
    tempo = (double)h/CLOCKS_PER_SEC;
    escreverSolucao(sol, 0);
    printf("Construtiva Aleatoria...: %.5f seg.\n",tempo);

    //---
    h = clock();
    for(int r = 0; r < repeticoes; r++)
        heuConGul(sol);
    calcFO(sol);
    h = clock() - h;
    tempo = (double)h/CLOCKS_PER_SEC;
    escreverSolucao(sol, 0);
    printf("Construtiva Gulosa...: %.5f seg.\n",tempo);

    //---
    h = clock();
    for(int r = 0; r < repeticoes; r++)
        heuConAleGul(sol, 10); // 10% de aleatoriedade
    calcFO(sol);
    h = clock() - h;
    tempo = (double)h/CLOCKS_PER_SEC;
    escreverSolucao(sol, 0);
    printf("Construtiva Aleatoria Gulosa...: %.5f seg.\n",tempo);
}

void testar_estruturas()
{
    const int repeticoes = 1000;

    //---
    clock_t h;
    double tempo;
    Solucao sol;
    SolucaoBIN solB;

    //---
    printf("\n\n>>> TESTE DE COMPARACAO DAS ESTRUTURAS DA SOLUCAO - PMM3 - %d REPETICOES\n\n", repeticoes);
    ler_instancia("pmm3.txt");

    //---
    h = clock();
    for(int r = 0; r < repeticoes; r++)
        heuConAle(sol);
    h = clock() - h;
    tempo = (double)h/CLOCKS_PER_SEC;
    printf("Heuristica construtiva aleatoria................: %.5f seg.\n",tempo);

    //---
    h = clock();
    for(int r = 0; r < repeticoes; r++)
        heuConAleBIN(solB);
    h = clock() - h;
    tempo = (double)h/CLOCKS_PER_SEC;
    printf("Heuristica construtiva aleatoria (BINARIA)......: %.5f seg.\n",tempo);

    //---
    h = clock();
    for(int r = 0; r < repeticoes; r++)
        calcFO(sol);
    h = clock() - h;
    tempo = (double)h/CLOCKS_PER_SEC;
    printf("\nCalculo da FO...................................: %.5f seg.\n",tempo);

    //---
    h = clock();
    for(int r = 0; r < repeticoes; r++)
        calcFOBIN(solB);
    h = clock() - h;
    tempo = (double)h/CLOCKS_PER_SEC;
    printf("Calculo da FO (BINARIA).........................: %.5f seg.\n",tempo);

    //---
    heuConAle(sol);
    heuConAleBIN(solB);
    printf("\nSolucao:");
    escreverSolucao(sol, 0);
    printf("\nSolucao BINARIA:");
    escreverSolucaoBIN(solB, 0);
}

void testar_alocacao()
{
    const int tamanho = 500;
    const int repeticoes = 1000;
    //---
    clock_t h;
    double tempo;
    //---
    int **matDinamica;
    int matEstatica[tamanho][tamanho];

    printf("\n>>> TESTE COM UMA MATRIZ %d x %d - %d REPETICOES\n\n", tamanho, tamanho, repeticoes);

    //---
    h = clock();
    for(int r = 0; r < repeticoes; r++)
    {
        matDinamica = (int**)malloc(sizeof(int*) * tamanho);
        for(int i = 0; i < tamanho; i++)
            matDinamica[i] = (int*)malloc(sizeof(int) * tamanho);
        for(int i = 0; i < tamanho; i++)
            free(matDinamica[i]);
        free(matDinamica);
    }
    h = clock() - h;
    tempo = (double)h/CLOCKS_PER_SEC;
    printf("Tempo para alocar e desalocar usando MALLOC.....: %.5f segundos\n", tempo);
    //---

    //---
    h = clock();
    for(int r = 0; r < repeticoes; r++)
    {
        matDinamica = new int*[tamanho];
        for(int i = 0; i < tamanho; i++)
            matDinamica[i] = new int[tamanho];
        for(int i = 0; i < tamanho; i++)
            delete[] matDinamica[i];
        delete[] matDinamica;
    }
    h = clock() - h;
    tempo = (double)h/CLOCKS_PER_SEC;
    printf("Tempo para alocar e desalocar usando NEW........: %.5f segundos\n", tempo);
    //---

    //---
    h = clock();
    for(int r = 0; r < repeticoes; r++)
    {
        for(int i = 0; i < tamanho; i++)
            for(int j = 0; j < tamanho; j++)
                matEstatica[i][j] = 0;
    }
    h = clock() - h;
    tempo = (double)h/CLOCKS_PER_SEC;
    printf("\nTempo para zerar a matriz estatica com FOR......: %.5f segundos\n", tempo);
    //---

    //---
    h = clock();
    for(int r = 0; r < repeticoes; r++)
        memset(&matEstatica, 0, sizeof(matEstatica));
    h = clock() - h;
    tempo = (double)h/CLOCKS_PER_SEC;
    printf("Tempo para zerar a matriz estatica com MEMSET...: %.5f segundos\n", tempo);
    //---
}

void heuConAleBIN(SolucaoBIN &s)
{
	for(int i = 0; i < numMoc; i++)
		for(int j = 0; j < numObj; j++)
			s.matSol[i][j] = rand()%2;
}

void calcFOBIN(SolucaoBIN &s)
{
	memset(&s.vetPesMoc, 0, sizeof(s.vetPesMoc));
	memset(&s.vetQtdMocObj, 0, sizeof(s.vetQtdMocObj));
	s.funObj = 0;
	for(int i = 0; i < numMoc; i++)
		for(int j = 0; j < numObj; j++)
		{
			s.funObj += s.matSol[i][j] * vetValObj[j];
			s.vetPesMoc[i] += s.matSol[i][j] * vetPesObj[j];
			s.vetQtdMocObj[j] += s.matSol[i][j];
		}
	for(int i = 0; i < numMoc; i++)
		//if (s.vetPesMoc[i] > vetCapMoc[i])
		//	s.funObj -= PESO * (s.vetPesMoc[i] - vetCapMoc[i]);
		s.funObj -= PESO * MAX(0, s.vetPesMoc[i] - vetCapMoc[i]);
	for(int j = 0; j < numObj; j++)
		//if(s.vetQtdMocObj[j] > 1)
		//	s.funObj -= PESO2 * (s.vetQtdMocObj[j] - 1);
		s.funObj -= PESO2 * MAX(0, s.vetQtdMocObj[j] - 1);
}

void escreverSolucaoBIN(SolucaoBIN &s, const bool flag)
{
	printf("FO: %d\n", s.funObj);
	if(flag)
	{
		printf("PES:\n");
		for(int i = 0; i < numMoc; i++)
			printf("%d  ", s.vetPesMoc[i]);
		printf("\nQTD:\n");
		for(int j = 0; j < numObj; j++)
			printf("%d  ", s.vetQtdMocObj[j]);
		printf("\nMAT:\n");
		for(int i = 0; i < numMoc; i++)
		{
			for(int j = 0; j < numObj; j++)
				printf("%d  ", s.matSol[i][j]);
			printf("\n");
		}
	}
}
