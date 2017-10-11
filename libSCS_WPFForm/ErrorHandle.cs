using System;
using System.Windows.Forms;

namespace libSCS_WPFForm
{
    public partial class ErrorHandle : Form
    {
        public static CSemphella.EMailInfo eminfo=null;
        public static string SendTo = null;
        public ErrorHandle()
        {
            InitializeComponent();
            //this.FormBorderStyle = FormBorderStyle.Fixed3D;
            if (eminfo == null || SendTo == null)
                button3.Enabled = false;
        }

        public string Title
        {
            set
            {
                //this.Text = value;
                label1.Text = value;
            }
            get
            {
                return label1.Text;
            }
        }

        public string Data
        {
            set
            {
                richTextBox1.Text = value;
            }
            get
            {
                return richTextBox1.Text;
            }
        }

        public static void Show(string title, string data)
        {
            ErrorHandle hand = new ErrorHandle();
            hand.Title = title;
            hand.Data = data;
            hand.ShowDialog();
        }

        public static void Show(string title, Exception data)
        {
            Show(title, data.ToString());
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Clipboard.SetText(richTextBox1.Text);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (CSemphella.EMail.SendTextMail(SendTo, "应用程序错误报告:"+Title, Data, eminfo))
                ExtraCS.Tips("发送成功!");
            else
                ExtraCS.Tips("发送失败!");
        }
    }
}