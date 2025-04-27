# Implementa uma API Flask para validar o Sudoku por meio de requisições HTTP.

from flask import Flask, request, jsonify, render_template, send_from_directory
from flask_cors import CORS
import os
import ctypes
import numpy as np

BASE_DIR = os.path.abspath(os.path.dirname(__file__))

app = Flask(__name__,
            static_folder=os.path.join(BASE_DIR, "..", "assets", "static"),
            template_folder=os.path.join(BASE_DIR, "..", "assets", "templates"))

CORS(app)

class SudokuBackend:
    def __init__(self):
        self.puzzle = None
        self.solution = None
        self.lib_validator9 = ctypes.cdll.LoadLibrary(os.path.join(os.path.dirname(__file__), "..", "c", "sudoku_validator_9thread.so"))
        self.lib_validator1 = ctypes.cdll.LoadLibrary(os.path.join(os.path.dirname(__file__), "..", "c", "sudoku_validator_1thread.so"))
        self.lib_get_sudoku = ctypes.cdll.LoadLibrary(os.path.join(os.path.dirname(__file__), "..", "c", "get_sudoku.so"))

    def new_game(self):
        """Gera um novo Sudoku"""

        try:
            self.lib_get_sudoku.get_puzzle.argtypes = (ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int))

            puzzle = (ctypes.c_int * 81)()
            solution = (ctypes.c_int * 81)()

            self.lib_get_sudoku.get_puzzle(puzzle, solution)

            self.puzzle = np.ctypeslib.as_array(puzzle).reshape(9, 9)
            self.solution = np.ctypeslib.as_array(solution).reshape(9, 9)


            return self.puzzle.tolist(), self.solution.tolist(), None
        except Exception as e:
            return None, None, str(e)

    def solve_game(self):
        """Retorna a solução do Sudoku atual"""
        if self.solution is None:
            return None, "Nenhum puzzle carregado"
        return self.solution.tolist(), None

    def validate_game(self, grid):
        """Valida o grid atual"""

        try:
            grid_c = (ctypes.c_int * 81)(*grid)

            self.lib_validator9.verificar_puzzle.argtypes = (ctypes.POINTER(ctypes.c_int),)
            self.lib_validator9.verificar_puzzle.restype = ctypes.c_int

            #Configurando que a função usa um vetor e retorna um valor inteiro (Validador usando 1 thread)
            self.lib_validator1.verificar_puzzle.argtypes = (ctypes.POINTER(ctypes.c_int),)
            self.lib_validator1.verificar_puzzle.restype = ctypes.c_int

            #chamando a função de verificação
            is_valid1 = self.lib_validator1.verificar_puzzle(grid_c)
            is_valid9 = self.lib_validator9.verificar_puzzle(grid_c)
            return bool(is_valid1 and is_valid9), None
        except Exception as e:
            return False, str(e)

# Inicializa o backend
sudoku_backend = SudokuBackend()

@app.route('/static/<path:path>')
def send_static(path):
    static_dir = os.path.join(BASE_DIR, "..", "assets", "static")
    return send_from_directory(static_dir, path)

@app.route("/", methods=["GET"])
def home():
    return render_template("index.html")

@app.route("/validate", methods=["POST"])
def validate():
    try:
        data = request.json
        grid = data.get("grid")

        valid, error = sudoku_backend.validate_game(grid)
        if error:
            return jsonify({"error": error}), 500

        if valid:
            return jsonify({"valid": True, "message": "Parabéns! Sudoku válido!"})
        else:
            return jsonify({"valid": False, "message": "Sudoku inválido, verifique os números!"})
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route("/new-game", methods=["GET"])
def new_game():
    puzzle, solution, error = sudoku_backend.new_game()
    if error:
        return jsonify({"error": error}), 500
    return jsonify({"puzzle": puzzle, "solution": solution})

@app.route("/solve", methods=["GET"])
def solve():
    solution, error = sudoku_backend.solve_game()
    if error:
        return jsonify({"error": error}), 400
    return jsonify({"solution": solution})

if __name__ == "__main__":
    port = int(os.environ.get("PORT", 8080))
    app.run(host="0.0.0.0", port=port, debug=True)
