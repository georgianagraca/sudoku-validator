FROM python:3.12

# Cria diretório de trabalho
WORKDIR /app

# Instala gcc e o make
RUN apt-get update && apt-get install -y gcc make

# Copia o conteúdo de src para dentro de /app
COPY assets /app/assets
COPY c /app/c
COPY python /app/python
COPY requirements.txt /app/

# Compila o .so usando make
WORKDIR /app/c
RUN make

# Volta para a pasta correta do Python
WORKDIR /app/python

# Instala as dependências necessárias
RUN pip install -r /app/requirements.txt

# Define variáveis de ambiente para Cloud Run
ENV PORT=8080
ENV FLASK_ENV=development

# Configura a variável DISPLAY e o acesso ao servidor X 
ENV DISPLAY=:0
ENV XAUTHORITY=/root/.Xauthority


# Define comando de inicialização
CMD ["python", "server.py"]
