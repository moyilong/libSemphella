using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CSemphella;
namespace CSAPDConvert
{
    class Program
    {
        static bool decrypt = false;
        static string password = null;
        static string input = null;
        static string output = null;
        static void Main(string[] args)
        {
            foreach (string s in args)
            {
                if (s[0] == '-')
                    switch(s[1])
                    {
                        case 'p':
                            password = s.Substring(2);
                            break;
                        case 'i':
                            input = s.Substring(2);
                            break;
                        case 'o':
                            output = s.Substring(2);
                            break;
                        case 'd':
                            decrypt = true;
                            break;
                    }
            }
            if (input == null || output == null)
                return;
            Console.WriteLine("From:" + input + " => " + output);
            apd a = new apd(input,password);
            a.BinaryMode = !decrypt;
            a.WriteFile(output);
        }
    }
}
