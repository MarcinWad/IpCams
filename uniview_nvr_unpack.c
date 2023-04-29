#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wolfssl/options.h> 
#include <wolfssl/ssl.h>
#include <wolfssl/openssl/rsa.h>

int main(int argc, char* argv[]) {

	printf("Uniview NVR firmware splitter by Speed666\n");
	if (argc !=2)
	{
		printf("Usage: unpack [filename] \n");
		return 1;
	}

	FILE* fp;
	FILE* fpw;
	fp=fopen(argv[1], "rb");
	
	unsigned char buffer[144];

	
	unsigned char a[4];
	unsigned char bufferout[128];
	
	
	unsigned int size;
	unsigned int packs = 32; //Temporary
	unsigned int offset;
	char *memory;
	char *name;
	
	//Skip Uniview header
	fseek(fp,0x800,0);
	
	//First 4 bytes is number of packs inside
	
	for (int i = 0;i<packs;i++)
	{
		fseek(fp,0x800+144*i,0);
		
		fread(a, 1, 4, fp);
		if (i == 0)
		{
			//First byte in first pack is number of packs
			packs = (a[3] << 24) | (a[2] << 16) | (a[1] << 8) | a[0];
		}
		
		fread(buffer, 1, 128, fp); //Puste dane
		
		fread(a, 1, 4, fp);
		size = (a[3] << 24) | (a[2] << 16) | (a[1] << 8) | a[0];
		printf("Image size: %d bytes\n",size);
		fread(a, 1, 4, fp);
		offset = (a[3] << 24) | (a[2] << 16) | (a[1] << 8) | a[0];
		printf("Offset: 0x%X \n",offset);
		memory = (char*)malloc(size);
		name = (char*)malloc(strlen(buffer)+1);
		strcpy (name, buffer);
		printf("Image name: %s\n\n",name);
		fseek(fp,offset,0);
		fread(memory, 1, size, fp);
		fpw=fopen(name, "wb");
		fwrite(memory, size, 1, fpw);
		free(memory);
		fclose(fpw);
	}	

}