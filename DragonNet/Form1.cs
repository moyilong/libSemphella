using CSemphella;
using libSCS_WPFForm;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.NetworkInformation;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DragonNet
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            CheckForIllegalCrossThreadCalls = false;
            progressBar1.Maximum = 255;
            button1_Click(null, null);
            ActiveThread = 0;
            MutexLock(true);
        }

        private List<IPAddress> poll = null;

        private void button1_Click(object sender, EventArgs e)
        {
            comboBox1.Items.Clear();
            poll = web.GetIPList();
            foreach (IPAddress ip in poll)
            {
                comboBox1.Items.Add(ip.ToString());
            }
        }

        private void MutexLock(bool stat)
        {
            button2.Enabled = stat;
            button1.Enabled = stat;
            button3.Enabled = stat;
            button4.Enabled = !stat;
            comboBox1.Enabled = stat;
        }

        private int active_thread = 0;

        private int ActiveThread
        {
            set
            {
                active_thread = value;
                if (active_thread < 0)
                    active_thread = 0;
                textBox1.Text = active_thread.ToString();
            }
            get
            {
                return active_thread;
            }
        }

        private static string head_prefix = "DRAGONOS_NIGHTFURY_DRIVER_VERSION";
        private bool stop_label = false;

        private void SingalIPCheck(string ipaddr)
        {
            PingReply ping_data = new Ping().Send(ipaddr);
            if (ping_data != null && ipaddr != null)
                if (ping_data.Status == IPStatus.Success)
                {
                    System.DateTime start = DateTime.UtcNow;
                    string data = web.GetContentByURL("http://" + ipaddr + ":570");
                    System.TimeSpan diff = DateTime.UtcNow - start;
                    if (data.Length != 0)
                    {
                        if (data.Substring(0, head_prefix.Length) == head_prefix)
                        {
                            Hashtable rdata = utils.EqualFormat(data);
                            string[] array = data.Split('\n');
                            ListViewItem item = new ListViewItem();
                            item.SubItems.Add(ipaddr);
                            item.SubItems.Add(
                                rdata["DRAGONOS_SDK"] as string + " " +
                                rdata["DRAGONOS_BUILD_ID"] as string);
                            item.SubItems.Add(diff.TotalMilliseconds.ToString());
                            item.SubItems.Add(ping_data.RoundtripTime.ToString());
                            try
                            {
                                item.SubItems.Add(rdata["DEVICE_SERIAL"].ToString());
                            }
                            catch (Exception)
                            {
                                item.SubItems.Add("Unknow");
                            }
                            listView1.Items.Add(item);
                        }
                    }
                }
        }

        private void Search()
        {
            IPAddress local = null;
            foreach (IPAddress ip in poll)
            {
                if (ip.ToString() == comboBox1.SelectedItem as string)
                {
                    local = ip;
                    break;
                }
            }
            if (local == null)
            {
                debugwindow.Push("Invalid IP!");
                goto end;
            }
            progressBar1.Value = 0;
            Parallel.For(0, 255, new ParallelOptions()
            {
                MaxDegreeOfParallelism = 200,
            }, i =>
            {
                if (stop_label)
                    return;
                ActiveThread += 1;
                progressBar1.Value++;
                try
                {
                    string[] temp = local.ToString().Split('.');
                    string ipaddr = temp[0] + "." + temp[1] + "." + temp[2] + "." + i.ToString();
                    temp = null;
                    SingalIPCheck(ipaddr);
                }
                catch
                {
                }
                ActiveThread -= 1;
            });
            end:
            MutexLock(true);
        }

        private Thread search_t = null;

        private void button2_Click(object sender, EventArgs e)
        {
            if (search_t != null)
            {
                search_t.Abort();
                search_t = null;
            }
            try
            {
                IPAddress test = IPAddress.Parse(comboBox1.Text as string);
            }
            catch
            {
                MessageBox.Show("请输入一个标准的IPv4地址\n比如:192.168.1.1", "IP地址不标准!", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            MutexLock(false);
            stop_label = false;
            search_t = new Thread(new ThreadStart(Search));
            search_t.Start();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            listView1.Items.Clear();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            stop_label = true;
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            System.Environment.Exit(0);
        }

        private void 关于程序ToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            new AboutBox1().ShowDialog(this);
        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {
        }

        private void listView1_DoubleClick(object sender, EventArgs e)
        {
            /*ListViewItem selected = listView1.FocusedItem;
            System.Diagnostics.Process.Start("explorer.exe", "http://" + selected.SubItems[1].Text);*/
            new Signal(listView1.FocusedItem.SubItems[1].Text).ShowDialog();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            ExtraCS.UpdateComboToAutoComplete(comboBox1, false);
        }
    }
}