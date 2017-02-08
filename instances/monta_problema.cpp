/* **************************************************** *
 *  Arquivo: monta_problema.cpp	                            *
 *  Problema do Caixeiro Viajante com Aluguel de Carros       *
 *  Le arquivo de Instâncias, Calcula distâncias e monta GLPK *
 *  Autor: Matheus Menezes                                    *
 *  Orientador: Marco Cesar Goldbarg                          *
 *  Doutorado em Sistemas e Computacao # DIMAp/UFRN           *
 * ********************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define infinito 1000000

struct ponto { double x, y; } *P; 		// pontos oriundos da instancia
struct mylist {
				int value;
				int next;
				int prev;
				int size;
			  };

void leInstancia(char *fname);
//void back(int cnt, int iCar, int iCid);
//void setup_back(int size);
//void CalculaRota(int *sol, int troca, int iCar);
//void calculaEmExecucao(int *sol, int troca, int iCar, int cnt);

time_t begin;
time_t end;

char nome[50];      // Nome do arquivo
int nCid, nCar;     // nCid= numero de cidades (DIMENSION); nCar= numero de carros
int **dist;     	// Matriz de adjacencias (simetrica) do grafo de entrada
long int ***cust;	// Matrizes de custos de aluguel
int ***taxa;		// Matrizes de taxas de retorno
int *sol=NULL;
mylist *cand=NULL;
int *sol_melhor=NULL;
int melhorSolCar1, melhorSolCar2, melhorSolTroca, melhorTaxa, melhorRota;
int vlRota=0, vlTaxa=0, vlSol=0, vlBase=0;
int somaBase=0;

int N;
double conta;
int melhorSol;
unsigned long int contaMelhorSol;
int limiteSuperior;
int modoSaida= 0;				//0= Completo, 1=Planilha (resumo - modo rapido)

//********************************************************
int main(int argc, char *argv[])
//********************************************************
{
	char nomeArq[100];
	int iCid;
    int nInstIni, nInstFim;

	if (modoSaida == 1) printf("Instancia         , Cid, Car, LiSup, t(s),     #perm, #vez,  Cust, taxa,  $Sol, Solucao");

   	switch (argc) {
	   	case   1: nInstIni=201; nInstFim=202; break;						//Ex: back
      case 2: strcpy(nomeArq, argv[1]); break;
	   	// case   2: nInstIni=atoi(argv[1]); nInstFim=nInstIni+1; break;		//Ex: back 201
	   	case   3: nInstIni=atoi(argv[1]); nInstFim=atoi(argv[2]); break;	//Ex: back 201 203
	   	default : nInstIni=0; nInstFim=0; break;
   	}

    // if ((argc > 3) || (nInstIni>280) || (nInstFim>281) || (nInstIni >= nInstFim)) {
    // 	printf("\nErro de Sintaxe!\nDigite: 'ag n1'\nn1=Numero da Instancia (1-280)\n");
    // 	return(0);
	// }


  	/*for (int i=nInstIni; i<nInstFim; i++) {		//1-201
	   	switch (i) {
			case   1: strcpy(nomeArq, "BrasilRJ14e") ; break;
			case   2: strcpy(nomeArq, "BrasilRN16e"); break;

			case 101: strcpy(nomeArq, "BrasilRJ14n"); break;
			case 102: strcpy(nomeArq, "BrasilRN16n"); break;

	   		case 201: strcpy(nomeArq,"Afeganistao4e"); break;
	   		case 202: strcpy(nomeArq,"Mianmar5e"); break;
	   		case 203: strcpy(nomeArq,"Zambia6e"); break;
	   		case 204: strcpy(nomeArq,"Turquia7e"); break;
	   		case 205: strcpy(nomeArq,"Mocambique8e"); break;
	   		case 206: strcpy(nomeArq,"Nigeria9e"); break;
	   		case 207: strcpy(nomeArq,"Mauritania10e"); break;
	   		case 208: strcpy(nomeArq,"Colombia11e"); break;
	   		case 209: strcpy(nomeArq,"Angola12e"); break;
	   		case 210: strcpy(nomeArq,"Peru13e"); break;
	   		case 211: strcpy(nomeArq,"Libia14e"); break;
	   		case 212: strcpy(nomeArq,"Congo15e"); break;
	   		case 213: strcpy(nomeArq,"Argentina16e"); break;
	   		case 214: strcpy(nomeArq,"EUA17e"); break;
	   		case 215: strcpy(nomeArq,"Chile6e"); break;
	   		case 216: strcpy(nomeArq,"Paquistao7e"); break;
	   		case 217: strcpy(nomeArq,"Nanibia8e"); break;
	   		case 218: strcpy(nomeArq,"Tanzania9e"); break;
	   		case 219: strcpy(nomeArq,"Bolivia10e"); break;
	   		case 220: strcpy(nomeArq,"AfricaSul11e"); break;
	   		case 221: strcpy(nomeArq,"Niger12e"); break;
	   		case 222: strcpy(nomeArq,"Mongolia13e"); break;
	   		case 223: strcpy(nomeArq,"Indonesia14e"); break;
	   		case 224: strcpy(nomeArq,"Argelia15e"); break;
	   		case 225: strcpy(nomeArq,"India16e"); break;
	   		case 226: strcpy(nomeArq,"China17e"); break;
	   		case 227: strcpy(nomeArq,"Venezuela8e"); break;
	   		case 228: strcpy(nomeArq,"Egito9e"); break;
	   		case 229: strcpy(nomeArq,"Etiopia10e"); break;
	   		case 230: strcpy(nomeArq,"Mali11e"); break;
	   		case 231: strcpy(nomeArq,"Chade12e"); break;
	   		case 232: strcpy(nomeArq,"Ira13e"); break;
	   		case 233: strcpy(nomeArq,"Mexico14e"); break;
	   		case 234: strcpy(nomeArq,"Sudao15e"); break;
	   		case 235: strcpy(nomeArq,"Australia16e"); break;
	   		case 236: strcpy(nomeArq,"Canada17e"); break;
	   		case 237: strcpy(nomeArq,"Arabia14e"); break;
	   		case 238: strcpy(nomeArq,"Cazaquistao15e"); break;
	   		case 239: strcpy(nomeArq,"Brasil16e"); break;
	   		case 240: strcpy(nomeArq,"Russia17e"); break;

	   		case 241: strcpy(nomeArq,"Afeganistao4n"); break;
	   		case 242: strcpy(nomeArq,"Mianmar5n"); break;
	   		case 243: strcpy(nomeArq,"Zambia6n"); break;
	   		case 244: strcpy(nomeArq,"Turquia7n"); break;
	   		case 245: strcpy(nomeArq,"Mocambique8n"); break;
	   		case 246: strcpy(nomeArq,"Nigeria9n"); break;
	   		case 247: strcpy(nomeArq,"Mauritania10n"); break;
	   		case 248: strcpy(nomeArq,"Colombia11n"); break;
	   		case 249: strcpy(nomeArq,"Angola12n"); break;
	   		case 250: strcpy(nomeArq,"Peru13n"); break;
	   		case 251: strcpy(nomeArq,"Libia14n"); break;
	   		case 252: strcpy(nomeArq,"Congo15n"); break;
	   		case 253: strcpy(nomeArq,"Argentina16n"); break;
	   		case 254: strcpy(nomeArq,"EUA17n"); break;
	   		case 255: strcpy(nomeArq,"Chile6n"); break;
	   		case 256: strcpy(nomeArq,"Paquistao7n"); break;
	   		case 257: strcpy(nomeArq,"Nanibia8n"); break;
	   		case 258: strcpy(nomeArq,"Tanzania9n"); break;
	   		case 259: strcpy(nomeArq,"Bolivia10n"); break;
	   		case 260: strcpy(nomeArq,"AfricaSul11n"); break;
	   		case 261: strcpy(nomeArq,"Niger12n"); break;
	   		case 262: strcpy(nomeArq,"Mongolia13n"); break;
	   		case 263: strcpy(nomeArq,"Indonesia14n"); break;
	   		case 264: strcpy(nomeArq,"Argelia15n"); break;
	   		case 265: strcpy(nomeArq,"India16n"); break;
	   		case 266: strcpy(nomeArq,"China17n"); break;
	   		case 267: strcpy(nomeArq,"Venezuela8n"); break;
	   		case 268: strcpy(nomeArq,"Egito9n"); break;
	   		case 269: strcpy(nomeArq,"Etiopia10n"); break;
	   		case 270: strcpy(nomeArq,"Mali11n"); break;
	   		case 271: strcpy(nomeArq,"Chade12n"); break;
	   		case 272: strcpy(nomeArq,"Ira13n"); break;
	   		case 273: strcpy(nomeArq,"Mexico14n"); break;
	   		case 274: strcpy(nomeArq,"Sudao15n"); break;
	   		case 275: strcpy(nomeArq,"Australia16n"); break;
	   		case 276: strcpy(nomeArq,"Canada17n"); break;
	   		case 277: strcpy(nomeArq,"Arabia14n"); break;
	   		case 278: strcpy(nomeArq,"Cazaquistao15n"); break;
	   		case 279: strcpy(nomeArq,"Brasil16n"); break;
	   		case 280: strcpy(nomeArq,"Russia17n"); break;

		}*/
// 		limiteSuperior= infinito;
 	// 	limiteSuperior= 1000;

		// N=0; conta=0;
		// melhorSol= infinito; contaMelhorSol=0;
		// melhorTaxa= infinito; melhorRota= infinito;
//		melhorSolCar1=NULL, melhorSolTroca=NULL;

		leInstancia(nomeArq);
    // }
	return 0;

}

void leInstancia(char *fname)
{
  	double aux2;
  	FILE *stream;
	FILE *ptarq;
	char nome[50];      // Nome do arquivo
  	char buf[80];
	char tipo[10];			// tipo de instancia (EUC_2D ou EXPLICIT)
	char formato[10];		// formato da instancia (FULL_MATRIX ou VECTOR)
   	char nomeArqSaida[100]="";



  	if (modoSaida == 0)
	    printf("Instancia: %s ",fname );
	else printf("\n%18s,",fname );
    strcat (nomeArqSaida, fname);
  	strcpy(nome,"insts/");
	strcat(nome, fname);
	strcat(nome,".car");
  	strcpy(fname, nome);

  	/* abrir arquivo */
  	if ( (stream=fopen(fname,"r"))==NULL ) {
  		printf("Não foi possivel abrir %s\n",fname);
  		// system("PAUSE");
  		exit(1);
  	}
  	//Gravação do arquivo de problema
     strcat(nomeArqSaida, ".txt");
     if ((ptarq = fopen(nomeArqSaida, "w"))== NULL) {
         printf ("/n/nNao foi possivel criar arquivo para gravar Matriz Distancias");
     }
     //código inserido

  	/*numero de cidades e carros*/
  	while (strcmp(buf,"DIMENSION")!=0)
       	fscanf(stream,"%s", &buf);
  	fscanf(stream,"%s", &buf);
  	fscanf(stream," %d", &nCid);
  	fscanf(stream,"%s", &buf);
  	fscanf(stream,"%s", &buf);
  	fscanf(stream," %d", &nCar);
  	if (modoSaida == 0)
		printf("\nCidades= %d e Carros= %d", nCid,nCar);
  	else printf("%4d,%4d,", nCid,nCar);

  	fscanf(stream,"%s", &buf);
  	fscanf(stream,"%s", &buf);
  	fscanf(stream," %s", &tipo);
  	fscanf(stream,"%s", &buf);
  	fscanf(stream,"%s", &buf);
  	fscanf(stream," %s", &formato);

	if (modoSaida == 0) {
		printf("\nTipo de instancia= %s", tipo);
		printf("\nFormato= %s", formato);
	}

	//Inicializa Matriz de Custos de Aluguel
	cust = (long int***) malloc(sizeof(long int**)*nCar);

	for (int h=0; h<nCar; h++) {
	  	cust[h]=(long int**)malloc(sizeof(long int*)*nCid);
      	for (int i=0; i<nCid; i++) {
           	cust[h][i] = (long int*) malloc(sizeof(long int)*nCid);
       	   	for(int j=0;j<nCid;++j)
       	   		cust[h][i][j]=0;
    	}
  	}

	if (strcmp(formato,"FULL_MATRIX") ==0 ) {
  		while (strcmp(buf,"EDGE_WEIGHT_SECTION")!=0)
        	fscanf(stream,"%s", &buf);

		for (int h=0; h<nCar; h++) {
			fscanf(stream,"%d", &buf);
			for (int i=0; i<nCid; i++)
       			for (int j=0; j<nCid; j++)
	           		fscanf(stream,"%li", &cust[h][i][j]);
  		}
	}
	else {
  		if (!strcmp(tipo,"EUC_2D")) {
  			//Inicializa Matriz Simétrica de Distâncias
  			dist = (int**) malloc(sizeof(int*)*nCid);
  			for (int i=0; i<nCid; i++)
      			dist[i] = (int*) malloc(sizeof(int)*nCid);

  			while (strcmp(buf,"NODE_COORD_SECTION")!=0)
        		fscanf(stream,"%s", &buf);

  			P = (ponto*) malloc(sizeof(ponto)*nCid);

  			//Gera Matriz Simétrica de Distâncias
  			for (int i=0; i<nCid; i++)
      			fscanf(stream, "%lf %lf %lf\n", &aux2, &(P[i].x), &(P[i].y));

  			for (int i=0; i<nCid; i++)
       			for (int j=i; j<nCid; j++)
           			if (i == j)
               			dist[i][j] = 0;
           			else {
                 		dist[i][j] = (int) (sqrt(pow((P[i].x-P[j].x), 2) + pow((P[i].y-P[j].y), 2)));
                 		dist[j][i] = dist[i][j];
           			}
          //Monta os Conjuntos I, J e K
        if (modoSaida == 0)
	        fprintf(ptarq,"%d %d",nCid, nCar);

  			// Mostra matriz de distâncias na tela
 			/*if (modoSaida == 0) {

                fprintf(ptarq,"\n#Matriz de Distancias:");
  				for (int i=0; i<nCid; i++) {
	   				fprintf(ptarq,"\n #");
       				for (int j=0; j<nCid; j++)
        	    		fprintf(ptarq," %2d", dist[i][j]);
	   			}
   			} */
		}

  		//Gera Matriz de Custos de Aluguel
  		while (strcmp(buf,"EDGE_WEIGHT_SECTION")!=0)
        	fscanf(stream,"%s", &buf);

		// Com calculo baseado no vetor
  		int vetor [nCid];
  		for (int i=0; i<nCid; i++) vetor[i]=0;

  		for (int h=0; h<nCar; h++) {
    		fscanf(stream,"%s", &buf);

  			for (int i=0; i<nCid; i++)
      			fscanf(stream, "%d", &vetor[i]);

  			for (int i=0; i<nCid; i++)
       			for (int j=i; j<nCid; j++)
           			if (i == j)
            			cust[h][i][j] = 0;
           			else {
  						if (!strcmp(tipo,"EUC_2D"))
  							cust[h][i][j] = (long int)((vetor[i]*2+vetor[j]*3)/3)+(dist[i][j]);
  						else
  							cust[h][i][j] = (long int)((vetor[i]*2+vetor[j]*3)/3);
                		cust[h][j][i] = cust[h][i][j];
            		}
  		}
	}

  	// Mostra matriz de Custos
	if (modoSaida == 0) {

		// fprintf(ptarq, "\n\n#Matrizes de Custos:");
  		for (int h=0; h<nCar; h++) {
  			fprintf(ptarq,"\n");
    		// fprintf(ptarq, "\nparam C%d :", h+1);
    		// for (int c=0; c<nCid; c++) {
      // 	    		fprintf(ptarq," %4li", c+1);
	   		// 	}
	   		// fprintf(ptarq," := ");
    		for (int i=0; i<nCid; i++) {
		   		// fprintf(ptarq,"\n  %10li ",i+1);
		   		fprintf(ptarq,"\n");
    	   		for (int j=0; j<nCid; j++)
    	        	fprintf(ptarq," %li", cust[h][i][j]);
	   		}
	   			
	   		// fprintf(ptarq,"; ");
   		}
  	}


  	//Inicializa Matrizes de Taxas de Retorno
  	taxa = (int***) malloc(sizeof(int**)*nCar);

  	for (int h=0; h<nCar; h++) {
	  	taxa[h]=(int**)malloc(sizeof(int*)*nCid);
      	for (int i=0; i<nCid; i++) {
           	taxa[h][i] = (int*) malloc(sizeof(int)*nCid);
           	for(int j=0;j<nCid;++j)
           		taxa[h][i][j]=0;
       	}
  	}

  	//Gera Matrizes de Taxas de Retorno
  	while (strcmp(buf,"RETURN_RATE_SECTION")!=0)
    	fscanf(stream,"%s", &buf);

	if (strcmp(formato,"FULL_MATRIX") ==0 ) {
		for (int h=0; h<nCar; h++) {
    		fscanf(stream,"%d", &buf);
  			for (int i=0; i<nCid; i++)
       			for (int j=0; j<nCid; j++)
	    	       	fscanf(stream,"%d", &taxa[h][i][j]);
  		}
	}
  	else {
		// -------------------------- Com calculo
  		int iVetorTaxas [nCid];
  		for (int i=0; i<nCid; i++) iVetorTaxas[i]=0;

  		for (int h=0; h<nCar; h++) {
    		fscanf(stream,"%s", &buf);
  			for (int i=0; i<nCid; i++)
      			fscanf(stream, "%d", &iVetorTaxas[i]);

  			for (int i=0; i<nCid; i++)
       			for (int j=0; j<nCid; j++)
           			if (i == j)
            			taxa[h][i][j] = 0;
           			else {
	           			for (int k=0; k<nCid; k++)
	           				taxa[h][i][j] = ((iVetorTaxas[i]*3+iVetorTaxas[j])*4)/2;
            		}
  		}
	}


 	/* if (modoSaida == 0) {
		fprintf(ptarq,"\n#Matrizes de Taxas de Retorno:");
  		for (int h=0; h<nCar; h++) {
    		fprintf(ptarq,"\n param C%d : %d", h);
    		for (int i=0; i<nCid; i++) {
	   			fprintf(ptarq, "\n");
       			for (int j=0; j<nCid; j++)
        	    	fprintf(ptarq, " %3d", taxa[h][i][j]);
	   		}
  		}
	}  */




  	// Mostra matrizes de Taxas de Retorno

      if (modoSaida == 0) {

		// fprintf(ptarq, "\n\n#Matrizes de Taxas de Retorno:");
      	
  		for (int h=0; h<nCar; h++) {
  			fprintf(ptarq, "\n");
    		// fprintf(ptarq, "\nparam W%d :", h+1);
    		// for (int c=0; c<nCid; c++) {
      // 	    		fprintf(ptarq," %4li", c+1);
	   		// 	}
	   		// fprintf(ptarq," := ");
    		for (int i=0; i<nCid; i++) {
		   		fprintf(ptarq,"\n");
    	   		for (int j=0; j<nCid; j++)
    	        	fprintf(ptarq," %li", taxa[h][i][j]);
	   		}
	   			// fprintf(ptarq,"\n");
	   		// fprintf(ptarq,"; ");
   		}
       // fprintf(ptarq,"\n\nend;");
  	}




  	free (P);
	fclose(ptarq);
  	fclose(stream);
}
//--------------------------------------------------------
