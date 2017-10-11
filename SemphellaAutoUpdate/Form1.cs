using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SemphellaAutoUpdate
{
    public partial class Form1 : Form
    {
        string dpath;
        CompareProcessor cmp = null;
        private string v;

        public Form1(string path,UpdateServerHAL hal,string[]ut)
        {
            InitializeComponent();
            CheckForIllegalCrossThreadCalls = false;
            cmp = new CompareProcessor(hal,ut);
            dpath = path;
            
        }
        Thread tr = null;
        private void button1_Click(object sender, EventArgs e)
        {
            tr = new Thread(new ThreadStart(UpdateThread));
            tr.Start();
        }
        void UIUpdater(string processor,string file,int current,int all)
        {
            if (current != -1 && all != -1)
            {
                progressBar2.Maximum = all;
                progressBar2.Value = current;
            }
            else
            {
                progressBar2.Value = 0;
            }
            string text = processor;
            if (file != null && file != "")
                text += " <- " + file ;
            richTextBox1.Text += System.Environment.NewLine+text;
            libSCS_WPFForm.ExtraCS.RichTextBoxAutoScrool(richTextBox1);
        }
        void UpdateThread()
        {
            button1.Enabled = false;
            button2.Enabled = false;
            try
            {
                if (cmp.CompareDirectory(dpath, UIUpdater))
                    libSCS_WPFForm.ExtraCS.Tips("更新成功!");
            }catch(Exception e )
            {
                libSCS_WPFForm.ErrorHandle.Show("更新错误", e);
            }
            button1.Enabled = true;
            button2.Enabled = true;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            System.Environment.Exit(0);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            try
            {
                tr.Abort();
                button1.Enabled = true;
                button2.Enabled = true;
            }
            catch
            {

            }
        }
    }
}
