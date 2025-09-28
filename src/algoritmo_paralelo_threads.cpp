#include <iostream>
#include <chrono>
#include <fstream>
#include <pthread.h>
#include <vector>
#include <iomanip>
#include <cmath>
#include "matriz_utils.h"

// Estrutura para passar dados para as threads neste caso;
struct DadosThread_ {
    Matriz* A_;
    Matriz* B_;
    Matriz* C_;
    int elemento_inicio_;
    int elemento_fim_;
    int thread_id_;
    long tempo_inicio_;
};

// Função para carregar matriz de arquivo neste caso;
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
    
    // Carregamento dos dados da matriz do arquivo neste caso;
    for (int i_ = 0; i_ < linhas_; i_++) {
        for (int j_ = 0; j_ < colunas_; j_++) {
            arquivo_ >> m_->dados[i_][j_];
        }
    }
    
    arquivo_.close();
    return m_;
}

// Função para salvar resultado parcial no formato Figura 2 neste caso;
bool salvar_resultado_parcial_formato_figura2_(Matriz* C_, int elemento_inicio_, int elemento_fim_, int thread_id_, long tempo_) {
    std::string nome_arquivo_ = "results/resultado_thread_" + std::to_string(thread_id_) + ".txt";
    std::ofstream arquivo_(nome_arquivo_);
    if (!arquivo_.is_open()) {
        std::cerr << "Erro ao abrir arquivo para escrita: " << nome_arquivo_ << std::endl;
        return false;
    }
    
    // Escrita das dimensões da matriz resultado neste caso;
    arquivo_ << C_->n_linhas << " " << C_->n_colunas << "\n";
    
    // Escrita dos elementos calculados por esta thread neste caso;
    for (int i_ = 0; i_ < C_->n_linhas; i_++) {
        for (int j_ = 0; j_ < C_->n_colunas; j_++) {
            int elemento_atual_ = i_ * C_->n_colunas + j_;
            if (elemento_atual_ >= elemento_inicio_ && elemento_atual_ < elemento_fim_) {
                arquivo_ << "c" << (i_ + 1) << (j_ + 1) << " " << std::fixed << std::setprecision(4) << C_->dados[i_][j_] << "\n";
            }
        }
    }
    
    // Escrita do tempo de cálculo desta thread neste caso;
    arquivo_ << tempo_;
    
    arquivo_.close();
    return true;
}

// Função executada por cada thread neste caso;
void* thread_multiplicacao_(void* arg_) {
    DadosThread_* dados_ = (DadosThread_*)arg_;
    
    // Início da medição de tempo individual da thread neste caso;
    auto inicio_thread_ = std::chrono::high_resolution_clock::now();
    
    std::cout << "Thread " << dados_->thread_id_ << " processando elementos " << dados_->elemento_inicio_ 
              << " a " << dados_->elemento_fim_ - 1 << std::endl;
    
    // Multiplicação dos elementos atribuídos a esta thread neste caso;
    for (int elemento_ = dados_->elemento_inicio_; elemento_ < dados_->elemento_fim_; elemento_++) {
        int i_ = elemento_ / dados_->B_->n_colunas;
        int j_ = elemento_ % dados_->B_->n_colunas;
        
        dados_->C_->dados[i_][j_] = 0.0;
        for (int k_ = 0; k_ < dados_->A_->n_colunas; k_++) {
            dados_->C_->dados[i_][j_] += dados_->A_->dados[i_][k_] * dados_->B_->dados[k_][j_];
        }
    }
    
    // Fim da medição de tempo da thread neste caso;
    auto fim_thread_ = std::chrono::high_resolution_clock::now();
    auto duracao_thread_ = std::chrono::duration_cast<std::chrono::milliseconds>(fim_thread_ - inicio_thread_);
    
    // Salvamento do resultado parcial desta thread neste caso;
    salvar_resultado_parcial_formato_figura2_(dados_->C_, dados_->elemento_inicio_, dados_->elemento_fim_, dados_->thread_id_, duracao_thread_.count());
    
    std::cout << "Thread " << dados_->thread_id_ << " concluída em " << duracao_thread_.count() << " ms" << std::endl;
    return nullptr;
}

// Função principal de multiplicação paralela com threads neste caso;
Matriz* multiplicar_matrizes_paralelo_threads_(Matriz* A_, Matriz* B_, int P_) {
    // Verificação da compatibilidade das dimensões neste caso;
    if (A_->n_colunas != B_->n_linhas) {
        std::cerr << "Erro: Dimensões incompatíveis para multiplicação" << std::endl;
        return nullptr;
    }
    
    Matriz* C_ = criar_matriz(A_->n_linhas, B_->n_colunas);
    if (C_ == nullptr) {
        return nullptr;
    }
    
    // Cálculo do número total de elementos da matriz resultado neste caso;
    int total_elementos_ = A_->n_linhas * B_->n_colunas;
    
    // Cálculo do número de threads necessárias baseado em P elementos neste caso;
    int num_threads_ = std::ceil((double)total_elementos_ / P_);
    
    std::vector<pthread_t> threads_(num_threads_);
    std::vector<DadosThread_> dados_threads_(num_threads_);
    
    int elemento_atual_ = 0;
    
    // Criação das threads com divisão por P elementos neste caso;
    for (int i_ = 0; i_ < num_threads_; i_++) {
        int elementos_para_esta_thread_ = std::min(P_, total_elementos_ - elemento_atual_);
        int elemento_fim_ = elemento_atual_ + elementos_para_esta_thread_;
        
        // Configuração dos dados da thread neste caso;
        dados_threads_[i_].A_ = A_;
        dados_threads_[i_].B_ = B_;
        dados_threads_[i_].C_ = C_;
        dados_threads_[i_].elemento_inicio_ = elemento_atual_;
        dados_threads_[i_].elemento_fim_ = elemento_fim_;
        dados_threads_[i_].thread_id_ = i_;
        
        // Criação da thread neste caso;
        if (pthread_create(&threads_[i_], nullptr, thread_multiplicacao_, &dados_threads_[i_]) != 0) {
            std::cerr << "Erro ao criar thread " << i_ << std::endl;
            return nullptr;
        }
        
        std::cout << "Criada thread " << i_ << " para processar elementos " << elemento_atual_ 
                  << " a " << elemento_fim_ - 1 << std::endl;
        
        elemento_atual_ = elemento_fim_;
    }
    
    // Aguardar todas as threads terminarem neste caso;
    for (int i_ = 0; i_ < num_threads_; i_++) {
        if (pthread_join(threads_[i_], nullptr) != 0) {
            std::cerr << "Erro ao aguardar thread " << i_ << std::endl;
        } else {
            std::cout << "Thread " << i_ << " terminou" << std::endl;
        }
    }
    
    return C_;
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
              << " com P=" << P_ << " elementos por thread..." << std::endl;
    
    // Medição do tempo de execução total neste caso;
    auto inicio_ = std::chrono::high_resolution_clock::now();
    
    Matriz* resultado_ = multiplicar_matrizes_paralelo_threads_(m1_, m2_, P_);
    
    auto fim_ = std::chrono::high_resolution_clock::now();
    auto duracao_ = std::chrono::duration_cast<std::chrono::milliseconds>(fim_ - inicio_);
    
    if (resultado_ == nullptr) {
        std::cerr << "Erro na multiplicação" << std::endl;
        liberar_matriz(m1_);
        liberar_matriz(m2_);
        return 1;
    }
    
    std::cout << "Tempo total de execução: " << duracao_.count() << " ms" << std::endl;
    std::cout << "Arquivos individuais das threads salvos em results/resultado_thread_*.txt" << std::endl;
    
    // Liberação da memória alocada neste caso;
    liberar_matriz(m1_);
    liberar_matriz(m2_);
    liberar_matriz(resultado_);
    
    return 0;
}

