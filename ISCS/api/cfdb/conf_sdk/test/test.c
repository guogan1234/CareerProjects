#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* run(void* param){
	printf("running...\n");
	char* data = (char*)param;
	int i = 0;
	while(1){
		data[i] = 'a';
		printf("run - %d\n",i);
		if(i == 1023){
			i = 0;		
		}

		i++;
	}
}

void test(){
	char data[1024];

	pthread_t t;
	pthread_create(&t,NULL,run,data);
	
	printf("test exit!\n");
}

int main(){
	test();
	int a = 0;
	while(1){
		a++;
	}
	return 0;
}
