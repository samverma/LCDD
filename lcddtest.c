
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>

#define BUFFLEN 512               
static char receive[BUFFLEN];     

int main(){
   int ret, fd;
   char msg[BUFFLEN];
   printf("Starting LCD to test\n");
   fd = open("/dev/lcd", O_RDWR);           
   if (fd < 0){
      perror("Opening LCD failed"); 
      return errno; 
   }
   printf("Input string to write to module:\n");
   scanf("%[^\n]%*c", msg);                
   printf("Writing message: [%s]\n", msg);
   ret = write(fd, msg, strlen(msg)); 
   printf("Press 'r' to read back message from module\n");
   getchar();
   printf("Reading message: ");
   ret = read(fd, receive, BUFFLEN);        
   printf("[%s]\n", receive);
   printf("Test completed\n");
   return 0;
}