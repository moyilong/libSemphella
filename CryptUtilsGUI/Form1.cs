using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
namespace CryptUtilsGUI
{
    public partial class Form1 : Form
    {
        [DllImport("libERT.dll")]
        static extern  UInt64 crypt_to_file(string a, string b, string password, int alg = 6, int fid = 0, string extfil = "", int bs = 4096);
        public Form1()
        {
            InitializeComponent();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            FileDialog dialog = new OpenFileDialog();
            dialog.ShowDialog();
            textBox2.Text = dialog.FileName;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            FileDialog dialog=new OpenFileDialog();
            dialog.ShowDialog();
            textBox1.Text = dialog.FileName;
            if (!checkBox1.Checked&&textBox2.Text.Length==0)
            {
                textBox2.Text = textBox1.Text + ".ert4";
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (textBox1.Text.Length == 0)
            {
                label4.Text = "输入为空!";
                return;
            }
            if (textBox2.Text.Length == 0)
            {
                label4.Text = "输出为空!";
                return;
            }
            if (textBox3.Text.Length == 0)
            {
                label4.Text = "密码为空!";
                return;
            }
            crypt_to_file(textBox1.Text, textBox2.Text, textBox3.Text);
        }
    }
}
