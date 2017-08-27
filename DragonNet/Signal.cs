using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using libSCS_WPFForm;
using CSemphella;
using System.Collections;
using System.IO;

namespace DragonNet
{
    public partial class Signal : Form
    {
        Hashtable rdata = null;
        public Signal(string ip)
        {
            InitializeComponent();
            
            string data = web.GetContentByURL("http://" + ip + ":570");
            rdata = utils.EqualFormat(data);
            textBox1.Text = rdata["DNAME"] as string;
            textBox2.Text = rdata["MODEL"] as string;
            textBox3.Text = rdata["DRAGONOS_BUILD_TIME"] as string;
            textBox4.Text = rdata["DEVICE_SERIAL"] as string;
            textBox5.Text = rdata["UNAME"] as string;
            byte[] img = web.GetContentByURL_Byte("http://" + ip + "/img/nightfury.jpg");
            pictureBox1.Image = Image.FromStream(new MemoryStream(img),true);
            pictureBox1.BackgroundImageLayout = ImageLayout.Stretch;
            pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
            MaximizeBox = false;
            TopMost = true;
            FormBorderStyle = FormBorderStyle.FixedDialog;
            Text = "DragonAssistant => " + ip + " <= " + textBox4.Text;
        }

        private void Signal_Load(object sender, EventArgs e)
        {

        }
    }
}
