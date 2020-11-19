#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINHAS 40                  //define a quantidade de linhas que tem o arquivo

typedef struct{                     //struct d para armazenar data
    int dia;
    int mes;
    int ano;
}tData;

typedef struct{
    tData DataCadastro;             //struct data do dia, mes e ano da data de cadastro
    tData DataObito;                //struct data do dia, mes e ano da data de obito
    char Classificacao[12];         //string de clacificacao (suspeito, confirmado ou descartado)
    char Municipio[30];             //string de nome do municipio(tem q ver a cidade com o maior nome do ES)
    int IdadeNotificacao;         //quantos ANOS a pessoa tem quando foi notificada
    int ComorbidadePulmao;          //sim sera 1 e nao sera 0
    int ComorbidadeCardio;          //sim sera 1 e nao sera 0
    int ComorbidadeRenal;           //sim sera 1 e nao sera 0
    int ComorbidadeDiabetes;        //sim sera 1 e nao sera 0
    int ComorbidadeTabagismo;       //sim sera 1 e nao sera 0
    int ComorbidadeObesidade;       //sim sera 1 e nao sera 0
    int FicouInternado;             //sim sera 1, nao sera 0 e nao informado sera 2
}tPaciente;

tPaciente LeArquivo(FILE* arq);

int main(){
    FILE *arq;
    //Cria um vetor de struct para cada paciente
    tPaciente pessoa[LINHAS];
    int i;
    arq = fopen("covid19ESresumido.csv", "r");
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
            //printf("%d %d %s %s %d %d %d %d %d %d %d\n",pessoa[i].DataCadastro.ano, pessoa[i].DataObito.ano, pessoa[i].Classificacao, pessoa[i].Municipio, pessoa[i].ComorbidadePulmao, pessoa[i].ComorbidadeCardio, pessoa[i].ComorbidadeRenal, pessoa[i].ComorbidadeDiabetes, pessoa[i].ComorbidadeTabagismo, pessoa[i].ComorbidadeObesidade, pessoa[i].FicouInternado);
        }
        
    }
    //pessoa[0] = LeArquivo(arq);
    //printf("%d %d %s %s %d %d %d %d %d %d %d\n",pessoa[0].DataCadastro.ano, pessoa[0].DataObito.ano, pessoa[0].Classificacao, pessoa[0].Municipio, pessoa[22].ComorbidadePulmao, pessoa[0].ComorbidadeCardio, pessoa[0].ComorbidadeRenal, pessoa[0].ComorbidadeDiabetes, pessoa[0].ComorbidadeTabagismo, pessoa[0].ComorbidadeObesidade, pessoa[0].FicouInternado);


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
    strcpy(pessoa.Classificacao, strtok(NULL, ","));
    //Le o Municipio do paciente com strtok e copia pra pessoa.Municipio com atrcpy
    strcpy(pessoa.Municipio, strtok(NULL, ","));
    //Le no formato ""aa " e copia pra variavel temporaria
    strcpy(temp, strtok(NULL, " "));
    //Elimina a aspa dupla qua esta na posicao 0 do string
    temp[0]=' ';
    //converte a string " aa" para um numero inteiro que e a idade
    pessoa.IdadeNotificacao = atoi(temp);
    //copia o resto da formatancao que sera deconsiderada = "anos, mm meses, dd dias"
    strcpy(temp, strtok(NULL, ","));
    strcpy(temp, strtok(NULL, ","));
    strcpy(temp, strtok(NULL, ","));
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
    strcpy(temp, strtok(NULL, ","));
    if(!strcmp(temp,"Sim")){
        pessoa.FicouInternado = 1;
    }
    else{pessoa.FicouInternado = 0;}

    return pessoa;
}