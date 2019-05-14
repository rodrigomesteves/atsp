#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "read_file.c"
#define INF 0x7F800000


void print_header(){


	printf("  ____                        _                     _  \n");
	printf(" |  _ \\                      | |                   | | \n");
	printf(" | |_) |_   _ ___  ___ __ _  | |     ___   ___ __ _| | \n");
	printf(" |  _ <| | | / __|/ __/ _` | | |    / _ \\ / __/ _` | | \n");
	printf(" | |_) | |_| \\__ \\ (_| (_| | | |___| (_) | (_| (_| | | \n");
	printf(" |____/ \\__,_|___/\\___\\__,_| |______\\___/ \\___\\__,_|_| \n");
	printf("                                                            \n");
	printf("            Coded by Rodrigo Marins Esteves                         \n\n\n");




}


/*Como não criei uma rotina para ler os arquivos, estou definindo aqui o tamanho da
matriz e inicializando ela dentro do código*/

int n_visited = 0;				//Vetor auxiliar para guardar os nós ja visitados
long size_of_path = 0;
char name_of_file[50];
int dimention;

//A função print_matrix printa matrix carregada no problema.
void print_matrix(int s, int dim, int m[dim][dim]){
  for(int i = 0; i < s; i++){
    for(int j = 0; j < s; j++){
      printf("-- %i", m[i][j]);
    }
    printf("\n");
  }
}

//A função copy_array copia um array alocado dinâmicamente para outro.
void copy_array(int dim, int * o, int * d){
	for(int i=0; i<dim; i++)
		d[i] = o[i];
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


//Essa função printa a solução(caminho) encontrada
void print_solution(int dim, int sol[dim]){
  printf("\nSolução: ");
  for(int i =0; i<dim; i++){
    printf("%d", sol[i]);
    if(i<dim-1)
      printf("->");
  }
  printf("\n");
}


//Essa é a FUNÇÃO OBJETIVO
long evaluate_solution(int dim, int m[dim][dim], int sol[dimention+1]){
   long value = 0;
   for(int i=0; i<=dim; i++){
      int x = sol[i];
      int y = sol[i+1];
      value += m[x][y];
   }
   return value;
}


/*
Atualiza o valor de uma solução subtraindo o valo das arestas retiradas e inserindo o valor das arestas adicionadas. 
Pode ser utilizada quando dois nós ligados pela mesma arestas são trocados de posição. 
*/
long update_solution_value_1(int current_value, int sub1, int sub2, int sub3, int add1, int add2, int add3){
	return current_value+= add1+add2+add3-sub1-sub2-sub3;
}

//=============================================================================================================================

/*
Atualiza o valor de uma solução subtraindo o valo das arestas retiradas e inserindo o valor das arestas adicionadas. 
Pode ser utilizada quando dois nós que NÃO ESTÃO ligados pela mesma arestas são trocados de posição. 
*/
long update_solution_value_k(int current_value, int sub1, int sub2, int sub3, int sub4, int add1, int add2, int add3, int add4){
	return current_value+= add1+add2+add3+add4-sub1-sub2-sub3-sub4;
}

//=============================================================================================================================

//Busca Gulosa (Nearest Neighbor)
int* greedy_search(int dimention, int m[dimention][dimention]){

   int visited[dimention];
   int * solution = malloc((dimention+1)*sizeof(int));

  
  //Inicializa o vetor de visitados com um valor que nunca será usado
  for(int k = 0; k<= dimention; k++){
    visited[k] = INF;
  }

  int shorter = INF;
  int first_i = 0;
  int next_i = first_i;
  int *current_v = m[next_i];

  printf("\nIniciando a Busca Gulosa\n");
  //Esse FOR realiza um número de iterações igual ao número de nós totais.
  for(int k = 0; k< dimention; k++){
    int shorter = 0x7F800000;
    int current_i = next_i;

    //Se for a ultima iteração, volte ao nó inicial
    if(k==dimention-1){
      shorter = m[current_i][first_i];
      solution[k] = next_i;
      solution[k+1] = first_i;
      next_i = first_i;
      size_of_path += shorter;
    }
    //Se não for a ultima iteração, continue
    else{
      solution[k] = next_i;
      //Esse for checa qual é o caminho mais barato para um nó não visitado
      for(int i = 0; i < dimention; i++){
        if(current_v[i] < shorter && current_i!=i && !was_element_visited(i, dimention, visited)){
          shorter = current_v[i];
          next_i = i;
        }
      }
      size_of_path += shorter;
      current_v = m[next_i];
    }
    insert_into_visited(current_i, dimention, visited);
    n_visited++;
    //printf("De %i para %i com custo %i\n",current_i,next_i, shorter);
    //print_visited();
  }
  return solution;
}


//=============================================================================================================================


//Realiza a troca das arestas ára o algoritmo 2-opt
int * two_opt_swap(int dim, int * solution, int i, int k) {
	int * sol = malloc((dimention+1)*sizeof(int));
	copy_array(dimention+1,solution, sol);
	int aux = sol[i+1];
	sol[i+1] = sol[k];
	sol[k] = aux;
	return sol;
}


//=============================================================================================================================


//Busca local utilizando a vizinhança 2-opt
int * two_opt_neighborhood(int dimention, int m[dimention][dimention], int * init_sol, int strategy){
  //Inicializando arrays com a solução de partida
   int * new_sol = malloc((dimention+1)*sizeof(int));
   int * current_sol = malloc((dimention+1)*sizeof(int));
   copy_array(dimention+1,init_sol, new_sol);
   copy_array(dimention+1,init_sol, current_sol);

   //Inicializando variáveis com o valor da solução de partida.
   long current_sol_value = evaluate_solution(dimention, m, init_sol);
   long new_sol_value = current_sol_value;

	
   if(strategy == 0)		
      printf("\nIniciando a Busca Local utilizando o 2-opt para gerar uma vizinhança com estratégia de busca Mais Aprimorante\n");
   else if(strategy == 1)		
      printf("\nIniciando a Busca Local utilizando o 2-opt para gerar uma vizinhança com estratégia de busca Primeira Aprimorante\n");

   //Repita enquanto puderem ser encontradas melhores soluções
   do{
      current_sol_value = new_sol_value;
      copy_array(dimention+1,new_sol, current_sol);

      //Flag usada na política de Primeiro Aprimorante.
      int flag = 0;

      for (int i = 0; i < dimention; i++) {
         for (int k = i + 1; k < dimention+1; k++) {
            int * alt_sol = two_opt_swap(dimention,current_sol, i, k);


            //Aqui utilizo um novo calculo ao invés de atualizar o valor
            //Como estou trabalhando com o TSP assimétrico, 
            //ao religar as arestas pode ocorrer uma mudança na direção 
            //em que certos nós são percorridos, o que particularmente não consegui tratar.
            int new_distance = evaluate_solution(dimention, m, alt_sol);

            if (new_distance < current_sol_value) {
               copy_array(dimention+1,alt_sol, new_sol);
               new_sol_value = new_distance;
               //Se a estratégia for Primeiro Aprimorante
               if(strategy==1){
                  flag = 1;
                  break;
               }
            }
            free(alt_sol);
         }
         if (flag==1)
            break;
      }
    //printf("new sol: %ld  x  current sol: %ld\n", new_sol_value, current_sol_value);
   }while(new_sol_value < current_sol_value);

   free(new_sol);
   return current_sol;
}

//=============================================================================================================================


//Busca local utilizando a vizinhança local escolhida. 
int * local_neighborhood(int dimention, int m[dimention][dimention], int * init_sol, int strategy){

   //Inicializando arrays com a solução de partida
   int * new_sol = malloc((dimention+1)*sizeof(int));
   int * current_sol = malloc((dimention+1)*sizeof(int));
   copy_array(dimention+1,init_sol, new_sol);
   copy_array(dimention+1,init_sol, current_sol);

   //Inicializando variáveis com o valor da solução de partida.
   long current_sol_value = evaluate_solution(dimention, m, init_sol);
   long new_sol_value = current_sol_value;

   if(strategy == 0)		
      printf("\nIniciando a Busca Local utilizando uma Vizinhança Local com estratégia de busca Mais Aprimorante\n");
   else if(strategy == 1)		
      printf("\nIniciando a Busca Local utilizando uma Vizinhança Local com estratégia de busca Primeira Aprimorante\n");
   //Repita enquanto puderem ser encontradas melhores soluções
   do{
	   current_sol_value = new_sol_value;
	   copy_array(dimention+1,new_sol, current_sol);

	   //Flag usada na política de Primeiro Aprimorante.
           int flag = 0;

	/*A vizinhança escolhida foi a permutação de nós que distam entre 0 e 4 nós um dos outros
	  Por exemplo: A solução inicial  ABCDEFGH poderia ter como vizinhos as soluções ACBDEFGH (permutação de B e C, que distam 0 nós),
	  AGCDEFBH (permutação de B e G, que distam 4 nós), ABCFEDGH (permutação entre D e F, que distam 1 nó) e etc..
	*/


	/*
	Permuta nós adjacentes na solução. Por exemplo:
	Se a solução for ABCDEF, esse for testará sequencias como ACBDEF, ABDCEF, ABCEDF, etc..
	*/
	   for(int i = 1; i<dimention; i++){

		int * alt_sol = malloc((dimention+1)*sizeof(int));
		copy_array(dimention+1,current_sol, alt_sol);


		//Atualizando valor da solução.
		long value = update_solution_value_1(current_sol_value, m[alt_sol[i-1]][alt_sol[i]], m[alt_sol[i]][alt_sol[i+1]], m[alt_sol[i+1]][alt_sol[i+2]], m[alt_sol[i+1]][alt_sol[i]], m[alt_sol[i-1]][alt_sol[i+1]], m[alt_sol[i]][alt_sol[i+2]]);


		int aux = alt_sol[i];
		alt_sol[i] = alt_sol[i+1];
		alt_sol[i+1] = aux;

		//Testando se a nova solução melhora a solução atual
		if(value < current_sol_value){
		   copy_array(dimention+1,alt_sol, new_sol);
		   new_sol_value = value;

		   //Se a estratégia for Primeiro Aprimorante
		   if(strategy==1){
			flag = 1;
			break;
		   }
		}
		free(alt_sol);
	   }
	   if (flag==1)
		continue;


	/*
	Permuta nós separados por um nó na solução. Por exemplo:
	Se a solução for ABCDEF, esse for testará sequencias como CBADEF, ADCBEF, ABEDCF, etc..
	*/
	   for(int i = 1; i<dimention-1; i++){

		int * alt_sol = malloc((dimention+1)*sizeof(int));
		copy_array(dimention+1,current_sol, alt_sol);

		//Atualizando valor da solução.
		long value = update_solution_value_k(current_sol_value, m[alt_sol[i-1]][alt_sol[i]], m[alt_sol[i]][alt_sol[i+1]], m[alt_sol[i+1]][alt_sol[i+2]], m[alt_sol[i+2]][alt_sol[i+3]], m[alt_sol[i-1]][alt_sol[i+2]], m[alt_sol[i+2]][alt_sol[i+1]], m[alt_sol[i+1]][alt_sol[i]], m[alt_sol[i]][alt_sol[i+3]]);

		int aux = alt_sol[i];
		alt_sol[i] = alt_sol[i+2];
		alt_sol[i+2] = aux;

		//Testando se a nova solução melhora a solução atual
		if(value < current_sol_value){
		   copy_array(dimention+1,alt_sol, new_sol);
		   new_sol_value = value;

		   //Se a estratégia for Primeiro Aprimorante
		   if(strategy==1){
			flag = 1;
			break;
		   }
		}
		free(alt_sol);
	   }
	   if (flag==1)
		continue;


	/*
	Permuta nós separados por dois nós na solução. Por exemplo:
	Se a solução for ABCDEF, esse for testará sequencias como DBCAEF, AECDBF, ABFDEC, etc..
	*/
	   for(int i = 1; i<dimention-2; i++){

		int * alt_sol = malloc((dimention+1)*sizeof(int));
		copy_array(dimention+1,current_sol, alt_sol);

		//Atualizando valor da solução.
		long value = update_solution_value_k(current_sol_value, m[alt_sol[i-1]][alt_sol[i]], m[alt_sol[i]][alt_sol[i+1]], m[alt_sol[i+2]][alt_sol[i+3]], m[alt_sol[i+3]][alt_sol[i+4]],m[alt_sol[i-1]][alt_sol[i+3]], m[alt_sol[i+3]][alt_sol[i+1]], m[alt_sol[i+2]][alt_sol[i]], m[alt_sol[i]][alt_sol[i+4]]);

		int aux = alt_sol[i];
		alt_sol[i] = alt_sol[i+3];
		alt_sol[i+3] = aux;

		//Testando se a nova solução melhora a solução atual
		if(value < current_sol_value){
		   copy_array(dimention+1,alt_sol, new_sol);
		   new_sol_value = value;

		   //Se a estratégia for Primeiro Aprimorante
		   if(strategy==1){
			flag = 1;
			break;
		   }
		}
		free(alt_sol);
	   }
	   if (flag==1)
		continue;


	/*
	Permuta nós separados por três nós na solução. Por exemplo:
	Se a solução for ABCDEF, esse for testará sequencias como DBCAEF, AECDBF, ABFDEC, etc..
	*/
	   for(int i = 1; i<dimention-3; i++){

		int * alt_sol = malloc((dimention+1)*sizeof(int));
		copy_array(dimention+1,current_sol, alt_sol);

		//Atualizando valor da solução.
		long value = update_solution_value_k(current_sol_value, m[alt_sol[i-1]][alt_sol[i]], m[alt_sol[i]][alt_sol[i+1]], m[alt_sol[i+3]][alt_sol[i+4]], m[alt_sol[i+4]][alt_sol[i+5]],m[alt_sol[i-1]][alt_sol[i+4]], m[alt_sol[i+4]][alt_sol[i+1]], m[alt_sol[i+3]][alt_sol[i]], m[alt_sol[i]][alt_sol[i+5]]);

		int aux = alt_sol[i];
		alt_sol[i] = alt_sol[i+4];
		alt_sol[i+4] = aux;

		//Testando se a nova solução melhora a solução atual
		if(value < current_sol_value){
		   copy_array(dimention+1,alt_sol, new_sol);
		   new_sol_value = value;

		   //Se a estratégia for Primeiro Aprimorante
		   if(strategy==1){
			flag = 1;
			break;
		   }
		}
		free(alt_sol);
	   }
	   if (flag==1)
		continue;


	/*
	Permuta nós separados por quatro nós na solução. Por exemplo:
	Se a solução for ABCDEF, esse for testará sequencias como DBCAEF, AECDBF, ABFDEC, etc..
	*/
	   for(int i = 1; i<dimention-4; i++){

		int * alt_sol = malloc((dimention+1)*sizeof(int));
		copy_array(dimention+1,current_sol, alt_sol);
		
		//Atualizando valor da solução.
		long value = update_solution_value_k(current_sol_value, m[alt_sol[i-1]][alt_sol[i]], m[alt_sol[i]][alt_sol[i+1]], m[alt_sol[i+4]][alt_sol[i+5]], m[alt_sol[i+5]][alt_sol[i+6]],m[alt_sol[i-1]][alt_sol[i+5]], m[alt_sol[i+5]][alt_sol[i+1]], m[alt_sol[i+4]][alt_sol[i]], m[alt_sol[i]][alt_sol[i+6]]);

		int aux = alt_sol[i];
		alt_sol[i] = alt_sol[i+5];
		alt_sol[i+5] = aux;

		//Testando se a nova solução melhora a solução atual
		if(value < current_sol_value){
		   copy_array(dimention+1,alt_sol, new_sol);
		   new_sol_value = value;

		   //Se a estratégia for Primeiro Aprimorante
		   if(strategy==1){
			flag = 1;
			break;
		   }
		}
		free(alt_sol);
	   }
	   if (flag==1)
		continue;

	   //print_solution(dimention+1,new_sol);
	   //printf("Melhor Solução:  %ld\n", evaluate_solution(dimention, m, new_sol));   
      }while(new_sol_value < current_sol_value);
 
   free(new_sol);
   return current_sol;
}


//=============================================================================================================================


//Esta função faz uma triagem da vizinhança e da estratégia escolhida (mais aprimorante ou primeira aprimorante)
//e depois chama as funções responsáveis por tais processamentos.
int* local_search(int dimention, int m[dimention][dimention], int * init_sol, int strategy, int neighborhood){
   if(neighborhood == 0)
	return local_neighborhood(dimention,m,init_sol,strategy);
   else if(neighborhood==1)
	return two_opt_neighborhood(dimention,m,init_sol,strategy);
   else
	printf("Vizinhança desconhecida! Utilize 0 para '2-opt' e 1 para 'vizinhança local'. \n");
   return NULL;

}

//=============================================================================================================================

int main()
{


   print_header();
//Carregamento do arquivo na estrutura de dados >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   printf("Digite o nome do arquivo\n");
   scanf("%s", name_of_file);

   dimention = get_dimention(name_of_file);
   int m[dimention][dimention];
   file_to_matrix(name_of_file, dimention, m);
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//Executando a Busca Gulosa >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   clock_t beg = clock();
   int * sol = greedy_search(dimention, m);
   clock_t end = clock();   
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//Resultado da Busca Gulosa >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   print_solution(dimention+1, sol);
   printf("\nCusto da solução encontrada: %li\n", evaluate_solution(dimention, m, sol));
   printf("Tempo decorrido: %f seconds\n", (double)(end - beg) / CLOCKS_PER_SEC);
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//Executando Busca Local >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   beg = clock();
   // Best Improving Strategy: 0,  First Improving Strategy: 1
   // Local Neighborhood: 0,  2-opt Neighborhood: 1
   sol = local_search(dimention, m, sol, 0, 1);
   end = clock();   
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//Resultado da Busca Local >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   print_solution(dimention+1, sol);
   printf("\nCusto da solução encontrada: %li\n", evaluate_solution(dimention, m, sol));
   printf("Tempo decorrido: %f seconds\n", (double)(end - beg) / CLOCKS_PER_SEC);
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   return 0;
}

