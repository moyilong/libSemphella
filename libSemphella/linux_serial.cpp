#ifdef __linux__
/************************Copyright(c)*******************************
**                       �����ʵ�ѧԺ
**                       graduate school
**                                     XNMS��Ŀ��
**                       WebSite :blog.csdn.net/tigerjb
**------------------------------------------FileInfo-------------------------------------------------------
** File name:                 main.c
** Last modified Date:  2011-01-31
** Last Version:              1.0
** Descriptions:
**------------------------------------------------------------------------------------------------------
** Created by:               ����
** Created date:            2011-01-31
** Version:                            1.0
** Descriptions:             The original version
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
*******************************************************************/


//������ص�ͷ�ļ�  
#include<stdio.h>      /*��׼�����������*/  
#include<stdlib.h>     /*��׼�����ⶨ��*/  
#include<unistd.h>     /*Unix ��׼��������*/  
#include<sys/types.h>   
#include<sys/stat.h>     
#include<fcntl.h>      /*�ļ����ƶ���*/  
#include<termios.h>    /*PPSIX �ն˿��ƶ���*/  
#include<errno.h>      /*����Ŷ���*/  
#include<string.h>  

#include "debug.h"
//�궨��  
#define FALSE  false
#define TRUE   true

/*******************************************************************
* ���ƣ�                  UART0_Open
* ���ܣ�                �򿪴��ڲ����ش����豸�ļ�����
* ��ڲ�����        fd    :�ļ�������     port :���ں�(ttyS0,ttyS1,ttyS2)
* ���ڲ�����        ��ȷ����Ϊ1�����󷵻�Ϊ0
*******************************************************************/
int UART0_Open(int fd, char* port)
{

	fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
	if (FALSE == fd)
	{
		perror("Can't Open Serial Port");
		return(FALSE);
	}
	//�ָ�����Ϊ����״̬                                 
	if (fcntl(fd, F_SETFL, 0) < 0)
	{
		debug<<"fcntl failed!"<<endl;
		return(FALSE);
	}
	else
	{
		//printf("fcntl=%d\n", fcntl(fd, F_SETFL, 0));
	}
	//�����Ƿ�Ϊ�ն��豸      
	if (0 == isatty(STDIN_FILENO))
	{
		printf("standard input is not a terminal device\n");
		return(FALSE);
	}
	else
	{
		//printf("isatty success!\n");
	}
	//printf("fd->open=%d\n", fd);
	return fd;
}
/*******************************************************************
* ���ƣ�                UART0_Close
* ���ܣ�                �رմ��ڲ����ش����豸�ļ�����
* ��ڲ�����        fd    :�ļ�������     port :���ں�(ttyS0,ttyS1,ttyS2)
* ���ڲ�����        void
*******************************************************************/

void UART0_Close(int fd)
{
	close(fd);
}

/*******************************************************************
* ���ƣ�                UART0_Set
* ���ܣ�                ���ô�������λ��ֹͣλ��Ч��λ
* ��ڲ�����        fd        �����ļ�������
*                              speed     �����ٶ�
*                              flow_ctrl   ����������
*                           databits   ����λ   ȡֵΪ 7 ����8
*                           stopbits   ֹͣλ   ȡֵΪ 1 ����2
*                           parity     Ч������ ȡֵΪN,E,O,,S
*���ڲ�����          ��ȷ����Ϊ1�����󷵻�Ϊ0
*******************************************************************/
int UART0_Set(int fd, int speed, int flow_ctrl, int databits, int stopbits, int parity)
{

	int   i;
	int   speed_arr[] = { B115200, B57600,B19200, B9600, B4800, B2400, B1200, B300 };
	int   name_arr[] = { 115200,  57600,19200,  9600,  4800,  2400,  1200,  300 };

	struct termios options;

	/*tcgetattr(fd,&options)�õ���fdָ��������ز������������Ǳ�����options,�ú��������Բ��������Ƿ���ȷ���ô����Ƿ���õȡ������óɹ�����������ֵΪ0��������ʧ�ܣ���������ֵΪ1.
	*/
	if (tcgetattr(fd, &options) != 0)
	{
		//perror("SetupSerial 1");
		debug<<"Setup Serail Faild!"<<endl;
		debug<<"FD="<<fd<<endl;
		return(FALSE);
	}
	bool speed_set_success=false;
	//���ô������벨���ʺ����������  
	for (i = 0; i < sizeof(speed_arr) / sizeof(int); i++)
	{
		debug<<"Branching Speed:"<<name_arr[i]<<" == "<<speed<<endl;
		if (speed == name_arr[i])
		{
			cfsetispeed(&options, speed_arr[i]);
			cfsetospeed(&options, speed_arr[i]);
			debug<<"Set Current Speed:"<<name_arr[i]<<endl;
			speed_set_success=true;
			break;
		}
	}
	
	if (!speed_set_success)
	{
		debug<<"Set Speed Faild!"<<endl;
		return false;
	}
	//�޸Ŀ���ģʽ����֤���򲻻�ռ�ô���  
	options.c_cflag |= CLOCAL;
	//�޸Ŀ���ģʽ��ʹ���ܹ��Ӵ����ж�ȡ��������  
	options.c_cflag |= CREAD;

	//��������������  
	switch (flow_ctrl)
	{

	case 0://��ʹ��������  
		options.c_cflag &= ~CRTSCTS;
		break;

	case 1://ʹ��Ӳ��������  
		options.c_cflag |= CRTSCTS;
		break;
	case 2://ʹ�����������  
		options.c_cflag |= IXON | IXOFF | IXANY;
		break;
	}
	//��������λ  
	//����������־λ  
	options.c_cflag &= ~CSIZE;
	switch (databits)
	{
	case 5:
		options.c_cflag |= CS5;
		break;
	case 6:
		options.c_cflag |= CS6;
		break;
	case 7:
		options.c_cflag |= CS7;
		break;
	case 8:
		options.c_cflag |= CS8;
		break;
	default:
		fprintf(stderr, "Unsupported data size\n");
		return (FALSE);
	}
	//����У��λ  
	switch (parity)
	{
	case 'n':
	case 'N': //����żУ��λ��  
		options.c_cflag &= ~PARENB;
		options.c_iflag &= ~INPCK;
		break;
	case 'o':
	case 'O'://����Ϊ��У��      
		options.c_cflag |= (PARODD | PARENB);
		options.c_iflag |= INPCK;
		break;
	case 'e':
	case 'E'://����ΪżУ��    
		options.c_cflag |= PARENB;
		options.c_cflag &= ~PARODD;
		options.c_iflag |= INPCK;
		break;
	case 's':
	case 'S': //����Ϊ�ո�   
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;
	default:
		fprintf(stderr, "Unsupported parity\n");
		return (FALSE);
	}
	// ����ֹͣλ   
	switch (stopbits)
	{
	case 1:
		options.c_cflag &= ~CSTOPB; break;
	case 2:
		options.c_cflag |= CSTOPB; break;
	default:
		fprintf(stderr, "Unsupported stop bits\n");
		return (FALSE);
	}

	//�޸����ģʽ��ԭʼ�������  
	options.c_oflag &= ~OPOST;

	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);//�Ҽӵ�  
													   //options.c_lflag &= ~(ISIG | ICANON);  

													   //���õȴ�ʱ�����С�����ַ�  
	options.c_cc[VTIME] = 1; /* ��ȡһ���ַ��ȴ�1*(1/10)s */
	options.c_cc[VMIN] = 1; /* ��ȡ�ַ������ٸ���Ϊ1 */

							//�����������������������ݣ����ǲ��ٶ�ȡ ˢ���յ������ݵ��ǲ���  
	tcflush(fd, TCIFLUSH);

	//�������� (���޸ĺ��termios�������õ������У�  
	if (tcsetattr(fd, TCSANOW, &options) != 0)
	{
		perror("com set error!\n");
		return (FALSE);
	}
	debug<<"return success!"<<endl;
	return true;
}
/*******************************************************************
* ���ƣ�                UART0_Init()
* ���ܣ�                ���ڳ�ʼ��
* ��ڲ�����        fd       :  �ļ�������
*               speed  :  �����ٶ�
*                              flow_ctrl  ����������
*               databits   ����λ   ȡֵΪ 7 ����8
*                           stopbits   ֹͣλ   ȡֵΪ 1 ����2
*                           parity     Ч������ ȡֵΪN,E,O,,S
*
* ���ڲ�����        ��ȷ����Ϊ1�����󷵻�Ϊ0
*******************************************************************/
int UART0_Init(int fd, int speed, int flow_ctrl, int databits, int stopbits, int parity)
{
	debug<<"Use C Lib Default Configure to Load"<<endl;
	//���ô�������֡��ʽ  
	if (UART0_Set(fd, 19200, 0, 8, 1, 'N') == FALSE)
	{
		return FALSE;
	}
	else
	{
		return  TRUE;
	}
}

/*******************************************************************
* ���ƣ�                  UART0_Recv
* ���ܣ�                ���մ�������
* ��ڲ�����        fd                  :�ļ�������
*                              rcv_buf     :���մ��������ݴ���rcv_buf��������
*                              data_len    :һ֡���ݵĳ���
* ���ڲ�����        ��ȷ����Ϊ1�����󷵻�Ϊ0
*******************************************************************/
int UART0_Recv(int fd, char *rcv_buf, int data_len)
{
	/*fd_set fs_read;

	struct timeval time;

	FD_ZERO(&fs_read);
	FD_SET(fd, &fs_read);

	time.tv_sec = 10;
	time.tv_usec = 0;
*/
	return read(fd, rcv_buf, data_len);
}
/********************************************************************
* ���ƣ�                  UART0_Send
* ���ܣ�                ��������
* ��ڲ�����        fd                  :�ļ�������
*                              send_buf    :��Ŵ��ڷ�������
*                              data_len    :һ֡���ݵĸ���
* ���ڲ�����        ��ȷ����Ϊ1�����󷵻�Ϊ0
*******************************************************************/
int UART0_Send(int fd, char *send_buf, int data_len)
{
	int len = 0;

	len = write(fd, send_buf, data_len);
	if (len == data_len)
	{
		return TRUE;
	}
	else
	{
		tcflush(fd, TCOFLUSH);
		return FALSE;
	}

}
#endif