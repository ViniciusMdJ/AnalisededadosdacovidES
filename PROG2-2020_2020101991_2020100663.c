#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <sys/stat.h>

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
//verifica item 7
int VeridicaIntem7(int indice, tData inicio7, tData fim7);
//verifica se nao tem nenhuma comorbidade, se nao tem retorna 1, senao 0 
int VerificaComorbidade(int indice);
//Funcao para adicionar os casos comfirmados entre um certo intervalo
int CasosentreDatas(tData inicio, tData fim, int indicepaciente);
//Verifica se a cidade ja foi adicionada no vetor municipios
int VerificaseCidadecadastrada(tCidade municipios[], int Indice);

//retorna o quadrado da subtracao da media pela idade do paciente
float SomaDesvio(float media, int indice);

//Ordena o vetor de municipios em ordem alfabetica para item 3
void OrdemAlfabetica(tCidade* Municipios);
//Ordena o vetor de municipios em ordem descrescente em relacao aos casos entre datas do item 5
void OrdemCasosEntreDatas(tCidade* Municipos);
//Funcao para zerar as variaveis do struct tCidade
void ZeraCidades(tCidade* Municipios);
//Funcao para adicionar os casos confirmados do municipio entre as datas dadas do item 5
void AdicionaMunicipioseCasosEntreDatas(tCidade* Municipios, int Indice);
//Funcao para adicionar os casos confirmados do municipio
void AdicionaMunicipioseCasos(tCidade* Municipios, int Indice);
//Funcao para ler datas
tData LetData();
void ImprimeItem3(tCidade* Municiopios, int mincasos, char* caminho);
void ImprimeItem4(int totalconfirmados, char* caminho);
void ImprimeItem5(tCidade* Municipios, int topcidades, char* caminho);
void ImprimeItem6(float internadas6, float morreram6, float internadasMorreram,
		char* camiho, char* cidade);
void ImprimeItem7(float media, float desvioPadrao, float semcomorbidades,
		char* caminho);

//cria o vetor de pacientes globalmente
tPaciente pessoa[LINHAS];           

int main(){
    //cria o ponteiro do arquivo
    FILE *arq;

    //cria as variaveis usadas no programa
	char caminho[15];
	char cidade[40];
    int i, mincasos, topcidades, totalconfirmados = 0;
    int morreu, ficouinternado;
    float por100internadas, por100mortes, por100internadasMortes;
    float confirmadas6=0, internadas6=0, morreram6=0, internadasMorreram=0;
    float media=0, quantidade7=0, somatorio=0, zeroComorbidades=0, desvioPadrao, por100item7; 
	//Variaveis para cada item que possui intervalo de datas
	tData inicio4, fim4, inicio5, fim5, inicio7, fim7;
    //vetor de todas as cidades do estado
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
    //transforma os caracteres minusculos pra maiusculo do item 6
    for(i=0;cidade[i]!='\0';i++){
        cidade[i]= toupper(cidade[i]);
    }

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
            //verifica se o paciente eh compativel com os requisitos do item 7
            if(VeridicaIntem7(i, inicio7, fim7)){
                //soma a idade do paciente para a media
                media += pessoa[i].IdadeNotificacao;
                //quantidade de pessoas compativeis com o item 7
                quantidade7++;
            }
        }
    }
    //fechando o arquivo para que nao haja vazamento de memoria
    fclose(arq);

    //media para o desvio padrao
    media /= quantidade7;

    //zera as variaveis dentro do struct, a cidade com void e os casos com 0
    ZeraCidades(Municipios);

    //verificando os itens pedidos
	for(i=0; i<LINHAS; i++){
		//Verifica se a pessoa teve covid
        if(pessoa[i].Classificacao){
            //item 3
            AdicionaMunicipioseCasos(Municipios, i);
            //item 4
            if(CasosentreDatas(inicio4, fim4, i)){
			    totalconfirmados++;
		    }
            //item 5
            if(CasosentreDatas(inicio5, fim5, i)){
                AdicionaMunicipioseCasosEntreDatas(Municipios, i);
            }
            //item 6
            /*
            caso a cidade requerida do item 6 for TODAS sempre sera verdadeira para todas as iteracoes,
            caso contrario ele compara somente a cidade solicitada, pois a segunda comparacao sera sempre falsa
            */
            if(strcmp(cidade,pessoa[i].Municipio)==0 || strcmp(cidade,"TODAS")==0){
                morreu = pessoa[i].DataObito.ano;
                ficouinternado = pessoa[i].FicouInternado;
                //quantidade de pessoas confirmadas da cidade solicitada
                confirmadas6++;
                if(ficouinternado){
                    internadas6++;
                }
                if(morreu){
                    morreram6++;
                }
                if(morreu && ficouinternado){
                    internadasMorreram++;
                }
            }
            //item 7
            //verifica se atende os requisitos do item 7
            if(VeridicaIntem7(i, inicio7, fim7)){
                somatorio += SomaDesvio(media, i);
                //verifica se nao tem nenhuma comorbidade
                if(VerificaComorbidade(i)){
                    zeroComorbidades++;
                }

            }
        }

	}

	//Cria o diretorio onte serao armazenados os arquivos .txt
	mkdir(caminho, 0777);		

	
    OrdemAlfabetica(Municipios);
    //chama a função pra imprimir o item 3
	ImprimeItem3(Municipios, mincasos, caminho);
    //chama a função pra imprimir o item 4
	ImprimeItem4(totalconfirmados, caminho);
    OrdemCasosEntreDatas(Municipios);
    //chama a função pra imprimir o item 5
	ImprimeItem5(Municipios, topcidades, caminho);
	//Calcula as porcentagens do item 6 
    por100internadas = (internadas6/confirmadas6)*100;
    por100mortes = (morreram6/confirmadas6)*100;
    por100internadasMortes = (internadasMorreram/morreram6)*100;
    //chama a funcao pra imprimir o item 6
	ImprimeItem6(por100internadas, por100mortes, por100internadasMortes,
			caminho, cidade);
    desvioPadrao = sqrt(somatorio/(quantidade7-1));
    por100item7 = (zeroComorbidades/quantidade7)*100;
    //chama a funcao pra imprimir o item 7	
	ImprimeItem7(media, desvioPadrao, por100item7, caminho);

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
    //tranforma em um numero do tipo aaaammdd
	start = (inicio.ano * 10000) + (inicio.mes *100) + inicio.dia;
	end = (fim.ano * 10000) + (fim.mes *100) + fim.dia;
	entre = (pessoas.ano * 10000) + (pessoas.mes *100) + pessoas.dia;

    //verifica se esta entre as datas
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

int CasosentreDatas(tData inicio, tData fim, int indicepaciente){
	//cria um tData com a DataCadastro do indice recebido na main
	tData datapaciente = pessoa[indicepaciente].DataCadastro; 
	//verifica se a data esta entre o intevalo passado na funcao
	if(VerificaData(datapaciente, inicio, fim)){
	    return 1;
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

void OrdemAlfabetica(tCidade* Municipios){
    int k, j;
    tCidade aux;
    //algoritimo bubble sort para colocar em ordem alfabetica
    for (k = 0; k < QTDMUNICIPIOS - 1; k++) {
        for (j = 0; j < QTDMUNICIPIOS - k - 1; j++) {
            
            if (strcmp(Municipios[j].Cidade, Municipios[j+1].Cidade) > 0) {
                //inverte as vatiaveis
                aux = Municipios[j];
                Municipios[j] = Municipios[j+1];
                Municipios[j+1] = aux;
            }
        }
    }
};

void AdicionaMunicipioseCasosEntreDatas(tCidade* Municipios, int Indice){
    int i;
    for(i=0;i<QTDMUNICIPIOS;i++){
        //adiciona um caso pro item 5 na cidade do paciente
        if(strcmp(pessoa[Indice].Municipio,Municipios[i].Cidade) == 0){
            Municipios[i].QtdCasosEntreDatas++;
            break;
        }
    }
};

void OrdemCasosEntreDatas(tCidade* Municipios){
    int k, j;
    tCidade aux;
    //algoritimo bubble sort para colocar em ordem decrescente em relacao a quantidade de casos entre as datas
    for (k = 0; k < QTDMUNICIPIOS - 1; k++) {
        for (j = 0; j < QTDMUNICIPIOS - k - 1; j++) {

            if (Municipios[j].QtdCasosEntreDatas < Municipios[j+1].QtdCasosEntreDatas){
                aux = Municipios[j];
                Municipios[j] = Municipios[j+1];
                Municipios[j+1] = aux;
            }
        }
    }
};

int VeridicaIntem7(int indice, tData inicio7, tData fim7){
    //verifica se foi confirmado covid19
    if(pessoa[indice].Classificacao){
        //verifica se esta entre as datas do item 7
        if(CasosentreDatas(inicio7, fim7, indice)){
            //verifica se o paciente morreu
            if(pessoa[indice].DataObito.ano != 0){
                return 1;
            }
        }
    }

};

float SomaDesvio(float media, int indice){
    float result;
    //quadrado da subtracao da media pela idade do paciente
    result = (pessoa[indice].IdadeNotificacao - media) * (pessoa[indice].IdadeNotificacao - media);
    return result;
};

int VerificaComorbidade(int indice){
    //verifica se tem alguma comorbidade
    if(pessoa[indice].ComorbidadeCardio || pessoa[indice].ComorbidadeDiabetes || pessoa[indice].ComorbidadeObesidade
    || pessoa[indice].ComorbidadePulmao || pessoa[indice].ComorbidadeRenal || pessoa[indice].ComorbidadeTabagismo){
        return 0;
    }
    //retorna TRUE se nao tiver nenhuma comorbidade
    return 1;
};

void ImprimeItem3(tCidade* municipio, int mincasos, char* caminho){
	int i;
	//Cria a string path onde sera salvo o caminho para o arquivo
	char path[30];
	//copia o nome do diretorio para que possa ser adicionado o nome do txt
	strcpy(path, caminho);
	//Concatena o nome do arquivo no caminho
	strcat(path, "item3.txt");
	//Cria o arquivo
	FILE* arq;
	arq = fopen(path, "w+");
	if(arq == NULL){
		printf("Erro ao criar item3.txt");
		exit(1);
	}
	//Imprime as cidades com a quantidades de casos maior do que mincasos
	for(i = 0; i < QTDMUNICIPIOS; i++){
		if(municipio[i].QtdCasos > mincasos){
			fprintf(arq, "- %s: %d casos\n",
			municipio[i].Cidade, municipio[i].QtdCasos);
		}
	}
	fclose(arq);
};
void ImprimeItem4(int totalconfirmados, char* caminho){
	//Cria a string path onde sera salvo o caminho para o arquivo
	char path[30];
	//Copia o nome do diretorio para que possa ser adicionado o nome do txt
	strcpy(path, caminho);
	//Concatena o nome do arquivo no caminho
	strcat(path, "item4.txt");
	//Cria o arquivo
	FILE* arq;
	arq = fopen(path, "w+");
	if(arq == NULL){
		printf("Erro ao criar item4.txt");
		exit(1);
	}
	//Imprime no arquivo o total de casos confirmados entre as datas
	fprintf(arq, "- Total de pessoas: %d", totalconfirmados);
	fclose(arq);
};
void ImprimeItem5(tCidade* municipios, int topcidades, char* caminho){
	int i;
	//Cria a string path onde sera salvo o caminho para o arquivo
	char path[30];
	//Copia o nome do diretorio para que possa ser adicionado o nome do txt
	strcpy(path, caminho);
	strcat(path, "item5.txt");
	//Cria o arquivo
	FILE* arq;
	arq = fopen(path, "w+");
	if(arq == NULL){
		printf("Erro ao criar item5.txt");
		exit(1);
	}
	//Imprime no arquivo as topcidades em ordem decrescente durante certo
	//intervalo de datas
	for(i = 0; i < topcidades; i++){
		fprintf(arq, "- %s: %d casos\n",
			municipios[i].Cidade, municipios[i].QtdCasosEntreDatas);	
	}
	fclose(arq);
};
void ImprimeItem6(float por100internadas, float por100mortes,
		float por100internadasMortes, char* caminho, char* cidade){
	//Cria a string path onde sera salvo o caminho para o arquivo
	char path[30];
	//Copia o nome do diretorio para que possa ser adicionado o nome do txt
	strcpy(path, caminho);
	strcat(path, "item6.txt");
	//Cria o arquivo
	FILE* arq;
	arq = fopen(path, "w+");
	if(arq == NULL){
		printf("Erro ao criar item5.txt");
		exit(1);
	}
	//Imprime no arquivo a cidade solicitada e as porcentagens de internados,
	//mortos e internados que vieram a obito
	fprintf(arq, "- Resultados para %s:\n", cidade);
	fprintf(arq, "- A %% de pessoas com Covid-19 que ficaram internadas: %.3f%%\n",
			por100internadas);
	fprintf(arq, "- A %% de pessoas com Covid-19 que morreram: %.3f%%\n",
			por100mortes);
	fprintf(arq, "- A %% de pessoas que ficaram internadas e morreram: %.3f%%",
			por100internadasMortes);
	fclose(arq);
	};
void ImprimeItem7(float media, float desvioPadrao, float por100item7,
	char* caminho){
	int i;
	//Cria a string path onde sera salvo o caminho para o arquivo
	char path[30];
	//Copia o nome do diretorio para que possa ser adicionado o nome do txt
	strcpy(path, caminho);
	strcat(path, "item7.txt");
	//Cria o arquivo
	FILE* arq;
	arq = fopen(path, "w+");
	if(arq == NULL){
		printf("Erro ao criar item5.txt");
		exit(1);
	}
	//Imprime no arquivo a media e o desvio padrao das idades
	fprintf(arq, "A media e desvio padrao da idade: %.3f -- %.3f\n", 
			media, desvioPadrao);
	//Imprime no arquivo a porcentagem de pessoas que morreram sem comorbidades
	fprintf(arq, "A %% de pessoas que morreram sem comorbidade: %.3f%%",
			por100item7);
	fclose(arq);
	};
