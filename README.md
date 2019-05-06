# atsp
Heuristics and Metaheuristics implementations to solve the 
Asymmetric Travelling Salesman Problem in the C Language

Up to now, the Nearest Neighbor Algorithm, the Bi-Directional Nearest Neighbor Algorithm and the Randomized Nearest Neighbor Algorithm have been implemented. A file named "<b>read_file</b>" was implemented as a library which function is to read specific files with ATSP problems. Those files can be found at the following repository: https://wwwproxy.iwr.uni-heidelberg.de/groups/comopt/software/TSPLIB95/atsp/. Another file named "<b>MT19937.c</b>" is an implementation of Mersenne Twister algorithm that generates pseudo-random numbers. It works as a library and I have found it in the internet.

You can compile and execute any of them with the following command line on Linux:

<b>gcc -o <executable_name> <algorithm.c></b> 
<b>./<executable_name></b>
  
