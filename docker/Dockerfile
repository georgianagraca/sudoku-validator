FROM python:3.12

# Cria diretório de trabalho
WORKDIR /app

# Instala gcc, o make e o tkinter
RUN apt-get update && apt-get install -y gcc make python3 python3-tk

# Copia os arquivos C
COPY ../src/c /app/c

# Compila o .so usando make
WORKDIR /app/c
RUN make

# Copia os arquivos Python
WORKDIR /app
COPY ../src/python/ /app/

# Copia o requirements.txt 
COPY ../requirements.txt /app/

# Instala as dependências necessárias
RUN pip install -r requirements.txt

# Define variáveis de ambiente para Cloud Run
ENV PORT=8080

# Configura a variável DISPLAY e o acesso ao servidor X 
ENV DISPLAY=:0
ENV XAUTHORITY=/root/.Xauthority


# Define comando de inicialização
CMD ["python", "server.py"]
