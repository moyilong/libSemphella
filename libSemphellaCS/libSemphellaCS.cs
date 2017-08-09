using System;

namespace libSemphellaCS
{
    public class libSemphellaCS
    {
        public struct Version
        {
            public int api;
            public int release;
            public int fix;
            public string arch_name;
            public string StringOut
            {
                get
                {
                    return arch_name + " " + api.ToString() + "." + release.ToString() + "." + fix.ToString();
                }
            }
        }
        public static Version CurrentVersion
        {
            get
            {
                Version ret = new Version();
                ret.api = 0xF0;
                ret.release = 0x00;
                ret.fix = 0x00;
                ret.arch_name = "libSemphella CSharp Prelease";
                return ret;
            }
        }

        static int []compactable_api_list = {0xF0};
        static public bool CheckCompact(int api)
        {
            foreach (int compact in compactable_api_list)
                if (compact == api)
                    return true;
            return false;
        }

    }
}
