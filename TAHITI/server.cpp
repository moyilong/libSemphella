#include "stdafx.h"
#include "modules.h"

#ifndef __LINUX__
inline void close(SOCKET conn)
{
	closesocket(conn);
}
#endif

struct ServerInfo info;


void ErrSetBack(struct DATA_FORMAT &ret, const char *ERR_INFO)
{
	ret.def = ERR_BACK;
	ret.real_data_len = strlen(ERR_INFO);
	strcpy(ret.buff,ERR_INFO);
}

 void server_main()
 {
	 DEBUG_LINE {
		cout <<"=============TAHITI Project Debug Table==============="<<endl;
		cout <<"feature_define.h Version:"<<S_API_VERSION<<endl;
		cout <<"feature_define.h min support:"<<S_API_ALLOW_MIN<<endl;
		cout <<"feature_define.h Send Len:"<<SEND_LEN<<endl;
		cout << "feature_define.h error_id:" << ERR_BACK << endl;
		cout << "Count of Modules:" << get_modules_size() << endl;
		cout <<"=============TAHITI Project Debug Table==============="<<endl;
	}
	 SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);
	 if (sListen == INVALID_SOCKET){
		 cout << "Create Socket Faild!" << endl;
		 return ;
	 }

	 sockaddr_in serv;
	 serv.sin_family = AF_INET;
	 serv.sin_port = htons(PORT);	
	 serv.sin_addr.s_addr = htonl(INADDR_ANY);	

	 if (bind(sListen, (sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR){
		 cout << "Bind Socket Faild!" << endl;
		 return ;
	 }

	 if (listen(sListen, 5) == SOCKET_ERROR){
		 cout << "Listen Prot Faild!"<<endl;
		 return ;
	 }



	 char buff[SEND_LEN];

 while(true){
	 info.server_io_count++;
	 DEBUG_LINE cout << "=======================================" << endl << "Wait for Accept..." << endl;
	 DEBUG_LINE cout<<" Count Connect:"<<info.server_io_count<<endl;	 
	 
	 SOCKET sAccept;
	 struct DATA_FORMAT get_in;
	 struct DATA_FORMAT ret;
	 sockaddr_in client;
#ifdef __LINUX__
	 unsigned
#endif
	 int iLen = sizeof(client);

	 sAccept = accept(sListen, (sockaddr*)&client,&iLen);
	 if (sAccept == SOCKET_ERROR){
		 goto LOOP_STOP;
	 }
	 recv(sAccept, buff, SEND_LEN, 0);
	 if (ret.HEAD_CHECK_B != GetHeadCheck(ret.HEAD_CHECK_A))
	 {
		 cout << "Catch a Unknow Data,skip it!" << endl;
		 goto LOOP_STOP;
	 }
	 if ((get_in.API_LEVEL > S_API_VERSION) && (get_in.API_LEVEL < S_API_ALLOW_MIN))
	 {
		 ret.def = ERR_BACK;
		 strcpy(ret.buff, _ERROR_API_MISMATCH);
		 ret.real_data_len = strlen(_ERROR_API_MISMATCH);
		 goto SEND;
	 }
	 Convert(buff, sizeof(struct DATA_FORMAT));
	 memcpy(&get_in, buff, SEND_LEN);
	 DEBUG_LINE cout << "Securty Check..." << endl;

	 ErrSetBack(ret, _ERROR_NO_ERROR);
	 DEBUG_LINE cout << "Start to Match Modules:" <<get_in.def<< endl;
	for (int n = 0; n < get_modules_size(); n++)
		if (get_mod(n).get_api() == get_in.def)
			{
		DEBUG_LINE cout << "Find Modules:" << get_mod(n).get_name() << endl;
			ESS_Modules get_ent = get_mod(n).get_server_entry();
			if (get_ent == NULL)
			{
				DEBUG_LINE cout << "Modules is not Compact!" << endl;
				ErrSetBack(ret, _ERROR_SERVER_MODULES_NO_THIS);
				goto SEND;
			}
			else{
				ret.def = get_in.def;
				get_ent(get_in, ret, sAccept);
				goto SEND;
			}
			}
	ErrSetBack(ret,_ERROR_DEFINE_UNKNOW);

 SEND:
	DEBUG_LINE cout<<"Returning Data..."<<endl;
	 char sbuff[SEND_LEN];
	 memcpy(sbuff, &ret,SEND_LEN);
	 Convert(sbuff, SEND_LEN);
	 send(sAccept, sbuff, SEND_LEN, 0);
	 DEBUG_LINE cout << "Connect Over!" << endl;
	 
 LOOP_STOP:
	close(sAccept);
	 }
	close(sListen);
	return ;
 }
