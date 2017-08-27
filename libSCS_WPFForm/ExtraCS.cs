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

        public static void Error(string title,string data)
        {
            MessageBox.Show(data, title, MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
        public static void RichTextBoxAutoScrool(RichTextBox rt)
        {
            rt.SelectionStart = rt.Text.Length;
            rt.ScrollToCaret();
        }
        public static void TextboxNumberLimits(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 0x20) e.KeyChar = (char)0;  //禁止空格键  
            if ((e.KeyChar == 0x2D) && (((TextBox)sender).Text.Length == 0)) return;   //处理负数  
            if (e.KeyChar > 0x20)
            {
                try
                {
                    double.Parse(((TextBox)sender).Text + e.KeyChar.ToString());
                }
                catch
                {
                    e.KeyChar = (char)0;   //处理非法字符  
                }
            }
        }

    }
}
