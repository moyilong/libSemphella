using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace libWindows
{
    public partial class libDebugWindows : Form
    {
        public libDebugWindows()
        {
            InitializeComponent();
            richTextBox1.Text = "libDebugWindows 1.0";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            richTextBox1.Text = "";
        }

        public void StringPush(string str)
        {
            richTextBox1.Text += str;
        }

    }
}
