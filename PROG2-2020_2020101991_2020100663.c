#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINHAS 202362               //define a quantidade de linhas que tem o arquivo
#define QTDMUNICIPIOS 78            //define a quantidade de municipios

typedef struct{                     //struct d para armazenar data
    int dia;
    int mes;
    int ano;
}tData;

typedef struct{
    char Cidade[30];
    int QtdCasos;
    int QtdCasosEntreDatas;
}tCidade;

typedef struct{
    tData DataCadastro;             //struct data do dia, mes e ano da data de cadastro
    tData DataObito;                //struct data do dia, mes e ano da data de obito
    int Classificacao;              //armazena se tem covid ou nao, se confimado recebe 1, senao recebe 0
    char Municipio[30];             //string de nome do municipio(tem q ver a cidade com o maior nome do ES)
    int IdadeNotificacao;           //quantos ANOS a pessoa tem quando foi notificada
    int ComorbidadePulmao;          //sim sera 1 e nao sera 0
    int ComorbidadeCardio;          //sim sera 1 e nao sera 0
    int ComorbidadeRenal;           //sim sera 1 e nao sera 0
    int ComorbidadeDiabetes;        //sim sera 1 e nao sera 0
    int ComorbidadeTabagismo;       //sim sera 1 e nao sera 0
    int ComorbidadeObesidade;       //sim sera 1 e nao sera 0
    int FicouInternado;             //sim sera 1 e nao ou nao irformado sera 0
}tPaciente;

tPaciente LeArquivo(FILE* arq);
// Funcao para verificar se a data de cadastro esta entre um certo intervalo
int VerificaData(tData pessoa, tData inicio, tData fim);

//Funcao para printar o struct tData
void printtData(tData data);
//Funcao para zerar as variaveis do struct tCidade
void ZeraCidades(tCidade* Municipios);
//Funcao para adicionar os casos confirmados do municipio
void AdicionaMunicipioseCasos(tCidade* Municipios, int Indice);
//Funcao para ler datas
tData LetData();

//Funcao para adicionar os casos comfirmados entre um certo intervalo
int CasosentreDatas(tData inicio, tData fim, int indicepaciente);
//Verifica se a cidade ja foi adicionada no vetor municipios
int VerificaseCidadecadastrada(tCidade municipios[], int Indice);

//cria o vetor de pacientes globalmente
tPaciente pessoa[LINHAS];           

int main(){
    FILE *arq;

    //cria as variaveis usadas no programa
	char caminho[15];
	char cidade[40];
    int i, mincasos, topcidades, totalconfirmados = 0;
	//Variaveis para cada item que possui intervalo de datas
	tData inicio4, fim4, inicio5, fim5, inicio7, fim7;
    tCidade Municipios[QTDMUNICIPIOS];


	//lendo o arquivo de teste
	scanf("%[^\n]s", caminho);
	scanf("%d", &mincasos);
	inicio4 = LetData();
	fim4 = LetData();
	scanf("%d", &topcidades);	
	inicio5 = LetData();
	fim5 = LetData();
	//getchar responsavel por receber \n restante do LetData
	getchar();
	scanf("%[^\n]s", cidade);
	inicio7 = LetData();
	fim7 = LetData();

    //arbrindo o arquivo .csv
    arq = fopen("covid19ES.csv", "r");
    if(arq==NULL){
        printf("Erro na arbertura do arquivo");
        exit(1);
    }
    
    //lendo o arquivo .csv
    for(i=-1;i<LINHAS;i++){
        if(i==-1){
            char lido[1000];
            fgets(lido, 1000, arq);
        }
        else{
            pessoa[i] = LeArquivo(arq);
            /*printf("%d %d %d %s %d %d %d %d %d %d %d\n",
					pessoa[i].DataCadastro.ano, pessoa[i].DataObito.ano, 
					pessoa[i].Classificacao, pessoa[i].Municipio, 
					pessoa[i].ComorbidadePulmao, pessoa[i].ComorbidadeCardio, 
					pessoa[i].ComorbidadeRenal, pessoa[i].ComorbidadeDiabetes, 
					pessoa[i].ComorbidadeTabagismo, 
					pessoa[i].ComorbidadeObesidade, pessoa[i].FicouInternado);*/

        }
    }
    //fechando o arquivo para que nao haja vazamento de memoria
    fclose(arq);

    //zera as variaveis dentro do struct, a cidade com void e os casos com 0
    ZeraCidades(Municipios);

    //verificando os itens pedidos
	for(i=0; i<LINHAS; i++){
        //verifica o item 4, se tem covid confirmada e se esta entre as datas fornecidas
		if(CasosentreDatas(inicio4, fim4, i)){
			totalconfirmados++;
		}
		//Verifica se a pessoa teve covid e Adiciona ao total de casos do municipio
        if(pessoa[i].Classificacao){
            AdicionaMunicipioseCasos(Municipios, i);
        }

	}
	//printf("%d\n", totalconfirmados);
    for(i=0;i<QTDMUNICIPIOS;i++){
        printf("%s %d %d\n", Municipios[i].Cidade, Municipios[i].QtdCasos, i);
    }
	

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

int VerificaData(tData pessoas, tData inicio, tData fim){
	int start, end, entre;
	start = (inicio.ano * 10000) + (inicio.mes *100) + inicio.dia;
	end = (fim.ano * 10000) + (fim.mes *100) + fim.dia;
	entre = (pessoas.ano * 10000) + (pessoas.mes *100) + pessoas.dia;

	if(start <= entre && entre <= end){
		return 1;
	}
return 0;
};

tData LetData(){
	tData lido;
	//Recebe da entrada padrao a data formatada
	scanf("%4d-%2d-%2d", &lido.ano, &lido.mes, &lido.dia);
return lido;
};

void printtData(tData data){
	//Imprime na saida padrao a data formatada
	printf("%d-%d-%d\n", data.ano, data.mes, data.dia);
};

int CasosentreDatas(tData inicio, tData fim, int indicepaciente){
	//cria um tData com a DataCadastro do indice recebido na main
	tData datapaciente = pessoa[indicepaciente].DataCadastro; 
	//verifica se a data esta entre o intevalo passado na funcao
	if(VerificaData(datapaciente, inicio, fim)){
		//verifica se o paciente esta com covid caso sim returna 1
		if(pessoa[indicepaciente].Classificacao){
			return 1;
		}
	}	
return 0;
};

//Funcao para popular todas as variaveis do struct
void ZeraCidades(tCidade* Municipios){
    int i;

    for(i=0; i<QTDMUNICIPIOS; i++){
		//Popula todas as variaveis cidade do estruct com "void"
        strcpy(Municipios[i].Cidade, "void");
		//Zera todos os casos da cidade
        Municipios[i].QtdCasos = 0;
        Municipios[i].QtdCasosEntreDatas = 0;
    }
};

//Funcao que adiciona o municipio no vetor caso ele nao esteja
//E adiciona mais 1 caso confimado
void AdicionaMunicipioseCasos(tCidade* Municipios, int Indice){
    int i, retornado = VerificaseCidadecadastrada(Municipios, Indice);

	//Retornado com valor -1 significa que o municipio ainda nao foi adicionado ao vetor
    if(retornado==-1){
		//Como o municipio ainda nao estava salvo ele e adicionado e somado mais 1 aos casos confirmados
        for(i=0;i<QTDMUNICIPIOS;i++){
            if(strcmp(Municipios[i].Cidade, "void") == 0){
                strcpy(Municipios[i].Cidade, pessoa[Indice].Municipio);
                Municipios[i].QtdCasos++;
                break;
            }
        }
    }
    else{
		//Adiciciona mais um aos casos confirmados dos municipios os quais ja estao salvos
        Municipios[retornado].QtdCasos++;
    }    
};

int VerificaseCidadecadastrada(tCidade municipios[], int Indice){
    int i;

	//Busca dentro do vetor municipios se o municipio em questao ja foi adicionado
    for(i=0;i<QTDMUNICIPIOS;i++){
		//Retorna o indice onde o municipio ja esta caso ele esteja salvo
        if(strcmp(pessoa[Indice].Municipio,municipios[i].Cidade) == 0){
            return i;
        }
		//Retorna -1 caso o municipio nao esteja salvo
        if(strcmp(municipios[i].Cidade, "void") == 0){
            return -1;
        }
    }
return 0;
};
