from flask import Flask, request, jsonify
import subprocess
import os

app = Flask(__name__)

@app.route("/", methods=["GET"])
def home():
    return "Welcome to the Sudoku Validator API! Use the /validar endpoint with a POST request."

@app.route("/validar", methods=["POST"])
def validar():
    data = request.json
    matriz = data.get("matriz")

    # Validate the input matrix
    if not matriz or len(matriz) != 9 or any(len(row) != 9 for row in matriz):
        return jsonify({"erro": "Matriz inválida! A matriz deve ser 9x9."}), 400

    # Save the matrix to a file for the C program to read
    sudoku_file_path = os.path.join(os.path.dirname(__file__), "../c/sudoku.txt")
    with open(sudoku_file_path, "w") as f:
        for row in matriz:
            f.write(" ".join(str(num) for num in row) + "\n")

    # Run the C binary to validate the Sudoku
    binary_path = os.path.join(os.path.dirname(__file__), "../c/sudoku_validator")
    result = subprocess.run([binary_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # Check the result of the C program
    if result.returncode == 0:
        return jsonify({"valido": True, "mensagem": "O Sudoku é válido!"})
    else:
        return jsonify({"valido": False, "mensagem": "O Sudoku é inválido!"})

if __name__ == "__main__":
    # Ensure the server listens on port 8080 for Cloud Run
    port = int(os.environ.get("PORT", 8080))
    app.run(host="0.0.0.0", port=port)
