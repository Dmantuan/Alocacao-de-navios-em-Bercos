#define MAX_NAV 100
#define MAX_BER 20

// Estruturas de dados
typedef struct tSolucao
{
	int vetTemp[MAX_NAV]; 
	int vetPos[MAX_NAV]; 
	int vetSol[MAX_NAV]; //qual navio est� em qual ber�o
	int funObj;
	int tempServ;
}Solucao;

// Dados de entrada
int numNav;
int numBer;
int vetTamNav[MAX_NAV];
int vetInNav[MAX_NAV];
int vetOutNav[MAX_NAV];
int vetOpBer[MAX_BER];
int vetClsBer[MAX_BER];
int matTempNavBer[MAX_BER][MAX_NAV];
int matBercos[MAX_BER][2];
int vetIndNavOrd[MAX_NAV];

//Metodos iniciais
void ordenarObjetos();
void ler_instancia(char *arq);
void testar_instancia(char *arq);

//Outros
int vetTempAux[MAX_BER];

//Iniciaclizoes
void calcFO(Solucao &s);
void escreverSolucao(Solucao &s, const bool flag);

void clonarSolucao(Solucao &s, Solucao &clone);
void lerSolucao(char *arq, Solucao &s);
void heuConGul(Solucao &s);
