#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_COMMAND_LENGTH 14
#define MAX_NUM 4294967295

typedef enum { false, true } bool;

//struttura dati principale, ad albero
typedef struct {
    unsigned long index;
    unsigned long sum_paths;
}element;

typedef struct minHeap{
    unsigned long size;
    element *elem;
    unsigned long maxSize;
}minHeap;

unsigned long d, k, indice=0;
bool fine = false;
minHeap Heap;
long **adj_matrix;
char *command;

void swap(unsigned long a, unsigned long b){
    element tmp = Heap.elem[a];
    Heap.elem[a] = Heap.elem[b];
    Heap.elem[b] = tmp;
}


unsigned long parent(unsigned long pos){
    return pos/2;
}

void printTopK(){
    if(Heap.size==0){
        printf("\n");
    }
    else {
        if (k <= Heap.size) {
            for (int i = 0; i < k; i++) {
                if (i != k - 1) {
                    printf("%lu ", Heap.elem[i].index);
                } else {
                    printf("%lu", Heap.elem[i].index);
                    printf("\n");
                }
            }
        } else {
            unsigned long size = Heap.size;
            for (int i = 0; i < size; i++) {
                if (i != size - 1) {
                    printf("%lu ", Heap.elem[i].index);
                } else {
                    printf("%lu", Heap.elem[i].index);
                    printf("\n");
                }
            }
        }
    }

}

bool containsKey(unsigned long key){
    for(int i=0; i<k; i++){
        if(Heap.elem[i].sum_paths==key){
            return true;
        }
    }
    return false;
}
unsigned long  availableIn(unsigned long key){
    for(unsigned long i=k-1; i>0; i--){
        if(Heap.elem[i].sum_paths>key){
            return i;
        }
    }
    return -1;
}

unsigned long maxEl(){
    unsigned long i=0;
    unsigned long tmp = 0;
    for(int n=0; n<k; n++){
        if(tmp<Heap.elem[n].sum_paths){
            tmp = Heap.elem[n].sum_paths;
            i=n;
        }
    }
    return i;
}

void insert_Node(unsigned long ind, long minDis) {
    if(Heap.size==Heap.maxSize) {
        Heap.elem[k].index = ind;
        Heap.elem[k].sum_paths= minDis;
        if((containsKey(minDis) && Heap.elem[k-1].sum_paths!= minDis) || availableIn(minDis)!=-1){
            unsigned long m = maxEl();
            Heap.elem[m].sum_paths = Heap.elem[k].sum_paths;
            Heap.elem[m].index = Heap.elem[k].index;
            unsigned long current = m;

            while (Heap.elem[current].sum_paths < Heap.elem[parent(current)].sum_paths) {
                swap(current, parent(current));
                current = parent(current);
            }
        }

    } else {
        Heap.elem[Heap.size].index = ind;
        Heap.elem[Heap.size].sum_paths = minDis;
        unsigned long current = Heap.size++;

        while (Heap.elem[current].sum_paths < Heap.elem[parent(current)].sum_paths) {
            swap(current, parent(current));
            current = parent(current);
        }
    }

}

long extractMin(const long a[d], const bool set[d]){
    long min = MAX_NUM, min_index=0;

    for (int v = 0; v < d; v++)
        if (set[v] == false && a[v] <= min )
            min = a[v], min_index = v;

    return min_index;
}

long calcMin() {
    long tmpMatrix[d][d];
    long dist[d];
    long distMin = 0;
    bool sptSet[d];
    memcpy(tmpMatrix, adj_matrix, d*d* sizeof(long));
    for (int i = 0; i < d; i++)
        dist[i] = MAX_NUM, sptSet[i] = false;

    dist[0] = 0;
    for (int count = 0; count < d - 1; count++) {
        long u = extractMin(dist, sptSet);

        sptSet[u] = true;

        for (int v = 0; v < d; v++)

            if (!sptSet[v] && adj_matrix[u][v] && dist[u] != MAX_NUM
                && dist[u] + adj_matrix[u][v] < dist[v])
                dist[v] = dist[u] + adj_matrix[u][v];
    }

    for(int i=0; i<d; i++){
        if(dist[i]==MAX_NUM){
            dist[i]=0;
        }
        distMin += dist[i];
    }
    return distMin;
}

void saveGraph(unsigned long index) {
    long minDis = calcMin(); //mi calcolo la somma dei cammini minimi
    insert_Node(index, minDis);

}
//OK
//legge il grafo riga per riga e lo salva in una matrice
void parseGraph(unsigned long index){
    int j=0;
    char *pEnd;
    int ch;
    int m=0;
    //continuo a leggere tante volte quanti sono i nodi perchè corrispondono alle righe
    // i indica le righe

    for(int i=0; i<d; i++){//leggo il primo carattere della riga
        ch = getchar_unlocked();
        while(ch==10 || ch==32){
            ch = getchar_unlocked(); // vado avanti a leggere finchè non trovo una nuova riga
        }
        char token[10];
        while(ch!=10){     //finchè non vado a capo continuo a leggere caratteri
            if(ch!=44){           //se è diverso dalla virgola lo salvo
                switch (ch) {
                    case 48:
                        token[m] = '0';
                        break;
                    case 49:
                        token[m] = '1';
                        break;
                    case 50:
                        token[m] = '2';
                        break;
                    case 51:
                        token[m] = '3';
                        break;
                    case 52:
                        token[m] = '4';
                        break;
                    case 53:
                        token[m] = '5';
                        break;
                    case 54:
                        token[m] = '6';
                        break;
                    case 55:
                        token[m] = '7';
                        break;
                    case 56:
                        token[m] = '8';
                        break;
                    case 57:
                        token[m] = '9';
                        break;
                    default:
                        token[m] = 'x';

                }
                m++;
                ch = getchar_unlocked();
            }
            else {
                long var = strtol(token, &pEnd, 10); //aggiungo token alla mia matrice
                adj_matrix[i][j] = var;
                j++;
                for(int u=0; u<m; u++){
                    token[u]=0;
                }
                m = 0;
                ch = getchar_unlocked(); //cambio colonna, ma resto sulla riga finchè non esco dal while
            }

        }

        adj_matrix[i][j] = strtol(token, &pEnd, 10);
        for(int u=0; u<m; u++){
            token[u]=0;
        }
        m = 0;
        j=0;
    }
    //quando ho finito di leggere tutte le righe salvo il grafo nel mio albero
    saveGraph(index);
}


//OK
void parseCommand(){
    if(fgets(command, MAX_COMMAND_LENGTH, stdin)!=NULL) {
        if (strcmp(command, "AggiungiGrafo") == 0) {
            parseGraph(indice);  //legge il grafo
            indice++; //tiene conto dell'indice del nuovo grafo
        } else if (strcmp(command, "TopK\n") == 0) {
            printTopK();
        }
    }
    else{
        fine=true;
    }

}

minHeap initMinHeap(){
    minHeap hp;
    hp.size=0;
    return hp;
}

//OK
//main del mio programma, leggo la prima riga e salvo d e k.
int main() {
    int i=0;
    char *pEnd;
    char tmp1[10];
    char tmp2[10];
    char requires[22];
    Heap = initMinHeap();
    bool avanti = false;
    command = malloc(MAX_COMMAND_LENGTH* sizeof(char ));
    while(!avanti) {
        if (fgets(requires, 22, stdin) != NULL) {
            while (requires[i] !=
                   ' ') {  //legge e salva in un array temporaneo fino a che non incontra uno spazio vuoto
                tmp1[i] = requires[i];
                i++;
            }
            tmp1[i] = '\0';
            i++;

            d = strtol(tmp1, &pEnd, 10); // trasforma un array di caratteri (numeri) in numero in base 10
            adj_matrix = malloc(d * sizeof(long *));

            for (int m = 0; m < d; m++) {
                adj_matrix[m] = malloc(d * sizeof(long));
            }

            int h = 0;
            while (requires[i] != '\n') {    //legge e salva in un array temporaneo fino a che non finisce la riga
                tmp2[h] = requires[i];
                i++;
                h++;
            }
            tmp2[h] = '\0';
            k = strtol(tmp2, &pEnd, 10);  // trasforma un array di caratteri (numeri) in numero in base 10
            Heap.elem = malloc((k + 1) * sizeof(element *));
            Heap.maxSize = k;
            avanti=true;
        }
    }
    while(!fine){
        parseCommand();//legge il comando fino a che ce ne sono
    }
    return 0;
}
