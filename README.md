# Trabalho Prático - Sistemas Operacionais
## Multiplicação de Matrizes: Sequencial vs Paralelo

### 📋 Descrição
Implementação de algoritmos de multiplicação de matrizes utilizando diferentes abordagens de programação paralela para análise de desempenho.

### 🚀 Funcionalidades Implementadas

#### 1. **Auxiliar** (`auxiliar.exe`)
- Gera matrizes aleatórias de dimensões especificadas
- Salva matrizes em arquivos para uso pelos outros programas
- **Uso**: `auxiliar.exe n1 m1 n2 m2`

#### 2. **Sequencial** (`algoritmo_sequencial.exe`)
- Multiplicação de matrizes de forma tradicional
- Salva resultado no formato Figura 2
- **Uso**: `algoritmo_sequencial.exe arquivo_matriz1 arquivo_matriz2`

#### 3. **Paralelo Threads** (`algoritmo_paralelo_threads.exe`)
- Multiplicação usando threads com divisão por P elementos
- Gera múltiplos arquivos individuais no formato Figura 2
- **Uso**: `algoritmo_paralelo_threads.exe arquivo_matriz1 arquivo_matriz2 P`

#### 4. **Paralelo Processos** (`algoritmo_paralelo_process.exe`)
- Multiplicação usando processos com divisão por P elementos
- Gera múltiplos arquivos individuais no formato Figura 2
- **Uso**: `algoritmo_paralelo_process.exe arquivo_matriz1 arquivo_matriz2 P`

### 📁 Estrutura do Projeto

```
├── src/                           # Código fonte
│   ├── auxiliar.cpp              # Gerador de matrizes
│   ├── algoritmo_sequencial.cpp  # Algoritmo sequencial
│   ├── algoritmo_paralelo_threads.cpp  # Algoritmo com threads
│   ├── algoritmo_paralelo_process.cpp  # Algoritmo com processos
│   ├── matriz_utils.h            # Estruturas e funções auxiliares
│   └── matriz_utils.cpp          # Implementação das funções auxiliares
├── bin/                          # Executáveis compilados
├── data/                         # Matrizes de entrada
├── results/                      # Resultados dos algoritmos
├── report/                       # Relatório do trabalho
├── scripts/                      # Scripts de experimentos
└── README.md                     # Este arquivo
```

### 🔧 Compilação

```bash
# Compilar todos os programas
cd src
g++ -std=c++11 -Wall -Wextra -O2 -o ../bin/auxiliar.exe auxiliar.cpp matriz_utils.cpp
g++ -std=c++11 -Wall -Wextra -O2 -o ../bin/algoritmo_sequencial.exe algoritmo_sequencial.cpp matriz_utils.cpp
g++ -std=c++11 -Wall -Wextra -O2 -o ../bin/algoritmo_paralelo_threads.exe algoritmo_paralelo_threads.cpp matriz_utils.cpp -pthread
g++ -std=c++11 -Wall -Wextra -O2 -o ../bin/algoritmo_paralelo_process.exe algoritmo_paralelo_process.cpp matriz_utils.cpp
```

### 📊 Experimentos

#### Experimento E1: Sequencial vs Paralelo
Execute o script de experimentos para testar diferentes tamanhos de matriz:

```bash
cd scripts
executar_experimentos.bat
```

#### Experimento E2: Análise do Valor de P
Teste diferentes valores de P para otimizar o desempenho:

- P = ⌈(n1*m2)/8⌉ (mínimo)
- P = ⌈(n1*m2)/4⌉
- P = ⌈(n1*m2)/2⌉
- P = ⌈(n1*m2)⌉
- P = ⌈2*(n1*m2)⌉
- P = ⌈4*(n1*m2)⌉ (máximo)

### 📈 Formato de Saída

Todos os algoritmos geram arquivos no formato Figura 2:

```
3 3
c11 43.1728
c12 108.4212
c13 151.6455
c21 38.7268
c22 91.4893
c23 123.3572
c31 36.9738
c32 90.7083
c33 122.8149
0
```

### 🎯 Características do Código

- ✅ **Nomenclatura**: Todas as variáveis e classes terminam com "_"
- ✅ **Comentários**: Terminam com ";" e usam "neste caso" e "Isso faz com que"
- ✅ **Estrutura**: Código bem organizado e documentado
- ✅ **Formato**: Saída no formato Figura 2 especificado
- ✅ **Paralelização**: Implementação correta de threads e processos

### 📋 Relatório

O relatório completo está disponível em `report/relatorio_trabalho_pratico.md` e inclui:

- Análise dos experimentos E1 e E2
- Discussão dos resultados obtidos
- Determinação do valor ideal de P
- Conclusões sobre programação paralela

### 🚀 Como Executar

1. **Compile os programas** (veja seção Compilação)
2. **Gere matrizes de teste**:
   ```bash
   .\bin\auxiliar.exe 100 100 100 100
   ```
3. **Execute os algoritmos**:
   ```bash
   .\bin\algoritmo_sequencial.exe data\m1.txt data\m2.txt
   .\bin\algoritmo_paralelo_threads.exe data\m1.txt data\m2.txt 1250
   .\bin\algoritmo_paralelo_process.exe data\m1.txt data\m2.txt 1250
   ```
4. **Execute os experimentos**:
   ```bash
   cd scripts
   executar_experimentos.bat
   ```

### 📊 Resultados Esperados

- **Sequencial**: Arquivo único com resultado completo
- **Threads**: Múltiplos arquivos `resultado_thread_*.txt`
- **Processos**: Múltiplos arquivos `resultado_processo_*.txt`

### 🔗 Repositório

**GitHub**: https://github.com/KXINACC/IMD0036_SISTEMAS-OPERACIONAIS.git

---

**Desenvolvido para**: IMD0036 - Sistemas Operacionais  
**Universidade Federal do Rio Grande do Norte**  
**Instituto Metrópole Digital**