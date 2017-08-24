using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
namespace libSCS_WPFForm
{
    public static class ExtraCS
    {
        public static bool Confirm(string title,string data)
        {
            return (MessageBox.Show(data, title, MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.Yes);
        }
        public static void RichTextBoxAutoScrool(RichTextBox rt)
        {
            rt.SelectionStart = rt.Text.Length;
            rt.ScrollToCaret();
        }
    }
}
