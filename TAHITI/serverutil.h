#define SERVER_INFO_CLIENT_DEF	0xFFFF994D
void ServerInfoClient(struct DATA_FORMAT in, struct DATA_FORMAT &out, SOCKET& conn);
int ServerInfoCLI(vector<string>arg);