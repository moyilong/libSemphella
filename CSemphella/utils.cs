using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace CSemphella
{
    public static class utils
    {
        private static DebugNode node = new DebugNode("Utils");
        public static string GetMD5(string data)
        {
            return GetMD5(Encoding.UTF8.GetBytes(data));
        }

        public static string GetMD5(byte[] data)
        {
            MD5 md5 = new MD5CryptoServiceProvider();
            byte[] hash = md5.ComputeHash(data);
            string ret = "";
            for (int i = 0; i < hash.Length; i++)
                ret += hash[i].ToString("x2");
            return ret;
        }

        public static bool in_array<T>(T[] get, T find)
        {
            bool stat = false;
            Parallel.ForEach(get, who =>
             {
                 if (who.Equals(find))
                     stat = true;
             });
            return stat;
        }

        public static string SearchInArray(string[] array, string search)
        {
            foreach (string get in array)
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
            foreach (string _line in format)
            {
                string line = _line;
                if (line == "" || line[0] == '#')
                    continue;
                int poffset = -1;
                for (int n = 0; n < line.Length; n++)
                    if (line[n] == '=')
                    {
                        poffset = n;
                        break;
                    }
                if (poffset == -1)
                {
                    ret.Add(line, "true");
                }
                else
                {
                    ret.Add(line.Substring(0, poffset), line.Substring(poffset + 1));
                }
            }
            return ret;
        }
        private static string[] storage_unit =
        {
            "B",
            "KB",
            "MB",
            "GB",
            "TB",
            "PB",
            "EB",
        };

        public static string StorageSizeCheck(long val, int begin_level = 0)
        {
            return UnitConvert(val, begin_level, storage_unit, 1024);
        }

        public static string UnitConvert(long val, int current_level, string[] level_array, int step)
        {
            double rv = val;
            int level = current_level;
            while (level > step && level < level_array.Count())
            {
                rv /= step;
                level++;
            }
            return String.Format("{0:N2}" + level_array[level], rv);
        }

        public static int SecureIntConvert(string str, int def = 0)
        {
            try
            {
                return int.Parse(str);
            }
            catch
            {
                return def;
            }
        }

        public static bool PostVerify(string orig, string header, bool on_foot=false)
        {
            if (header.Length > orig.Length)
                return false;
            if (orig == header)
                return true;
            string str = null;
            if (on_foot)
                str = orig.Substring(orig.Length - header.Length);
            else
                str = orig.Substring(0, header.Length);
            node.Push("Verify:" + str + " == " + header + " ret=" + (str==header).ToString());
            return (str == header);
        }

        public static bool PostVerifyNoBigLittle(string orig, string header, bool onoot = false)
        {
            return PostVerify(orig.ToUpper(), header.ToUpper(), onoot);
        }

        public class ModulesVersionInfo
        {
            public Version ver;
            public FileInfo info;
        }

        public static ModulesVersionInfo[] GetDirectoryModulesVersionInfo()
        {
            string[] default_filter =
            {
                "dll",
                "exe"
            };
            return GetDirectoryModulesVersionInfo(System.Environment.CurrentDirectory, default_filter);
        }

        public static ModulesVersionInfo[] GetDirectoryModulesVersionInfo(string path, string[] filter)
        {
            DirectoryInfo info = new DirectoryInfo(path);
            List<ModulesVersionInfo> ret = new List<ModulesVersionInfo>();
            foreach (FileInfo file in info.GetFiles())
            {
                string[] spl = file.Name.Split('.');
                string subname = spl[spl.Length - 1];
                foreach (string f in filter)
                    if (f == subname)
                    {
                        try
                        {
                            ModulesVersionInfo temp = new ModulesVersionInfo();
                            temp.info = file;
                            Assembly asm = Assembly.LoadFile(file.FullName);
                            temp.ver = asm.GetName().Version;
                            ret.Add(temp);
                        }
                        catch
                        {

                        }
                        break;
                    }
            }
            return ret.ToArray();
        }

        public static string CurrentIP
        {
            get
            {
                string ret = "";
                foreach (IPAddress ip in Dns.GetHostAddresses(Dns.GetHostName()))
                    if (ip.AddressFamily == System.Net.Sockets.AddressFamily.InterNetwork)
                        if (ip.ToString() != "127.0.0.1")
                            ret += ip.ToString() + " ";
                return ret;
            }
        }

        public static IPAddress[] CurrentIPAddress
        {
            get
            {
                return Dns.GetHostAddresses(Dns.GetHostName());
            }
        }

        public static bool StringIsAllow(string str)
        {
            if (str == null || str.Length == 0)
                return false;
            return true;
        }

        public static string PaddingString(string orig_data,int padlen,string accept_str = "0123456789ZBCDEFGHIJKLMNOPQRSTUVWXYZ")
        {
            string ret = "";
            while (ret.Length < padlen)
            {
                int argment = orig_data.Length ^ ret.Length ^ accept_str.Length ^ padlen;
                argment = (argment << 4) ^ orig_data[Math.Abs(Math.Sign(argment) * (orig_data.Length - 1))];
                ret += accept_str[Math.Abs(Math.Sign(argment) * (accept_str.Length - 1))];
            }
            return ret;
        }

        public static byte[] PaddingByte(byte[] orig_data,int padlen,byte[] accept_pad=null)
        {
            byte[] ret = new byte[padlen];
            Parallel.For(0, padlen, i =>
            {
                int temp = i ^ padlen ^ orig_data.Length;
                temp = (temp << 4) + (int)orig_data[Convert.ToInt32(Math.Abs(Math.Sin(temp) * (orig_data.Length - 1)))];
                if (accept_pad!=null)
                {
                    temp = (temp << 4) ^ accept_pad.Length;
                    ret[i] = accept_pad[Math.Abs(Math.Sign(temp) * (accept_pad.Length - 1))];
                }
                else
                {
                    ret[i] = Convert.ToByte(temp);
                }

            });
            return ret;
        }

    }
}