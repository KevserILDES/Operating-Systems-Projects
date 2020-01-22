

#check if user enter two arguments
if [ $# -eq 2 ]
then
	#check if user enter two integers
	re='^[0-9]+$'
	if [[ $1 =~ $re && $1 =~ $re ]]
	then
		s=$(( $1 - $2 ))
		s2=$(( $s / 2 ))
		m=$(( $s % 2 ))
		#check if first integer greater or not
		if [ $s -le 0 ]; then
			echo "First integer must be greater than second integer"
		#check if the difference between numbers even or not
		elif [ $m -ne 0 ]; then
			echo "The difference between integers must be even"
		else
			#row transition
			for (( i=0; i<$1; i++ ))
			do
				#print '2*' or blank sign in every row
				for (( k=0; k<$1; k++ ))
				do
					#print blank sign when it comes to the column which is half of difference between integer (initial or inner square)
					if [[ $k -eq $(( $s / 2 )) ]]
					then
						#indicates the rows which blank sign is printed
						if [[ $i -ge $s2 && $i -lt $(($1 - $s2)) ]]
						then
							#prints blank sign up to second integer (forms inner square)
							for (( j=0; j<$2; j++ ))
							do
								printf " "
								((k++))
							done
							printf "*"
						else
							printf "*"
						fi
					else
						printf "*"
					fi
				done
				echo
			done
		fi
	else
		echo "You must enter two integers!"
	fi
else
	echo "You must enter two integers!"
fi					

		
