
all: problema1_signal problema2_shared_memory problema3 problema4

problema1_signal: problema1_signal.c
	gcc -o problema1_signal problema1_signal.c 

problema2_shared_memory: problema2_shared_memory.c
	gcc -o problema2_shared_memory problema2_shared_memory.c

problema3: problema3.c
	gcc -o problema3 problema3.c -pthread -Wno-deprecated

problema4: problema4.c
	gcc -o problema4 problema4.c -pthread -Wno-deprecated