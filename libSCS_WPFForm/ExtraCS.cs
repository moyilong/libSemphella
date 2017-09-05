using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
namespace libSCS_WPFForm
{
    public static class ExtraCS
    {
        public static bool Confirm(string data, string title)
        {
            return (MessageBox.Show(data, title, MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.Yes);
        }

        public static void Error(string data, string title)
        {
            MessageBox.Show(data, title, MessageBoxButtons.OK, MessageBoxIcon.Error);
        }

        public static void Tips(string data, string title)
        {
            MessageBox.Show(data, title, MessageBoxButtons.OK, MessageBoxIcon.Information);
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

        public static void WriteListViewToCSV(ListView view, string path = null, string decoder = "utf-16", string encoder = "gb2312")
        {
            string rpath = path;
            if (rpath == null)
            {
                OpenFileDialog dialog = new OpenFileDialog();
                dialog.FileName = "output.csv";
                dialog.Filter = "CSV文件|*.csv";
                dialog.Multiselect = false;
                dialog.CheckFileExists = false;
                dialog.CheckPathExists = true;
                if (dialog.ShowDialog() != DialogResult.OK)
                    return;
                rpath = dialog.FileName;
            }
            if (rpath == null)
                return;
            string write = "";
            for (int n = 0; n < view.Items.Count; n++)
            {
                string line = "";
                for (int x = 0; x < view.Items[n].SubItems.Count; x++)
                {
                    if (line != "")
                        line += ",";
                    line += view.Items[n].SubItems[x].Text;
                }
                write += line + "\n";
            }
            Encoding target = null;
            try
            {
                target = System.Text.Encoding.GetEncoding(encoder);
            }
            catch (Exception e)
            {
                EncodingInfo[] list = Encoding.GetEncodings();
                string codelist = "";
                foreach (EncodingInfo info in list)
                    codelist += "\n" + info.Name + " == " + info.DisplayName;
                Error("编码器获取错误 ：" + encoder + codelist, "编码器错误");
                return;
            }
            Encoding orig = Encoding.GetEncoding(decoder);
            Encoding output = Encoding.GetEncoding(encoder);
            byte[] convert = orig.GetBytes(write);
            convert = Encoding.Convert(orig, output, convert);
            File.WriteAllBytes(rpath, convert);
        }
    }
}
