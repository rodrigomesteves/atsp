#include <stdio.h>
#include <string.h>
#include <math.h>
#include "read_file.c"

#define INF 0x7F800000

/*Como não criei uma rotina para ler os arquivos, estou definindo aqui o tamanho da
matriz e inicializando ela dentro do código*/

int n_visited = 0;				//Vetor auxiliar para guardar os nós ja visitados
long size_of_path = 0;
char name_of_file[50];
int dimention;


//A função print_matrix printa matrix carregada no problema
void print_matrix(int s, int dim, int m[dim][dim]){
  for(int i = 0; i < s; i++){
    for(int j = 0; j < s; j++){
      printf("-- %i", m[i][j]);
    }
    printf("\n");
  }
}


//A função print_visited printa os nós que ja foram visitados.
void print_visited(int dim, int visited[dim]){
  for(int i =0; i<=n_visited; i++){
    printf("%i\n", visited[i]);
  }
}


//A função insert_into_visited insere um nós no vetor de visitados
void insert_into_visited(int elem, int dim, int visited[dim]){
  if(!was_element_visited(elem)){
    visited[n_visited] = elem;
  }
}



//Essa função checa se determinado nó já foi visitado
int was_element_visited(int elem, int dim, int visited[dim]){
  for(int i =0; i<=n_visited; i++){
    if (elem == visited[i])
	return 1;
  }
  return 0;
}


int main()
{
  printf("Digite o nome do arquivo\n");
  scanf("%s", name_of_file);
  dimention = get_dimention(name_of_file);
  int m[dimention][dimention];
  file_to_matrix(name_of_file, dimention, m);
  int visited[dimention];

  //Inicializa o vetor de visitados com um valor que nunca será usado
  for(int k = 0; k< dimention; k++){
    visited[k] = INF;
  }

  int shorter = INF;
  int first_i = 0;
  int current_i_1 = first_i;
  int current_i_2 = first_i;
  int next_i = first_i;
  int *current_v_1 = m[current_i_1];
  int *current_v_2 = m[current_i_2];


  insert_into_visited(first_i, dimention, visited);

  //Esse FOR realiza um número de iterações igual ao número de nós totais.
  for(int k = 0; k< dimention; k++){


    //Se for a ultima iteração, volte ao nó inicial
    if(k==dimention-1){
      if(m[current_i_1][current_i_2] < m[current_i_2][current_i_1])
        shorter = m[current_i_1][current_i_2];
      else
        shorter = m[current_i_2][current_i_1];
      size_of_path += shorter;
      //printf("\nDe %i para %i com custo %i\n\n",current_i_1,current_i_2, shorter);
    }

    //Se não for a ultima iteração, continue
    else{
      int shorter = 0x7F800000;
      int direction = 0;

      int aux_i_1 = -1;
      int aux_s_1 = 0x7F800000;

      int aux_i_2 = -1;
      int aux_s_2 = 0x7F800000;

      //Esse FOR checa o caminho mais barato em um dos nós extremos
      for(int i = 0; i < dimention; i++){
        if(current_v_1[i] < aux_s_1 && current_i_1!=i && !was_element_visited(i,dimention,visited)){
          aux_s_1 = current_v_1[i];
          aux_i_1 = i;
        }
      }

      //Esse FOR checa o caminho mais barato no outro nó extremo
      for(int i = 0; i < dimention; i++){
        if(current_v_2[i] < aux_s_2 && current_i_2!=i && !was_element_visited(i,dimention,visited)){
          aux_s_2 = current_v_2[i];
          aux_i_2 = i;
        }
      }

    //printf("Opcao 1: Ir de %i para %i com custo %i\n", current_i_1,aux_i_1, aux_s_1);
    //printf("Opcao 2: Ir de %i para %i com custo %i\n", current_i_2,aux_i_2, aux_s_2);

      //Se os dois nós extremos possuem caminhos viáveis, escolha o menor.
      if(aux_i_1 != -1 && aux_i_2 != -1){
        if(aux_s_1 <= aux_s_2){
          shorter = aux_s_1;
          next_i = aux_i_1;
          direction = 1;
        }
        else{
          shorter = aux_s_2;
          next_i = aux_i_2;
          direction =2;
        }
      }

      //Se só um dos nós possuir caminhos viáveis, use ele.
      else if(aux_i_1 == -1){
          shorter = aux_s_2;
          next_i = aux_i_2;
          direction =2;
      }

      else if(aux_i_2 == -1){
          shorter = aux_s_1;
          next_i = aux_i_1;
          direction = 1;
      }

      //print_visited();
      size_of_path += shorter;

      //De acordo com o nó do qual saimos para pegar a menor aresta, atualiza os nós atuais e insere o nó antigo na lista de visitados.
      if(direction == 1){
        current_v_1 = m[next_i];
        insert_into_visited(next_i, dimention, visited);
	n_visited++;
        printf("\nDe %i para %i com custo %i",current_i_1,next_i, shorter);
        current_i_1 = next_i;
      }
      else if(direction == 2){
        current_v_2 = m[next_i];
        insert_into_visited(next_i, dimention, visited);
	n_visited++;
        printf("\nDe %i para %i com custo %i",current_i_2,next_i, shorter);
        current_i_2 = next_i;
      }

    }
    //print_visited();
    //printf("\n");
  }
  printf("\nTamanho do caminho: %li\n", size_of_path);
  return 0;
}
