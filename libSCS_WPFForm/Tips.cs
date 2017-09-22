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

namespace libSCS_WPFForm
{
    public partial class Tips : Form
    {
        public Tips(string prestore="",IWin32Window show=null,bool effect_status=false)
        {
            InitializeComponent();
            TipsString = prestore;
            timer.Elapsed += Effects;
            timer.Interval = 500;
            if (show!=null)
                Show(show);
            TipsEffects = effect_status;
        }

        UInt64 count = 0;
        void Effects(object sender ,EventArgs e)
        {
            try
            {
                count++;
                if (count % 5 == 0)
                {
                    count = 0;
                    label1.Text = current_cacue;
                }
                else
                    label1.Text += ".";
            }
            catch
            {

            }
        }
        System.Timers.Timer timer = new System.Timers.Timer();
        public string current_cacue = "";

        public string TipsString
        {
            get
            {
                return label1.Text;
            }
            set
            {
                label1.Text = value;
                current_cacue = value;
            }
        }
        public bool TipsEffects
        {
            get
            {
                return timer.Enabled;
            }
            set
            {
                timer.Enabled = value;
            }
        }
    }
}
