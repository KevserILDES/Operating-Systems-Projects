

#check if there is a given path
if [ $# -eq 0 ]
then
	#check if there is file whose name ends with .c
	res=$( find . -maxdepth 1 -name '*.c' -type f -print -quit)
	if [ -n "$res" ]
	then
		#create directory as 'cprogs'
		mkdir cprogs
		#move all .c files into 'cprogs'
		mv *.c cprogs
		echo "A directory named cprogs is created under current working directory."
		echo "All the files whose name ends with .c in the curent working director are moved into cprogs directory"
	else
		echo "There is no file whose name ends with .c!"
	fi
else
cd ~
	#check if there is no such path given by user
	if [ ! -e $1 ]
	then
		echo "No such path"
		exit
	else
		#go to given directory
		cd
		cd $1
		#check if there is file whose name ends with .c
		res=$( find . -maxdepth 1 -name '*.c' -type f -print -quit)
		if [ -n "$res" ]
		then
			#create directory as 'cprogs'
			mkdir cprogs
			#move all .c files into 'cprogs'
			mv *.c cprogs
			echo "A directory named cprogs is created under $1 directory."
			echo "All the files whose name ends with .c in the $1 director are moved into cprogs directory"
		else
			echo "There is no file whose name ends with .c!"
		fi
	fi
fi
