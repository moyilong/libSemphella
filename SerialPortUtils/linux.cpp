#include "api.h"

#define FALSE false
#define TRUE true

int set_Parity(int fd,int databits,int stopbits,int parity)

{

       struct termios options;

       if  ( tcgetattr( fd,&options)  !=  0) {

              perror("SetupSerial 1");    

              return(FALSE); 

       }

       options.c_cflag &= ~CSIZE;

       options.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/

       options.c_oflag  &= ~OPOST;   /*Output*/

 

       switch (databits) /*设置数据位数*/

       {  

       case 7:          

              options.c_cflag |= CS7;

              break;

       case 8:    

              options.c_cflag |= CS8;

              break;  

       default:   

              fprintf(stderr,"Unsupported data size/n"); return (FALSE); 

       }

switch (parity)

{  

       case 'n':

       case 'N':   

              options.c_cflag &= ~PARENB;   /* Clear parity enable */

              options.c_iflag &= ~INPCK;     /* Enable parity checking */

              break; 

       case 'o':  

       case 'O':    

              options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/ 

              options.c_iflag |= INPCK;             /* Disnable parity checking */

              break; 

       case 'e': 

       case 'E':  

              options.c_cflag |= PARENB;     /* Enable parity */   

              options.c_cflag &= ~PARODD;   /* 转换为偶效验*/    

              options.c_iflag |= INPCK;       /* Disnable parity checking */

              break;

       case 'S':

       case 's':  /*as no parity*/  

           options.c_cflag &= ~PARENB;

              options.c_cflag &= ~CSTOPB;break; 

       default:  

              fprintf(stderr,"Unsupported parity/n");   

              return (FALSE); 

       } 

/* 设置停止位*/ 

switch (stopbits)

{  

       case 1:   

              options.c_cflag &= ~CSTOPB; 

              break; 

       case 2:   

              options.c_cflag |= CSTOPB; 

          break;

       default:   

               fprintf(stderr,"Unsupported stop bits/n"); 

               return (FALSE);

}

/* Set input parity option */

if (parity != 'n')  

       options.c_iflag |= INPCK;

tcflush(fd,TCIFLUSH);

options.c_cc[VTIME] = 0; /* 设置超时0 seconds*/  

options.c_cc[VMIN] = 13; /* define the minimum bytes data to be readed*/

if (tcsetattr(fd,TCSANOW,&options) != 0)  

{

       perror("SetupSerial 3");  

       return (FALSE); 

}

return (TRUE); 

}

IO_TYPE pinit(string port, DEVICE_STAT stat)
{
	struct termios Opt;
	IO_TYPE ret=open(port.data(),O_RDWR);
	tcflush(ret,TCIOFLUSH);
	cfsetispeed(&Opt,stat.speed);
	cfsetospeed(&Opt,stat.speed);
	if (tcsetattr(ret,TCSANOW,&Opt)!=0)
		return INVALID_HANDLE_VALUE;
	tcflush(ret,TCIOFLUSH);
	set_Parity(ret,stat.byte_size,stat.stop_bit,N);
	return ret;
}

int pread(IO_TYPE io,char *buff,size_t read_len)
{
	read(io,buff,read_len);
}

int pwrite(IO_TYPE io,const char *buff,size_t send_len)
{
	write(io,buff,write_len);
}
