# Implementa uma API Flask para validar o Sudoku por meio de requisições HTTP.

from flask import Flask, request, jsonify, render_template, send_from_directory
from flask_cors import CORS  # Para permitir requisições externas (ex: se usar HTML separado)
import subprocess
import os
import ctypes
import numpy as np
import sys

app = Flask(__name__, 
           static_folder="assets", 
           template_folder="assets/templates")
CORS(app) # Para acessar de domínios externos

class SudokuBackend:
    def __init__(self):
        self.puzzle = None
        self.solution = None
        self.lib = self._load_c_library()
        
    def _load_c_library(self):
        """Carrega a biblioteca compartilhada C com tratamento de erros"""
        try:
            lib_path = os.path.join(os.path.dirname(__file__), "c", "sudoku_validator.so")
            
            # Verifica se o arquivo existe
            if not os.path.exists(lib_path):
                raise FileNotFoundError(f"Arquivo da biblioteca não encontrado: {lib_path}")
            
            # Carrega a biblioteca
            lib = ctypes.CDLL(lib_path, mode=ctypes.RTLD_GLOBAL)
            
            # Configura tipos de argumentos e retorno
            lib.get_puzzle.argtypes = (ctypes.POINTER(ctypes.c_int), (ctypes.POINTER(ctypes.c_int)))
            lib.verificar_puzzle.argtypes = (ctypes.POINTER(ctypes.c_int),)
            lib.verificar_puzzle.restype = ctypes.c_int

            return lib

        except Exception as e:
            print(f"Erro ao carregar biblioteca C: {e}", file=sys.stderr)
            return None

    def new_game(self):
        """Gera um novo puzzle usando a biblioteca C"""
        if not self.lib:
            return None, None, "Biblioteca C não carregada"
        
        try:
            puzzle = (ctypes.c_int * 81)()
            solution = (ctypes.c_int * 81)()
            self.lib.get_puzzle(puzzle, solution)
            
            self.puzzle = np.ctypeslib.as_array(puzzle).reshape(9, 9)
            self.solution = np.ctypeslib.as_array(solution).reshape(9, 9)
            
            return self.puzzle.tolist(), self.solution.tolist(), None
        except Exception as e:
            return None, None, str(e)

    def solve_game(self):
        """Retorna a solução do puzzle atual"""
        if self.solution is None:
            return None, "Nenhum puzzle carregado"
        return self.solution.tolist(), None

    def validate_game(self, grid):
        """Valida o grid atual usando a biblioteca C"""
        if not self.lib:
            return False, "Biblioteca C não carregada"
        
        try:
            if len(grid) != 81:
                return False, "Grid deve conter exatamente 81 valores"
            
            grid_c = (ctypes.c_int * 81)(*grid)
            is_valid = self.lib.verificar_puzzle(grid_c)
            return bool(is_valid), None
        except Exception as e:
            return False, str(e)

# Inicializa o backend
sudoku_backend = SudokuBackend()

@app.route('/assets/<path:path>')
def send_static(path):
    return send_from_directory('assets', path)

@app.route("/", methods=["GET"])
def home():
    return render_template("index.html")  # Interface web

@app.route("/validate", methods=["POST"])
def validate():
    try:
        data = request.json
        grid = data.get("grid")  # pegar diretamente o vetor

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
    # Certifique-se de que o servidor escuta na porta 8080 no Cloud Run
    port = int(os.environ.get("PORT", 8080))
    app.run(host="0.0.0.0", port=port, debug=True)
