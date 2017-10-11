using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace SemphellaAutoUpdate
{
    public class CompareProcessor
    {
        static CSemphella.DebugNode node = new CSemphella.DebugNode("CompareProcessor");
        List<string> unTouchFile = new List<string>();
        static string[] UnTouchList =
        {
            "version",
        };
        UpdateServerHAL hal = null;
        public CompareProcessor(UpdateServerHAL dhal,string[] utl=null)
        {
            hal = dhal;
            unTouchFile.Union(UnTouchList);
            if (utl != null)
                unTouchFile.Union(utl);
        }
        public struct CompareFileInfo
        {
            public string filename;
            public DateTime LocalChangeDate;
            public DateTime RemoteChangeDate;
            public string RemoteMD5;
            public string LocalMD5;
            public bool RemoteExist;
            public string fullpath;
        }
        List<string> ResicureDirectory(string path,string parent=null)
        {
            List<string> ret = new List<string>();
            DirectoryInfo info = new DirectoryInfo(path);
            foreach (FileInfo file in info.GetFiles())
            {
                string subname = "";
                if (parent != null)
                    subname = parent + "/";
                ret.Add(subname + file.Name);
            }
            foreach (DirectoryInfo dir in info.GetDirectories())
            {
                string subname = "";
                if (parent != null)
                    subname = parent + "/";
                ret.Union(ResicureDirectory(path + "/" + dir.Name, subname));
            }
            return ret;
        }
        bool InList(string rpath)
        {
            foreach (string name in unTouchFile)
                if (name == rpath)
                    return true;
            return false;
        }
        bool Download(string fullpath,string rpath,string rmd5,DateTime d)
        {
            byte[] download = hal.GetFileByte(rpath);
            string cmd5 = CSemphella.utils.GetMD5(download);
            if (cmd5 != rmd5)
            {
                libSCS_WPFForm.ExtraCS.Error("下载错误 循环冗余检查失败!\n" + rmd5 + "\n\t !=\n" + cmd5 + "\n数据长度:" + download.Length.ToString());
                return false;
            }
            File.WriteAllBytes(fullpath, download);
            File.SetLastWriteTime(fullpath, d);
            return true;
        }
        public bool  CompareDirectory(string dir,Action<string ,string,int,int> action = null)
        {
            int Current = 0;
            int.TryParse(File.ReadAllText(dir + "/version"),out Current);
            int Remote = hal.GetRemoteVersion();
            if (!libSCS_WPFForm.ExtraCS.Confirm("服务器版本:" + Remote.ToString() + "\n本地版本:" + Current.ToString(), "\n是否更新?"))
                return false;
            action("正在获取服务器文件列表....", "",-1,-1);
            string[] rlist = hal.GetFileList();
            List<CompareFileInfo> ret = new List<CompareFileInfo>();
            int count = 0;
            List<string> list = ResicureDirectory(dir);
            foreach (string file in list)
            {
                if (InList(file))
                    continue;
                action("检查本地文件...", file, ret.Count, list.Count);
                CompareFileInfo info = new CompareFileInfo();
                info.filename = file;
                info.LocalChangeDate = File.GetLastWriteTime(dir + "/" + file);
                foreach (string rname in rlist)
                    if (rname == file)
                    {
                        info.RemoteExist = true;
                        break;
                    }
                if (!info.RemoteExist)
                {
                    ret.Add(info);
                    continue;
                }
                info.RemoteChangeDate = hal.GetFileChangeDate(file);
                info.RemoteMD5 = hal.GetFileMD5(file);
                info.fullpath = dir + "/" + file;
                info.LocalMD5 = CSemphella.utils.GetMD5(File.ReadAllBytes(info.fullpath));
                
                if (info.RemoteMD5 != info.LocalMD5)
                {
                    node.Push("远程:" + info.RemoteMD5);
                    node.Push("本地:" + info.LocalMD5);
                    ret.Add(info);
                }
            }
            foreach (CompareFileInfo comp in ret)
            {

                action("正在更新现有文件....", comp.filename,count,ret.Count);
                string md5 =CSemphella.utils.GetMD5( File.ReadAllBytes(comp.fullpath));
                action("正在下载文件....", comp.filename,count,ret.Count);
                bool dstat = Download(comp.fullpath, comp.filename, comp.RemoteMD5, comp.RemoteChangeDate);
                if (!dstat)
                    return false;
                count++;
            }
            count = 0;
            foreach (string rpath in rlist)
            {
                if (InList(rpath))
                    continue;
                count++;
                if (rpath == "" || rpath == "." || rpath == "..")
                    continue;
                bool already_exist = false;
                foreach (CompareFileInfo comp in ret)
                {
                    if (comp.filename == rpath)
                    {
                        already_exist = true;
                        break;
                    }
                }
                if (already_exist || File.Exists(dir + "/" + rpath))
                    continue;
                action("正在下载缺失文件....", rpath, count, rlist.Count());
                bool dstat= Download(dir + "/" + rpath, rpath, hal.GetFileMD5(rpath), hal.GetFileChangeDate(rpath));
                if (!dstat)
                    return false;
            }
            action("更新版本标记...", null, 0, 1);
            File.WriteAllText(dir + "/version",Remote.ToString());
            action("完成", null, 1, 1);
            return true;
        }
    }
}
