#include "matriz_utils.h"
#include <iostream>  
#include <fstream>    
#include <iomanip>    
#include <random>     
#include <stdexcept>  

Matriz* criar_matriz(int linhas_, int colunas_) {
    Matriz* m_ = new Matriz;
    m_->n_linhas = linhas_;
    m_->n_colunas = colunas_;
    m_->dados.resize(linhas_, std::vector<double>(colunas_));
    return m_;
}

void liberar_matriz(Matriz* m_) {
    delete m_;
}

void preencher_matriz(Matriz* m_) {
    if (m_ == nullptr) return;
    // Algoritmo gerador de números aleatórios moderno neste caso;
    // Isso faz com que os números sejam gerados de forma uniforme entre 0 e 10;
    static std::mt19937 gen_(std::random_device{}());
    std::uniform_real_distribution<> dis_(0.0, 10.0);
    for (int i_ = 0; i_ < m_->n_linhas; i_++) {
        for (int j_ = 0; j_ < m_->n_colunas; j_++) {
            m_->dados[i_][j_] = dis_(gen_);
        }
    }
}

bool salvar_matriz_arquivo(Matriz* m_, const std::string& nome_arquivo_) {
    std::ofstream arq_saida_(nome_arquivo_);
    if (!arq_saida_.is_open()) {
        std::cerr << "Erro ao abrir arquivo para salvar a matriz: " << nome_arquivo_ << std::endl;
        return false;
    }
    // Escrita das dimensões da matriz no arquivo neste caso;
    arq_saida_ << m_->n_linhas << " " << m_->n_colunas << "\n";
    arq_saida_ << std::fixed << std::setprecision(4);

    for (int i_ = 0; i_ < m_->n_linhas; i_++) {
        for (int j_ = 0; j_ < m_->n_colunas; j_++) {
            arq_saida_ << m_->dados[i_][j_] << " ";
        }
        arq_saida_ << "\n";
    }
    arq_saida_.close();
    return true; 
}
