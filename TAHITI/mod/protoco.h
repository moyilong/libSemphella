struct PROTOCO_INFO{
	int protoco_count = 0;
};




#define PROTOCO_API	0x800000AA

#ifdef __LINUX__
#define COMMAND_HEAD	"sh "
#define SCRIPT_DIR		"/etc/protoco.d"
#else
#define COMMAND_HEAD	"cmd.exe /C "
#define SCRIPT_DIR		"protoco.d"
#endif