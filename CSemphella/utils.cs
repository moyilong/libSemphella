using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;

namespace CSemphella
{
    public static class utils
    {
        public static bool in_array<T>(T [] get,T find)
        {
            bool stat = false;
            Parallel.ForEach(get, who =>
             {
                 if (who.Equals(find))
                     stat = true;
             });
            return stat;
        }
        public static string SearchInArray(string[] array,string search)
        {
            foreach(string get in array)
            {
                if (get.IndexOf(search) != 0)
                    return get;
            }
            return null;
        }

        public static Hashtable EqualFormat(string raw_data)
        {
            Hashtable ret = new Hashtable();
            string[] format = raw_data.Split('\n');
            foreach (string line in format)
            {
                if (line == "" || line[0] == '#')
                    continue;
                int poffset = -1;
                for(int n=0;n<line.Length;n++)
                    if (line[n]=='=')
                    {
                        poffset = n;
                        break;
                    }
                if (poffset==-1)
                {
                    ret.Add(line, "true");
                }
                else
                {
                    ret.Add(line.Substring(0, poffset), line.Substring(poffset+1));
                }
            }
            return ret;
        }
    }
}
