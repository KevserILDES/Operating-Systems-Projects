#
# Script to find whether a string is a palindrome or not
#

if [ $# -eq 0 ] #if no argument is given
then
	echo "$0 : You must enter a string"
	echo "Format : ./myprog1.sh string"
	exit 1
fi

str0=$1
str0=$(sed -e 's/^"//' -e 's/"$//' <<<"$str0")
str0=$(sed -e "s/^'//" -e "s/'$//" <<<"$str0")

##To check whether given argument is all alphabet or not
for w in $str0
do
	if [[ "$w" =~ [^[:alnum:]]+ ]]	
	then
		echo "No special characters allowed"
		exit 2;
	fi
done
echo $str0 | grep [0-9] > test_alpha.log
if test -s test_alpha.log
then
	echo "Invalid: 'not a string'"	
	exit; 
else
	str1=$str0
	str=${str1,,} #Make the given argument all lowercase
	len=${#str} #Length of the string
	
	#For loop to reverse a string char by char
	for ((i=$len;i>0;i--))
	do
	        rstr=$rstr""${str:$i-1:$i}
	        str=${str%${str:$i-1:$i}}
	done
	str=${str1,,} #Update
	
	#For loop to check whether they are same or not
	for ((i = 0; i<$len; i++)) 
	do
		if test "${rstr:$i:1}" != "${str:$i:1}" #if a mismatch occurs on a char exit 	(not palindrome
		then
			echo "$str0 is not a palindrome"
			exit
		fi 
	done
	echo "$str0 is a palindrome" #If all characters match then it is a palindrome
fi
