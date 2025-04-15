FROM python:3.12

# Define o diretório de trabalho
WORKDIR /app

# Instala o GCC, o Make e ox11 para a interface
RUN apt-get update && apt-get install -y gcc make python3 python3-tk x11-apps

# Copia os arquivos C
COPY ../src/c/ /app/c

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

# Configura a variável de ambiente
ENV DOCKER_ENV=1

# Configura a variável DISPLAY e o acesso ao servidor X a
ENV DISPLAY=:0
ENV XAUTHORITY=/root/.Xauthority

# Inicia a aplicação
CMD ["python", "app.py"]