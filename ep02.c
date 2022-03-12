#include <stdio.h>
#include <stdlib.h>
#define MAX 100 

typedef struct RegPilha{
 int dado;
 struct RegPilha *prox;
} RegPilha;

typedef RegPilha* Pilha;

typedef enum boolean {false,true} bool;

RegPilha *AlocaRegPilha(){
 RegPilha* q;
 q = (RegPilha*)malloc(sizeof(RegPilha));
 if(q==NULL) exit(-1);
 return q;
}

Pilha CriaPilha(){
 Pilha p;
 p = AlocaRegPilha();
 p->prox = NULL;
 return p;
}

void LiberaPilha(Pilha p){
 RegPilha *q,*t;
 q = p;
 while(q!=NULL){
 t = q;
 q = q->prox;
 free(t);
 }
}

bool PilhaVazia(Pilha p){
 return (p->prox==NULL);
}

void Empilha(Pilha p, int x){
 RegPilha *q;
 q = AlocaRegPilha();
 q->dado = x;
 q->prox = p->prox;
 p->prox = q;
}

char Desempilha(Pilha p){
 RegPilha *q;
 int x;
 q = p->prox;
 if(q==NULL) exit(-1);
 x = q->dado;
 p->prox = q->prox;
 free(q);
 return x;
}

int Checagem(Pilha open, Pilha close){
    Pilha returnO, returnC;//retornar as letras a pilha analisada, em caso de erro no meio da palavra
    int A, B, aux, stop, nlinha, dtc;
    returnO = CriaPilha();
    returnC = CriaPilha();
    dtc = Desempilha(open);//recebe :
    nlinha = Desempilha(open);//recebe numero da linha
    stop = Desempilha(open);// recebe > ou espaco
    do{//analisando pares<>
        A = Desempilha(open);
        B = Desempilha(close);
        Empilha(returnO, A);
        Empilha(returnC, B);
        if(A + 32 == B || A - 32 == B){//regra 1
            while(!PilhaVazia(returnO)){//retorna a palavra para a pilha
                aux = Desempilha(returnO);
                Empilha(open, aux);
            }
            Empilha(open, stop);//devolve > ou espaco, necessario p saber qunado parar de desempilhar
            Empilha(open, nlinha);
            Empilha(open, dtc);//detector que identifica a linha da palavra
            while(!PilhaVazia(returnC)){//retorna a palavra para a pilha
                aux = Desempilha(returnC);
                Empilha(close, aux);
            }
            return 1;
        }
        else if(A != B){//regra 2
            while(!PilhaVazia(returnO)){//retorna a palavra para a pilha
                aux = Desempilha(returnO);
                Empilha(open, aux);
            }
            Empilha(open, stop);
            Empilha(open, nlinha);
            Empilha(open, dtc);
            while(!PilhaVazia(returnC)){//retorna a palavra para a pilha
                aux = Desempilha(returnC);
                Empilha(close, aux);
            } 
            return 2;
        }
    }while(!PilhaVazia(close));
    LiberaPilha(returnO);
    LiberaPilha(returnC);
    return 0;
}

void Violacao(Pilha open, Pilha close, int erro, int linha){
    Pilha auxiliar;
    int termo, Olinha;//Olinha, referenta a pilha open
    auxiliar = CriaPilha();//serve para inverter a palavra

    switch (erro){
        case 0:
        printf("Tags balanceadas\n");
        break;
        case 1:
        printf("Violacao da regra #1: \n");
        printf("Tags incompativeis (abertura '");
        //abaixo a palavra e desinvertida
        termo = Desempilha(open);//descarta os :, que identificam a linha
        Olinha = Desempilha(open);
        termo = Desempilha(open);//descarta o primeiro > ou espaco
        while (1){//printar palavra com erro 
            termo = Desempilha(open);
            if(termo == ' ')
                break;
            else if(termo == '>')
                break;
            else if(termo == ':')
                break;
            Empilha(auxiliar, termo); 
        }
        while (!PilhaVazia(auxiliar)){
            termo = Desempilha(auxiliar);
            printf("%c", termo);
        }
        printf("' na linha %d e fechamento '", Olinha);
        while(!PilhaVazia(close)){
            termo = Desempilha(close);
            Empilha(auxiliar, termo);
        }
        while (!PilhaVazia(auxiliar)){
            termo = Desempilha(auxiliar);
            printf("%c", termo);
        }
        printf("' na linha %d)\n", linha);
        LiberaPilha(auxiliar);
        break;
        case 2:
        printf("Violacao da regra #2: \n");
        printf("Tags incompativeis (abertura '");
        //abaixo a palavra e desinvertida
        termo = Desempilha(open);//descarta os :, que identificam a linha
        Olinha = Desempilha(open);
        termo = Desempilha(open);//descarta o primeiro > ou espaco
        while (1){//printar palavra com erro 
            termo = Desempilha(open);
            if(termo == ' ')
                break;
            else if(termo == '>')
                break;
            else if(termo == ':')
                break;
            Empilha(auxiliar, termo); 
        }
        while (!PilhaVazia(auxiliar)){
            termo = Desempilha(auxiliar);
            printf("%c", termo);
        }
        printf("' na linha %d e fechamento '", Olinha);
        while(!PilhaVazia(close)){
            termo = Desempilha(close);
            Empilha(auxiliar, termo);
        }
        while (!PilhaVazia(auxiliar)){
            termo = Desempilha(auxiliar);
            printf("%c", termo);
        }
        printf("' na linha %d)\n", linha);
        LiberaPilha(auxiliar);
        break;
        case 3:
        printf("Violacao da regra #3: \n");
        printf("Raiz adicional ('");
        linha++;//compensar o break q fez com que nao fosse comtabilizado a nova linha
        //abaixo a palavra e desinvertida
        while(!PilhaVazia(close)){
            termo = Desempilha(close);
            Empilha(auxiliar, termo);
        }
        while (!PilhaVazia(auxiliar)){
            termo = Desempilha(auxiliar);
            printf("%c", termo);
        }
        printf("' na linha %d)\n", linha);
        LiberaPilha(auxiliar);
        break;
    }
}

void Analise(FILE *fp){   
    int ctr, erro, aux;// ctr(caractere), aux(variavel auxiliar, para retornar letra)
    int raiz = 1, linha = 1, i = 0;
    Pilha open, close;
    open = CriaPilha();
    close = CriaPilha();
    Empilha(open, '>');//ponto de parada
    while(1){
        ctr = getc(fp);
        if(ctr == -1)
            break;
        if(ctr == 10){ //10 em ASCII significa \n, analise da raiz(se houver)
            i = 0;//ao detectar nova linha, permite q uma nova linha seja somada ao zerar i 
            ctr = getc(fp);//<
            if(ctr == '<'){
                aux = getc(fp);//"capturo" letra
                if(aux != '/'){
                    raiz ++;
                    if(raiz > 1){
                        while(aux != '>'){
                            if(aux == ' ')
                                break;
                            Empilha(close, aux);
                            aux = getc(fp);
                        }
                        erro = 3;
                        break;//viola regra 3
                    }
                }
                ungetc(aux, fp);//"devolve" letra
            }
        }
        if(i == 0){
            linha++;
            i++;
        }
        if(ctr == '<'){//analise dos <>, da linha
            ctr = getc(fp);//pega proximo caractere
            if(ctr == '/'){//empilhar o fechamento
                ctr = getc(fp);//descarta a '/', 'captura' letra
                while(ctr != '>'){
                    Empilha(close, ctr);
                    ctr = getc(fp);
                }
            }
            else{//empilhar a abertura
                Empilha(open, ctr);
                while(ctr != '>'){
                    ctr = getc(fp);
                    if(ctr == ' '){
                        Empilha(open, ctr);
                        break;//sai do loop while
                    }
                    Empilha(open, ctr);
                }
                Empilha(open, linha);//identificar de qual linha a palavra vem
                Empilha(open, ':');
            }   
        }
        if(PilhaVazia(open) == PilhaVazia(close))
            erro = Checagem(open, close);
        if(erro == 1 || erro == 2)
            break;
    }
    Violacao(open, close, erro, linha);
    LiberaPilha(open);
    LiberaPilha(close);
}

int main(){
    FILE *arquivo;
    char nome [MAX];
    char versao [MAX];
    scanf("%s", nome);
    arquivo = fopen(nome, "r"); //abertura aquivo
    if(arquivo == NULL){
        printf("Erro na leitura\n");
        return 0;
    }
    fgets(versao, sizeof(versao), arquivo);//versao xml
    Analise(arquivo);
    fclose(arquivo);
    return 0;
}