# Configuração do Docker

FROM ubuntu:latest

RUN apt update && apt install -y gcc python3 python3-tk make

WORKDIR /app
COPY src/c /app/c
COPY src/python /app/python

WORKDIR /app/c
RUN make

ENTRYPOINT ["/app/c/sudoku_validator"]
CMD []

