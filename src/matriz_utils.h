#ifndef MATRIZ_UTILS_H
#define MATRIZ_UTILS_H

#include <vector> 
#include <string>  

struct Matriz {
    int n_linhas;
    int n_colunas;
    std::vector<std::vector<double>> dados;
};

Matriz* criar_matriz(int linhas_, int colunas_);

void liberar_matriz(Matriz* m_);

void preencher_matriz(Matriz* m_);

bool salvar_matriz_arquivo(Matriz* m_, const std::string& nome_arquivo_);

#endif 
