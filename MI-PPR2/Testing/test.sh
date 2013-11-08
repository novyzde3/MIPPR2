#!/bin/bash

usage="\"./test.sh -s\" pro sekvencni reseni (jeden CPU)\n\"./test.sh -p P\" pro paralelni reseni s P procesory."

if [[ "$1" == "-s" ]] ; then
	qrun="qrun.sh 4c 1 default parallel_job.sh"
elif [[ "$1" == "-p" ]] ; then
	if [[ $2 =~ ^[0-9]+$ ]] && (( $2 <= 12 )) ; then
		 qrun="qrun.sh 12c $2 default parallel_job.sh"
	else
		echo -e "Spatne parametry nebo moc procesoru (maximum je 12), pouziti:\n${usage}"
		exit 1
	fi
else
	echo -e "Spatne parametry, pouziti:\n${usage}"
	exit 2
fi

#echo $qrun
#exit 0

MAXDATA=8

makefile="my-makefile-parallel"


cd ../
#make -f my-makefile-sequence clean
make -f my-makefile-parallel

cd Testing/

# tvorba parallel_job.sh
pj_template="parallel_job.sh-template"
pj="parallel_job.sh"

echo -e "\n##############################\n\n"

for  ((i=1; i<=MAXDATA; i++)) ; do
	cp $pj_template $pj

	out="../out-coin-machine Test-data/data${i}.txt"
	echo $out

	# Nahrazeni programu za nas vlastni output
	sed -i "s|MY_PARALLEL_PROGRAM=\"./my_parallel_program arg1 arg2 ...\"|MY_PARALLEL_PROGRAM=\"${out}\"|" $pj

	# Spusteni ulohy
#	qrun.sh 4c 1 default parallel_job.sh
	`echo $qrun`
	sleep 1
done


exit 0


for
	cmd="../output < Test-data/data${i}.txt"
	cmd="qrun.sh 12c 8 fast parallel_job.sh"
	echo "---------------------------------------"
	echo -e "$cmd\n"
	../output < Test-data/data${i}.txt

done



exit 0
