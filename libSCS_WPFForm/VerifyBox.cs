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
    public partial class VerifyBox : Form
    {
        bool view_mode = false;
        static DebugSection section = new DebugSection("VerifyBox");
        static bool DebugStatus
        {
            get
            {
                return section.Enable;
            }
            set
            {
                section.Enable = value;
            }
        }
        public VerifyBox(bool center_mode = false, bool view = false)
        {
            InitializeComponent();
            CheckForIllegalCrossThreadCalls = false;
            if (center_mode)
            {
                FormBorderStyle = FormBorderStyle.None;
                StartPosition = FormStartPosition.CenterParent;
            }
            if (availabel_str == null)
                AvailabelString = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            button3_Click(null, null);
            section.Push("长 x 宽 = " + pictureBox1.Width.ToString() + " x " + pictureBox1.Height.ToString());
            TimerValue = 60;
            timer.Interval = 1000 / 120;
            section.Push("刷新延迟:" + timer.Interval.ToString());
            timer.Elapsed += TimerThread;
            view_mode = view;
        }
        static string remove_list = "0691IOMW";
        static VerifyBox()
        {
            AvailabelString = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        }
        void Reset()
        {
            progressBar1.Value = progressBar1.Maximum;
        }

        public int TimerValue
        {
            set
            {
                progressBar1.Maximum = Convert.ToInt32(value * 1000);
                Reset();
            }
        }
        void TimerThread(object send, EventArgs e)
        {
            progressBar1.Value -= Convert.ToInt32(timer.Interval);
            if ((progressBar1.Value == 0) || (view_mode && progressBar1.Value % 100 == 0))
            {
                button3_Click(null, null);
            }
        }
        System.Timers.Timer timer = new System.Timers.Timer();
        string current = "";
        static string availabel_str = null;
        int len = 5;
        static public string AvailabelString
        {
            get
            {
                return availabel_str;
            }
            set
            {
                string ret = "";
                while (ret.Length < 256)
                {
                    char data = value[FancyRandom(0, value.Length)];
                    if (remove_list.IndexOf(data) != -1)
                        continue;
                    ret += data;
                }
                availabel_str = ret.ToUpper().Trim();
                section.Push("填充密钥:" + availabel_str);
            }
        }
        public int Len
        {
            get
            {
                return len;
            }
            set
            {
                len = value;
            }
        }
        static int seed = 0;
        static int FancyRandom(int min = int.MinValue, int max = int.MaxValue)
        {
            seed += min ^ max ^ seed;
            int ret = new Random(min ^ max ^ DateTime.UtcNow.Millisecond ^ seed).Next(min, max);
            seed += ret ^ min - max << 4;
            return ret;
        }
        static Color GetRandomColor()
        {
            return Color.FromArgb(FancyRandom(128, 255), FancyRandom(128, 255), FancyRandom(128, 255));
        }
        private void button3_Click(object sender, EventArgs e)
        {
            Reset();
            current = "";
            for (int n = 0; n < len; n++)
                current += availabel_str[FancyRandom(0, availabel_str.Length)];
            section.Push("验证码已经生成:" + current);
            Bitmap bmp = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            Graphics gpu = Graphics.FromImage(bmp);
            Font font = new Font("微软雅黑", pictureBox1.Height * (float)0.4, FontStyle.Regular);
            gpu.FillRectangle(Brushes.Black, 0, 0, bmp.Width, bmp.Height);
            for (int n = 0; n < len; n++)
            {
                Color c = GetRandomColor();
                while (c.GetBrightness() < 0.7)
                {
                    section.Push("重新生成颜色:" + c.GetBrightness().ToString());
                    c = GetRandomColor();
                }
                Brush brush = new SolidBrush(c);
                gpu.DrawString(current.Substring(n, 1), font, brush, new Point(((pictureBox1.Width - (35 * len)) / 2) + ((35 + FancyRandom(-5, 5)) * n), Convert.ToInt32((pictureBox1.Height - font.Height) / 2) + FancyRandom(-10, 10)));
            }
            gpu.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
            gpu.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
            gpu.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.HighQuality;
            for (int n = 0; n < FancyRandom(10, 30); n++)
            {
                Pen tpen = new Pen(GetRandomColor());
                gpu.DrawLine(tpen,
                    new Point(FancyRandom(0, pictureBox1.Width), FancyRandom(0, pictureBox1.Height)),
                    new Point(FancyRandom(0, pictureBox1.Width), FancyRandom(0, pictureBox1.Height))
                    );
            }
            for (int n = 0; n < FancyRandom(10, 20); n++)
            {
                Pen tpen = new Pen(GetRandomColor());
                Point[] plist = new Point[FancyRandom(3, 6)];
                for (int x = 0; x < plist.Length; x++)
                    plist[x] = new Point(FancyRandom(0, pictureBox1.Width), FancyRandom(0, pictureBox1.Height));
                gpu.DrawCurve(tpen, plist);
            }
            pictureBox1.Image = bmp;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            Close();
        }
        int error_count = 0;
        private void button1_Click(object sender, EventArgs e)
        {
            bool stat = true;
            textBox1.Text = textBox1.Text.ToUpper().Trim();
            for (int n = 0; n < len; n++)
            {
                section.Push("比较:" + Convert.ToChar(current[n]) + " <=> " + Convert.ToChar(textBox1.Text[n]));
                if (current[n] != textBox1.Text[n])
                {
                    section.Push("比较位错误:" + n.ToString());
                    stat = false;
                    break;
                }
            }
            if (!stat)
            {
                if (error_count > 10)
                {
                    ExtraCS.Error("尝试次数太多!");
                    DialogResult = DialogResult.Cancel;
                    Close();
                    return;
                }
                ExtraCS.Error("验证码错误");
                error_count++;
                return;
            }
            DialogResult = DialogResult.OK;
            Close();
        }

        private void VerifyBox_Load(object sender, EventArgs e)
        {
            timer.Start();
        }

        private void progressBar1_Click(object sender, EventArgs e)
        {

        }
    }
}