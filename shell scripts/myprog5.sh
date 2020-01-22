#check if argument is given or not
if  [ $# -ne 0 ]
then
	#if -r option is given then proceed as recursive
	if [ $1 == "-R" ] || [ $1 == "-r" ]
	then
		#if directory is not given take current working directory
		if [ -z $2 ]
		then
			drc=$(pwd)
		else
			#path checking
			if [ ! -e $2 ]
			then
				echo "No such path"
				exit
			else
			drc=$2
			fi		
		fi
		#assign a to all files (even under the folders as recursive) with size 0
		a=$(find  $drc -size 0)
	else
	#proceed as not recursive with specific directory
		if [ ! -e $1 ]
		then
			echo "No such path"
			exit
		else
			drc=$1
		fi
		#assign a to just files under given directory (not goes into folders)
		a=$(find  $drc -maxdepth 1 -size 0)
	fi
else
#with no arguments just find all empty files under current working directory
	drc=$(pwd)
	a=$(find  $drc -maxdepth 1 -size 0)
fi

if [ -z $a ]
then
	echo "There is no files whose size is 0"
	exit
fi

#Loop through all files and ask whether to delete it or not and according to choice delete with rm command or pass that file
for word in $a
do
	echo "Delete '$word' 1 for yes 2 for no" 
	read ch
	case $ch in
		1) rm $word; echo "1 file deleted";;
		2) ;;
		*) echo "Invalid"; exit;;
	esac
done
