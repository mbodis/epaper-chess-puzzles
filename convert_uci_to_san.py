#!/usr/bin/env python3
"""Prepare Lichess puzzle CSV rows for the e-paper puzzle box.

Lichess stores the position before the opponent's blunder as FEN and includes
that blunder as the first move. The box must show the position after that move
and only reveal the remaining solution moves.
Requires: pip install chess
"""

import sys
import csv
import chess


def uci_to_san(fen, uci_moves):
    """Apply the opponent's first move, then convert the solution to SAN."""
    board = chess.Board(fen)
    moves = uci_moves.split()
    if len(moves) < 2:
        raise ValueError("Puzzle must contain an opponent move and a solution move")

    opponent_move = chess.Move.from_uci(moves[0])
    if opponent_move not in board.legal_moves:
        raise ValueError(f"Illegal opponent move: {moves[0]}")
    board.push(opponent_move)
    puzzle_fen = board.fen()

    san_moves = []
    for uci_move in moves[1:]:
        move = chess.Move.from_uci(uci_move)
        if move not in board.legal_moves:
            raise ValueError(f"Illegal solution move: {uci_move}")
        san_moves.append(board.san(move))
        board.push(move)

    return puzzle_fen, " ".join(san_moves)


def convert_csv(input_file, output_file):
    """Convert UCI to SAN in CSV file and update the puzzle FEN."""
    with open(input_file, 'r', newline='') as infile, \
         open(output_file, 'w', newline='') as outfile:

        # Count total rows for progress
        infile.seek(0)
        total_rows = sum(1 for _ in infile)
        infile.seek(0)

        reader = csv.reader(infile)
        writer = csv.writer(outfile)

        for i, row in enumerate(reader, 1):
            if len(row) >= 3:
                try:
                    row[1], row[2] = uci_to_san(row[1], row[2])
                except (ValueError, IndexError) as e:
                    raise ValueError(f"Error processing row {i}: {e}") from e
            writer.writerow(row)

            # Progress logging
            if i % 100 == 0 or i == total_rows:
                percent = (i / total_rows) * 100
                print(f"[Progress] {i}/{total_rows} ({percent:.1f}%)")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 convert_uci_to_san.py input.csv output.csv")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]
    convert_csv(input_file, output_file)
    print(f"Converted {input_file} to {output_file}")
