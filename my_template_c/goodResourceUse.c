#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>

bool doResourceUsing()
{
	/* Initialized */
    bool result = false;

	/* Initialized */
    char *pBuffer = NULL; 
    FILE *fd = NULL;
    int sockek_fd = -1;

    do {

        if ((pBuffer = (char *)malloc(sizeof(char))) == NULL)
        {
            perror("malloc fail");
            break;
        }

        if ((fd = fopen("./test_file", "w+"))== NULL)
		{
            perror("file open");
            break;                                                                
        }

        if ((sockek_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("socket open");
            break;
        }

        /* Do something */
		// Use pBuffer 
		// Use fd
		// Use socket_fd

		/* Do something in function is ok */
		result = true; 

    } while (false); 

	/* Release Resource sequence is reverse of create sequence: socket_fd -> fd -> pBuffer */
	if (sockek_fd != -1)	// If fd resource is creadted, it will relesase it.
    {
        close(socket);
        sockek_fd = -1;		// Remember to re-assign  NULL to prevent someone uses it.
    }

	if (fd != NULL)		
	{
		fclose(fd);
		fd = NULL;	
	}

    if (pBuffer != NULL)
    {
        free(pBuffer);
        pBuffer = NULL;
    }

    return result;
}

int main()
{
	bool isGoodResourceUse = false;

	isGoodResourceUse = doResourceUsing();

	if (isGoodResourceUse)
		fprintf(stdout, "No Resource leak\n");
	else
		fprintf(stdout, "Occur Resource leak!\n");

	return 0;
}
