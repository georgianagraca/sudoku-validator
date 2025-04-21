FROM python:3.12

# Define o diretório de trabalho no container
WORKDIR /app

# Copia o requirements.txt para o container
COPY ../requirements.txt /app/

# Instala dependências do sistema (GCC, Make, Python3-Tk, X11)
RUN apt-get update && \
    apt-get install -y gcc make python3-tk x11-apps && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Copia o código C para o container
COPY ../src/c /app/c

# Compila o código C
WORKDIR /app/c
RUN make

# Copia o código Python para o container
WORKDIR /app
COPY ../src/python /app

# Instala dependências do Python
RUN pip install --no-cache-dir -r requirements.txt

# Define variáveis de ambiente
ENV DOCKER_ENV=1
ENV DISPLAY=:0
ENV XAUTHORITY=/root/.Xauthority

# Comando para iniciar a aplicação
CMD ["python", "app.py"]
