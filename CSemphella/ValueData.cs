using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSemphella
{
    public class ValueData
    {
        private string name;
        private string data;
        private bool ro;
        public bool ReadOnly
        {
            set
            {
                if (ro)
                    return;
                ro = value;
            }
            get
            {
                return ro;
            }
        }
        
        public string Name
        {
            get
            {
                return name;
            }
            set
            {
                if (ro)
                    return;
                name = value;
            }
        }
        public string Data
        {
            get
            {
                return data;
            }
            set
            {
                if (ro)
                    return;
                data = value;
            }
        }

        public static ValueData Search(string nodename,ValueData[] list)
        {
            foreach (ValueData data in list)
                if (nodename == data.Name)
                    return data;
            return null;
        }

        public static string SearchData(string nodename,ValueData[] data)
        {
            try
            {
                return Search(nodename, data).Data;
            }
            catch
            {

            }
            return null;
        }
    }
}
