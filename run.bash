#!/bin/bash

rm ./debug/projet &>/dev/null
gcc *.c -Wall -Wextra -Werror -o ./debug/projet

if test -e debug/projet
then
	./debug/projet
else
	echo "Erreur gcc"
fi
