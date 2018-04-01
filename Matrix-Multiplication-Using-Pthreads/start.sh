#!/bin/bash

matrix=" gcc -pthread mPthreads.c "

total_time=0


if [ -e "r.dat" ] 
	then
		rm -rf "r.dat" 
		touch "r.dat"
	else
		touch "r.dat"
	fi



num_threads=( 1 2 4 32 64 128 512 )


echo "<----------------------generating datafile- r.dat ----------------------->"


                printf "NumOfPthreads      Runtime(s)\n" >> "r.dat"
for i in "${num_threads[@]}"
	do
		:

		TIMES=($(seq 0  30))
		total="0"
		echo "N=$i"
		for (( j=0; j<=30; j++ ))
		do 
			:
			eval $matrix
			resp=$(./a.out $i)
			total=$(bc <<< "scale=30; $total+$resp")
			echo "# Round=$j T=$resp"
		done
		avg=$(bc <<< "scale=30; $total/${#TIMES[@]}")
		printf "$i     $avg\n" >> "r.dat"
		echo "Time avg. = $avg"
	done



echo "<----------------------Written data to $outputfile ------------------------->"
clear
echo "<_______________________________FINISHED____________________________________>"

echo "Generating pdf..."
pdflatex PDF.tex
pdflatex PDF.tex
echo "Written to PDF.pdf"

