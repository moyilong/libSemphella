using CSemphella;
using System.Drawing;
using System.Windows.Forms;

namespace libSCS_WPFForm
{
    public partial class StartupForm : Form
    {
        public StartupForm()
        {
            InitializeComponent();
            FormBorderStyle = FormBorderStyle.None;
            StartPosition = FormStartPosition.CenterScreen;
            label2.Text = "ELONE inside 2010 - 2017 " + libSemphellaCS.CurrentVersion.StringOut;
        }

        public string Tips
        {
            set
            {
                label1.Text = value;
            }
            get
            {
                return label1.Text;
            }
        }

        public Image SetPircture
        {
            set
            {
                pictureBox1.Image = value;
            }
        }

        public bool ShowLibVer
        {
            set
            {
                label2.Enabled = value;
            }
        }
    }
}