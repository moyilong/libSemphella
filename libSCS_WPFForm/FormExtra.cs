using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections;
using System.IO;

namespace libSCS_WPFForm
{
    public struct DataCollect
    {
        public int disp_id;
        public string disp_name;
        public object direct_data;

        public string DisplayName
        {
            get
            {
                return disp_id.ToString() + ":" + disp_name;
            }
        }

        public static DataCollect[] FormatCollect(DataCollect[] collect)
        {
            DataCollect[] ret = new DataCollect[collect.Length];
            Parallel.For(0, collect.Length, n =>
            {
                collect[n] = collect[n];
                collect[n].disp_id = n;
            });
            return ret;
        }
    }

    public class ComboExtra : ComboBox
    {
        private DataCollect[] collect = null;

        public void InitCollect(DataCollect[] data)
        {
            collect = DataCollect.FormatCollect(data);
            UpdateDisplay();
        }

        private void UpdateDisplay()
        {
            Items.Clear();
            foreach (DataCollect dc in collect)
                Items.Add(dc.DisplayName);
        }

        public object CurrentObject
        {
            get
            {
                string read = SelectedItem as string;
                int id = int.Parse(read.Split(':')[0]);
                foreach (DataCollect dc in collect)
                    if (dc.disp_id == id)
                        return dc.direct_data;
                return null;
            }
        }
    }

    public class ListViewExtra : ListView
    {
        private DataCollect[] collect = null;

        public void InitCollect(DataCollect[] data, Func<object, ListViewItem> call)
        {
            collect = DataCollect.FormatCollect(data);
            UpdateDisplay(call);
        }

        private void UpdateDisplay(Func<object, ListViewItem> call)
        {
            foreach (DataCollect dc in collect)
            {
                ListViewItem item = call(dc.direct_data);
                if (item == null)
                    return;
                item.SubItems.Insert(1, new ListViewItem.ListViewSubItem(item, dc.disp_id.ToString()));
                Items.Add(item);
            }
        }

        public object FocuesdObject
        {
            get
            {
                if (!Focused)
                    return null;
                int id = int.Parse(FocusedItem.SubItems[1].Text);
                foreach (DataCollect dc in collect)
                    if (dc.disp_id == id)
                        return dc.direct_data;
                return null;
            }
        }

        public object[] SlectedObject
        {
            get
            {
                object[] ret = new object[SelectedItems.Count];
                Parallel.For(0, SelectedItems.Count, n =>
                {
                    ret[n] = null;
                    int id = int.Parse(SelectedItems[n].SubItems[1].Text);
                    foreach (DataCollect fc in collect)
                        if (fc.disp_id == id)
                        {
                            ret[n] = fc.direct_data;
                        }
                });
                return ret;
            }
        }
    }
}