using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSemphella
{
    public static class BinaryString
    {

        public static string BinaryToString(byte[] data)
        {
            string ret = "EVD:";
            for (int n = 0; n < data.Length; n++)
                ret += data[n].ToString() + ":";
            return ret + "EVD";
        }

        public static byte[] StringToBinary(string data)
        {
            string[] spd = data.Split(':');
            if (spd[0] != "EVD")
            {
                Console.WriteLine("NULL RET");
                return null;
            }
            List<byte> ret = new List<byte>();
            int pos = 0;
            while (true)
            {
                pos++;
                if (spd[pos].Length > 3)
                {
                    if (spd[pos][0] == 'E' && spd[pos][1] == 'V' && spd[pos][2] == 'D')
                    {
                        return ret.ToArray();
                    }
                }
                ret.Add(Convert.ToByte(spd[pos]));
            }
        }
    }
}
