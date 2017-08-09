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
    public partial class ErrorHandle : Form
    {
        public ErrorHandle()
        {
            InitializeComponent();
            this.FormBorderStyle = FormBorderStyle.Fixed3D;
        }
        public string Title
        {
            set
            {
                this.Text = value;
                label1.Text = value;
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
        public static void Show(string title,string data)
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

        }
    }
}
