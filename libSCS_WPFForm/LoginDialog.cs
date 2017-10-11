using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace libSCS_WPFForm
{
    public partial class LoginDialog : Form
    {
        public LoginDialog(Func<string, string, bool> call_back)
        {
            InitializeComponent();
            CallResult = call_back;
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            checkBox1.Checked = !checkBox1.Checked;
            if (checkBox1.Checked)
                textBox2.PasswordChar = '\0';
            else
                textBox2.PasswordChar = '*';
        }

        public Image Banner
        {
            set
            {
                pictureBox1.Image = value;
            }
        }
        int error_count = 0;
        Func<string, string, bool> CallResult = null;
        private void button2_Click(object sender, EventArgs e)
        {
            if (textBox1.Text == "" || textBox2.Text == "")
            {
                ExtraCS.Error("请输入用户名密码");
                error_count++;
                return;
            }
            if (!CallResult(textBox1.Text,textBox2.Text))
            {
                ExtraCS.Error("用户名或密码错误!");
                error_count++;
                return;
            }
            if (error_count > 3)
            {
                VerifyBox box = new VerifyBox(true);
                if (box.ShowDialog(this) == DialogResult.Cancel)
                {
                    error_count++;
                    return;
                }
                if (error_count > 10)
                {
                    ExtraCS.Error("尝试次数太多！拒绝登陆！");
                    DialogResult = DialogResult.Cancel;
                    Close();
                    return;
                }
            }
            DialogResult = DialogResult.OK;
            Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            Close();
        }

        public static DialogResult ShowDialog(IWin32Window win, Func<string, string, bool> call_back)
        {
            if (call_back == null)
                throw new Exception("INVALID_LOGIN_CALLBACK");
            LoginDialog login = new LoginDialog(call_back);
            return login.ShowDialog(win);
        }

        private void LoginDialog_Load(object sender, EventArgs e)
        {

        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }
    }
}
