#include<stdlib.h>
#include<stdio.h>

#define BLK_SIZE (1024*1024)

int main (int argc, char *argv[])
{
	void *vptr;
	unsigned int blk_count;

	for(;;) {
		vptr = malloc(BLK_SIZE);
		if (NULL == vptr) {
			printf("Memory Exhausted !!!\n");
			break;
		}
		blk_count++;
		printf("Succeed in allocating the %u-th block.\n", blk_count);
		//printf("address of the block =%d\n\n", (long)vptr);
		printf("address of the block =%x\n\n", (unsigned int)vptr);

	}
	free(vptr);
	return 0;
}
