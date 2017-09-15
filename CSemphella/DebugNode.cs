using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSemphella
{
    public class DebugNode
    {
        public string section_name = "UnDefine";
        public bool local_enable = true;
        private Int64 Count = -1;
        public static bool GlobalEnable=true;

        public bool Enable
        {
            get
            {
                /* if (!GlobalEnable)
                     return false;
                 return local_enable;*/
                return true;
            }
            set
            {
                local_enable = value;
            }
        }

        public DebugNode(string n,bool def_stat=true)
        {
            section_name = n;
            Enable = def_stat;
            if (Enable)
                Push("Current Status is Enable!");
        }

        public string Msg(string str)
        {
            if (!Enable)
                return null;
            string count_str = "";
            if (Count != -1)
            {
                count_str += "[" + Count.ToString() + "]";
                Count++;
            }
            return "[" +section_name + "]" + count_str + str;
        }

        public void ConsoleOut(string str)
        {
            Console.WriteLine(str);
        }

        public virtual void Push(string str)
        {
            if (Enable)
                ConsoleOut(Msg(str));
        }
    }
}
