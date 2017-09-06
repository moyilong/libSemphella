using CSemphella;
using System;

namespace CSAPDConvert
{
    internal class Program
    {
        private static bool decrypt = false;
        private static string password = null;
        private static string input = null;
        private static string output = null;

        private static void Main(string[] args)
        {
            foreach (string s in args)
            {
                if (s[0] == '-')
                    switch (s[1])
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
            apd a = new apd(input, password);
            a.BinaryMode = !decrypt;
            a.WriteFile(output);
        }
    }
}