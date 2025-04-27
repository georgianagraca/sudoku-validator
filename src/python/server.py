# Implementa uma API Flask para validar o Sudoku por meio de requisições HTTP.

from flask import Flask, request, jsonify, render_template, send_from_directory
from flask_cors import CORS
import subprocess
import os
import ctypes
import numpy as np
import sys

BASE_DIR = os.path.abspath(os.path.dirname(__file__))

app = Flask(__name__,
            static_folder=os.path.join(BASE_DIR, "..", "assets", "static"),
            template_folder=os.path.join(BASE_DIR, "..", "assets", "templates"))

CORS(app)

class SudokuBackend:
    def __init__(self):
        self.puzzle = None
        self.solution = None
        self.lib_get_puzzle = self._load_get_puzzle_library()
        self.lib_validate = self._load_validator_library()

    def _load_get_puzzle_library(self):
        """Carrega a biblioteca de geração de Sudoku"""
        try:
            lib_path = os.path.join(os.path.dirname(__file__), "..", "c", "get_sudoku.so")
            if not os.path.exists(lib_path):
                raise FileNotFoundError(f"Biblioteca get_sudoku.so não encontrada: {lib_path}")
            
            lib = ctypes.CDLL(lib_path, mode=ctypes.RTLD_GLOBAL)
            # Configura os tipos corretos da função get_puzzle
            lib.get_puzzle.argtypes = (ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int))
            lib.get_puzzle.restype = None
            return lib
        except Exception as e:
            print(f"Erro ao carregar biblioteca get_sudoku.so: {e}", file=sys.stderr)
            return None

    def _load_validator_library(self):
        """Carrega a biblioteca de validação de Sudoku"""
        try:
            # Trocar o nome do arquivo aqui conforme quiser usar 1 thread ou 9 threads
            lib_path = os.path.join(os.path.dirname(__file__), "..", "c", "sudoku_validator_1thread.so")
            # lib_path = os.path.join(os.path.dirname(__file__), "..", "c", "sudoku_validator_9thread.so")
            
            if not os.path.exists(lib_path):
                raise FileNotFoundError(f"Biblioteca de validação não encontrada: {lib_path}")
            
            lib = ctypes.CDLL(lib_path, mode=ctypes.RTLD_GLOBAL)
            lib.verificar_puzzle.argtypes = (ctypes.POINTER(ctypes.c_int),)
            lib.verificar_puzzle.restype = ctypes.c_int
            return lib
        except Exception as e:
            print(f"Erro ao carregar biblioteca de validação: {e}", file=sys.stderr)
            return None

    def new_game(self):
        """Gera um novo Sudoku"""
        if not self.lib_get_puzzle:
            return None, None, "Biblioteca get_sudoku.so não carregada"

        try:
            puzzle = (ctypes.c_int * 81)()
            solution = (ctypes.c_int * 81)()
            self.lib_get_puzzle.get_puzzle(puzzle, solution)

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
        if not self.lib_validate:
            return False, "Biblioteca de validação não carregada"

        try:
            if len(grid) != 81:
                return False, "Grid deve conter exatamente 81 valores"
            
            grid_c = (ctypes.c_int * 81)(*grid)
            is_valid = self.lib_validate.verificar_puzzle(grid_c)
            return bool(is_valid), None
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

        if not grid or len(grid) != 81:
            return jsonify({"error": "Grid inválido! Deve conter exatamente 81 números."}), 400

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
    return jsonify({"puzzle": puzzle})

@app.route("/solve", methods=["GET"])
def solve():
    solution, error = sudoku_backend.solve_game()
    if error:
        return jsonify({"error": error}), 400
    return jsonify({"solution": solution})

if __name__ == "__main__":
    port = int(os.environ.get("PORT", 8080))
    app.run(host="0.0.0.0", port=port, debug=True)
