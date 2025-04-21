from flask import Flask, request, jsonify
import subprocess
import os

app = Flask(__name__)

@app.route("/", methods=["GET"])
def home():
    return "Welcome to the Sudoku Validator API! Use the /validar endpoint with a POST request."

@app.route("/validar", methods=["POST"])
def validar():
    try:
        data = request.json
        matriz = data.get("matriz")

        # Validar a matriz de entrada
        if not matriz or len(matriz) != 9 or any(len(row) != 9 for row in matriz):
            return jsonify({"erro": "Matriz inválida! A matriz deve ser 9x9."}), 400

        # Salva a matriz em um arquivo para o programa C processar
        sudoku_file_path = os.path.join(os.path.dirname(__file__), "sudoku.txt")
        with open(sudoku_file_path, "w") as f:
            for row in matriz:
                f.write(" ".join(str(num) for num in row) + "\n")

        # Executa o programa C
        binary_path = os.path.join(os.path.dirname(__file__), "sudoku_validator")
        result = subprocess.run([binary_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        # Verifica o resultado do programa C
        if result.returncode == 0:
            return jsonify({"valido": True, "mensagem": "O Sudoku é válido!"})
        else:
            return jsonify({"valido": False, "mensagem": "O Sudoku é inválido!"})

    except Exception as e:
        return jsonify({"erro": str(e)}), 500

if __name__ == "__main__":
    # Certifique-se de que o servidor escuta na porta 8080 no Cloud Run
    port = int(os.environ.get("PORT", 8080))
    app.run(host="0.0.0.0", port=port)
