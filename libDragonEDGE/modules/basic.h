enum SYS_TYPE {
	DragonOS,
	Windows,
	Linux,
};

struct IO_INFO {
	SYS_TYPE type;
	char API_VER;
};