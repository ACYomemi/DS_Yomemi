line=0
for word in `cat ${1}`
do
	( (line=line+1) )
	if [ ${line} -eq 10 ]
	then
		echo ${word}
	fi
done
