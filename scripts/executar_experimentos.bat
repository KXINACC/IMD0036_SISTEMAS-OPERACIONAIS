@echo off
echo Executando Experimentos E1 e E2 - Sistemas Operacionais
echo ========================================================

REM Criar diretorio de resultados dos experimentos
if not exist "experimentos" mkdir experimentos

echo.
echo Experimento E1: Sequencial vs Paralelo
echo ======================================

REM Gerar matrizes de teste
echo Gerando matrizes de teste...
..\bin\auxiliar.exe 100 100 100 100
copy data\m1.txt experimentos\m1_100.txt
copy data\m2.txt experimentos\m2_100.txt

..\bin\auxiliar.exe 200 200 200 200
copy data\m1.txt experimentos\m1_200.txt
copy data\m2.txt experimentos\m2_200.txt

..\bin\auxiliar.exe 400 400 400 400
copy data\m1.txt experimentos\m1_400.txt
copy data\m2.txt experimentos\m2_400.txt

..\bin\auxiliar.exe 800 800 800 800
copy data\m1.txt experimentos\m1_800.txt
copy data\m2.txt experimentos\m2_800.txt

..\bin\auxiliar.exe 1600 1600 1600 1600
copy data\m1.txt experimentos\m1_1600.txt
copy data\m2.txt experimentos\m2_1600.txt

echo.
echo Executando testes para matriz 100x100...
echo Sequencial:
..\bin\algoritmo_sequencial.exe experimentos\m1_100.txt experimentos\m2_100.txt

echo Threads:
..\bin\algoritmo_paralelo_threads.exe experimentos\m1_100.txt experimentos\m2_100.txt 1250

echo Processos:
..\bin\algoritmo_paralelo_process.exe experimentos\m1_100.txt experimentos\m2_100.txt 1250

echo.
echo Executando testes para matriz 200x200...
echo Sequencial:
..\bin\algoritmo_sequencial.exe experimentos\m1_200.txt experimentos\m2_200.txt

echo Threads:
..\bin\algoritmo_paralelo_threads.exe experimentos\m1_200.txt experimentos\m2_200.txt 5000

echo Processos:
..\bin\algoritmo_paralelo_process.exe experimentos\m1_200.txt experimentos\m2_200.txt 5000

echo.
echo Executando testes para matriz 400x400...
echo Sequencial:
..\bin\algoritmo_sequencial.exe experimentos\m1_400.txt experimentos\m2_400.txt

echo Threads:
..\bin\algoritmo_paralelo_threads.exe experimentos\m1_400.txt experimentos\m2_400.txt 20000

echo Processos:
..\bin\algoritmo_paralelo_process.exe experimentos\m1_400.txt experimentos\m2_400.txt 20000

echo.
echo Executando testes para matriz 800x800...
echo Sequencial:
..\bin\algoritmo_sequencial.exe experimentos\m1_800.txt experimentos\m2_800.txt

echo Threads:
..\bin\algoritmo_paralelo_threads.exe experimentos\m1_800.txt experimentos\m2_800.txt 80000

echo Processos:
..\bin\algoritmo_paralelo_process.exe experimentos\m1_800.txt experimentos\m2_800.txt 80000

echo.
echo Executando testes para matriz 1600x1600...
echo Sequencial:
..\bin\algoritmo_sequencial.exe experimentos\m1_1600.txt experimentos\m2_1600.txt

echo Threads:
..\bin\algoritmo_paralelo_threads.exe experimentos\m1_1600.txt experimentos\m2_1600.txt 320000

echo Processos:
..\bin\algoritmo_paralelo_process.exe experimentos\m1_1600.txt experimentos\m2_1600.txt 320000

echo.
echo Experimento E2: Analise do valor de P
echo =====================================

REM Testar diferentes valores de P para matriz 1600x1600
echo Testando diferentes valores de P para matriz 1600x1600...

echo P = 32000 (n1*m2/8):
..\bin\algoritmo_paralelo_threads.exe experimentos\m1_1600.txt experimentos\m2_1600.txt 32000
..\bin\algoritmo_paralelo_process.exe experimentos\m1_1600.txt experimentos\m2_1600.txt 32000

echo P = 64000 (n1*m2/4):
..\bin\algoritmo_paralelo_threads.exe experimentos\m1_1600.txt experimentos\m2_1600.txt 64000
..\bin\algoritmo_paralelo_process.exe experimentos\m1_1600.txt experimentos\m2_1600.txt 64000

echo P = 128000 (n1*m2/2):
..\bin\algoritmo_paralelo_threads.exe experimentos\m1_1600.txt experimentos\m2_1600.txt 128000
..\bin\algoritmo_paralelo_process.exe experimentos\m1_1600.txt experimentos\m2_1600.txt 128000

echo P = 256000 (n1*m2):
..\bin\algoritmo_paralelo_threads.exe experimentos\m1_1600.txt experimentos\m2_1600.txt 256000
..\bin\algoritmo_paralelo_process.exe experimentos\m1_1600.txt experimentos\m2_1600.txt 256000

echo P = 512000 (2*n1*m2):
..\bin\algoritmo_paralelo_threads.exe experimentos\m1_1600.txt experimentos\m2_1600.txt 512000
..\bin\algoritmo_paralelo_process.exe experimentos\m1_1600.txt experimentos\m2_1600.txt 512000

echo P = 1024000 (4*n1*m2):
..\bin\algoritmo_paralelo_threads.exe experimentos\m1_1600.txt experimentos\m2_1600.txt 1024000
..\bin\algoritmo_paralelo_process.exe experimentos\m1_1600.txt experimentos\m2_1600.txt 1024000

echo.
echo Experimentos concluidos!
echo Os resultados estao nos arquivos results/
echo Verifique os tempos de execucao nos arquivos de resultado.
