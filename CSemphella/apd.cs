using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;

namespace CSemphella
{
    public class apd
    {
        static DebugNode node = new DebugNode("APD");
        public static bool DebugFlag
        {
            set
            {
                node.Enable = value;
            }
            get
            {
                return node.Enable;
            }
        }
        public struct Node
        {
            public string name;
            public string data;

            public Node(string _name, string _data)
            {
                name = _name;
                data = _data;
            }

            public void Set(string _data)
            {
                data = _data;
            }
        }

        public struct Section
        {
            public List<Node> collect;
            public string name;

            public Section(string _name)
            {
                name = _name;
                collect = new List<Node>();
            }
        }

        public apd(string file, string pwd = null)
        {
            OpenFile(file, pwd);
        }
        public apd()
        {
            Create();
        }
        private List<Section> data = null;

        public bool IsOpen
        {
            get
            {
                return (data != null);
            }
        }

        private string _password = "CSEMPHELLA_AES_DEFAULT_CIPHER_PASSWORD";

        public string Password
        {
            set
            {
                _password = value;
            }
            private get
            {
                return _password;
            }
        }

        private static string TestHead = "PASSWORD_VERIFY_INDA";
        public bool BinaryMode = false;

        public void OpenFile(string file, string password = null, Func<string> PasswordCorrectFun = null)
        {
            if (password != null)
                Password = password;
            Create();
            string read = File.ReadAllText(file);
            node.Push("Current Read File Complete!");
            OpenStream(read, password, PasswordCorrectFun);
        }
        public void OpenStream(string read,string password = null,Func<string> PasswordCorrectFun =null)
        {
            string[] buff = read.Split('\n');
            if (utils.PostVerify(read, "APD_BIN:"))
            {
                node.Push("BinaryMode!");
                BinaryMode = true;
                string pwd_head = buff[0].Substring(8);
                node.Push("Password Head:" + pwd_head);
                if (TestHead != AESHelper.AESDecrypt(pwd_head, _password))
                {
                    node.Push("Invalid Password Value!");
                    if (PasswordCorrectFun == null)
                        throw new Exception("PASSWORD_INVALID");
                    else
                    {
                        int count = 0;
                        while (true)
                        {
                            node.Push("Try to Decrypting...");
                            count++;
                            string get = PasswordCorrectFun();
                            if (get == null)
                                throw new Exception("PASSWORD_INVALID");
                            if (count > 30)
                                throw new Exception("TO_MANY_TRY");
                            Password = get;
                            if (TestHead != AESHelper.AESDecrypt(buff[0].Substring(8), _password))
                            {
                                count++;
                                continue;
                            }
                            break;
                        }
                    }
                }
                node.Push("Password Match!");
                node.Push("Decrypting...");
                string dec = AESHelper.AESDecrypt(buff[1], Password);
                buff = dec.Split('\n');
                node.Push("Get Line:" + buff.Length);
            }
            string opname = null;
            for (UInt64 p = 0; p < Convert.ToUInt64(buff.Length); p++)
            {
                string line = buff[p].Trim();
                node.Push("Processing Line:" + line);
                if (line.Length == 0 || line[0] == '#')
                {
                    continue;
                }
                if (line[0] == '[' && line[line.Length - 1] == ']')
                {
                    opname = line.Substring(1, line.Length - 2);
                }
                else
                {
                    if (opname == null)
                        throw new Exception("Syntax Error! No Section Name");
                    Node g = new Node();
                    g.name = line;
                    g.data = "defs";
                    for (int n = 0; n < line.Length; n++)
                        if (line[n] == '=')
                        {
                            g.name = line.Substring(0, n).Trim();
                            g.data = line.Substring(n + 1).Trim();
                        }
                    Insert(opname, g);
                }
            }
        }
        public string[] SectionList
        {
            get
            {
                string[] ret = new string[data.Count];
                Parallel.For(0, data.Count, i =>
                {
                    ret[i] = data[i].name;
                });
                return ret;
            }
        }

        public Node[] NameList(string namelist)
        {
            int id = checksection(namelist);
            if (id == -1)
                return null;
            return data[id].collect.ToArray();
        }
        public void Create()
        {
            data = new List<Section>();
        }

        public void WriteFile(string file)
        {
            File.WriteAllText(file, ExportStream());
        }
        public string ExportStream()
        {
            string pdata = "";
            foreach (Section sec in data)
            {
                pdata += "[" + sec.name + "]\n";
                foreach (Node n in sec.collect)
                {
                    pdata += n.name + "=" + n.data + "\n";
                }
            }
            if (BinaryMode)
            {
                string head = "APD_BIN:" + AESHelper.AESEncrypt(TestHead, _password) + "\n";
                pdata = head + AESHelper.AESEncrypt(pdata, Password);
            }
            return pdata;
        }

        private int checksection(string section)
        {
            if (!IsOpen)
                return -1;
            for (int n = 0; n < data.Count; n++)
            {
                if (data[n].name == section)
                    return n;
            }
            return -1;
        }

        public bool Exist(string section)
        {
            if (checksection(section) != -1)
                return true;
            return false;
        }

        private int checknode(string section, string node)
        {
            int sec = checksection(section);
            if (sec == -1)
                return -1;
            for (int n = 0; n < data[sec].collect.Count; n++)
            {
                if (data[sec].collect[n].name == node)
                    return n;
            }
            return -1;
        }

        public bool Exist(string section, string node)
        {
            if (checknode(section, node) == -1)
                return false;
            return true;
        }

        public void Insert(string sectionname, string name, string data)
        {
            Insert(sectionname, new Node(name, data));
        }

        public void AddSection(string sectionanme)
        {
            if (checksection(sectionanme)==-1)
            {
                this.data.Add(new Section(sectionanme));
            }
        }

        public void Insert(string sectionname, Node vdata)
        {
            node.Push("Insert:" + sectionname + "/" + vdata.name + "=" + vdata.data);
            int sec = checksection(sectionname);
            if (sec == -1)
            {
                AddSection(sectionname);
                sec = checksection(sectionname);

            }
            if (sec == -1)
                throw new Exception("ADD_FAILD");
            int nod = checknode(sectionname, vdata.name);
            if (nod == -1)
            {
                data[sec].collect.Add(vdata);
                return;
            }
            data[sec].collect[nod].Set(vdata.data);
        }

        public string ReadSection(string sectionname, string name, string auto_set = null)
        {
            int sec = checksection(sectionname);
            if (sec == -1)
            {
                if (auto_set != null)
                {
                    this.data.Add(new Section(sectionname));
                    this.data[checksection(sectionname)].collect.Add(new Node(name, auto_set));
                }
                return auto_set;
            }
            int nod = checknode(sectionname, name);
            if (nod == -1)
            {
                if (auto_set != null)
                {
                    this.data[sec].collect.Add(new Node(name, auto_set));
                }
                return auto_set;
            }
            return this.data[sec].collect[nod].data;
        }

        public void Delete(string sectionname)
        {
            int id = checksection(sectionname);
            if (id != -1)
                data.RemoveAt(id);
        }

        public void Clean(string sectionname)
        {
            Delete(sectionname);
            AddSection(sectionname);
        }

        public void Clean()
        {
            Create();
        }

        public void Delete(string sectionname,string nodename)
        {
            int id = checksection(sectionname);
            if (id != -1)
            {
                int node = checknode(sectionname, nodename);
                data[id].collect.RemoveAt(node);
            }
        }

        /*RSA操作合集*/
        RSAHelper helper = new RSAHelper();
        public void RSAOpenFile(string file,string password,string key, Func<string> PasswordCorrectFun = null)
        {
            RSAOpenStream(File.ReadAllText(file), password, key, PasswordCorrectFun);
        }

        public void RSAOpenStream(string stream,string password,string key, Func<string> PasswordCorrectFun = null)
        {
            helper.Key = key;
            OpenStream(System.Text.Encoding.Default.GetString((helper.Decrypt(stream))), password, PasswordCorrectFun);
        }

        public string RSAExportStream(string password, string key)
        {
            helper.Key = key;
            if (helper.IsPublicKey)
                throw new Exception("Public Key Can't Encrypt");
            return helper.Crypt(System.Text.Encoding.Default.GetBytes(ExportStream()));
        }

        public void RSAWriteFile(string file,string password,string key)
        {
            File.WriteAllText(file, RSAExportStream(password, key));
        }
    }
}