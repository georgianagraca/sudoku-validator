let selectedCell = null;
let puzzle = null;
let solution = null;

// Seleciona célula
document.querySelectorAll('.sudoku-cell').forEach(cell => {
    cell.addEventListener('click', function() {
        // Remove seleção anterior
        if (selectedCell) {
            selectedCell.classList.remove('selected');
        }
        
        // Seleciona nova célula
        this.classList.add('selected');
        selectedCell = this;
    });
});

// Valida entrada (apenas números 1-9)
function validateInput(input) {
    const value = input.value;
    if (value && (!/^[1-9]$/.test(value))) {
        input.value = '';
    }
}

function selectCell(cell) {
    if (selectedCell) {
        selectedCell.classList.remove('selected');
    }
    cell.classList.add('selected');
    selectedCell = cell;
}

function insertNumber(num) {
    if (selectedCell) {
        if (num === 'Apagar') {
            selectedCell.value = '';
        } else {
            selectedCell.value = num;
        }
    }
}


// Novo jogo
async function newGame() {
    try {
        const response = await fetch('/new-game');
        const data = await response.json();
        
        if (data.error) {
            alert('Erro: ' + data.error);
            return;
        }
        
        puzzle = data.puzzle;
        solution = data.solution;
        
        const cells = document.querySelectorAll('.sudoku-cell');
        for (let i = 0; i < 81; i++) {
            const row = Math.floor(i / 9);
            const col = i % 9;
            const cell = cells[i];
            const value = puzzle[row][col];
            
            cell.value = value !== 0 ? value : '';
            cell.readOnly = value !== 0;
            cell.classList.remove('error'); // Remove marcação de erro se existir
        }
    } catch (error) {
        alert('Erro de conexão: ' + error.message);
    }
}

// Resolver jogo
function solveGame() {
    if (!solution) {
        alert('Carregue um novo jogo primeiro!');
        return;
    }
    
    const cells = document.querySelectorAll('.sudoku-cell');
    for (let i = 0; i < 81; i++) {
        const row = Math.floor(i / 9);
        const col = i % 9;
        cells[i].value = solution[row][col];
        cells[i].readOnly = true;
    }
}

// Limpar jogo
function clearGame() {
    document.querySelectorAll('.sudoku-cell').forEach(cell => {
        if (!cell.readOnly) {
            cell.value = '';
        }
    });
}

// Validar jogo
async function validateGame() {
    const cells = document.querySelectorAll('.sudoku-cell');
    const grid = [];
    
    for (let i = 0; i < 81; i++) {
        const value = cells[i].value || '0';
        grid.push(parseInt(value));
    }
    
    try {
        const response = await fetch('/validate', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ grid: grid }),
        });
        
        const data = await response.json();
        
        if (data.error) {
            alert('Erro: ' + data.error);
            return;
        }
        
        if (data.valid) {
            alert(data.message || 'Parabéns! Você solucionou o puzzle.');
            cells.forEach(cell => cell.readOnly = true);
        } else {
            alert(data.message || 'Existe um erro na solução proposta.');
        }
    } catch (error) {
        alert('Erro de conexão: ' + error.message);
    }
}

// Inicializa um jogo ao carregar a página
window.onload = newGame;