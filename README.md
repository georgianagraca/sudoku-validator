# Threads e concorrência: Sudoku Solution Validator

Projeto Final  
Disciplina: Sistemas Operacionais I    
Professor: Davi Henrique dos Santos  
Universidade Federal da Paraíba  
Autores: Beatriz Montenegro, Fellype Dias, Georgiana Graça

## Este repositório contém o código para o Projeto Final da Disciplina
## Sobre o Projeto
Este trabalho apresenta uma interface capaz de criar, resolver e validar uma partida de sudoku.  
Para isso, a aplicação será rodada em um servidor web que está responsável pela interface e interação com o jogo.  
Entretanto, o real objetivo do projeto é analisar as diferentes formas de validação do puzzle sudoku.  
Essa verificação é iniciada com um arquivo python chamando os arquivos em C que realmente implementam a validação do puzzle. Além disso, mais de uma abordagem é utilizada, usando mais ou menos Threads para executar os testes com cada linha, coluna e quadro do jogo. Dessa maneira, os resultados obtidos gerados sempre que o usuário clica em "Verificar" na aplicação, são na verdade, parte essencial do estudo desse trabalho e como eles se relacionam com os conceitos da área de Sistemas Operacionais.  

## Imagens da Interface
### Imagem da Tela Inicial do Sudoku
### Imagem da Tela com Jogo Novo Criado (Em branco)
### Imagem da Tela com o Jogo com algum Erro
### Imagem da Tela com o Jogo Parcialmente Correto
### Imagem da Tela com o Jogo Completamente Correto

## Instruções de Compilação

### Rodar a Aplicação Utilizando Docker
Para utilizar o Docker, basta abrir o terminal e executar o seguinte comando:
```bash
docker compose up --build
```
Após isso, quando a construção da imagem docker estiver completa e o servidor rodando, basta acessar o localhost:8080 ou localhost:9090, mas é de suma importância garantir que essas portas não estejam sendo utilizadas por outra aplicação.  
Em relação aos resultados da verificação, eles serão gerados na pasta "resultados", na área principal do projeto.

### Rodar a Aplicação Localmente
Uma observação importante, é que devido a natureza dos arquivos gerados para execução desse projeto, ele foi pensado para ser rodado no Sistema Operacional Linux, podendo haver problemas para tentar executar em Windows Localmente seguindo esses passos. Além disso, é preciso ter o Python na versão mais recente possível instalado no local de execução.

#### 1. Instalar as Dependências (caso ainda não tenha)
Antes de rodar a aplicação localmente, certifique-se de que as bibliotecas necessárias listadas no `requirements.txt` estão instaladas.  
Para acessá-las, navegue até a pasta `src`, usando:
```bash
cd src
```
Depois, execute no terminal:
```bash
pip install -r requirements.txt
```
#### 2. Executar o Arquivo Python:
Já estando no diretório `src`, acesse a pasta `python`, que é onde está localizado o arquivo `server.py`
```bash
cd python
```
Depois disso, basta executar o arquivo python usando (comando de execução em Linux):
```bash
python3 server.py
```
Para acessar a aplicação, é semelhante a execução usando docker, acessando localhost:8080 ou localhost:9090, tendo o cuidado de estarem livres. Entretanto, ao contrário do uso do docker, os relatórios dos tempos de execução com diferentes usos de threads serão gerados no mesmo diretório do arquivo `server.py`.  

## Instruções de Uso
### Criação de um Jogo
Após seguir os passos de execução do projeto e acesso a aplicação web, a tela inicial trará uma interface com um sudoku em branco, então basta clicar em **Novo Jogo** para iniciar uma partida. Essa ação gerará uma partida automaticamente e de forma aleatória, apenas deve-se tomar cuidado para não criar outro jogo por acidente ao tocar novamente no botão.  
### Como jogar
Com o novo jogo em mãos, clique nos quadrados vazios, que são possíveis editar para tentar solucionar o jogo seguindo as regras do Sudoku, completando todo o quadro 9x9 sem repetir números nas linhas, nas colunas e em cada quadro 3x3 menor. Todo quadrado editável pode ser mudado livremente durante o jogo e o botão **Limpar** apaga tudo aquilo que você escreveu, caso queira começar do zero.
### Resolução e Verificação
Por fim, se quiser verificar seu resultado, basta clicar em **Verificar** que uma mensagem será enviada, informando se o jogo está parcialmente correto, e você deixou ainda campos em brancos, se ele tem algum erro que fere as regras do Sudoku ou se está correto e você solucionou o puzzle. Além disso, o botão **Resolver** lhe dará o resultado correto daquele puzzle presente, mas também irá sobrescrever qualquer campo que foi editado. Quando acabar, basta apertar em **Novo Jogo** e começar de novo!

## Divisões do Projeto (Quem realizou cada parte)
### Beatriz Montenegro
Ficou responsável pela parte de integração da área de servidor e o código em C, ajudando na verificação na parte de threads também, integrando isso aos códigos em Python que chamavam os arquivos C.  
### Fellype Fontes
Trabalhou principalmente na parte de criação e verificação do puzzle no código C, estudando como o uso de diferentes threads poderiam representar mais ou menos desempenho para realização da validação do puzzle.
### Georgiana Maria
Teve uma essencial contribuição para criação de toda a estrutura do docker, os github actions, a integração com a nuvem e como o servidor em python iria acessar todos esses elementos para interagir com as funções de validação.
### Observação:
Apesar das tarefas indicadas nesta divisão, por muitas vezes, principalmente na ocorrência de erros em uma área do código, o grupo teve que trabalhar em conjunto em todas as partes do projeto, não havendo muita divisão clara de tarefas.
