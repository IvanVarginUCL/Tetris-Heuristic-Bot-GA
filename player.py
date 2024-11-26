from board import Direction, Rotation, Action
from random import Random
import time


class Player:
    def choose_action(self, board):
        raise NotImplementedError


class RandomPlayer(Player):
    def __init__(self, seed=None):
        self.random = Random(seed)

    def print_board(self, board):
        print("--------")
        for y in range(24):
            s = ""
            for x in range(10):
                if (x,y) in board.cells:
                    s += "#"
                else:
                    s += "."
            print(s, y)
                
    def choose_action(self, board):
        self.print_board(board)
        if self.random.random() > 0.97:
            # 3% chance we'll discard or drop a bomb
            return self.random.choice([
                Action.Discard,
                Action.Bomb,
            ])
        else:
            # 97% chance we'll make a normal move
            return self.random.choice([
                Direction.Left,
                Direction.Right,
                Direction.Down,
                Rotation.Anticlockwise,
                Rotation.Clockwise,
            ])


class HeuristicPlayer(Player):
    def __init__(self, seed=None):
        self.random = Random(seed)
        self.to_move = 0
        self.to_rotate = 0
        self.last_cells = None
        self.calls = 0

    def print_board(self, board):
        print("--------")
        for y in range(24):
            s = ""
            for x in range(10):
                if (x,y) in board.cells:
                    s += "#"
                else:
                    s += "."
            print(s, y)

    def choose_action(self, board):
        if self.last_cells != board.cells:
            
            to_rotate, to_move, highest = self.search_moves(board, False)

            if board.discards_remaining > 0 and self.consider_discard(board, highest):
                return Action.Discard
            
            to_bomb, to_move_b = self.consider_bomb(board, highest)
            if to_bomb and board.bombs_remaining > 0:
                self.to_rotate = 0
                self.to_move = to_move_b
                self.last_cells = board.cells.copy()
                return Action.Bomb

            self.to_rotate, self.to_move = to_rotate, to_move
            self.last_cells = board.cells.copy()

        return self.select_action()
    
    def consider_bomb(self, board, eval) -> tuple:
        to_move, highest = self.choose_bomb_location(board)
        cell_map = self.create_cell_map(board)
        return self.score_max_height(cell_map) > 0.75, to_move

    def consider_discard(self, board, highest) -> bool:
        new_board = board.clone()
        new_board.discard()
        _, __, highest_d = self.search_moves(new_board, False)
        return False

    def search_moves(self, board, bomb) -> tuple:
        bestRot = None; bestMove = None
        bestScore = float('-inf')
        for rot in range(4):
            for pos in range(10):
                new_board = board.clone()
                if bomb: new_board.bomb()
                movement = 0
                try:
                    if not bomb:
                        for i in range(rot):
                            new_board.rotate(Rotation.Clockwise)
                    if pos < new_board.falling.left:
                        for i in range(new_board.falling.left - pos):
                            new_board.move(Direction.Left)
                            movement -= 1
                    else:
                        for i in range(pos - new_board.falling.left):
                            new_board.move(Direction.Right)
                            movement += 1
                except:
                    continue
                if (new_board.falling is None) or new_board.falling.left != pos:
                    continue
                new_board.move(Direction.Drop)
                score = self.evaluate(new_board)
                if score > bestScore:
                    bestRot = rot if not bomb else 0; bestMove = movement
                    bestScore = score
        to_rotate = -1 if bestRot == 3 else bestRot
        to_move = bestMove
        return to_rotate, to_move, bestScore
    
    def choose_bomb_location(self, board) -> tuple:
        _, to_move, score = self.search_moves(board, True)
        return to_move, score

    def select_action(self):
        if self.to_rotate == 0 and self.to_move == 0:
            return Direction.Drop
        if self.to_rotate < 0:
            self.to_rotate += 1
            return Rotation.Anticlockwise
        if self.to_rotate > 0:
            self.to_rotate -= 1
            return Rotation.Clockwise
        if self.to_move < 0:
            self.to_move += 1
            return Direction.Left
        if self.to_move > 0:
            self.to_move -= 1
            return Direction.Right
    

    def evaluate(self, board) -> float:
        cell_map = self.create_cell_map(board)
        score = 0
        
        score += self.score_max_height(cell_map) * 1
        score += self.score_total_height(cell_map) * 1
        score += self.score_covered_cells(cell_map) * 1
        score += self.score_bumpiness(cell_map) * 1
        score += self.score_row_transitions(cell_map) * 1
        score += self.score_roof_cells(cell_map) * 1

        return float(score)
    
    # Heuristics

    def score_max_height(self, cell_map) -> float:
        for y in range(24):
            for x in range(10):
                if cell_map[y][x]:
                    return ((24 - y) / 24) ** 2
        return 0
    
    def score_total_height(self, cell_map) -> float:
        total = 0
        for x in range(10):
            for y in range(24):
                if cell_map[y][x]:
                    total += 24 - y
                    break
        return total / 240
    
    def score_covered_cells(self, cell_map) -> float:
        total = 0
        for x in range(10):
            covered = False
            for y in range(24):
                if cell_map[y][x]:
                    covered = True
                else:
                    if covered:
                        total += 1
        return total / 240
    
    def score_bumpiness(self, cell_map) -> float:
        last = 0
        for y in range(24):
            if cell_map[y][0]:
                last = 24 - y
                break
        total = 0
        for x in range(1, 10):
            for y in range(24):
                if cell_map[y][x]:
                    total += abs(24 - y - last)
                    last = 24 - y
                    break
        return total / 240

    def score_row_transitions(self, cell_map) -> float:
        total = 0
        for y in range(24):
            for x in range(1, 10):
                if cell_map[y][x] != cell_map[y][x - 1]:
                    total += 1
        return total / 240
    
    def score_roof_cells(self, cell_map) -> float:
        total = 0
        for x in range(10):
            hole = False
            for y in range(23, -1, -1):
                if cell_map[y][x]:
                    if hole: total += 1
                else:
                    hole = True
        return total / 240

    def create_cell_map(self, board) -> list:
        cell_map = [[False for x in range(10)] for y in range(24)]
        for cell in board.cells:
            cell_map[cell[1]][cell[0]] = True
        return cell_map

SelectedPlayer = HeuristicPlayer