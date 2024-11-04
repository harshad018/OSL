//EXP 9 Interprocess communication using FIFO
// IPC using FIFO
//first fifo
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include<stdio.h>
int main()
{
int fd,fd1;
char * myfifo = "myfifo";
char buf[512], ch[512];
int words = 1, lines = 1, i=0;
FILE *fp;
char * myfifo1 = "myfifo1";
mkfifo(myfifo1, 0777);
fd = open(myfifo, O_RDONLY);
read(fd, buf, 1024);
printf("Message Received by reader in FIFO1: \n%s\n",
buf);
sleep(2);
while(buf[i]!='\0')
{
if(buf[i]==' ')
words++;
if(buf[i]=='\n')
lines++;
i++;
}
printf("Analysis of message in reader...\n");
printf("Total words = %d \n",words);
printf("Total lines = %d \n",lines);
printf("Total characters = %d \n",i);
sleep(2);
fp = fopen("test.txt", "w");
fprintf(fp,"Total words = %d \n",words);
fprintf(fp,"Total lines = %d \n",lines);
fprintf(fp,"Total characters = %d \n",i);
fclose(fp);
printf("Analysis in wrote in a file test.txt...\n");
sleep(2);
i = 0;
fp = fopen("test.txt", "r");
while(!feof(fp))
{
ch[i] = fgetc(fp);
i++;
}
fclose(fp);
close(fd);
unlink(myfifo);
fd1 = open(myfifo1, O_WRONLY);
write(fd1, ch, sizeof(ch));
printf("Message wrote in FIFO2 by reader...\n");
sleep(2);
close(fd1);
return 0;
}