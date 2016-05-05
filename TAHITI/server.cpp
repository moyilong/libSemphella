#include "stdafx.h"
#include "ExtenisionLIB/translate.h"

 

#define server_dbg if (!kernel().quiet)cout<<"[ServerMain]"

void ErrSetBack(struct DATA_FORMAT &ret, const char *ERR_INFO)
{
	ret.def = ERR_BACK;
	ret.real_data_len = strlen(ERR_INFO);
	strcpy(ret.buff,ERR_INFO);
}

 void server_main(ESS_Modules api_entry,permission_t min_call_level)
 {
	 ENV_DATA __ENV_CALL;
	 trigger(SERVER_INIT, __ENV_CALL);
	 SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);
	 if (sListen == INVALID_SOCKET){
		 cout <<translate( "Create Socket Faild!") << endl;
		 ENV_DATA temp;
		 trigger(SOCK_FAILD, temp);
		 return ;
	 }

	 sockaddr_in serv;
	 serv.sin_family = AF_INET;
	 serv.sin_port = htons(kernel().port);	
	 serv.sin_addr.s_addr = htonl(INADDR_ANY);	

	 if (bind(sListen, (sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR){
		 cout <<translate( "Bind Socket Faild!") << endl;
		 trigger(BIND_FAILD);
		 return ;
	 }

	 if (listen(sListen, 5) == SOCKET_ERROR){
		 cout << "Listen Prot Faild!"<<endl;
		 trigger(LISTEN_FAILD);
		 return ;
	 }

#ifdef __LINUX__
#define __UNSIGNED_INT
#endif

#ifdef __ANDROID__
#undef __UNSIGNED_INT
#endif

 while(true){	 
	 server_dbg << "======================================================" << endl;
	 SOCKET sAccept;
	 struct DATA_FORMAT get_in;
	 struct DATA_FORMAT ret;
	 sockaddr_in client;
#ifdef __UNSIGNED_INT
	 unsigned
#endif
	 int iLen = sizeof(client);

	 sAccept = accept(sListen, (sockaddr*)&client,&iLen);
	 if (sAccept == SOCKET_ERROR){
		 goto LOOP_STOP;
	 }
	 int err_stat;
	 err_stat=data_recv(sAccept, get_in);
	 if (err_stat == ERR_API_MISMATCH)
	 {
		 server_dbg << "Catch a unsupport package! skip it!" << endl;
		 ErrSetBack(ret, _ERROR_API_MISMATCH);
		 goto SEND;
	 }
	 if (err_stat == ERR_UNKNOW_DATA)
	 {
		 server_dbg << "Catch a unknow package! skip it!" << endl;
		 goto LOOP_STOP;
	 }
	 
	 ErrSetBack(ret, _ERROR_NO_ERROR);
	 server_dbg << translate("Getting Permission Need") << endl;
	 permission_t pid;
	 pid=test_user_alloc(get_in);
	 server_dbg << translate("API Return Level:@") << pid << endl;
	 server_dbg << translate("Start to Match Modules:") <<get_in.def<< endl;
	 bool mod_stat;
 	 mod_stat=false;
	 if (api_entry == NULL)
	 {
		 for (int n = 0; n < get_modules_size(); n++)
			 if (get_mod(n).get_api() == get_in.def)
			 {
				 mod_stat = true;
				 if (get_mod(n).get_level() <= pid)
				 {
					 server_dbg << translate("Find Modules:") << get_mod(n).get_name() << endl;
					 ESS_Modules get_ent = get_mod(n).get_server_entry();
					 if (get_ent == NULL)
					 {
						 server_dbg << translate("Modules is not Compact!") << endl;
						 ErrSetBack(ret, _ERROR_SERVER_MODULES_NO_THIS);
						 goto SEND;
					 }
					 else{
						 ret.def = get_in.def;
						 get_ent(get_in, ret, sAccept);
						 goto SEND;
					 }
				 }
				 else{
					 ErrSetBack(ret, _ERROR_PERMISSION_DINED);

				 }
			 }
	 }
		else
		{
			if (pid>=min_call_level)
			api_entry(get_in, ret, sListen);
			mod_stat = true;
		}
	 if (!mod_stat)
		 ErrSetBack(ret, _ERROR_DEFINE_UNKNOW);

 SEND:
	server_dbg<<translate("Returning Data...")<<endl;
	data_send(ret, sAccept);
	 
 LOOP_STOP:
	close(sAccept);
	 }
	close(sListen);
	return ;
 }
