using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSemphella
{
    public interface CacheObject
    {
        int ID { get; }
    }
    public class CacheOperator
    {
        List<CacheObject> cache_poll = new List<CacheObject>();
        public void AddCachePoll(CacheObject obj)
        {
            this.DeleteFromCache(obj);
            cache_poll.Add(obj);
        }

        public CacheObject ReadFromCache(int id)
        {
            foreach (CacheObject ifd in cache_poll)
                if (ifd.ID == id)
                    return ifd;
            return null;
        }

        public void DeleteFromCache(CacheObject obj)
        {
            for (int n=0;n<cache_poll.Count;n++)
                if (cache_poll[n].ID == obj.ID)
                {
                    cache_poll.RemoveAt(n);
                    return;
                }
        }

        public int CacheNumber
        {
            get
            {
                return cache_poll.Count;
            }
        }

        public void Clean()
        {
            cache_poll.Clear();
        }
    }
}
