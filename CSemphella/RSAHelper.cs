using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Security.Cryptography;
using System.IO;
using System.Security.Cryptography.X509Certificates;

namespace CSemphella
{
    public class RSAHelper
    {
        DebugNode node = new DebugNode("RSAHelper");
        public static string GetPublicKey(string private_key)
        {
            RSACryptoServiceProvider rsa = new RSACryptoServiceProvider();
            rsa.FromXmlString(private_key);
            return rsa.ToXmlString(false);
        }

        public static string GeneratePrivateKey()
        {
            RSACryptoServiceProvider rsa = new RSACryptoServiceProvider();
            return rsa.ToXmlString(true);
        }

        public string Crypt(byte[] data)
        {
            /*byte[] vdata = rsa.Encrypt(data, false);
            return BinaryString.BinaryToString(vdata);*/
            int block_len = rsa.KeySize / 8 - 16;
            node.Push("加密长度:" + block_len.ToString());
            node.Push("密钥长度:" + rsa.KeySize.ToString());
            string ret = "";
            for (int n = 0; n < data.Count(); n += block_len)
            {
                byte[] cache = new byte[block_len];
                for (int x = n; x < block_len; x++)
                    cache[x - n] = data[x];
                node.Push("重映射:" + n.ToString() + " + " + block_len.ToString());
                ret += BinaryString.BinaryToString(rsa.Encrypt(cache, false)) + "\n";
            }
            return ret;
        }
        RSACryptoServiceProvider rsa = new RSACryptoServiceProvider();
        public byte[] Decrypt(string data)
        {
            //return rsa.Decrypt(BinaryString.StringToBinary(data), false);
            string[] block = data.Split('\n');
            List<byte> ret = new List<byte>();
            for (int i=0;i<block.Length;i++)
            {
                byte[] b= rsa.Decrypt(BinaryString.StringToBinary(block[i]), false);
                for (int n = 0; n < b.Length; n++)
                    ret.Add(b[n]);
            }
            return ret.ToArray();
        }

        public string Key
        {
            set
            {
                rsa.FromXmlString(value);
            }
        }

        public bool IsPublicKey
        {
            get
            {
                return rsa.PublicOnly;
            }
        }

        public static object RSACrypto { get; private set; }
    }
}
