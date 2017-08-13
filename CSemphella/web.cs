using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Http;
using System.Web;
using System.Net;
using System.IO;
using System.Net.Security;
using System.Security.Cryptography.X509Certificates;
namespace CSemphella
{
    
    public static class web
    {
        private static bool CheckValidationResult(object sender, X509Certificate certificate, X509Chain chain, SslPolicyErrors errors)
        {
            return true; //总是接受  
        }
        public static string GetContentByURL(string url)
        {
            WebClient wc = new WebClient();
            ServicePointManager.ServerCertificateValidationCallback = new System.Net.Security.RemoteCertificateValidationCallback(CheckValidationResult);
            HttpWebRequest request = null;
            request = WebRequest.Create(url) as HttpWebRequest;
            HttpWebResponse getdata = request.GetResponse() as HttpWebResponse;
            StreamReader read = new StreamReader(getdata.GetResponseStream(), System.Text.Encoding.UTF8);
            return read.ReadToEnd();            
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
