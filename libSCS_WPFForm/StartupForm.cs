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
            label1.Parent = pictureBox1;
            label2.Parent = pictureBox1;
        }
        public Color TipsColor{
            set
            {
                label1.ForeColor = value;
                label2.ForeColor = value;
            }

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

        private void pictureBox1_Click(object sender, System.EventArgs e)
        {

        }
    }
}