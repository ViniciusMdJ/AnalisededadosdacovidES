#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINHAS 40                   //define a quantidade de linhas que tem o arquivo

typedef struct{                     //struct d para armazenar data
    int dia;
    int mes;
    int ano;
}tData;

typedef struct{
    tData DataCadastro;             //struct data do dia, mes e ano da data de cadastro
    tData DataObito;                //struct data do dia, mes e ano da data de obito
    char Classificacao[12];         //string de clacificacao (suspeito, confirmado ou descartado)
    char Municipio[30];              //string de nome do municipio(tem q ver a cidade com o maior nome do ES)
    tData IdadeNaDataNotificacao;   //struct data de quantos anos, meses e dias do paciente
    int ComorbidadePulmao;          //sim sera 1 e nao sera 0
    int ComorbidadeCardio;          //sim sera 1 e nao sera 0
    int ComorbidadeRenal;           //sim sera 1 e nao sera 0
    int ComorbidadeDiabetes;        //sim sera 1 e nao sera 0
    int ComorbidadeTabagismo;       //sim sera 1 e nao sera 0
    int ComorbidadeObesidade;       //sim sera 1 e nao sera 0
    int FicouInternado;             //sim sera 1, nao sera 0 e nao informado sera 2
}tPaciente;

int main(){
    FILE *arq;
    //FILE *teste;
    //teste = fopen("arquivotexte.txt", "w");

    tPaciente pessoas[LINHAS];
    arq = fopen("covid19ESresumid.csv", "r");
    if(arq==NULL || teste==NULL){
        printf("Erro na arbertura do arquivo");
        exit(1);
    }

    //Cria um vetor de struct para cada paciente
    tPaciente pessoa[LINHAS]; 


return 0;
}