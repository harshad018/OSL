//2nd fifo
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include<stdio.h>
int main()
{
int fd,fd1;
char * myfifo = "myfifo";
char * myfifo1 = "myfifo1";
char buf[512];
mkfifo(myfifo, 0666);
mkfifo(myfifo1, 0777);
fd = open(myfifo, O_WRONLY);
write(fd, "Hello friends... \nWelcome...\nI FIFO",
1024);
printf("Data wrote in FIFO1 by writer\n");
sleep(2);
close(fd);
unlink(myfifo);
fd1 = open(myfifo1, O_RDONLY);
read(fd1, buf, sizeof(buf));
printf("Data received by FIFO2 by writer\n");
printf("%s",buf);
unlink(myfifo1);
close(fd1);
return 0;
}
