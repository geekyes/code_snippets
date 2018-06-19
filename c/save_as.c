#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUF_MAX_SIZE 1024


int main(int argc, char **argv)
{
    int cnt = 0;
    unsigned char buf[BUF_MAX_SIZE] = {0};
    FILE *pfInFile = NULL;
    FILE *pfOutFile = NULL;

    if (argc != 2 && argc != 3)
    {
        printf("Usage: %s file_path\n", *argv);
        exit(1);
    }

    pfInFile = fopen(*(argv + 1), "rb");
    if (pfInFile == NULL)
    {
        printf("open %s failed!!!\n", *(argv + 1));
        exit(1);
    }
    else
    {
        if (3 == argc)
        {
            pfOutFile = fopen(*(argv + 2), "wb");
            if (NULL == pfOutFile)
            {
                printf("save as file open failed!!!\n");
                exit(1);
            }
        }
        else if (2 == argc)
        {
            pfOutFile = fopen("./tmp", "rb");
            if (pfOutFile) // 检测./tmp是存在
            {
                printf("save as file existed!!!\n");
                printf("Usage: %s file_path [save_as_file_name]\n", *argv);
                exit(1);
            }
            else
            {
                pfOutFile = fopen("./tmp", "wb");
                if (NULL == pfOutFile)
                {
                    printf("save as file open failed!!!\n");
                    exit(1);
                }
            }
        }
    }

    while (!feof(pfInFile))
    {
        cnt = fread(buf, sizeof(unsigned char), BUF_MAX_SIZE, pfInFile);
        fwrite(buf, sizeof(unsigned char), cnt, pfOutFile);
    }

    fclose(pfInFile);
    fclose(pfOutFile);
    printf("save as Done!!!\n");

    return 0;
}

