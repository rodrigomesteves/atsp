#include <stdio.h>
#include <math.h>
#include "MT19937.c"
#include "read_file.c"

#define INF 0x7F800000

/*Como não criei uma rotina para ler os arquivos, estou definindo aqui o tamanho da
matriz e inicializando ela dentro do código*/

int n_visited = 0;
long size_of_path = 0;
float alpha = 0;
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
  //printf("%i\n", elem);
  for(int i =0; i<=n_visited; i++){
    if (elem == visited[i])
	return 1;
  }
  return 0;
}



int main()
{

 sgenrand(1337); //Gerador de números aleatorios Mersenne Twister

 printf("Digite o nome do arquivo\n");
 scanf("%s", name_of_file);
 dimention = get_dimention(name_of_file);
 int m[dimention][dimention];
 file_to_matrix(name_of_file, dimention, m);
 int visited[dimention];

 //Esse FOR abaixo randomiza o código, rodando o número de iterações especificadas na variavel num_it
 int num_it = 100;
 printf("DIMENTION: %d\n", dimention);



 for(int a =0; a<num_it; a++){
  size_of_path = 0;
  n_visited=0;

  //Inicializa o vetor de visitados com um valor que nunca será usado
  for(int k = 0; k< dimention; k++){
    visited[k] = INF;
  }


  int shorter = INF;
  int first_i = 0;
  int next_i = first_i;
  int *current_v = m[next_i];

  for(int k = 0; k< dimention; k++){
    int shorter = 0x7F800000;
    int greater = 0;
    int current_i = next_i;
    int choosen_path_size =0x7F800000;

    //Se for a ultima iteração, volte ao nó inicial
    if(k==dimention-1){
      choosen_path_size = m[current_i][first_i];
      next_i = first_i;
      size_of_path += choosen_path_size;
    }

    //Se não for a ultima iteração, continue
    else{

      //Esse for checa quais são os caminhos mais barato e mais caro para um nó não visitado
      for(int i = 0; i < dimention; i++){
        if(current_v[i] < shorter && current_i!=i && !was_element_visited(i, dimention, visited)){
          shorter = current_v[i];
        }
        if(current_v[i] > greater && current_i!=i && !was_element_visited(i, dimention, visited)){
          greater = current_v[i];
        }
      }


      int upper_limit = shorter + alpha*(greater-shorter);  //Calculando o tamanho limite para um caminho ser escolhido


      //printf("\nBuscando candidatos com limite %i", upper_limit);
      int candidates[] = {};
      int n_candidates = 0;


      //Seleciona os nós candidatos baseado no limite calculado anteriormente
      for(int i = 0; i < dimention; i++){
        if(current_v[i] <= upper_limit && current_i!=i && !was_element_visited(i, dimention, visited)){
          candidates[n_candidates] = i;
          n_candidates++;
        }
      }
      // printf("\nExistem %i candidatos", n_candidates);


      //A number between 0 and 1
      //Escolhe randomicamente um caminho na lista de caminhos candidatos
      float random = genrand();
      int choosen = (int) (random*(n_candidates-1));
      //printf("Escolhido: %i entre %i disponiveis\n", choosen, n_candidates);
      next_i = candidates[(int) choosen];
      choosen_path_size = current_v[next_i];

      size_of_path += choosen_path_size;
      current_v = m[next_i];
    }

    insert_into_visited(current_i, dimention, visited);
    n_visited++;

    //printf("Escolhi o candidato\n");
    //printf("\nDe %i para %i com custo %i. Ja foram visitados %i nós",current_i,next_i, choosen_path_size, n_visited);

  }
  //printf("Iteracao: %i  Tamanho do caminho: %li\n", a,size_of_path);
  printf("Menor caminho na iteração %i  :  %li\n", a , size_of_path);
 }
  return 1;
}
