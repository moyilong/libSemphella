using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Net.Security;
using System.Security.Cryptography.X509Certificates;
using System.Text;

namespace CSemphella
{
    public static class web
    {
        private static bool CheckValidationResult(object sender, X509Certificate certificate, X509Chain chain, SslPolicyErrors errors)
        {
            return true; //总是接受
        }
        static DebugNode node = new DebugNode("WebAccess");
        public static byte[] GetContentByURL_Byte(string url)
        {
            node.Push("Byte Mode URL:" + url);
            WebClient wc = new WebClient();
            ServicePointManager.ServerCertificateValidationCallback = new System.Net.Security.RemoteCertificateValidationCallback(CheckValidationResult);
            HttpWebRequest request = null;
            request = WebRequest.Create(url) as HttpWebRequest;
            HttpWebResponse getdata = request.GetResponse() as HttpWebResponse;
            Stream read = getdata.GetResponseStream();
            //getdata.GetResponseStream()
            List<byte> ret = new List<byte>();
            while (true)
            {
                int get = read.ReadByte();
                if (get == -1)
                    return ret.ToArray();
                ret.Add(Convert.ToByte(get));
            }
        }

        public static string GetContentByURL(string url)
        {
            node.Push("URL:" + url);
            byte[] get = GetContentByURL_Byte(url);
            return Encoding.UTF8.GetString(get);
        }

        public static List<IPAddress> GetIPList()
        {
            //return Dns.GetHostAddresses(Dns.GetHostName());
            List<IPAddress> ret = new List<IPAddress>();
            foreach (IPAddress get in Dns.GetHostAddresses(Dns.GetHostName()))
            {
                if (get.AddressFamily == System.Net.Sockets.AddressFamily.InterNetwork)
                    ret.Add(get);
            }
            return ret;
        }
    }
}