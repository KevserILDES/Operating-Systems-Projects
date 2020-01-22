#
# Script to search given string in all txt files and change all of them to uppercase
#

if [ $# -eq 0 ] #if no argument is given
then
	echo "$0 : You must enter a wildcard string"
	echo "Example format : ./of2.sh 'str*ng' /pathname (optinal)" 
	exit 1
fi

str=$1
#to get rid of double or single quotes
str=$(sed -e 's/^"//' -e 's/"$//' <<<"$str")
str=$(sed -e "s/^'//" -e "s/'$//" <<<"$str")
cd ~
if [ -z $2 ] #if directory is not given
then
	drc=$(pwd) #current working directory
elif [ -n $2 ]
then
	if [ ! -e $2 ] #if it is not a valid path
	then
		echo "No such path"
		exit
	fi	
	drc=$2
fi

cd $drc


res=$( find . -maxdepth 1 -name '*.txt' -type f -print -quit )
if [ -n "$res" ] #Check if there is a file with given extension
then
	fil=$(ls *.txt) #Find all txt files' names
	for fn in $fil #Take file names one by one and proceed each one in 1 iteration
	do

		text=$(cat $fn) #The content of the file is assigned to text variable

		for word in $text #Loop through all words in file one by one
		do

			if [[ $word == $str ]]; #if there is a match with wildcard argument given
			then
				#To change first occurence of word to uppercase
				a=$(sed -i -e 0,/$word' '/s/$word' '/${word^^}' '/ $fn) 
				#To change last word in the file (no spaces at the end) to uppercase
				if [ -z $a ]
				then
					sed -i -e 0,/$word/s/$word/${word^^}/ $fn
				fi
				echo "The word '$word' inside '$fn' file is changed to '${word^^}'"
			fi
		done
	done
else
echo "There is no .txt files in directory"
fi
