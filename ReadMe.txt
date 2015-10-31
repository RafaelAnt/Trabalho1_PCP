gcc -fopenmp -o quicksort quicksort.c
qsub  -lnodes=1:ppn=48,walltime=00:02:00 run
