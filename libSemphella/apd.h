#include "libSemphella.h"


	typedef 	long long COUNT_TYPE;
	
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
		APD(string filename);
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

	private:
		string filename;
		fstream fileio;
		vector<NODE> poll;
		bool bin_mode = false;
		bool cryptd = false;
		string p_password;
	};
