from flask import Flask, request, jsonify
import subprocess

app = Flask(__name__)

@app.route("/validar", methods=["POST"])
def validar():
    data = request.json
    matriz = data.get("matriz")

    if not matriz or len(matriz) != 9 or any(len(row) != 9 for row in matriz):
        return jsonify({"erro": "Matriz inválida!"}), 400

    # Salva a matriz no arquivo para o programa C ler
    with open("../c/sudoku.txt", "w") as f:
        for row in matriz:
            f.write(" ".join(str(num) for num in row) + "\n")

    # Executa o validador em C
    resultado = subprocess.run(["./sudoku_validator"])

    if resultado.returncode == 0:
        return jsonify({"valido": True})
    else:
        return jsonify({"valido": False})

if __name__ == "__main__":
    app.run(debug=True)
