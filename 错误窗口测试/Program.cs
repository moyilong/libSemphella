using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace 错误窗口测试
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            libSCS_WPFForm.ErrorHandle.eminfo = new CSemphella.EMailInfo();
            libSCS_WPFForm.ErrorHandle.eminfo.SendName = "自动更新程序";
            libSCS_WPFForm.ErrorHandle.eminfo.SendMail = "13291917395@163.com";
            libSCS_WPFForm.ErrorHandle.eminfo.SMTPServer = "smtp.163.com";
            libSCS_WPFForm.ErrorHandle.eminfo.SMTPUser = "13291917395";
            libSCS_WPFForm.ErrorHandle.eminfo.SMTPPassword = "moyilong10250396";
            libSCS_WPFForm.ErrorHandle.SendTo = "1025039646@qq.com";
            try
            {
                Application.Run(new Form1());
            }
            catch (Exception e)
            {
                libSCS_WPFForm.ErrorHandle.Show("测试", e);
            }
        }
    }
}
