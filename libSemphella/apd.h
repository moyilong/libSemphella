#include "libSemphella.h"



namespace APD_UTILS{

	typedef 	long long COUNT_TYPE;

#define PSTR "libSemphella-APD-File"

	struct APD_BIN{
		char diff_str[sizeof(PSTR)];
		COUNT_TYPE node_size;
	};

	struct NODE_INFO{
		COUNT_TYPE label_size;
		char n_name[MAX_BUFF_SIZE];
	};

	struct LABEL_INFO{
		COUNT_TYPE namelen;
		COUNT_TYPE datalen;
	};
	
	struct label{
		string name;
		string data;
	};

	struct NODE{
		string n_name;
		vector<label> label;
	};

	typedef void(for_each_api)(NODE node_link,COUNT_TYPE nid);
	typedef bool(for_each_check)(string data);
	class API APD {
	public:
		APD();
		APD(string filename,bool bin_stat=false,bool crypt_stat=false,string passwd="");
		~APD();
		void load(string filename);
		string get_label(string node, string lab);
		void write_label(string node, string lab, string data);
		COUNT_TYPE check_node(string node);
		COUNT_TYPE check_label(string node, string label);
		void node_for_each(for_each_api *api,for_each_check *check,bool omp=true);
		void remove(string node);
		void remove(string node, string lab);
		void save();
		inline COUNT_TYPE check(string node)
		{
			return check_node(node);
		}
		inline COUNT_TYPE check(string node, string lab)
		{
			return check_label(node, lab);
		}
		inline void remove_node(string node)
		{
			remove(node);
		}
		inline void remove_label(string node, string lab)
		{
			remove(node, lab);
		}
		void export_bin();
		void import_bin(string filename);

		/*
		void enable_bin_mode();
		void disable_bin_mode();
		void enable_crypt(string password);
		void disable_crypt();
		*/

		void set_bin_mode(bool stat);
		void set_crypt_mode(bool cpy);
		void set_password(string pwd);

	private:
		string filename;
		fstream fileio;
		vector<NODE> poll;
		bool bin_mode = false;
		bool cryptd = false;
		string p_password;
	};

};
