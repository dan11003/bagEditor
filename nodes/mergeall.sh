folder=$1
var=1
for entry in `ls $folder`; do
	if [ "$var" -eq "2" ] 
	then
		echo "merge $folder/$prev with $folder/${entry} to file output1.bag"
		./bagmerge.py $folder/$prev $folder/$entry -o $folder/output1.bag
	fi
	if [ "$var" -ge "3" ]
	then
		(( prev_bag = var - 2 ))
		(( output = var - 1 ))
		echo "merge output ${prev_bag}.bag with $folder/$entry to file output${output}.bag"
		./bagmerge.py $folder/output${prev_bag}.bag $folder/$entry -o $folder/output${output}.bag
		rm $folder/output${prev_bag}.bag
	fi
	((var+=1))
	prev=$entry
done
