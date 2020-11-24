#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINHAS 202362                //define a quantidade de linhas que tem o arquivo

typedef struct{                     //struct d para armazenar data
    int dia;
    int mes;
    int ano;
}tData;

typedef struct{
    tData DataCadastro;             //struct data do dia, mes e ano da data de cadastro
    tData DataObito;                //struct data do dia, mes e ano da data de obito
    int Classificacao;         //string de clacificacao (suspeito, confirmado ou descartado)
    char Municipio[30];             //string de nome do municipio(tem q ver a cidade com o maior nome do ES)
    int IdadeNotificacao;           //quantos ANOS a pessoa tem quando foi notificada
    int ComorbidadePulmao;          //sim sera 1 e nao sera 0
    int ComorbidadeCardio;          //sim sera 1 e nao sera 0
    int ComorbidadeRenal;           //sim sera 1 e nao sera 0
    int ComorbidadeDiabetes;        //sim sera 1 e nao sera 0
    int ComorbidadeTabagismo;       //sim sera 1 e nao sera 0
    int ComorbidadeObesidade;       //sim sera 1 e nao sera 0
    int FicouInternado;             //sim sera 1, nao sera 0 e nao informado sera 2
}tPaciente;

tPaciente LeArquivo(FILE* arq);
// Funcao para verificar se a data de cadastro esta entre um certo intervalo
int VerificaData(tData pessoa, tData inicio, tData fim);

void printtData(tData data);
tData LetData();

tPaciente pessoa[LINHAS];
int main(){
    FILE *arq;
    //Cria um vetor de struct para cada paciente
	char caminho[15];
	char cidade[40];
    int i, mincasos, topcidades;
	tData inicio4, fim4, inicio5, fim5, inicio7, fim7;


    arq = fopen("covid19ES.csv", "r");
    if(arq==NULL){
        printf("Erro na arbertura do arquivo");
        exit(1);
    }
     
    for(i=-1;i<LINHAS;i++){
        if(i==-1){
            char lido[1000];
            fgets(lido, 1000, arq);
        }
        else{
            pessoa[i] = LeArquivo(arq);
            printf("%d %d %d %s %d %d %d %d %d %d %d\n",pessoa[i].DataCadastro.ano, pessoa[i].DataObito.ano, pessoa[i].Classificacao, pessoa[i].Municipio, pessoa[i].ComorbidadePulmao, pessoa[i].ComorbidadeCardio, pessoa[i].ComorbidadeRenal, pessoa[i].ComorbidadeDiabetes, pessoa[i].ComorbidadeTabagismo, pessoa[i].ComorbidadeObesidade, pessoa[i].FicouInternado);
        }
        
    }
	//lendo o arquivo de teste
	scanf("%[^\n]s", caminho);
	scanf("%d", &mincasos);
	inicio4 = LetData();
	fim4 = LetData();
	scanf("%d", &topcidades);	
	inicio5 = LetData();
	fim5 = LetData();
	//getchar responsavel por receper \n restante do LetData
	getchar();
	scanf("%[^\n]s", cidade);
	inicio7 = LetData();
	fim7 = LetData();
    //pessoa[0] = LeArquivo(arq);
    //printf("%d %d %d %s %d %d %d %d %d %d %d\n",pessoa[0].DataCadastro.ano, pessoa[0].DataObito.ano, pessoa[0].Classificacao, pessoa[0].Municipio, pessoa[22].ComorbidadePulmao, pessoa[0].ComorbidadeCardio, pessoa[0].ComorbidadeRenal, pessoa[0].ComorbidadeDiabetes, pessoa[0].ComorbidadeTabagismo, pessoa[0].ComorbidadeObesidade, pessoa[0].FicouInternado);


return 0;
}

tPaciente LeArquivo(FILE* arq){
    char lido[1000], temp[1000];
    tPaciente pessoa;
    fgets(lido, 1000, arq);
    //Le o ano da data de cadastro
    pessoa.DataCadastro.ano = atoi(strtok(lido, "-"));
    //Le o mes da data de cadastro
    pessoa.DataCadastro.mes = atoi(strtok(NULL, "-"));
    //Le o dia da data de cadastro
    pessoa.DataCadastro.dia = atoi(strtok(NULL, ","));
    //Le o ano da data de obito
    pessoa.DataObito.ano = atoi(strtok(NULL, "-"));
    //Le o mes da data de obito
    pessoa.DataObito.mes = atoi(strtok(NULL, "-"));
    //Le o dia da data de obito
    pessoa.DataObito.dia = atoi(strtok(NULL, ","));
    //Le a classificacao do paciente com strtok e copia pra pessoa.Classificacao com atrcpy
    strcpy(temp, strtok(NULL, ","));
	if(!strcmp(temp,"Confirmados")){
		pessoa.Classificacao = 1;
	}
	else{
		pessoa.Classificacao = 0;
	}
    strcpy(pessoa.Municipio, strtok(NULL, ","));
    //Le no formato ""aa " e copia pra variavel temporaria
    strcpy(temp, strtok(NULL, " "));
    //Elimina a aspa dupla qua esta na posicao 0 do string
    temp[0]=' ';
    //converte a string " aa" para um numero inteiro que e a idade
    pessoa.IdadeNotificacao = atoi(temp);
    //copia o resto da formatancao que sera deconsiderada = "anos, mm meses, dd dias"
    //strcpy(temp, strtok(NULL, ","));
	do{
		strcpy(temp,strtok(NULL, ","));
	}while(temp[strlen(temp)-1] != '"');
	//printf("%s\n", strtok(NULL, "\n"));
    //strcpy(temp, strtok(NULL, ","));
    //Le se tem ou nao comorbidades, se tiver atribui o valor 1, senao atribui 0
    strcpy(temp, strtok(NULL, ","));
    if(!strcmp(temp,"Sim")){
        pessoa.ComorbidadePulmao = 1;
    }
    else{pessoa.ComorbidadePulmao = 0;}
    strcpy(temp, strtok(NULL, ","));
    if(!strcmp(temp,"Sim")){
        pessoa.ComorbidadeCardio = 1;
    }
    else{pessoa.ComorbidadeCardio = 0;}
    strcpy(temp, strtok(NULL, ","));
    if(!strcmp(temp,"Sim")){
        pessoa.ComorbidadeRenal = 1;
    }
    else{pessoa.ComorbidadeRenal = 0;}
    strcpy(temp, strtok(NULL, ","));
    if(!strcmp(temp,"Sim")){
        pessoa.ComorbidadeDiabetes = 1;
    }
    else{pessoa.ComorbidadeDiabetes = 0;}
    strcpy(temp, strtok(NULL, ","));
    if(!strcmp(temp,"Sim")){
        pessoa.ComorbidadeTabagismo = 1;
    }
    else{pessoa.ComorbidadeTabagismo = 0;}
    strcpy(temp, strtok(NULL, ","));
    if(!strcmp(temp,"Sim")){
        pessoa.ComorbidadeObesidade = 1;
    }
    else{pessoa.ComorbidadeObesidade = 0;}
    //verifica se foi ou nao internado, se foi atribui o valor 1, senao atribui 0
    strcpy(temp, strtok(NULL, "\n"));
    if(!strcmp(temp,"Sim")){
        pessoa.FicouInternado = 1;
    }
    else{pessoa.FicouInternado = 0;}

    return pessoa;
}

int VerificaData(tData pessoa, tData inicio, tData fim){
	//verifica se o ano de cadastro esta entre o intervalo
	if(inicio.ano <= pessoa.ano && pessoa.ano <= fim.ano){
		//verifica se o mes de cadastro esta entre o intervalo
		if(inicio.mes < pessoa.mes && pessoa.mes < fim.mes){
			return 1;
		}
		else if(inicio.mes == pessoa.mes){
			//verifica se o dia de cadastro esta entre o intervalo
			if(inicio.dia <= pessoa.dia){
				return 1;
			}
		else if(pessoa.mes == fim.mes){
			if(pessoa.dia <= fim.dia){
				return 1;
			}
		}
		}
	}
	return 0;
};

tData LetData(){
	tData lido;
	scanf("%4d-%2d-%2d", &lido.ano, &lido.mes, &lido.dia);
return lido;
};

void printtData(tData data){
	printf("%d-%d-%d\n", data.ano, data.mes, data.dia);
};
