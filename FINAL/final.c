#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "MT19937.c"
#include "read_file.c"
#define INF 0x7F800000
#define ELITESET 5
#define MAXPOP 300
#define INITPOP 100



void print_header(){

	printf("   _____   _____                _____   _____  \n");
	printf("  \/ ____| |  __ \\      \/\\      \/ ____| |  __ \\ \n");
	printf(" | |  __  | |__) |    \/  \\    | (___   | |__) |\n");
	printf(" | | |_ | |  _  \/    \/ \/\\ \\    \\___ \\  |  ___\/ \n");
	printf(" | |__| | | | \\ \\   \/ ____ \\   ____) | | |     \n");
	printf("  \\_____| |_|  \\_\\ \/_\/    \\_\\ |_____\/  |_|     \n");
	printf("                                               \n");
	printf("       Coded By Rodrigo Marins Esteves\n\n     \n");

}


/*Como não criei uma rotina para ler os arquivos, estou definindo aqui o tamanho da
matriz e inicializando ela dentro do código*/

long size_of_path = 0;
int dimention;
float alpha = 0.1;
int n_visited = 0;
int SURVIVOR = INITPOP/10;

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
	for(int i=0; i<dim; i++){
		d[i] = o[i];
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
   for(int i=0; i<dim; i++){
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
    if(k>=dimention-1){
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

  n_visited = 0;
  return solution;
}


//=============================================================================================================================


//Busca Semi-Gulosa (Nearest Neighbor)
int* semigreedy_search(int dimention, int m[dimention][dimention]){

  int * visited = malloc((dimention)*sizeof(int));
  int n_visited = 0;
  int * solution = malloc((dimention+1)*sizeof(int));

  //Inicializa o vetor de visitados com um valor que nunca será usado
  for(int k = 0; k< dimention; k++){
    visited[k] = 0;
  }

  int shorter = INF;
  int greater = 0;
  int first_i = 0;
  int next_i = first_i;
  int *current_v = m[next_i];

  //printf("\nIniciando a Busca Gulosa\n");
  //Esse FOR realiza um número de iterações igual ao número de nós totais.
  for(int k = 0; k< dimention; k++){
    shorter = 0x7F800000;
    greater = 0;
    int current_i = next_i;
    int choosen_path_size =0x7F800000;
    int * candidates = malloc((dimention)*sizeof(int));
    int n_candidates = 0;

    //Se for a ultima iteração, volte ao nó inicial
    if(k>=dimention-1){
      shorter = m[current_i][first_i];
      solution[k] = next_i;
      solution[k+1] = first_i;
      next_i = first_i;
      size_of_path += shorter;
      free(candidates);
    }
    //Se não for a ultima iteração, continue
    else{
      solution[k] = next_i;

      for(int i = 0; i < dimention; i++){
        if(current_v[i] < shorter && current_i!=i && visited[i]==0){
          shorter = current_v[i];
        }
        if(current_v[i] > greater && current_i!=i && visited[i]==0){
          greater = current_v[i];
        }
      }

      int upper_limit = shorter + alpha*(greater-shorter);  //Calculando o tamanho limite para um caminho ser escolhido

      //Seleciona os nós candidatos baseado no limite calculado anteriormente
      for(int i = 0; i < dimention; i++){
        if(current_v[i] <= upper_limit && current_i!=i && visited[i]==0){
          candidates[n_candidates] = i;
          n_candidates++;
        }
      }

      //A number between 0 and 1
      //Escolhe randomicamente um caminho na lista de caminhos candidatos
      float random = genrand();
      //printf("RANDOM: %f\n", random);
      int choosen = (int) (random*n_candidates);
      next_i = candidates[(int) choosen];
      choosen_path_size = current_v[next_i];
      size_of_path += shorter;
      current_v = m[next_i];
      //printf("\nIteração %d    Escolhido %d    Existem %i candidatos",k, choosen, n_candidates);
      free(candidates);
    }
    visited[current_i] = 1;
    n_visited++;
    //printf("De %i para %i com custo %i\n",current_i,next_i, shorter);
    //print_visited();
  }

  free(visited);
  n_visited = 0;
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

/*	
   if(strategy == 0)		
      printf("\nIniciando a Busca Local utilizando o 2-opt para gerar uma vizinhança com estratégia de busca Mais Aprimorante\n");
   else if(strategy == 1)		
      printf("\nIniciando a Busca Local utilizando o 2-opt para gerar uma vizinhança com estratégia de busca Primeira Aprimorante\n");
*/


   //Repita enquanto puderem ser encontradas melhores soluções
   do{
      current_sol_value = new_sol_value;
      copy_array(dimention+1,new_sol, current_sol);

      //Flag usada na política de Primeiro Aprimorante.
      int flag = 0;

      for (int i = 1; i < dimention-1; i++) {
         for (int k = i + 1; k < dimention; k++) {
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
	   for(int i = 1; i<dimention-1; i++){

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
	   for(int i = 1; i<dimention-2; i++){

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
	   for(int i = 1; i<dimention-3; i++){

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
	   for(int i = 1; i<dimention-4; i++){

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
	   for(int i = 1; i<dimention-5; i++){

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
	printf("Vizinhança desconhecida! Utilize 1 para '2-opt' e 0 para 'vizinhança local'. \n");
   return NULL;

}


int* grasp(int dimention, int m[dimention][dimention], int strategy, int its){


  //double media = 0;
  double menor = INF;
  //int index = 0;

  int * bestSolution = malloc((dimention+1)*sizeof(int));
  for(int a=0; a<its; a++){
	int * sol = semigreedy_search(dimention, m);
	int * l_sol;
	if(strategy == 1)
		l_sol = local_search(dimention, m, sol, 0, 1);
	else if(strategy == 2)
		l_sol = local_search(dimention, m, sol, 1, 1);
	double newValue = evaluate_solution(dimention, m, l_sol);
	if(newValue < menor){
		menor = newValue;
		copy_array(dimention+1,l_sol, bestSolution);
	}
	//media +=newValue;
	free (sol);
	free (l_sol);
        //printf("%s,%f,%f,%f,%f\n", name_of_file, alpha, menor, media/index, (double)(clock() - c_beg)/CLOCKS_PER_SEC);
	//index++;
   }
   return bestSolution;


}

//=============================================================================================================================

int getWorstSolution(int ** eliteSet, int n_ep, int dimention, int m[dimention][dimention]){
	int value = 0,index = 0;
	for(int i=0; i<n_ep; i++){
		int aux = evaluate_solution(dimention, m, eliteSet[i]);
		if(aux>value){
			value = aux;
			index = i;
		}
	}
	return index;
}


int distanceBetweenSolutions(int ** eliteSet, int *newSolution, int n_ep,int dimention){
	int distance = INF;
	for(int i=0; i<n_ep; i++){
		int count = 0;
		for(int j=0; j<dimention; j++){
			if(newSolution[j] != eliteSet[i][j])
				count++;			

		}
		if(count < distance)
			distance = count;
	}
	return distance;
}


int distanceBetweenTwoSolutions(int * firstSol, int *secondSol){
	int count = 0;
	for(int j=0; j<dimention; j++){
		if(firstSol[j] != secondSol[j])
			count++;			
	}
	return count;
}



int updateEliteSet(int ** eliteSet, int *newSolution, int n_ep, int dimention, int m[dimention][dimention]){
   
   if(n_ep < ELITESET){
      if(n_ep==0){   
         eliteSet[n_ep] = newSolution;
	 return 1;
      }
      else{
         int minDistance = distanceBetweenSolutions(eliteSet, newSolution, n_ep, dimention);
	 //printf("Distância: %d\n", minDistance);
	 if(minDistance > 0){
	    eliteSet[n_ep] = newSolution;
	    return 1;
  	 }
	 return 0;
      }
   }
   else{

      int index = getWorstSolution(eliteSet, ELITESET, dimention, m);

      if(evaluate_solution(dimention, m, eliteSet[index]) <= evaluate_solution(dimention, m, newSolution)){ 
         //printf("A solução não pode entrar no conjunto elite devido ao seu valor\n");
         return 0;
      }
      else{
         int minDistance = distanceBetweenSolutions(eliteSet, newSolution, ELITESET, dimention); 
         if(minDistance>0){
	    free(eliteSet[index]);
	    eliteSet[index] = newSolution;
            //printf("Trocando solução %d por nova solução\n", index);
	    return 1;
	 }
         //printf("A solução já existe no conjunto elite\n");
	 return 0;
      }

   }
   
}


int updatePop(int ** pop, int *newSolution, int n_ep, int dimention, int m[dimention][dimention]){
   
   if(n_ep < INITPOP){
      if(n_ep==0){   
         pop[n_ep] = newSolution;
	 return 1;
      }
      else{
         int minDistance = distanceBetweenSolutions(pop, newSolution, n_ep, dimention);

	 //printf("Distância: %d\n", minDistance);
	 if(minDistance > 0){
	    pop[n_ep] = newSolution;
	    return 1;
  	 }
	 return 0;
      }
   }
   else{

      int index = getWorstSolution(pop, INITPOP, dimention, m);

      if(evaluate_solution(dimention, m, pop[index]) <= evaluate_solution(dimention, m, newSolution)){ 
         //printf("A solução não pode entrar no conjunto elite devido ao seu valor\n");
         return 0;
      }
      else{
         int minDistance = distanceBetweenSolutions(pop, newSolution, INITPOP, dimention); 
         if(minDistance>0){
	    free(pop[index]);
	    pop[index] = newSolution;
            //printf("Trocando solução %d por nova solução\n", index);
	    return 1;
	 }
         //printf("A solução já existe no conjunto elite\n");
	 return 0;
      }

   }
   
}



int * backwardPathRelink(int* init_sol, int* g_sol, int dimention, int m[dimention][dimention]){

	if(evaluate_solution(dimention, m, init_sol) > evaluate_solution(dimention, m, g_sol)){
		int * aux = init_sol;
		init_sol = g_sol;
		g_sol = aux;
	}

	int * i_sol = malloc((dimention+1)*sizeof(int));
	copy_array(dimention+1,init_sol, i_sol);
	
	int * best_sol = malloc((dimention+1)*sizeof(int));
	copy_array(dimention+1,init_sol, best_sol);

	long value = evaluate_solution(dimention, m, i_sol);
	//printf("Distancia: %d    Melhor: %ld\n",distanceBetweenTwoSolutions(i_sol, g_sol), value);

	while(distanceBetweenTwoSolutions(i_sol, g_sol)>0){
		int * aux_sol = malloc((dimention+1)*sizeof(int));
		for(int i=1; i<dimention-1; i++){
			for(int j=i+1; j<dimention; j++){
				copy_array(dimention+1,i_sol, aux_sol);
				int aux = aux_sol[i];
				aux_sol[i] = aux_sol[j];
				aux_sol[j] = aux;

				if(distanceBetweenTwoSolutions(aux_sol, g_sol) < distanceBetweenTwoSolutions(i_sol, g_sol)){	
					free(i_sol);
					i_sol = aux_sol;
					if(evaluate_solution(dimention, m, i_sol) < value){
						copy_array(dimention+1,i_sol, best_sol);
						value = evaluate_solution(dimention, m, i_sol);
					}
					i=dimention;
					//printf("Distancia: %d    Melhor: %ld\n",distanceBetweenTwoSolutions(i_sol, g_sol), value);
					break;
				}
				
			}
		}
	}


	return best_sol;
}



int * grasp_pr(int dimention, int m[dimention][dimention]){

   int **eliteSet = (int**) malloc(ELITESET*sizeof(int*));
   int n_eliteSet=0;
   int * path;
   int * bestSol;
   long bestValue=INF;

   for(int i=0; i<100; i++){
   //while(bestValue > 6000){

	//printf("%ld\n", bestValue);


	int *sol = grasp(dimention, m, 1, 1);


	if(n_eliteSet>0){
		//float random = genrand();
		int choosen = rand()%n_eliteSet;//(int) (random*(n_eliteSet-1));

		path = backwardPathRelink(sol, eliteSet[(int)choosen], dimention, m);
		if(updateEliteSet(eliteSet, path, n_eliteSet, dimention,m)){
			if(n_eliteSet < ELITESET)
				n_eliteSet++;
			long auxValue = evaluate_solution(dimention, m, path);
			if(auxValue < bestValue){
				bestValue = auxValue;
				bestSol = path;
			}
		}
	}
	else{

		if(updateEliteSet(eliteSet, sol, n_eliteSet, dimention, m)){
			n_eliteSet++;
			long auxValue = evaluate_solution(dimention, m, sol);
			if(auxValue < bestValue){
				bestValue = auxValue;
				bestSol = sol;
			}
		}
	}

   }

   int* bestPath = malloc((dimention+1)*sizeof(int));
   copy_array(dimention+1, bestSol, bestPath);

   for(int i=0; i<n_eliteSet; i++){
	free(eliteSet[i]);
   }
   free(eliteSet);

  return bestPath;
}



void print_population(int ** pop, int n){
	for(int i=0; i<n; i++ ){
		if(i==0)printf("40 melhores:\n\n");
		if(i==40)printf("\n\n10 piores:\n\n");		
		print_solution(dimention+1, pop[i]);
	}

}

void update_best_individuals(int * sol, int dimention, int m[dimention][dimention],  int ** best){
	
	int worst_i = 0;
	int worst_v = 0;
	int sol_value = evaluate_solution(dimention, m, sol);

	for(int i=0; i< SURVIVOR; i++){
		if(distanceBetweenTwoSolutions(best[i], sol)<1) return;
		long aux = evaluate_solution(dimention, m, best[i]);
		if(aux > worst_v){
			worst_v = aux;
			worst_i = i;
		}
	}
	if(worst_v > sol_value){
		best[worst_i] = sol;
	}

}



int * evolutive(int dimention, int m[dimention][dimention], int n_generations){

   int count = 0;
   int reset = 1;
   int n_pop=0;
   int **new_pop =  (int**) malloc(MAXPOP*sizeof(int*));
   int* bestPath = malloc((dimention+1)*sizeof(int));
   int * bestSol;
   long bestValue=INF;
   clock_t beg = clock();
   //for(int i=0; i<n_generations; i++){
   //for(int i=0; bestValue>6000; i++){
   for(int i = 0; (clock() - beg)/CLOCKS_PER_SEC < 600; i++){
	   int * path;
	   int **population = (int**) malloc(MAXPOP*sizeof(int*));
	   int **best_of_population =  (int**) malloc((SURVIVOR)*sizeof(int*));



	   //Criando população inicial baseado nas regras de diversidade e valor das soluções
	   //Para uma nova geração, são aproveitados os 50 melhores indivíduos e criados mais 50
	   if(reset==1){
		   while(n_pop < INITPOP){
			int * sol = grasp(dimention, m, 1,1);

			if(updatePop(population, sol, n_pop, dimention, m)){
				long auxValue = evaluate_solution(dimention, m, sol);
				n_pop++;
				if(auxValue < bestValue){
					bestValue = auxValue;
					bestSol = sol	;
				}
			}	

		   }
		   reset=0;
		   free(new_pop);
		   new_pop = (int**) malloc(MAXPOP*sizeof(int*));
		   bestSol = bestPath;

	   }
	   else{
		   for(int i=0; i<SURVIVOR; i++){
				population[i] = malloc((dimention+1)*sizeof(int));
				copy_array(dimention+1,new_pop[i], population[i]);
				free(new_pop[i]);
		   }
		   n_pop = SURVIVOR;
		   while(n_pop < INITPOP){
			int * sol = grasp(dimention, m,1,1);
			if(updatePop(population, sol, n_pop, dimention, m)){
				n_pop++;

			}	
		   }
		   free(new_pop);
		   new_pop = (int**) malloc(MAXPOP*sizeof(int*));
		   bestSol = bestPath;

           }

	   //Reprodução entre os indivíduos utilizando path relink
	   while(n_pop < MAXPOP){

		int choosen_1 = rand()%(SURVIVOR);
		int choosen_2 = SURVIVOR + rand()%(INITPOP-SURVIVOR);

		path = backwardPathRelink(population[choosen_1], population[choosen_2], dimention, m);


		//Pode haver mutação ou não
		int mutation = rand()%2;
		if(mutation){
			int pos = 1 + rand()%(dimention-2);
			int aux = path[pos];
			path[pos] = path[pos+1];
			path[pos+1] = aux;
		}

		population[n_pop] = path;
		n_pop++;

		long auxValue = evaluate_solution(dimention, m, path);
		if(auxValue < bestValue){
			bestValue = auxValue;
			bestSol = path;
		}
	   }



	   //Selecionando so n melhores da geração
	   for(int i=0; i<n_pop; i++){
		if(i<SURVIVOR){
			best_of_population[i] = population[i];

		}
		else{			
			update_best_individuals(population[i], dimention, m, best_of_population);
		}

	   }

	   //Salvando os escolhidos
	   for(int i=0; i<SURVIVOR; i++){
		new_pop[i] = malloc((dimention+1)*sizeof(int));
		copy_array(dimention+1,best_of_population[i], new_pop[i]);
	   }

	   //print_population(new_pop, 50);


	   //Mantendo a menor solução
	   if(i==0){
		copy_array(dimention+1, bestSol, bestPath);
	   }
	   else{
		if(bestValue < evaluate_solution(dimention, m, bestPath)){
			copy_array(dimention+1, bestSol, bestPath);
			count=0;
		}
		else{
			count++;
			/*if(count>200){
				reset=1;
				count=0;	
			}*/
		}
	   }


	   for(int i=0; i<MAXPOP; i++){
		free(population[i]);
	   }
	   free(population);
	   free(best_of_population);
	   n_pop =0;
	   printf("%lf,%ld\n",(double)(clock() - beg)/CLOCKS_PER_SEC,bestValue);

   }
   return bestPath;
}




int main(int argc, char* argv[], int seed)
{

   //print_header();
   char *name_of_file = malloc(50*sizeof(char));

//Carregamento do arquivo na estrutura de dados >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   strcpy(name_of_file,argv[1]);
   
   dimention = get_dimention(name_of_file);
   int m[dimention][dimention];
   file_to_matrix(name_of_file, dimention, m);
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

 
  

   //sgenrand(seed); //Gerador de números aleatorios Mersenne Twister
   sgenrand(seed);
   clock_t beg = clock();

   int * result = evolutive(dimention, m, 10000);
   //int * result = grasp(dimention, m, 1, 1);

   clock_t end = clock();
   //printf("%f\n", (double)(end - beg)/CLOCKS_PER_SEC);
   printf("%f\n", (double)(end - beg)/CLOCKS_PER_SEC);
   //print_solution(dimention+1, result);
   //printf("\n");

   return 0;
}

