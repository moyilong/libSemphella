using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SemphellaAutoUpdate
{
    public class CompareProcessor
    {
        UpdateServerHAL hal = null;
        public CompareProcessor(UpdateServerHAL dhal)
        {
            hal = dhal;
        }
        public struct CompareFileInfo
        {
            public string filename;
            public DateTime LocalChangeDate;
            public DateTime RemoteChangeDate;
            public string RemoteMD5;
            public string LocalMD5;
            public bool RemoteExist;
        }

        void CompareDirectory(string dir,Action<string ,string> action = null)
        {
            DirectoryInfo ldir = new DirectoryInfo(dir);
            string[] rlist = hal.GetFileList();
            List<CompareFileInfo> ret = new List<CompareFileInfo>();
            foreach (FileInfo file in ldir.GetFiles())
            {
                action("检查本地文件...", file.Name);
                CompareFileInfo info = new CompareFileInfo();
                info.filename = file.Name;
                info.LocalChangeDate = file.LastWriteTime;
                foreach (string rname in rlist)
                    if (rname == file.Name)
                    {
                        info.RemoteExist = true;
                        break;
                    }
                if (!info.RemoteExist)
                {
                    ret.Add(info);
                    continue;
                }
                info.RemoteChangeDate = hal.GetFileChangeDate(file.Name);
                info.RemoteMD5 = hal.GetFileMD5(file.Name);
                ret.Add(info);
            }
            foreach (CompareFileInfo comp in ret)
            {
                action("正在处理文件....", comp.filename);
                string md5 =CSemphella.utils.GetMD5( File.ReadAllBytes(comp.filename));
                if (md5 == comp.RemoteMD5)
                    continue;
                action("正在下载文件....", comp.filename);
                byte[] download = hal.GetFileByte(comp.filename);
                if (CSemphella.utils.GetMD5(download) != )
            }
        }
    }
}
