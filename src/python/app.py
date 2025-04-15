import tkinter as ttk
from sudoku import SudokuInterface

if __name__ == "__main__":
    root = ttk.Tk()
    app = SudokuInterface(root)
    root.mainloop()