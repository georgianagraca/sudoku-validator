import tkinter as ttk
import ctypes
import numpy as np
import os

class SudokuInterface:
    def __init__(self, root):
        self.root = root
        self.root.title("Jogo Sudoku")
        self.create_grid()
        self.create_control_buttons()
        self.create_numbers_buttons()
        self.celula_selecionada = None 
        self.lib = ctypes.cdll.LoadLibrary(os.path.join(os.path.dirname(__file__), "teste.so")) # Carregando a biblioteca sudoku em C

    # Valida a entrada digitada em cada célula para aceitar apenas um único número
    def number_validator(self, entry):
        if entry == "": return True
        elif len(entry) == 1 and entry.isdigit() and entry != "0": return True
        return False

    # Armazena a celula clicada pelo usuário pegando a posição da linha e da coluna
    def selecionar_celula(self, linha, coluna):
        self.celula_selecionada = (linha, coluna)

    # Insere um número na célula selecionada com base no botão clicado
    def inserir_numero(self, num):
        if self.celula_selecionada:
            linha, coluna = self.celula_selecionada
            if num == "Apagar": self.celulas[linha][coluna].delete(0, ttk.END)
            else:
                self.celulas[linha][coluna].delete(0, ttk.END)
                self.celulas[linha][coluna].insert(0, str(num))

    def create_grid(self):

        # Cria o frame principal
        self.mainframe = ttk.Frame(self.root)
        self.mainframe.pack(expand=True)

        # Cria o frame do grid do jogo
        grid_frame = ttk.Frame(self.mainframe)
        grid_frame.pack(side=ttk.LEFT, padx=50, pady=20, expand=True)

        validation = self.root.register(self.number_validator)

        # Cria o grid 9x9 do jogo Sudoku
        self.celulas = []
        self.values = []
        for linha in range(9):
            linha_celulas = []
            for coluna in range(9):
                celula = ttk.Entry(
                    grid_frame,
                    width=5,
                    font=('Arial', 18),
                    justify='center',
                    validate='key',
                    validatecommand=(validation, '%P')
                )
                celula.grid(row=linha, column=coluna, ipady=5)
                
                # Coloca separador para ficar 9 blocos 3x3
                if linha % 3 == 0 and linha != 0:
                    celula.grid(pady=(6, 0))
                if coluna % 3 == 0 and coluna != 0:
                    celula.grid(padx=(6, 0))
                
                celula.bind("<Button-1>", lambda e, l=linha, c=coluna: self.selecionar_celula(l, c))
                linha_celulas.append(celula)
            self.celulas.append(linha_celulas)

    
    def create_control_buttons(self):

        # Cria o frame dos botões de controle
        control_frame = ttk.Frame(self.mainframe)
        control_frame.pack(side=ttk.RIGHT, padx=20)

        # Botões de controle
        ttk.Button(control_frame, text="Novo jogo", background='#2196f3', width=30, command=self.criar_jogo).pack(pady=5) # Botão responsável por criar um novo jogo
        ttk.Button(control_frame, text="Resolver", background='#4caf50', width=30).pack(pady=5) # Botão responsável por resolver o puzzle
        ttk.Button(control_frame, text="Limpar", background='#f44336', width=30, command=self.limpar_jogo).pack(pady=5) # Botão responsável por limpar o puzzle
        ttk.Button(control_frame, text="Verificar", background='#ffc107', width=30).pack(pady=5) # Botão responsável por validar o puzzle

    def create_numbers_buttons(self):

        # Cria frame para os botões numéricos na parte inferior 
        number_buttons_frame = ttk.Frame(self.root)
        number_buttons_frame.pack(side=ttk.BOTTOM, pady=10)

        # Cria 9 botões de 1-9 que serão utlizados para preencher o puzzle
        number_buttons = ["1", "2", "3", "4", "5", "6", "7", "8", "9", "Apagar"]
        for num in number_buttons:
            ttk.Button(
                number_buttons_frame,
                text=num,
                width=5,
                height=3,
                background='#E3F2FD', 
                foreground='#1565C0', 
                font=('Arial', 12, 'bold'),
                command=lambda n=num: self.inserir_numero(n)
            ).pack(side=ttk.LEFT, padx=2, pady= 50)


    def criar_jogo(self):

        # Determinando os tipos de cada parametro da função ger_puzzle
        self.lib.get_puzzle.argtypes = (ctypes.c_int, ctypes.c_int, ctypes.POINTER(ctypes.c_int))

        # Cria matriz 9x9 
        puzzle = (ctypes.c_int * (9 * 9))()

        # Chamando a função 
        self.lib.get_puzzle(9, 9, puzzle)

        # Convertendo para numpy
        np_puzzle = np.ctypeslib.as_array(puzzle).reshape(9, 9)

        print("matriz recebida de c:")
        print(np_puzzle)

        for linha in range(9):
            for coluna in range(9):
                value = str(np_puzzle[linha][coluna]) if np_puzzle[linha][coluna] != 0 else ''
                self.celulas[linha][coluna].delete(0, 'end')
                self.celulas[linha][coluna].insert(0, value)

    def limpar_jogo(self):
        for linha in range(9):
            for coluna in range(9):
                self.celulas[linha][coluna].delete(0, 'end')


if __name__ == "__main__":
    root = ttk.Tk()
    app = SudokuInterface(root)
    root.mainloop()
