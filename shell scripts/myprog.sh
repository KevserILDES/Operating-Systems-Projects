#direk programı çalıştırıyo argumenla argument check ?!!!

 #
 # Menu that shows option and takes action according to user's choice
 #

#Loop through the menu untill user wants to exit
 while :
  do
     clear
     echo "-------------------------------------"
     echo " Main Menu "
     echo "-------------------------------------"
     echo "[1] Check for palindromes"
     echo "[2] Move .c files"
     echo "[3] Draw hollowed square"
     echo "[4] Uppercase conversion"
     echo "[5] Delete files"
	 echo "[6] Exit"
     echo "======================="
     echo -n "Enter your menu choice [1-6]: "
     read ch
	#Take action according to choice
     case $ch in
        1) echo "Enter a string" ; read str; ./myprog1.sh $str; read ;; #if 1 take a string and execute palindrome script
 		2) echo "Do you want to enter a specific directory? (y/n)"; read chd; #if 2 ask whether give directory or not
		case $chd in
			y) echo "Enter a directory: "; read dirc; #if yes take the directory 
				if [ ! -e $dirc ] #check if it is valid path
				then
					echo "No such path"
					exit
				fi
				./myprog2.sh $drc; read ;; #Execute the script 2 with given argument
			n) ./myprog2.sh; read;; #Execute the script 2 with no argument
			*) echo "Invalid"; exit;; 
		esac ;;
 		3) echo "Enter two integer: "; read a b; ./myprog3.sh $a $b; read ;; #if 3 take 2 numbers and execute script
 		4) echo "Enter a wildcard string"; read wildc; 
		   echo "Do you want to enter a specific directory? (y/n)"; read chd2; #Ask whether give directory or not
		   case $chd2 in
		   		y) echo "Enter a directory: "; read dirc; #if yes take the directory 
					if [ ! -e $dirc ] #check if it is valid path
					then
						echo "No such path"
						exit
					fi
				./myprog4.sh "$wildc" "$dirc"; read ;; #Execute the script with given directory
				n) ./myprog4.sh $wildc; read;; #Execute the script with just wildcard
				*) echo "Invalid"; exit;; 
			esac ;;
		5) echo "Do you want to enter a specific directory and/or r option? (1 for dir, 2 for r, 3 for both and 4 for none): "; read chd; #Ask and take the option
		   case $chd in
				1) echo "Enter a directory: "; read dirc; #Take just directory
					if [ ! -e $dirc ] #Check
					then
						echo "No such path"
						exit
					fi
					./myprog5.sh $dirc; read;;
				2) ./myprog5.sh -R; read;; #Just -R option
				3) echo "Enter a directory: "; read dirc; #Both
					if [ ! -e $dirc ]
					then
						echo "No such path"
						exit
					fi
					./myprog5.sh -R $drc; read ;;		
				4) ./myprog5.sh; read;;					
				*) echo "Invalid"; exit;;
			esac ;;		
		6) exit 0 ;; #Exit the program
 		*) echo "Opps!!! Please select choice between 1 - 6: ";
 		   echo "Press a key. . ." ; read ;;
     esac
  done
