#ifndef MATRIZ_UTILS_H
#define MATRIZ_UTILS_H

#include <vector> 
#include <string>  

struct Matriz_ {
    int n_linhas_;
    int n_colunas_;
    std::vector<std::vector<double>> dados_;
};

Matriz_* criar_matriz(int linhas_, int colunas_);

void liberar_matriz(Matriz_* m_);

void preencher_matriz(Matriz_* m_);

bool salvar_matriz_arquivo(Matriz_* m_, const std::string& nome_arquivo_);

#endif 
