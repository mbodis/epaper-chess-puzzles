#!/usr/bin/env python3
"""
Convert UCI notation to SAN notation in CSV puzzle file.
Requires: pip install chess
"""

import sys
import csv
import chess

def uci_to_san(fen, uci_moves):
    """Convert UCI moves string to SAN notation."""
    board = chess.Board(fen)
    san_moves = []
    
    for uci_move in uci_moves.split():
        try:
            move = chess.Move.from_uci(uci_move)
            san_move = board.san(move)
            san_moves.append(san_move)
            board.push(move)
        except Exception as e:
            print(f"Error converting {uci_move}: {e}", file=sys.stderr)
            san_moves.append(uci_move)  # Fallback to UCI if conversion fails
    
    return " ".join(san_moves)

def convert_csv(input_file, output_file):
    """Convert UCI to SAN in CSV file."""
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
                fen = row[1]
                uci_solution = row[2]
                san_solution = uci_to_san(fen, uci_solution)
                row[2] = san_solution
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
