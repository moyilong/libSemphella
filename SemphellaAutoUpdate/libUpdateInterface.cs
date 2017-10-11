using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CSemphella;

namespace SemphellaAutoUpdate
{
    public class DefaultPHPConfigure
    {
        string url,project;
        public DefaultPHPConfigure(string _url,string _project)
        {
            url = _url;
            project = _project;
        }
        public string GetFileMD5(string file)
        {
            return web.GetContentByURL(url + "?Project=" + project + "&&Command=GetFileMD5&&FileName=" + file);
        }
        public DateTime GetFileChangeDate(string file)
        {
            return DateTime.Parse(web.GetContentByURL(url + "?Project=" + project + "&&Command=GetFileChangeDate&&FileName=" + file));
        }
        public string[] GetFileList()
        {
            return web.GetContentByURL(url + "?Project=" + project + "&&Command=FetFileList").Split(';');
        }
        public byte[] GetFileByte(string file)
        {
            return web.GetContentByURL_Byte(url + "?Project=" + project + "&&Command=GetFileChangeDate&&FileName=" + file);
        }
    }

    public interface UpdateServerHAL
    {
        string GetFileMD5(string file);
        DateTime GetFileChangeDate(string file);
        string[] GetFileList();
        byte[] GetFileByte(string file);

    }
}
