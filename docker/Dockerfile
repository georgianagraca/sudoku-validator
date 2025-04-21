FROM python:3.12

# Cria diretório de trabalho
WORKDIR /app

# Instala gcc e make para compilar o C
RUN apt-get update && apt-get install -y gcc make

# Copia e compila os arquivos C
COPY ./src/c /app/c
WORKDIR /app/c
RUN make

# Volta para o diretório da aplicação Python
WORKDIR /app

# Copia os arquivos Python individualmente (não a pasta)
COPY ./src/python/*.py /app/

# Copia o requirements.txt e instala dependências
COPY ../requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Define variáveis de ambiente para Cloud Run
ENV PORT=8080

# Define comando de inicialização
CMD ["python", "server.py"]
