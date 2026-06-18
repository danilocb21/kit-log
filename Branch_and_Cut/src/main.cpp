#include <ilcplex/ilocplex.h>
#include <iostream>

int main() {
    IloEnv env;
    try {
        IloModel model(env);
        std::cout << "Ambiente CPLEX inicializado com sucesso no Linux!" << std::endl;
        
        // Aqui viria a lógica do seu modelo de otimização
        
    } catch (IloException& e) {
        std::cerr << "Erro no CPLEX: " << e << std::endl;
    } catch (...) {
        std::cerr << "Erro desconhecido." << std::endl;
    }
    
    env.end();
    return 0;
}