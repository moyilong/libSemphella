using System;
using System.Collections.Generic;
using System.IO;

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
            string[] buff = read.Split('\n');
            node.Push("Current Read File Complete!");
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

        public void Create()
        {
            data = new List<Section>();
        }

        public void WriteFile(string file)
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
            File.WriteAllText(file, pdata);
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

        public void Insert(string sectionname, Node vdata)
        {
            node.Push("Insert:" + sectionname + "/" + vdata.name + "=" + vdata.data);
            int sec = checksection(sectionname);
            if (sec == -1)
            {
                this.data.Add(new Section(sectionname));
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
    }
}