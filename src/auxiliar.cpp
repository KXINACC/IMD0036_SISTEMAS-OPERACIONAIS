#include <iostream>  
#include <string>    
#include <cstdlib>   
#include <chrono>
#include "matriz_utils.h" 

int main(int argc_, char* argv_[]) {    
    // Verificação se foram passados os 4 argumentos necessários neste caso;
    if (argc_ != 5) {
        std::cerr << "Uso: " << argv_[0] << " [n1] [m1] [n2] [m2], m1 precisa ser igual a n2." << std::endl;
        return 1;
    }

    // Conversão dos argumentos para inteiros neste caso;
    int n1_ = std::atoi(argv_[1]);
    int m1_ = std::atoi(argv_[2]);
    int n2_ = std::atoi(argv_[3]);
    int m2_ = std::atoi(argv_[4]);

    // Validação se as dimensões são positivas neste caso;
    if (n1_ <= 0 || m1_ <= 0 || n2_ <= 0 || m2_ <= 0) {
        std::cerr << "Programa encerrando: Dimensoes devem ser numeros positivos" << std::endl;
        return 1;
    }

    // Para multiplicar matrizes, o número de colunas da primeira deve ser igual ao número de linhas da segunda neste caso;
    if (m1_ != n2_) {
        std::cerr << "Programa encerrando: O número de colunas (m1) de M1, deve ser igual ao número delinhas (n2) de M2 " << std::endl;
        return 1; 
    }    

    std::cout << "Gerando matriz M1 (" << n1_ << "x" << m1_ << ") e M2 (" << n2_ << "x" << m2_ << ")" << std::endl;
    
    // Início da medição de tempo neste caso;
    auto inicio_ = std::chrono::high_resolution_clock::now();

    std::string file_m1_ = "data/m1.txt";
    Matriz_* mat1_ = criar_matriz(n1_, m1_);
    if (mat1_ == nullptr) {
        std::cerr << "Erro ao criar a M1" << std::endl;
        return 1;
    }
    
    preencher_matriz(mat1_);
    
    // Salvamento da primeira matriz neste caso;
    if (!salvar_matriz_arquivo(mat1_, file_m1_)) {
        std::cerr << "Erro ao salvar M1 em " << file_m1_ << std::endl;
        liberar_matriz(mat1_);
        return 1;
    }
    std::cout << "Matriz M1 salva em " << file_m1_ << std::endl;
    liberar_matriz(mat1_); 

    std::string file_m2_ = "data/m2.txt";
    Matriz_* mat2_ = criar_matriz(n2_, m2_);
    if (mat2_ == nullptr) {
        std::cerr << "Erro ao criar M2" << std::endl;
        return 1;
    }

    preencher_matriz(mat2_);

    // Salvamento da segunda matriz neste caso;
    if (!salvar_matriz_arquivo(mat2_, file_m2_)) {
        std::cerr << "Erro ao salvar M2 em " << file_m2_ << std::endl;
        liberar_matriz(mat2_);
        return 1;
    }

    // Fim da medição de tempo e cálculo da duração neste caso;
    auto fim_ = std::chrono::high_resolution_clock::now();
    auto duracao_ = std::chrono::duration_cast<std::chrono::milliseconds>(fim_ - inicio_);

    std::cout << "Matriz M2 salva em " << file_m2_ << " , Programa concluido" << std::endl;
    std::cout << "Tempo total de geração: " << duracao_.count() << " ms" << std::endl;
    liberar_matriz(mat2_);

    return 0;
}