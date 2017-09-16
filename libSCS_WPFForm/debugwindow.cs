using System;
using System.IO;
using System.Windows.Forms;
using CSemphella;
namespace libSCS_WPFForm
{
    partial class debugwindow : Form
    {
        private static debugwindow formal = new debugwindow();
        public static bool WriteToConsole = true;
        public static bool Enable = true;
        public static void Display()
        {
            formal.Show();
        }

        public static void Push(string str)
        {
            formal.AddString(str);
        }

        debugwindow()
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
            try
            {
                richTextBox1.Text = richTextBox1.Text + System.Environment.NewLine + data;
                if (checkBox1.Checked)
                {
                    try
                    {
                        ExtraCS.RichTextBoxAutoScrool(richTextBox1);
                    }
                    catch
                    {

                    }
                }
            }
            catch
            {

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

    public class DebugSection : DebugNode
    {
        public bool GraphiceOutput = true;
        public bool ConsoleOutput = true;
        public DebugSection(string n,bool stat=true) : base(n,stat)
        {

        }

        public void GraphiceOut(string str)
        {
            if (str == null)
                return;
            debugwindow.Push(str);
        }

        public override void Push(string str)
        {
            if (Enable)
            {
                string data = Msg(str);
                if (GraphiceOutput)
                    GraphiceOut(data);
                if (ConsoleOutput)
                    ConsoleOut(data);
            }
        }
    }
}