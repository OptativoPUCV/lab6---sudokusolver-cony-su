#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode()
{
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n)
{
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

/*
No se repitan números en las filas
No se repitan números en las columnas
No se repitan números en las submatrices de 3x3
Si el estado es válido la función retorna 1, si no lo es retorna 0.

Para marcar los números que vayan apareciendo en una fila/columna/submatriz puede usar un arreglo de enteros de largo 10 inicializado con 0s. Cada vez que aparezca un número i, verifique que la casilla i del arreglo sea igual a 0, luego márquela con un '1'. Si la casilla es '1' quiere decir que el número ya estaba marcado por lo que la fla/columna/submatriz no es válida.
*/


int is_valid(Node* n)
{

   for(int i = 0; i < 9; i+=3) //recorro las filas de 3 en 3
      {
         for(int j = 0; j < 9; j+=3) //recorro las columnas de 3 en 3
         {
            int contador_submatriz[10]= {0}; //inicializo el arreglo de contadores
            for(int sub_i = 0; sub_i < 3; sub_i++) //recorro las filas de la submatriz
            {
               for(int sub_j = 0; sub_j < 3; sub_j++) //recorro las columnas de la submatriz
               {
                  int num = n->sudo[i+sub_i][j+sub_j]; //obtengo el numero de la submatriz
                  contador_submatriz[num]++; //incremento el contador del numero
                  if(contador_submatriz[num] > 1 && num != 0) return 0; //si el numero se repite, la submatriz no es valida
               }
            }
         }
      }

   for (int k = 0; k < 9; k++) 
   {
      int contador_filas[10] = {0};
      int contador_columnas[10] = {0};
      for (int x = 0; x < 9; x++) 
      {
         int num_fila = n->sudo[k][x];
         int num_columna = n->sudo[x][k];
         contador_filas[num_fila]++;
         contador_columnas[num_columna]++;
         if ((contador_filas[num_fila] > 1 && num_fila != 0) || (contador_columnas[num_columna] > 1 && num_columna != 0)) return 0;
       }
   }


   
    return 1;
}

/*
1.Cree una función que a partir de un nodo genere una lista con los nodos adyacentes:

Recuerde que los nodos adyacentes son generados aplicando las acciones al estado actual.

Para el caso del ejemplo, la función debería retornar una lista con 9 nodos. Cada uno de ellos idéntico al nodo original pero cambiando el valor de la primera casilla vacía, es decir: sudo[0][2], por 1, 2, ..., 9.
Utilice la función Node* copy(Node* n) para copiar nodos


Modifique la función get_adj_nodes para que sólo los nodos válidos sean retornados (use la función is_valid).
*/

List* get_adj_nodes(Node* n)
{

    List* list = createList();
    int i, j;
   //recorro la matriz
   for (i = 0; i < 9; i++)
   {
      for(j = 0; j < 9; j++)
      {
         if(n->sudo[i][j] == 0) //si encuentro un 0
         {
            for(int k = 1; k < 10; k++) //recorro los numeros del 1 al 9
            {
               //creo un aux que es una copia del nodo
               n->sudo[i][j] = k; //le asigno el valor k en donde habia un 0
               Node *aux = copy(n);
               if(is_valid(aux)) pushBack(list, aux); //agregel nodo a la lista
            }
         }
      }
   }
    return list;
}

/*
Implemente la función int is_final(Node * n). Esta función retorna 1 si el nodo corresponde a un nodo final (es decir, todos los valores de la matriz son distintos a 0) y 0 en caso contrario.

Ya está listo el grafo implícito, ahora necesitamos construir un método de búsqueda que a partir del estado inicial sea capaz de explorar el grafo de manera sistemática hasta alcanzar un nodo final.
*/


int is_final(Node* n)
{
   int i, j;
   for(i = 0; i < 9; i++)
   {
      for(j = 0; j < 9; j++)
      {
         if(n->sudo[i][j] == 0)return 0;
      }
   }  
    return 1;
}

/*
   Implemente la función Node* DFS(Node* n, int* cont). Esta función realiza una búsqueda en profundidad a partir del nodo n. El algoritmo es el siguiente:
   Cree un stack S (pila) e inserte el nodo.
   Mientras el stack S no se encuentre vacío:
   a) Saque y elimine el primer nodo de S.
   b) Verifique si corresponde a un estado final, si es así retorne el nodo.
   c) Obtenga la lista de nodos adyacentes al nodo.
   d) Agregue los nodos de la lista (uno por uno) al stack S.
   e) Libere la memoria usada por el nodo.
   Si terminó de recorre el grafo sin encontrar una solución, retorne NULL.
   Almacene en la variable cont, la cantidad de iteraciones que realiza el algoritmo.
   Puede ver un código de ejemplo en las diapos.
   Recuerde revisar las operaciones del TDA Stack en el archivo list.h.
*/

Node* DFS(Node* initial, int* cont)
{
   Stack* S = createStack();
   push(S, initial);
   int contador = 0;
   while(!is_empty(S))
   {
      Node* n = top(S);
      pop(S);
      if(is_final(n)) return n;
      else
      {
         List *lista = get_adj_nodes(n);
         Node* aux = first(lista);
         while(aux != NULL)
         {
            push(S, aux);
            aux = next(lista);
         }
         free(aux);
      }
      contador++;
   }

   *cont = contador;
  return NULL;
}



/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}*/