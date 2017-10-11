using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SemphellaAutoUpdate
{
    
    static class Program
    {
        static string server = "http://localhost/updateserver.php";
        static string project = "Sample";
        static string dir = null;
        static List<string> unTouch = new List<string>();
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main(string[] arg)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            if (dir == null)
            {
                FolderBrowserDialog cdir = new FolderBrowserDialog();
                if (cdir.ShowDialog() != DialogResult.OK)
                    System.Environment.Exit(0);
                dir = cdir.SelectedPath;
            }foreach (string s in arg)
            {
                if (s[0] != '-')
                    continue;
                string args = s.Substring(2);
                switch(s[1])
                {
                    case 'd':
                        dir = args;
                        break;
                    case 's':
                        server = args;
                        break;
                }
            }
            libSCS_WPFForm.ErrorHandle.eminfo = new CSemphella.EMailInfo();
            libSCS_WPFForm.ErrorHandle.eminfo.SendName = "自动更新程序";
            libSCS_WPFForm.ErrorHandle.eminfo.SendMail = "1025039646@qq.com";
            libSCS_WPFForm.ErrorHandle.eminfo.SMTPServer = "smtp.163.com";
            libSCS_WPFForm.ErrorHandle.eminfo.SMTPUser = "13291917395";
            libSCS_WPFForm.ErrorHandle.eminfo.SMTPPassword = "moyilong10250396";
            libSCS_WPFForm.ErrorHandle.SendTo = "1025039646@qq.com";
            Application.Run(new Form1(dir, new DefaultPHPConfigure(server, project),unTouch.ToArray()));
        }
    }
}
