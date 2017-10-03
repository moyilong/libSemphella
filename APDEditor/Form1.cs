using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using CSemphella;
using libSCS_WPFForm;
using System.IO;

namespace APDEditor
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            apd = new apd();
            ExtraCS.UpdateComboToAutoComplete(comboBox1, false);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox3.Text != "" && textBox2.Text == "")
            {
                ExtraCS.Error("使用密钥必须同时使用密码!");
                return;
            }
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                textBox1.Text = openFileDialog1.FileName;
            }
            if (File.Exists(textBox1.Text))
            {
                string pwd = null;
                if (label2.Text != "")
                    pwd = label2.Text;
                if (textBox3.Text != "")
                    apd.OpenFile(textBox1.Text, pwd);
                else
                    apd.RSAOpenFile(textBox1.Text, pwd, textBox3.Text);
                Flush();
            }
        }
        void Flush()
        {
            comboBox1.Items.Clear();
            foreach (string name in apd.SectionList)
                comboBox1.Items.Add(name);
        }
        apd apd =null;
        private void button4_Click(object sender, EventArgs e)
        {
            textBox3.Text = RSAHelper.GeneratePrivateKey();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            string key = RSAHelper.GetPublicKey(textBox3.Text);
            Clipboard.SetText(key);
            libSCS_WPFForm.ExtraCS.Tips("公钥已经复制到剪切板");
        }

        private void button5_Click(object sender, EventArgs e)
        {
            
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (textBox3.Text!="" && textBox2.Text == "")
            {
                ExtraCS.Error("RSA加密必须配合密码使用!");
                return;
            }
            apd.BinaryMode = (textBox2.Text != "");  
            if (textBox3.Text=="")
            {
                apd.WriteFile(textBox1.Text);
            }
            else
            {
                apd.RSAWriteFile(textBox1.Text, textBox2.Text, textBox3.Text);
            }
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            dataGridView1.Rows.Clear();
            foreach(apd.Node node in apd.NameList(comboBox1.Text))
            {
                int id = dataGridView1.Rows.Add();
                dataGridView1.Rows[id].Cells[0].Value = node.name;
                dataGridView1.Rows[id].Cells[1].Value = node.data;
            }
        }

        private void dataGridView1_AllowUserToDeleteRowsChanged(object sender, EventArgs e)
        {
            
        }

        private void dataGridView1_AllowUserToAddRowsChanged(object sender, EventArgs e)
        {

        }

        private void dataGridView1_CancelRowEdit(object sender, QuestionEventArgs e)
        {
            
        }

        private void dataGridView1_UserAddedRow(object sender, DataGridViewRowEventArgs e)
        {
            apd.Insert(comboBox1.Text, e.Row.Cells[0].Value as string, e.Row.Cells[1].Value as string);
        }

        private void dataGridView1_UserDeletedRow(object sender, DataGridViewRowEventArgs e)
        {
            apd.Delete(comboBox1.Text, e.Row.Cells[0].Value as string);
        }

        private void dataGridView1_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            apd.Insert(comboBox1.Text, dataGridView1.Rows[e.RowIndex].Cells[0].Value as string, dataGridView1.Rows[e.RowIndex].Cells[1].Value as string);
        }
    }
}
