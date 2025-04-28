# Threads e concorrência: Sudoku Solution Validator

Projeto Final  
Disciplina: Sistemas Operacionais I    
Professor: Davi Henrique dos Santos  
Universidade Federal da Paraíba  
Autores: Beatriz Montenegro, Fellype Dias, Georgiana Graça

## Sobre o Projeto
Este trabalho tem como objetivo desenvolver um validador para o jogo Sudoku, explorando diferentes abordagens de concorrência utilizando Threads. O sistema foi implementado para rodar em um servidor, permitindo a análise de desempenho sob as seguintes estratégias:

#### 1. Validação com Três Threads:

- Uma thread para verificar as linhas
- Uma thread para verificar as colunas
- Uma thread para verificar os quadros (grid 3x3)

#### 2. Validação com Nove Threads:

- Nove threads, em que cada uma é responsável por validar uma linha
- Nove threads, em que cada uma é responsável por validar uma coluna
- Nove threads, em que cada uma é responsável por validar umm quadro (grid 3x3)

De forma complementar, também foi desenvolvida uma aplicação web interativa, hospedada em um servidor, que permite ao usuário:

- Criar novas partidas
- Resolver tabuleiros automaticamente
- Limpar o jogo
- Verificar a validade de uma solução

A implementação combina código em C, para a validação e criação do jogo, com uma interface simples em Python. Os resultados gerados ao clicar em "Verificar" não apenas validam do partida, mas também servem como base para analisar o desempenho das diferentes abordagens utilizando Threads, relacionando-as diretamente com os conceitos estudados na disciplina de Sistemas Operacionais.

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
#### 2. Compilar a Biblioteca .so
 Primeiro, acesse o diretório `src/c`, que é onde está localizado o arquivo C `sudoku_validator.c` e, logo depos, compile os arquivos C para gerar as bibliotecas compartilhadas `.so`
 
 ```bash
cd src/c
make
 ```

#### 3. Executar o Arquivo Python:
 Em seguida, acesse o diretório `src/python` e execute o seguinte comando para rodar a aplicação Python:
 ```bash
 cd src/python
 python3 server.py
 ``````
Para acessar a aplicação, é semelhante a execução usando docker, acessando localhost:8080 ou localhost:9090, tendo o cuidado de estarem livres. Entretanto, ao contrário do uso do docker, os relatórios dos tempos de execução com diferentes usos de threads serão gerados no mesmo diretório do arquivo `server.py`.  

## Instruções de Uso
### Criação de um Jogo
Após seguir os passos de execução do projeto e acesso a aplicação web, a tela inicial trará uma interface com um sudoku em branco, como mostrado na imagem abaixo: 

![tela_inicial](https://github.com/user-attachments/assets/4e84b926-f44e-44c3-9258-3659ef820bf8)

Para criar uma partida, basta clicar no botão **Novo Jogo**. Essa ação gerará uma partida automaticamente e de forma aleatória, apenas deve-se tomar cuidado para não criar outro jogo por acidente ao tocar novamente no botão. 

![tela_inicial](https://github.com/user-attachments/assets/81a7cf35-0a27-4da0-9d24-de24320395f8)

### Como jogar
Com o novo jogo em mãos, clique nos quadrados vazios, que são possíveis editar para tentar solucionar o jogo seguindo as regras do Sudoku, completando todo o quadro 9x9 sem repetir números nas linhas, nas colunas e em cada quadro 3x3 menor. Todo quadrado editável pode ser mudado livremente durante o jogo e o botão **Limpar** apaga tudo aquilo que você escreveu, caso queira começar do zero. 

### Resolução e Verificação
Por fim, se quiser verificar seu resultado, basta clicar em **Verificar** que uma mensagem será enviada, informando se o jogo está parcialmente correto, e você deixou ainda campos em brancos, se ele tem algum erro que fere as regras do Sudoku ou se está correto e você solucionou o puzzle, como mostrado nas imagens abaixo: 

- Verificação mostrando menssagem de erro durante a resolução do puzzle:

![verificar_erro](https://github.com/user-attachments/assets/2476f190-3121-4db5-b155-2d8f8333bbb3)

- Verificação mostrando menssagem de que o jogo em andamento se encontra até o momento correto:

![verificar_emAndamento](https://github.com/user-attachments/assets/78c05093-ed85-4b75-ace4-69fadbe0c335)

- Verificação mostrando mensagem de que o jogo foi resolvido com sucesso:

![verificar_completo](https://github.com/user-attachments/assets/9eed78b9-385d-4a0a-ab1f-8977ab9b6162)


Além disso, o botão **Resolver** lhe dará o resultado correto daquele puzzle presente, mas também irá sobrescrever qualquer campo que foi editado. Quando acabar, basta apertar em **Novo Jogo** e começar de novo! 

![resolver](https://github.com/user-attachments/assets/eeef111f-8f54-43a6-b855-8f04b5d0a7cb)

## Divisões do Projeto (Quem realizou cada parte)
### Beatriz Montenegro
Ficou responsável pela parte de integração da área de servidor e o código em C e verificação na parte de threads também, integrando isso aos códigos em Python que chamavam os arquivos C.  
### Fellype Fontes
Trabalhou principalmente na parte de criação e verificação do puzzle no código C, estudando como o uso de diferentes threads poderiam representar mais ou menos desempenho para realização da validação do puzzle.
### Georgiana Maria
Teve uma essencial contribuição para criação de toda a estrutura do docker, os github actions, a integração com a nuvem e como o servidor em python iria acessar todos esses elementos para interagir com as funções de validação.
### Observação:
Apesar das tarefas indicadas nesta divisão, por muitas vezes, principalmente na ocorrência de erros em uma área do código, o grupo teve que trabalhar em conjunto em todas as partes do projeto, não havendo muita divisão clara de tarefas.
