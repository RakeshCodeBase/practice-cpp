#include "Queue.h"
#include<string.h>

struct FileRead{
	char* data;
	int Count;
	friend std::ostream& operator<<(std::ostream& os,FileRead* P);
};

std::ostream& operator<<(std::ostream& os,FileRead* P){
	os<<P->data<<" "<<P->Count<<std::endl;
	return os;
}

Queue<FileRead*> Q;
int STOPPED=0;
int count=0;

void* Producer(void* args){
	FILE* fp=fopen("test.txt","r+");
	if(fp==NULL) { printf("File not open.\n"); STOPPED=1; return NULL; }
	struct FileRead* FileReadData=(FileRead*)malloc(sizeof(FileRead));
	char* string=(char*)malloc(sizeof(char)*20);
	while(fscanf(fp,"%s",string)==1){
		std::cout<<"Produced: "<<string<<std::endl;
		count++;
		FileReadData->data=string;
		FileReadData->Count=count;
		Q.push(FileReadData);
		FileReadData=(FileRead*)malloc(sizeof(FileRead));
		string=(char*)malloc(sizeof(char)*20);		
	}
	free(FileReadData);
	free(string);
	fclose(fp);
	STOPPED=1;
	//return NULL;
	pthread_exit(NULL);
}

void* Consumer(void* args){
	while(!STOPPED || Q.size()!=0 ){
		struct FileRead* FileReadData=Q.pop();
//		if(strcmp(data,"Empty")==0){ continue; }
		if(FileReadData==NULL) continue;
		std::cout<<"Consumed"<<*((int*)args)<<" :"<<FileReadData->data<<std::endl;
		free(FileReadData->data);
		free(FileReadData);
//		Q.PrintQueue();
	}
//	return NULL;
	pthread_exit(NULL);
}
	
int main(){
	pthread_t producer,consumer1,consumer2,consumer3;
	int i=0,j=1,k=2;
	printf("Thread started\n");
	int prod=pthread_create(&producer,NULL,Producer,NULL);
	int cons=pthread_create(&consumer1,NULL,Consumer,&i);
	int cons1=pthread_create(&consumer2,NULL,Consumer,&j);
	int cons2=pthread_create(&consumer3,NULL,Consumer,&k); 
	if(!prod) pthread_join(producer,NULL);
	if(!cons) pthread_join(consumer1,NULL);
	if(!cons1) pthread_join(consumer2,NULL);
	if(!cons2) pthread_join(consumer3,NULL); 
	printf("Thread ended\n");
}	
