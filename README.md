# Threads e concorrência: Sudoku Solution Validator

Projeto Final  
Disciplina: Sistemas Operacionais I    
Professor: Davi Henrique dos Santos  
Universidade Federal da Paraíba  
Autores: Beatriz Montenegro, Fellype Dias, Georgiana Graça

## Este repositório contém o código para o Projeto Final da Disciplina
(Add Explicação do Projeto)  
## Instruções de Uso

### Rodar a Aplicação Utilizando Docker

#### 1. Construir a Imagem Docker

Para construir a imagem Docker a partir do `Dockerfile` localizado na pasta `docker`, execute o seguinte comando:

```bash
docker build -t sudoku -f docker/Dockerfile .
```

#### 2. Permitir Acesso ao Servidor X 
Antes de executar o container, no terminal rode o seguinte comando para liberar o acesso ao servidor X para o usuário `root`local:
```bash
xhost +local:root
```

#### 3. Rodar o Container Docker
Após construir a imagem, rode o seguinte comando para iniciar o container e permitir que a interface gráfica seja exibida:

```bash
docker run -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix sudoku
```

### Rodar a Aplicação Localmente

#### 1. Instalar as Dependências (caso ainda não tenha)
Antes de rodar a aplicação localmente, certifique-se de que as bibliotecas necessárias listadas no `requirements.txt` estão instaladas:

```bash
pip install -r requirements.txt
```

Se o `tkinter` não estiver instalado, instale com:
```bash
sudo apt-get install python3-tk
```

#### 2. Compilar a Biblioteca .so
Primeiro, acesse o diretório `src/c`, que é onde está localizado o arquivo C `sudoku_validator.c`

```bash
cd src/c
```

Logo depois, compile o arquivo C `sudoku_validator.c` para gerar a biblioteca compartilhada `.so`, podendo ser feito de duas maneiras:

```bash
make
```

Ou

```bash
gcc -shared -o sudoku_validator.so -fPIC sudoku_validator.c
```

#### 3. Rodar a Aplicação Python
Em seguida, acesse o diretório `src/python` e execute o seguinte comando para rodar a aplicação Python:
```bash
cd src/python
python3 app.py
```


(Add Instruções de Compilação)  
(Add Instruções de Uso)  
(Add quem fez o que)
