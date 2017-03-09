#include "hal.h"

#ifdef __linux__
int UART0_Open(int fd, char* port);
void UART0_Close(int fd);
int UART0_Set(int fd, int speed, int flow_ctrl, int databits, int stopbits, int parity);
int UART0_Init(int fd, int speed, int flow_ctrl, int databits, int stopbits, int parity);
int UART0_Recv(int fd, char *rcv_buf, int data_len);
int UART0_Send(int fd, char *send_buf, int data_len);
#endif


void Serial::SetPath(const string xpath)
{
	path = xpath;
}

void Serial::close()
{
	if (!status)
		return;
#ifdef _WINDOWS
	if (CloseHandle(handle))
		status = false;
#elif defined(__linux__)
	
#endif
}

void Serial::open()
{
	if (is_opened())
		return;
#ifdef _WINDOWS
	handle = CreateFile(path.data(), GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, 0, NULL);
	if (handle == (HANDLE)-1)
		return;
	status = true;
	DCB dcfg;
	if (!GetCommState(handle, &dcfg))
	{
		debug << "Import Configure Faild!" << endl;
		close();
		return;
	}
	dcfg.BaudRate = cfg.speed;
	dcfg.fBinary = true;
	dcfg.fParity = (cfg.parity != NONE);
	switch (cfg.parity)
	{
	case ODD:
		dcfg.Parity = ODDPARITY;
		break;
	case NONE:
		dcfg.Parity = NOPARITY;
		break;
	case MARK:
		dcfg.Parity = MARKPARITY;
		break;
	case EVEN:
		dcfg.Parity = EVENPARITY;
		break;
	}
	dcfg.fOutxCtsFlow = FALSE;  // No CTS output flow control  
	dcfg.fOutxDsrFlow = FALSE;  // No DSR output flow control  
	dcfg.fDtrControl = DTR_CONTROL_DISABLE; // No DTR flow control  
	dcfg.fDsrSensitivity = FALSE; // DSR sensitivity  
	dcfg.fTXContinueOnXoff = TRUE; // XOFF continues Tx  
	dcfg.fOutX = FALSE;     // No XON/XOFF out flow control  
	dcfg.fInX = FALSE;        // No XON/XOFF in flow control  
	dcfg.fErrorChar = FALSE;    // Disable error replacement  
	dcfg.fNull = FALSE;  // Disable null stripping  
	dcfg.fRtsControl = RTS_CONTROL_DISABLE;   //No RTS flow control  
	dcfg.fAbortOnError = FALSE;  // 当串口发生错误，并不终止串口读写  
	dcfg.ByteSize = cfg.data_bit;
	//dcfg.StopBits = cfg.stop_bit;
	if (cfg.stop_bit == 2)
		dcfg.StopBits = TWOSTOPBITS;
	else
		dcfg.StopBits = ONESTOPBIT;
	if (!SetCommState(handle, &dcfg))
	{
		debug << "Setting Faild!" << endl;
		close();
		return;
	}
#elif defined(__linux__)
	/*if (!UART0_Open(&handle, path.data()))
	{
		debug << "Enable Handle Faild!" << endl;
		return;
	}*/
	handle=UART0_Open(handle, path.data());
	if (handle == 0)
	{
		debug << "Handle is Zero!" << endl;
	}
	debug << "Handle ID=" << handle << endl;
	int parity_mode = 'N';
	switch (cfg.parity)
	{
	case ODD:
		parity_mode = 'O';
		break;
	case MARK:
		parity_mode = 'S';
		break;
	case EVEN:
		parity_mode = 'E';
		break;
	case NONE:
		parity_mode = 'N';
		break;
	}
	int value = UART0_Set(handle, cfg.speed, 0, cfg.data_bit, cfg.stop_bit, parity_mode);
	debug << "Return Value:" << value << endl;
	if (!value)
	{
		debug << "Linux Init Faild! Handle Return Empty" << endl;;
		return;
}
#endif
	status = true;
	debug << "Port Open Success!" << endl;
	return;
}

void Serial::open(const string xpath)
{
	SetPath(xpath);
	open();
}

void Serial::open(const SerialConfig xcfg)
{
	SetConfig(xcfg);
	open();
}

void Serial::open(const string xpath, const SerialConfig xcfg)
{
	SetConfig(xcfg);
	SetPath(xpath);
	open();
}

void Serial::SetConfig(const SerialConfig xcfg)
{
	cfg = xcfg;
}

SerialConfig Serial::GetConfig()
{
	return cfg;
}

bool Serial::is_opened()
{
	return status;
}

bool Serial::read(char * buff, int len)
{
#ifdef _WINDOWS
	LPDWORD get=NULL;
	return ReadFile(handle, buff, len, get, NULL);
#elif defined(__linux__)
	return UART0_Recv(handle, buff, len);
#endif
}

bool Serial::write(const char * buff, const int len)
{
#ifdef _WINDOWS
	LPDWORD get = NULL;
	debug << "Writting ...." << endl;
	return WriteFile(handle, buff, len, get, NULL);
#elif defined(__linux__)
	return UART0_Send(handle, buff, len);
#endif
}
