#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct Vertice{
    double x;
    double y;
    struct Vertice *prox;
};
typedef struct Vertice* Poligono;

int NumeroDeVertices(Poligono P){
    int num = 0;
    Poligono T = P;
    do{
        num ++;
        T = T->prox;
    } while(T != P);
    return num;
}

double Distancia(struct Vertice *A, struct Vertice *B){
    double raiz, soma, potx, poty;
    potx = pow((A->x - B->x), 2);
    poty = pow((A->y - B->y), 2);
    raiz = sqrt(potx + poty);
    return raiz;    
}

double Perimetro(Poligono P){
    Poligono A, B; 
    int i, numvert;
    double *D; //armazena a distancia entre dois pontos
    double soma = 0;
    numvert = NumeroDeVertices(P);
    D = (double*)malloc(sizeof(double)*numvert);
    A = P;
    B = P->prox;
    for (i = 0; i < numvert; i++){
        D[i] = Distancia(A, B);
        A = B;
        B = B->prox;
        soma = soma + D[i];
    }
    free(D);
    return soma;
}

double Area(Poligono P){
    Poligono A, B;
    int i, numvert;
    double *D, *J; //armazena a distancia entre dois pontos
    double S, area; 
    double soma = 0;
    numvert = NumeroDeVertices(P);
    D = (double*)malloc(sizeof(double)*numvert);//ida
    J = (double*)malloc(sizeof(double)*numvert);//volta negativa
    A = P;
    B = P->prox;
    for (i = 0; i < numvert ; i++){
        D[i] =  (A->x * B->y);
        J[i] = -(A->y * B->x);
        soma = soma + (D[i]+J[i]);
        A = B;
        B = B->prox;
    }
    area = soma / 2; 
    free(D);
    free(J);   
    return area;
}
void LiberaPoligono(Poligono P){
    Poligono q;
    while (P != NULL){
        q = P->prox;
        if (P == q){
            P = NULL;
        }
        else{
            P->prox = q->prox;
        }
        free(q);
    }
}

Poligono Insere(Poligono p, double x, double y){
    Poligono q;
    q = (Poligono)malloc(sizeof(struct Vertice));
    q->x = x;
    q->y = y;
    if (p == NULL){
        q->prox = q;
        p = q;
    }
    else
    {
        q->prox = p->prox;
        p->prox = q;
        p = q;
    }
    return p;
}

int main(){
    int vertices;
    double x, y, perimetro, area;
    double *D;
    int i;
    Poligono p = NULL;
    printf("Vertices: ");
    scanf(" %d", &vertices);
    for(i = 0; i < vertices; i++){
        printf("Coordenadas: ");
        scanf(" %lf %lf", &x, &y);
        p = Insere(p, x, y);
    }
    perimetro = Perimetro(p);
    area = Area(p);
    printf("Perimetro: %.1lf \nArea: %.1lf\n", perimetro, area);
    LiberaPoligono(p);
    return 0;
}