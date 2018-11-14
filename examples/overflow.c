#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char buffer[20];

    int readNum = read(0, buffer, 100);

    char buf2[20];

    for(int i=0; i<20; ++i)
        buf2[i] = buffer[i];

    write(1, buffer, readNum);

    return 0;
}