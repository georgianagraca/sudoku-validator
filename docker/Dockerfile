# Define como construir o container 
# Python 3.12, compila C, instala dependências, roda server.py
FROM python:3.12

# Cria diretório de trabalho
WORKDIR /app

# Instala dependências
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    python3-dev \
    libc6-dev \
    && rm -rf /var/lib/apt/lists/*

# Copia arquivos C
COPY src/c/ /app/c/
COPY src/c/sudoku.csv /app/

# Compila a biblioteca
WORKDIR /app/c
RUN make clean && make && \
    file sudoku_validator.so && \
    ldd sudoku_validator.so && \
    ls -la

# Volta para o diretório principal e copia o resto
WORKDIR /app
COPY src/python/ /app/
COPY requirements.txt /app/

# Instala as dependências necessárias
RUN pip install --no-cache-dir -r requirements.txt

# Variáveis de ambiente para o Cloud Run
ENV PORT=8080
ENV DOCKER_ENV=1
ENV FLASK_APP=server.py
ENV FLASK_ENV=development

# Configura a variável DISPLAY e o acesso ao servidor X 
# ENV DISPLAY=:0
# ENV XAUTHORITY=/root/.Xauthority

# Define comando de inicialização
CMD ["python", "server.py"]
