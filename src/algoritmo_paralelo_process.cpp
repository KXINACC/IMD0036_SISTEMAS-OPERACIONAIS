#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <windows.h>
#include <process.h>
#include <cstring>
#include <iomanip>
#include <cmath>
#include "matriz_utils.h"

// Estrutura para compartilhar dados entre processos neste caso;
struct DadosProcesso_ {
    int n_linhas_A_;
    int n_colunas_A_;
    int n_linhas_B_;
    int n_colunas_B_;
    int n_linhas_C_;
    int n_colunas_C_;
    int elemento_inicio_;
    int elemento_fim_;
    int processo_id_;
    std::string arquivo_m1_;
    std::string arquivo_m2_;
    std::string arquivo_saida_;
};

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

// Função executada por cada processo filho neste caso;
unsigned __stdcall processo_filho_(void* param_) {
    DadosProcesso_* dados_ = (DadosProcesso_*)param_;
    
    // Início da medição de tempo individual do processo neste caso;
    auto inicio_processo_ = std::chrono::high_resolution_clock::now();
    
    std::cout << "Processo " << dados_->processo_id_ << " processando elementos " << dados_->elemento_inicio_ 
              << " a " << dados_->elemento_fim_ - 1 << std::endl;
    
    // Fim da medição de tempo do processo neste caso;
    auto fim_processo_ = std::chrono::high_resolution_clock::now();
    auto duracao_processo_ = std::chrono::duration_cast<std::chrono::milliseconds>(fim_processo_ - inicio_processo_);
    
    // Salvamento do resultado parcial deste processo neste caso;
    salvar_resultado_parcial_formato_figura2_(dados_->n_linhas_C_, dados_->n_colunas_C_, 
                                             dados_->elemento_inicio_, dados_->elemento_fim_, 
                                             dados_->arquivo_saida_, duracao_processo_.count());
    
    std::cout << "Processo " << dados_->processo_id_ << " concluído em " << duracao_processo_.count() << " ms" << std::endl;
    return 0;
}

// Função principal de multiplicação paralela com processos neste caso;
bool multiplicar_matrizes_paralelo_processos_(Matriz* A_, Matriz* B_, int P_) {
    // Verificação da compatibilidade das dimensões neste caso;
    if (A_->n_colunas != B_->n_linhas) {
        std::cerr << "Erro: Dimensões incompatíveis para multiplicação" << std::endl;
        return false;
    }
    
    // Cálculo do número total de elementos da matriz resultado neste caso;
    int total_elementos_ = A_->n_linhas * B_->n_colunas;
    
    // Cálculo do número de processos necessários baseado em P elementos neste caso;
    int num_processos_ = std::ceil((double)total_elementos_ / P_);
    
    std::vector<HANDLE> processos_(num_processos_);
    std::vector<DadosProcesso_> dados_(num_processos_);
    
    int elemento_atual_ = 0;
    
    // Criação dos processos com divisão por P elementos neste caso;
    for (int i_ = 0; i_ < num_processos_; i_++) {
        int elementos_para_este_processo_ = std::min(P_, total_elementos_ - elemento_atual_);
        int elemento_fim_ = elemento_atual_ + elementos_para_este_processo_;
        
        // Configuração dos dados do processo neste caso;
        dados_[i_].n_linhas_A_ = A_->n_linhas;
        dados_[i_].n_colunas_A_ = A_->n_colunas;
        dados_[i_].n_linhas_B_ = B_->n_linhas;
        dados_[i_].n_colunas_B_ = B_->n_colunas;
        dados_[i_].n_linhas_C_ = A_->n_linhas;
        dados_[i_].n_colunas_C_ = B_->n_colunas;
        dados_[i_].elemento_inicio_ = elemento_atual_;
        dados_[i_].elemento_fim_ = elemento_fim_;
        dados_[i_].processo_id_ = i_;
        dados_[i_].arquivo_m1_ = "data/m1.txt";
        dados_[i_].arquivo_m2_ = "data/m2.txt";
        dados_[i_].arquivo_saida_ = "results/resultado_processo_" + std::to_string(i_) + ".txt";
        
        // Criação do processo neste caso;
        processos_[i_] = (HANDLE)_beginthreadex(NULL, 0, processo_filho_, &dados_[i_], 0, NULL);
        
        if (processos_[i_] == NULL) {
            std::cerr << "Erro ao criar processo " << i_ << std::endl;
            // Limpar processos já criados neste caso;
            for (int j_ = 0; j_ < i_; j_++) {
                CloseHandle(processos_[j_]);
            }
            return false;
        }
        
        std::cout << "Criado processo " << i_ << " com handle " << processos_[i_] << std::endl;
        elemento_atual_ = elemento_fim_;
    }
    
    // Aguardar todos os processos terminarem neste caso;
    WaitForMultipleObjects(num_processos_, processos_.data(), TRUE, INFINITE);
    
    // Fechar handles dos processos neste caso;
    for (int i_ = 0; i_ < num_processos_; i_++) {
        CloseHandle(processos_[i_]);
        std::cout << "Processo " << i_ << " terminou" << std::endl;
    }
    
    return true;
}

int main(int argc_, char* argv_[]) {
    // Verificação dos argumentos de linha de comando neste caso;
    if (argc_ != 4) {
        std::cerr << "Uso: " << argv_[0] << " <arquivo_matriz1> <arquivo_matriz2> <P>" << std::endl;
        return 1;
    }
    
    std::string arquivo_m1_ = argv_[1];
    std::string arquivo_m2_ = argv_[2];
    int P_ = std::atoi(argv_[3]);
    
    // Validação do parâmetro P neste caso;
    if (P_ <= 0) {
        std::cerr << "Valor de P deve ser positivo" << std::endl;
        return 1;
    }
    
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
              << " e " << m2_->n_linhas << "x" << m2_->n_colunas 
              << " com P=" << P_ << " elementos por processo..." << std::endl;
    
    // Medição do tempo de execução total neste caso;
    auto inicio_ = std::chrono::high_resolution_clock::now();
    
    bool sucesso_ = multiplicar_matrizes_paralelo_processos_(m1_, m2_, P_);
    
    auto fim_ = std::chrono::high_resolution_clock::now();
    auto duracao_ = std::chrono::duration_cast<std::chrono::milliseconds>(fim_ - inicio_);
    
    if (!sucesso_) {
        std::cerr << "Erro na multiplicação" << std::endl;
        liberar_matriz(m1_);
        liberar_matriz(m2_);
        return 1;
    }
    
    std::cout << "Tempo total de execução: " << duracao_.count() << " ms" << std::endl;
    std::cout << "Arquivos individuais dos processos salvos em results/resultado_processo_*.txt" << std::endl;
    
    // Liberação da memória alocada neste caso;
    liberar_matriz(m1_);
    liberar_matriz(m2_);
    
    return 0;
}

