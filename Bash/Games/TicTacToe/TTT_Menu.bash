#!/bin/bash

#  This is an implementation of the game Tic-Tac-Toe
# it's rules and assumptions are as follows:
#   - We will print a game board
#   - We will have 1 player and 2 player modes

width=$(tput cols)
height=$(tput lines)
step_y=$((height / 5))
step_x=$((width / 5))

#### FUNCTION TO DRAW THE BOARD ####
function draw_board {

    # clear
    i=$((2 * step_y))
    j=$step_x;
    while [[ $j -lt $((1 + (4 * width / 5))) ]]; do
	tput cup $i $j
	echo "#"
	tput cup $((i + (1 + step_y))) $j
	echo "#"
	((j += 2)) #skipping makes it look better
    done

    i=$step_y
    j=$((2 * step_x))
    while [[ $i -lt $((1 + (4 * height / 5))) ]]; do
	tput cup $i $j
	echo "#"
	tput cup $i $((j + (1 + step_x)))
	echo "#"
	((i += 1))
    done
}

function print_title {

    # clear
    title="YOU ARE PLAYING TIC-TAC-TOE!!!"
    title_len=${#title}
    tput cup $((step_y / 2)) $(((width / 2) - (title_len / 2)))
    echo $title
}

# trap print_title WINCH
# trap draw_board WINCH

# print_title
# draw_board
# while true;
# do
#     :
# done

### MENU SHOWING USER COMMANDS AND BOARD BELOW IT ###
BG_WHITE_MENU="$(tput setab 7)"
BG_BLACK_BOARD="$(tput setab 0)"
FG_BLUE_MENU="$(tput setaf 4)"
FG_WHITE_BOARD="$(tput setaf 7)"

tput smcup

while [[ $REPLY != 0 ]]; do
    echo -n ${BG_WHITE_MENU}${FG_BLUE_MENU}
    clear
    cat <<- _EOF_
      Please Select:

      1. New Game
      2. Enter an X
      3. Enter an O
      0. Quit

_EOF_

    #    print_title
    read -p "Enter selection: > " selection

    tput cup 11 0
    echo -n ${BG_BLACK_BOARD}${FG_WHITE_BOARD}
    tput ed
    tput cup 12 0

    case $selection in
	1) echo "We are starting a new game"
	   draw_board
	   ;;
	2) echo "We are drawing an X"
	   ;;
	3) echo "We are drawing an O"
	   ;;
	0) break
	   ;;
	*) echo "Invalid Entry"
	   ;;
    esac
    printf "\n\nPress any key to continue."
    read -n 1
done

tput rmcup
echo "Program Terminated."
