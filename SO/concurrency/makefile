
all: problema1_signal problema2_shared_memory problema3 problema4 problema4_condition_var thread_producer_consumer

problema1_signal: problema1_signal.c
	gcc -o problema1_signal problema1_signal.c 

problema2_shared_memory: problema2_shared_memory.c
	gcc -o problema2_shared_memory problema2_shared_memory.c

problema3: problema3.c
	gcc -o problema3 problema3.c -pthread -Wno-deprecated

problema4: problema4.c
	gcc -o problema4 problema4.c -pthread -Wno-deprecated

problema4_condition_var: problema4_condition_var.c
	gcc -o problema4_condition_var problema4_condition_var.c -pthread -Wno-deprecated

thread_producer_consumer: thread_producer_consumer.c
	gcc -o thread_producer_consumer thread_producer_consumer.c -pthread -Wno-deprecated