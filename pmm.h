#define MAX_OBJ 500
#define MAX_MOC 50

// Estruturas de dados
typedef struct tSolucao
{
	int vetPesMoc[MAX_MOC];
	int vetSol[MAX_OBJ];
	int funObj;
}Solucao;


// Dados de entrada
int numObj;
int numMoc;
int vetValObj[MAX_OBJ];
int vetPesObj[MAX_OBJ];
int vetCapMoc[MAX_MOC];
int vetIndObjOrd[MAX_OBJ];

// Metodos

//---
void heuBLMM(Solucao &s);
void heuBLPM(Solucao &s);
void heuBLRA(Solucao &s, const int iteracoes);
//---
void heuConAleGul(Solucao &s, const int percentual);
void heuConGul(Solucao &s);
void heuConAle(Solucao &s);
//---
void calcFO(Solucao &s);
void escreverSolucao(Solucao &s, const bool flag);
//---
void ordenarObjetos();
void ler_instancia(char *arq);
void testar_instancia(char *arq);


//======== ESTRUTURAS E METODOS AUXILIARES ========
// Usados apenas para comparacao de desempenho
typedef struct tSolucaoBIN
{
	int vetQtdMocObj[MAX_OBJ];
	int vetPesMoc[MAX_MOC];
	int matSol[MAX_MOC][MAX_OBJ];
	int funObj;
}SolucaoBIN;

void heuConAleBIN(SolucaoBIN &s);
void calcFOBIN(SolucaoBIN &s);
void escreverSolucaoBIN(SolucaoBIN &s, const bool flag);
//---
void testar_alocacao();
void testar_estruturas();
void testar_heuConstrutivas();
void testar_buscaLocal();
