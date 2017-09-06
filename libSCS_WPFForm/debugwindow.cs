using System;
using System.IO;
using System.Windows.Forms;

namespace libSCS_WPFForm
{
    public partial class debugwindow : Form
    {
        private static debugwindow formal = new debugwindow();

        public static void Display()
        {
            formal.Show();
        }

        public static void Push(string str)
        {
            formal.AddString(str);
        }

        public debugwindow()
        {
            InitializeComponent();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            OpenFileDialog file = new OpenFileDialog();
            file.Multiselect = false;
            if (file.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    File.WriteAllText(file.FileName, richTextBox1.Text);
                }
                catch (Exception xe)
                {
                    ErrorHandle.Show("保存时发生错误", xe);
                }
            }
        }

        public void AddString(string data)
        {
            richTextBox1.Text = richTextBox1.Text + System.Environment.NewLine + "[" + System.DateTime.UtcNow.ToString() + "]" + data;
            if (checkBox1.Checked)
            {
                ExtraCS.RichTextBoxAutoScrool(richTextBox1);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            richTextBox1.Text = "";
        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {
        }
    }
}