#include "logd.h"

namespace LogDaemon {
	API VER meta_info(1, 0, 0, 1);
#define META_LEN sizeof(META_INFO)
	logd::~logd()
	{
		if (meta_file.is_open())
		{
			meta_file.flush();
			meta_file.close();
		}
		if (data_file.is_open())
		{
			data_file.flush();
			data_file.close();
		}
	}
	logd::logd(string path, string password,bool create)
	{
		Load(path, password,create);
	}
	uint64_t logd::getLogSize()
	{
		return meta.SectionSize;
	}

	void logd::SetPassword(string password)
	{
		loaded_password = password;
		lp_hash = hash(password.data(), password.size());
	}

	bool logd::VerifyFile()
	{
		try {
			ReadMeta();
		}
		catch (EXCEPTION except)
		{
			return false;
		}
		return true;
	}

	void logd::Load(string path, string password,bool create)
	{
		SetPassword(password);
		string meta_name = path + "/logd.meta";
		string data_name = path + "/data.meta";
		meta_file.open(meta_name,ios_base::in|ios_base::out| ios_base::binary);
		if (!meta_file.is_open())
			throw META_READ_EXCEPTED;
		data_file.open(data_name, ios_base::in | ios_base::out | ios_base::binary);
		if (!data_file.is_open())
			throw DATA_FILE_EXCEPTED;
		if (create)
		{
			strcpy(meta.begin, BEGIN_MASX);
			debug << "Create Mode..." << endl;
			if (loaded_password.size() == 0)
				meta.crypted = false;
			else
			{
				meta.crypted = true;
				meta.hash_code = hash(password.data(), password.size());
			}
			meta.meta_ver = meta_info;
			section.clear();
			memset(meta.CDATA,'c', CUSTMON_DATA);
			meta.a = 'v';
			meta.b = 'e';
			debug << "Writing Meta File..." << endl;
			WriteMeta();
			debug << "Init Log..." << endl;
			Append("Init Log Daemon! for Version:" + meta_info.to_str());
		}else
		{
			debug << "Reading Meta Data..." << endl;
			ReadMeta();
		}

	}

	string logd::GetLogDescription(uint64_t id)
	{
		uint64_t offset = GetOffSet(id);
		if (!data_file.is_open())
			throw DATA_FILE_EXCEPTED;
		char *buff = (char *)malloc(section.at(id).descript_len);
		memset(buff, 0, section.at(id).descript_len);
		data_file.seekp(data_file.beg + offset);
		data_file.seekg(data_file.beg + offset);
		data_file.read(buff, section.at(id).descript_len);
		if (meta.crypted)
			crypt(loaded_password, buff, section.at(id).descript_len);
		string ret = buff;
		free(buff);
		return ret;
	}

	void logd::Append(string description)
	{
		debug << "Appending:" << description << endl;
		if (!data_file.is_open())
			throw DATA_FILE_EXCEPTED;
		debug << "Generate Section Info..." << endl;
		Section sect;
		sect.time_log = time(0);
		sect.descript_len = description.size();
		section.push_back(sect);
		debug << "Calcuating Offset..." << endl;
		int offset = data_file.beg + GetOffSet(section.size() - 1);
		data_file.seekp(offset);
		data_file.seekg(offset);
		//data_file.write(description.data(), description.size());
		debug << "Prepare to Crypto.." << endl;
		char *buff = (char*)malloc(description.size());
		strcpy(buff, description.data());
		debug << "Crypt Data.." << endl;
		if (meta.crypted)
			crypt(loaded_password, buff, description.size());
		else
			debug << "Skip!" << endl;
		debug << "Writting to Data File..." << endl;
		data_file.write(buff, description.size());
		debug << "Updating Meta File.." << endl;
		meta.SectionSize++;
		WriteMeta();

	}

	void logd::ReadCustmonData(char CDATA[CUSTMON_DATA])
	{
		ReadMeta();
		memcpy( CDATA, meta.CDATA, CUSTMON_DATA);
	}

	void logd::SaveCustmonData(const char CDATA[CUSTMON_DATA])
	{
		memcpy(meta.CDATA, CDATA, CUSTMON_DATA);
		WriteMeta();
	}

	time_t logd::GetLogTime(uint64_t id)
	{
		return section.at(id).time_log;
	}

	void logd::ReadMeta()
	{
		if (!meta_file.is_open())
			throw META_READ_EXCEPTED;
		//meta_file.seekp(meta_file.beg);
		//meta_file.seekg(meta_file.beg);
		meta_file.seekg(0);
		meta_file.seekp(0);
		char buff[sizeof(META_INFO)];
		memset(buff, 0, sizeof(buff));
		meta_file.read(buff, sizeof(META_INFO));
		display_dump(buff, sizeof(META_INFO));
		memcpy(&meta, buff, sizeof(META_INFO));
		uint64_t sum = meta.verify;
		meta.verify = 0;
		uint64_t code = getsumV2((char*)&meta, sizeof(META_INFO));
		if (sum != code)
		{
			debug << "Verify Faild:" << sum << " != " << code << endl;
			//throw META_DATA_CHECK_FIALD;
		}
		if (meta.crypted && meta.hash_code != lp_hash)
			throw PASSWORD_VERIFY_ERROR;
		debug << "Reading Section " << meta.SectionSize << endl;
		for (uint64_t n = 0; n < meta.SectionSize; n++)
		{
			char temp[sizeof(Section)];
			meta_file.read(temp,sizeof(Section));
			Section v;
			memcpy(&v, temp, sizeof(Section));
			section.push_back(v);
		}
	}

	void logd::WriteMeta()
	{
		if (!meta_file.is_open())
			throw META_WRITE_EXCEPTED;
		meta.verify = 0;
		uint64_t verify = getsumV2((char*)&meta, sizeof(META_INFO));
		meta.verify = verify;
		meta.SectionSize = section.size();
		meta_file.seekp(meta_file.beg);
		meta_file.seekg(meta_file.beg);
		
		meta_file.write((char*)&meta, sizeof(META_INFO));
		debug << "Writing Section " << meta.SectionSize << endl;
		for (uint64_t n = 0; n < meta.SectionSize; n++)
			meta_file.write((char*)&section.at(n), sizeof(Section));

		
	}
	uint64_t logd::GetOffSet(uint64_t id)
	{
		debug << "Calcuating for:" << id << endl;
		uint64_t ret = 0;
		for (int64_t n = 0; n < id; n++)
		{
			ret += section.at(n).descript_len;
		}
		debug << "Get Offset:" << ret << endl;
		return ret;
	}


};