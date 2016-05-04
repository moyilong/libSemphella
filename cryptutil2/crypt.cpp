#include "cryptutil2.h"
#include "fhandle.h"
#include "algorthim.h"
char *ext_data = nullptr;
int crypt_process()
{
	if (input.empty() || password.empty())
	{
		cout << "Argment Error!" << endl << "Input or Password Empty!" << endl;
		exit(-1);
	}
	if (output.empty())
	{
		if (!decrypt)
		{
			output = input + FILE_TYPE;
		}
		else {
			cout << "Not define output file!" << endl;
			exit(-1);
		}
	}
	if (output == "-")
	{
		if (decrypt)
			std_out = true;
		else
		{
			cout << "Error STD Out is unsupport in Crypt Method!" << endl;
			return false;
		}
	}
	uint64_t count = 0;
	string check = output;
	if (decrypt)
		check = input;
	DEBUG << "Checking Name Secure" << endl;
	if (check.size() > 5 && check != "-")
		if (!file_name_check(check) || check.size() <= 5)
		{
			cout << "Warring! File Name Secure Check Error!" << endl;
			cout << "The ERT File last name is must be .ert2 or .ert3" << endl;
			exit(-1);
		}
	if (password.size() > MAX_PASSWORD_LEN)
	{
		cout << "Password is too long!" << endl;
		exit(-1);
	}
	DEBUG << "Try to Open File!" << endl;
	file in;
	file out;
	in.open(input.data(), "r");
	if (!std_out)
		out.open(output.data(), "w");
	HEAD head;
	head.account_level = level;
	head.algrthom = alghtriom;
	head.ext[EXT_FHANDLE] = fhand;
	uint64_t len = in.tell_len();
	if (!in.is_open())
	{
		cout << "Input File Faild!" << endl;
		exit(-1);
	}
	if (!std_out && !out.is_open())
	{
		cout << "Out File Faild!" << endl;
		exit(false);
	}
	DEBUG << "preparing data...." << endl;
	if (!decrypt)
	{
		DEBUG << "Caculating Information.." << endl;
		head.bs = bs;
		DEBUG << "Calcing PSUM..." << endl;
		head.password_sum = APOLL[trans_id(head.algrthom)].px(password);
		DEBUG << "Alloc File Space" << endl;
		out.write((char*)&head, sizeof(HEAD));
		DEBUG << "Read File Handle:" << (int)head.ext[EXT_FHANDLE] << endl;
		DEBUG << "Updating External Table" << endl;
		if (ext_data == nullptr)
		{
			ex.checksum = 0;
			ex.length = 0;
			out.seekp(sizeof(HEAD));
			out.write(&ex, 1);
		}
		else {
			mask(ext_data, ex.length);
			ex.checksum = getsumV2(ext_data, ex.length);
			out.seekp(sizeof(HEAD));
			out.write(&ex, 1);
			out.write(ext_data, ex.length);
		}
	}
	else
	{
		cp2 << "Reading Head...." << endl;
		in.seekp(0);
		len = len - sizeof(HEAD);
		char buff[sizeof(HEAD)];
		in.read(buff, sizeof(HEAD));
		memcpy(&head, buff, sizeof(HEAD));
		//in.read((char*)&head, sizeof(HEAD));
		DEBUG_LINE display_dump(buff, sizeof(HEAD));
		bs = head.bs;
		cp2 << "HEAD is read!" << endl;
		if (!head.check())
		{
			cout << "Protoco Secure Check Faild!" << endl;
			cout << "Read Protoco Version:" << (unsigned int)head.account_level << endl;
			cout << "Compact Protoco Version:" << (unsigned int)level << endl;
			exit(-1);
		}
		if (head.ext[EXT_EXTABLE] == 1)
		{
			in.seekp(sizeof(HEAD));
			char buff[sizeof(EXT)];
			memset(buff, 0, sizeof(buff));
			in.read(buff, sizeof(EXT));
			ext_data = (char*)malloc(ex.length);
			in.read(ext_data, ex.length);
			mask(ext_data, ex.length);
			if (ex.checksum != getsumV2(ext_data, ex.length))
			{
				cout << "External Data Checksum Faild!";
				exit(-1);
			}
		}
	}
	if (!std_out)	cout << input << " => " << output << endl;
	cp2 << len << " of " << bs << endl;
	APOLL[trans_id(head.algrthom)].pa(password);
	if (decrypt)
	{
		if (APOLL[trans_id(head.algrthom)].px(password) != head.password_sum)
		{
			cout << "Password Correct!" << endl;
			exit(0);
		}
	}
	uint64_t sum = 0;
	time_t start = time(0);
	char *buff = (char*)malloc(sizeof(char)*bs);
	if (APOLL[trans_id(head.algrthom)].sa == NULL || APOLL[trans_id(head.algrthom)].ca == NULL || APOLL[trans_id(head.algrthom)].pa == NULL)
	{
		cout << "Program PTR Check Error!" << endl;
		exit(-1);
	}
	uint64_t step = len / head.bs;
	uint64_t fix = len - (head.bs * step);
	cp2 << "Resetting Address..." << endl;
	if (decrypt)
	{
		in.seekp(sizeof(HEAD));
		if (!std_out)
			out.seekp(0);
	}
	else {
		in.seekp(0);
		out.seekp(sizeof(HEAD));
	}
	double old_presend = 0, dlen = len;
	uint64_t ulen = 0;
	cp2 << "Using File Handle:" << (int)head.ext[EXT_FHANDLE] << endl;
	for (uint64_t n = 0; n < step; n++)
	{
		get_fhandle(head.ext[EXT_FHANDLE])(head, in, out, sum, head.bs, n*head.bs);
		if (!std_out)
		{
			double per = (double)((double)n*(double)head.bs) / (double)len;
			if (per != old_presend)
			{
				old_presend = per;
				ulen = (n* head.bs) / dZero(time(0) - start);
				ShowProcessBar(per, human_read(ulen, human_read_storage_str, 1024, 10) + "PS");
			}
		}
	}
	get_fhandle(head.ext[EXT_FHANDLE])(head, in, out, sum, fix, step*head.bs);
	cp2 << "Main Loop Over! SUM:" << sum << endl;
	if (!std_out) ShowProcessBar(1, "--");
	cout << endl;
	if (!decrypt)
	{
		cp2 << "updating head..." << endl;
		head.sum = sum;
		head.algrthom = alghtriom;
		head.ext[EXT_FHANDLE] = fhand;
		head.account_level = level;
		cp2 << "Redirecting..." << endl;
		out.seekp(0);
		cp2 << "Redirect to " << out.tellp() << endl;
		cp2 << "Writing Data..." << endl;
		head.check();
		out.write((char*)&head, sizeof(HEAD));
		DEBUG_LINE display_dump((char*)&head, sizeof(HEAD));
		cp2 << "head is updated!" << endl;
	}
	else
	{
		if (sum != head.sum)
		{
			cout << "Checksum Faild!" << endl;
			cout << hex << sum << " != " << hex << head.sum << endl;
		}
	}
	if (!std_out) cout << "Done. Checksum:" << hex << sum << endl;
	in.close();
	if (!std_out)
		out.close();
	if (!std_out&&ex.length != 0 && ex.checksum != 0)
	{
		cout << "External Data Was Be Writed!" << endl;
		cout << "CheckSum:" << ex.checksum << endl;
	}

	return 0;
}