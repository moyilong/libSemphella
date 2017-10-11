using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Mail;
using System.Text;
using System.Threading.Tasks;

namespace CSemphella
{
    public class EMailInfo
    {
        public string SendName;
        public string SendMail;
        public string SMTPServer;
        public string SMTPUser;
        public string SMTPPassword;
        public List<string> CopyTo;
        public bool InformationComplete
        {
            get
            {
                return (
                    SendMail != "" &&
                    SMTPServer != "" &&
                    SMTPUser != "" &&
                    SMTPPassword != "" && 
                    SendName != ""
                    );
            }
        }
        public SmtpClient GetSMTP
        {
            get
            {
                SmtpClient client = new SmtpClient();
                client.DeliveryMethod = SmtpDeliveryMethod.Network;
                client.Host = SMTPServer;
                client.UseDefaultCredentials = true;
                client.Credentials = new NetworkCredential(SMTPUser, SMTPPassword);
                return client;
            }
        }
    }
    static public class EMail
    {
        static DebugNode node = new DebugNode("电子邮件");
        static public bool SendTextMail(string SendTo,string Subject,string Data,EMailInfo info)
        {
            MailMessage mail = new MailMessage();
            node.Push("获取SMTP客户端...");
            SmtpClient client = info.GetSMTP;
            node.Push("设置邮件信息...");
            mail.Subject = Subject;
            mail.Body = Data;
            mail.BodyEncoding = System.Text.Encoding.Default;
            mail.IsBodyHtml = false;
            mail.Priority = MailPriority.High;
            node.Push("添加发送人和抄送人....");
            mail.From = new MailAddress(info.SendMail,info.SendName);
            mail.To.Add(SendTo);
            if (info.CopyTo != null)
                foreach (string cc in info.CopyTo)
                    mail.CC.Add(cc);
            try
            {
                node.Push("发送邮件...");
                client.Send(mail);
            }
            catch (System.Net.Mail.SmtpException ex)
            {
                node.Push(ex.ToString());
                return false;
            }
            return true;

        }
    }
}
