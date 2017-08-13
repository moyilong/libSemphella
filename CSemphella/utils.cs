using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSemphella
{
    public static class utils
    {
        public static bool in_array<T>(T [] get,T find)
        {
            bool stat = false;
            Parallel.ForEach(get, who =>
             {
                 if (who.Equals(find))
                     stat = true;
             });
            return stat;
        }
    }
}
