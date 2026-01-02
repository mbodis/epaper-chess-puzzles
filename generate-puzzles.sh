#!/bin/sh

FILE="lichess_db_puzzle.csv"

LINES=${1:-1000}
FILES=${2:-100}

MAX_MOVES=13

echo "[epaper-chess-puzzles]: generating $FILES files"
echo "[epaper-chess-puzzles]: each file has $LINES lines"


if [ ! -f $FILE ]; then
    echo "[epaper-chess-puzzles]: File $FILE not found, downloading ..."
	curl -O https://database.lichess.org/lichess_db_puzzle.csv.zst

	echo "[epaper-chess-puzzles]: unpacking puzzles"
	unzstd ./lichess_db_puzzle.csv.zst

	echo "[epaper-chess-puzzles]: removing zst file"
	rm ./lichess_db_puzzle.csv.zst
fi

echo "[epaper-chess-puzzles]: remove 1 line of CSV"
tail -n +2 $FILE > temp

echo "[epaper-chess-puzzles]: filter puzzles with solution length <= MAX_MOVES (3rd CSV column)"
awk -F',' -v max="$MAX_MOVES" '{
    moves = split($3, a, " ")
    if (moves <= max) print
}' temp > temp_filtered && mv temp_filtered temp


echo "[epaper-chess-puzzles]: cut first LINES * FILES lines"
CSV_LINES=$(expr $LINES '*' $FILES)
head -n $CSV_LINES temp > temp2 && mv temp2 temp

echo "[epaper-chess-puzzles]: split into files"
mkdir -p sd-card

split -d -l ${LINES} temp "sd-card/puzzle.csv"

num=1
for file in sd-card/puzzle*; do
    mv "$file" "sd-card/puzzle-$num.csv"
    ((num++))
done

rm temp

echo "[epaper-chess-puzzles]: create config file"
rm -f ./sd-card/config.txt
touch sd-card/config.txt
echo "$LINES" >> "./sd-card/config.txt"
echo "$FILES" >> "./sd-card/config.txt"

echo "[epaper-chess-puzzles]: done"