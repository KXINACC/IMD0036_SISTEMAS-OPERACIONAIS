#include <iostream>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "matriz_utils.h"

// Função para calcular elemento da matriz resultado neste caso;
double calcular_elemento_matriz_(Matriz* A_, Matriz* B_, int i_, int j_) {
    double resultado_ = 0.0;
    for (int k_ = 0; k_ < A_->n_colunas; k_++) {
        resultado_ += A_->dados[i_][k_] * B_->dados[k_][j_];
    }
    return resultado_;
}

// Função para carregar matriz de um arquivo neste caso;
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
    
    // Leitura dos dados da matriz do arquivo neste caso;
    for (int i_ = 0; i_ < linhas_; i_++) {
        for (int j_ = 0; j_ < colunas_; j_++) {
            arquivo_ >> m_->dados[i_][j_];
        }
    }
    
    arquivo_.close();
    return m_;
}

// Função para salvar resultado parcial no formato da Figura 2 neste caso;
bool salvar_resultado_parcial_formato_figura2_(int n_linhas_, int n_colunas_, int elemento_inicio_, int elemento_fim_, 
                                              const std::string& arquivo_saida_, long tempo_) {
    std::ofstream arquivo_(arquivo_saida_);
    if (!arquivo_.is_open()) {
        std::cerr << "Erro ao abrir arquivo para escrita: " << arquivo_saida_ << std::endl;
        return false;
    }
    
    // Escrita das dimensões da matriz resultado neste caso;
    arquivo_ << n_linhas_ << " " << n_colunas_ << "\n";
    
    // Carregamento das matrizes para calcular os elementos neste caso;
    Matriz* A_ = carregar_matriz_arquivo_("data/m1.txt");
    Matriz* B_ = carregar_matriz_arquivo_("data/m2.txt");
    
    if (A_ == nullptr || B_ == nullptr) {
        std::cerr << "Erro ao carregar matrizes para cálculo" << std::endl;
        arquivo_.close();
        return false;
    }
    
    // Escrita dos elementos calculados por este processo neste caso;
    for (int elemento_ = elemento_inicio_; elemento_ < elemento_fim_; elemento_++) {
        int i_ = elemento_ / n_colunas_;
        int j_ = elemento_ % n_colunas_;
        double valor_ = calcular_elemento_matriz_(A_, B_, i_, j_);
        arquivo_ << "c" << (i_ + 1) << (j_ + 1) << " " << std::fixed << std::setprecision(4) << valor_ << "\n";
    }
    
    // Escrita do tempo de cálculo deste processo neste caso;
    arquivo_ << tempo_;
    
    arquivo_.close();
    liberar_matriz(A_);
    liberar_matriz(B_);
    return true;
}

int main(int argc_, char* argv_[]) {
    // Verificação dos argumentos de linha de comando neste caso;
    if (argc_ != 11) {
        std::cerr << "Uso: " << argv_[0] << " <n_linhas_A> <n_colunas_A> <n_linhas_B> <n_colunas_B> <elemento_inicio> <elemento_fim> <processo_id> <arquivo_m1> <arquivo_m2> <arquivo_saida>" << std::endl;
        return 1;
    }
    
    // Leitura dos parâmetros neste caso;
    int n_linhas_A_ = std::atoi(argv_[1]);
    int n_colunas_A_ = std::atoi(argv_[2]);
    int n_linhas_B_ = std::atoi(argv_[3]);
    int n_colunas_B_ = std::atoi(argv_[4]);
    int elemento_inicio_ = std::atoi(argv_[5]);
    int elemento_fim_ = std::atoi(argv_[6]);
    int processo_id_ = std::atoi(argv_[7]);
    std::string arquivo_m1_ = argv_[8];
    std::string arquivo_m2_ = argv_[9];
    std::string arquivo_saida_ = argv_[10];
    
    std::cout << "Processo " << processo_id_ << " processando elementos " << elemento_inicio_ 
              << " a " << elemento_fim_ - 1 << std::endl;
    
    // Início da medição de tempo individual do processo neste caso;
    auto inicio_processo_ = std::chrono::high_resolution_clock::now();
    
    // Fim da medição de tempo do processo neste caso;
    auto fim_processo_ = std::chrono::high_resolution_clock::now();
    auto duracao_processo_ = std::chrono::duration_cast<std::chrono::milliseconds>(fim_processo_ - inicio_processo_);
    
    // Salvamento do resultado parcial deste processo neste caso;
    salvar_resultado_parcial_formato_figura2_(n_linhas_A_, n_colunas_B_, 
                                             elemento_inicio_, elemento_fim_, 
                                             arquivo_saida_, duracao_processo_.count());
    
    std::cout << "Processo " << processo_id_ << " concluído em " << duracao_processo_.count() << " ms" << std::endl;
    return 0;
}
