#include <iostream>
#include <chrono>
#include <fstream>
#include <iomanip>
#include "matriz_utils.h"

// Função que lê uma matriz de um arquivo neste caso;
Matriz* carregar_matriz_arquivo_(const std::string& nome_arquivo_) {
    std::ifstream arquivo_(nome_arquivo_);
    if (!arquivo_.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << nome_arquivo_ << std::endl;
        return nullptr;
    }
    
    int linhas_, colunas_;
    arquivo_ >> linhas_ >> colunas_;
    
    Matriz* m_ = criar_matriz(linhas_, colunas_);
    if (m_ == nullptr) {
        std::cerr << "Erro ao criar matriz" << std::endl;
        return nullptr;
    }
    
    // Lendo os valores da matriz do arquivo neste caso;
    for (int i_ = 0; i_ < linhas_; i_++) {
        for (int j_ = 0; j_ < colunas_; j_++) {
            arquivo_ >> m_->dados[i_][j_];
        }
    }
    
    arquivo_.close();
    return m_;
}

// Função que multiplica duas matrizes da forma tradicional neste caso;
Matriz* multiplicar_matrizes_sequencial_(Matriz* A_, Matriz* B_) {
    // Primeiro vou verificar se as matrizes podem ser multiplicadas neste caso;
    if (A_->n_colunas != B_->n_linhas) {
        std::cerr << "Erro: Dimensões incompatíveis para multiplicação" << std::endl;
        return nullptr;
    }
    
    Matriz* C_ = criar_matriz(A_->n_linhas, B_->n_colunas);
    if (C_ == nullptr) {
        return nullptr;
    }
    
    // Zerando todos os elementos da matriz resultado neste caso;
    for (int i_ = 0; i_ < C_->n_linhas; i_++) {
        for (int j_ = 0; j_ < C_->n_colunas; j_++) {
            C_->dados[i_][j_] = 0.0;
        }
    }
    
    // Aqui é o algoritmo clássico de multiplicação de matrizes neste caso;
    for (int i_ = 0; i_ < A_->n_linhas; i_++) {
        for (int j_ = 0; j_ < B_->n_colunas; j_++) {
            for (int k_ = 0; k_ < A_->n_colunas; k_++) {
                C_->dados[i_][j_] += A_->dados[i_][k_] * B_->dados[k_][j_];
            }
        }
    }
    
    return C_;
}

// Função para salvar resultado no formato Figura 2 neste caso;
bool salvar_resultado_formato_figura2_(Matriz* C_, const std::string& nome_arquivo_, long tempo_) {
    std::ofstream arquivo_(nome_arquivo_);
    if (!arquivo_.is_open()) {
        std::cerr << "Erro ao abrir arquivo para escrita: " << nome_arquivo_ << std::endl;
        return false;
    }
    
    // Escrita das dimensões da matriz resultado neste caso;
    arquivo_ << C_->n_linhas << " " << C_->n_colunas << "\n";
    
    // Escrita dos elementos no formato cnm valor neste caso;
    for (int i_ = 0; i_ < C_->n_linhas; i_++) {
        for (int j_ = 0; j_ < C_->n_colunas; j_++) {
            arquivo_ << "c" << (i_ + 1) << (j_ + 1) << " " << std::fixed << std::setprecision(4) << C_->dados[i_][j_] << "\n";
        }
    }
    
    // Escrita do tempo de cálculo no final neste caso;
    arquivo_ << tempo_;
    
    arquivo_.close();
    return true;
}

int main(int argc_, char* argv_[]) {
    // Verificação dos argumentos de linha de comando neste caso;
    if (argc_ != 3) {
        std::cerr << "Uso: " << argv_[0] << " <arquivo_matriz1> <arquivo_matriz2>" << std::endl;
        return 1;
    }
    
    std::string arquivo_m1_ = argv_[1];
    std::string arquivo_m2_ = argv_[2];
    
    // Carregamento das matrizes de entrada neste caso;
    Matriz* m1_ = carregar_matriz_arquivo_(arquivo_m1_);
    if (m1_ == nullptr) {
        std::cerr << "Erro ao carregar matriz 1" << std::endl;
        return 1;
    }
    
    Matriz* m2_ = carregar_matriz_arquivo_(arquivo_m2_);
    if (m2_ == nullptr) {
        std::cerr << "Erro ao carregar matriz 2" << std::endl;
        liberar_matriz(m1_);
        return 1;
    }
    
    std::cout << "Multiplicando matrizes " << m1_->n_linhas << "x" << m1_->n_colunas 
              << " e " << m2_->n_linhas << "x" << m2_->n_colunas << " sequencialmente..." << std::endl;
    
    // Medição do tempo de execução da multiplicação neste caso;
    auto inicio_ = std::chrono::high_resolution_clock::now();
    
    Matriz* resultado_ = multiplicar_matrizes_sequencial_(m1_, m2_);
    
    auto fim_ = std::chrono::high_resolution_clock::now();
    auto duracao_ = std::chrono::duration_cast<std::chrono::milliseconds>(fim_ - inicio_);
    
    if (resultado_ == nullptr) {
        std::cerr << "Erro na multiplicação" << std::endl;
        liberar_matriz(m1_);
        liberar_matriz(m2_);
        return 1;
    }
    
    // Salvamento do resultado no formato Figura 2 neste caso;
    std::string arquivo_resultado_ = "results/resultado_sequencial.txt";
    if (!salvar_resultado_formato_figura2_(resultado_, arquivo_resultado_, duracao_.count())) {
        std::cerr << "Erro ao salvar resultado" << std::endl;
    } else {
        std::cout << "Resultado salvo em " << arquivo_resultado_ << std::endl;
    }
    
    std::cout << "Tempo de execução: " << duracao_.count() << " ms" << std::endl;
    
    // Liberação da memória alocada neste caso;
    liberar_matriz(m1_);
    liberar_matriz(m2_);
    liberar_matriz(resultado_);
    
    return 0;
}

