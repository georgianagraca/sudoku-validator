# Inicia a classe `SudokuInterface`, que cria a interface gráfica do usuário 
# para o jogo de Sudoku.

import tkinter as ttk
from sudoku import SudokuInterface

if __name__ == "__main__":
    root = ttk.Tk()
    app = SudokuInterface(root)
    root.mainloop()