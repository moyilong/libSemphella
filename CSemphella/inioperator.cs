using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Threading.Tasks;

namespace CSemphella
{
    public class inioperator
    {
        public struct Node
        {
            public string name;
            public string data;
            public Node(string _name,string _data)
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
        public inioperator(string file)
        {
            OpenFile(file);
        }

        private List<Section> data=null;

        public bool IsOpen
        {
            get
            {
                return (data == null);
            }
        }
        string _password = "CSEMPHELLA_AES_DEFAULT_CIPHER_PASSWORD";
        public string Password
        {
            set
            {
                _password = value;
            }
        }
        static string TestHead = "PASSWORD_VERIFY_INDA";
        public bool BinaryMode = false;
        public void OpenFile(string file,string password=null)
        {
            if (password != null)
                Password = password;
            data = new List<Section>();
            string[] buff = File.ReadAllText(file).Split('\n');
            if (buff[0] == "APD_BIN:")
            {
                BinaryMode = true;
                if (TestHead  != AESHelper.AESDecrypt(buff[0].Substring(9),_password))
                {
                    throw new Exception("PASSWORD_INVALID");
                }
                System.Threading.Tasks.Parallel.For(1, buff.Length, i=>{
                    buff[i] = AESHelper.AESDecrypt(buff[i], _password);
                });
            }
            Section temp = new Section();
            bool first = true;
            for (UInt64 p=0;p<Convert.ToUInt64( buff.Length);p++)
            {
                if (BinaryMode && p == 0)
                    continue;
                string line = buff[p];
                char[] array = line.ToCharArray();
                if (array[0] == '#'|| line.Length==0)
                    continue;
                if (array[0]=='[' && array[line.Length-1] == ']')
                {
                    if (first)
                    {
                        first = false;
                    }
                    else
                    {
                        data.Add(temp);
                        temp = new Section();
                    }
                    temp.name = line.Substring(1, line.Length - 2);
                }else
                {
                    Node get=new Node();
                    int poffset = -1;
                    for (int n = 0; n < line.Length && poffset == -1; n++)
                        if (array[n] == '=')
                            poffset = n;
                    if (poffset==-1)
                    {
                        get.name = line;
                        get.data = "true";
                    }
                    else
                    {
                        get.name = line.Substring(0, poffset);
                        get.data = line.Substring(poffset + 1);
                    }
                    temp.collect.Add(get);
                }
            }
        }
        public void Create()
        {
            data = new List<Section>();
        }
        public void WriteFile(string file)
        {
            if (!IsOpen)
                throw new Exception("FILE_IS_NOT_OPEN");
            string pdata = "";
            foreach(Section sec in data)
            {
                pdata += "[" + sec.name + "]" + System.Environment.NewLine;
                foreach (Node n in sec.collect)
                    pdata += n.name + "=" + n.data + System.Environment.NewLine;
            }
            if (BinaryMode)
            {
                string head = "APD_BIN:" + AESHelper.AESEncrypt(TestHead, _password) + "\n";
                pdata = head + AESHelper.AESEncrypt(pdata,_password);
            }
            File.WriteAllText(file, pdata);
        }
        int  checksection(string section)
        {
            if (!IsOpen)
                return -1;
            for (int n=0;n<data.Count;n++)
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
        int checknode(string section, string node)
        {
            int sec = checksection(section);
            if (sec == -1)
                return -1;
            for (int n=0;n<data[sec].collect.Count;n++)
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

        public void AddSection(string sectionname,string name,string data)
        {
            int sec = checksection(sectionname);
            if (sec == -1)
            {
                this.data.Add(new Section(sectionname));
                sec = checksection(sectionname);
            }
            int nod = checknode(sectionname, name);
            if (nod == -1)
            {
                this.data[sec].collect.Add(new Node(name, data));
                return;
            }
            this.data[sec].collect[nod].Set(data);
        }

        public string ReadSection(string sectionname,string name,string auto_set = null)
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
                if (auto_set!=null)
                {
                    this.data[sec].collect.Add(new Node(name, auto_set));
                }
                return auto_set;
            }
            return this.data[sec].collect[nod].data;
        }
    }
}
