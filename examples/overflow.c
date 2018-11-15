#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    char buffer[20];

    // read from stdin
    int readNum = read(0, buffer, 100);

    char buf2[20];

    // copy buffer to another buffer
    for(int i=0; i<20; ++i)
        buf2[i] = buffer[i];

    // write to std out
    write(1, buffer, readNum);

    return 0;
}