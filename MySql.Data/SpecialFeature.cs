﻿using System;
using System.Linq;
using CSemphella;
using libSCS_WPFForm;
namespace MySql.Data.MySqlClient
{
#pragma warning disable CS1591 // 缺少对公共可见类型或成员的 XML 注释
    public static class DB
    {
        private static string host = "";
        private static string db = "";
        private static string password = "";
        private static string username = "";
        private static DebugSection DebugPush = new DebugSection("MySQL");
        private static UInt64 count = 0;

        public delegate bool SQLPreprocessEvent(string sql);
        public delegate void SQLExecuteEvent(bool result_event);

        private static SQLPreprocessEvent preprocess;
        private static SQLExecuteEvent execute;

        public static SQLPreprocessEvent PreProcess
        {
            set
            {
                preprocess += value;
            }
        }
        public static SQLExecuteEvent ExecuteEvent
        {
            set
            {
                execute += value;
            }
        }
        public static bool DebugSQL = false;
        public static string Host
        {
            set
            {
                host = value ?? throw new Exception("数据不能为NULL!");
            }
            private get
            {
                if (!utils.StringIsAllow(host))
                    throw new Exception("数据库配置出错");
                return host;
            }
        }

        public static string DataBase
        {
            set
            {
                db = value ?? throw new Exception("数据不能为NULL!");
            }
            private get
            {
                if (!utils.StringIsAllow(db))
                    throw new Exception("数据库配置出错");
                return db;
            }
        }

        public static string Password
        {
            set
            {
                password = value ?? throw new Exception("数据不能为NULL!");
            }
            private get
            {
                if (!utils.StringIsAllow(host))
                    throw new Exception("数据库配置出错");
                return password;
            }
        }

        public static string Username
        {
            set
            {
                username = value ?? throw new Exception("数据不能为NULL!");
            }
            private get
            {
                if (!utils.StringIsAllow(host))
                    throw new Exception("数据库配置出错");
                return username;
            }
        }

        public static MySqlDataReader RunSQL(string _sql)
        {
            return RunSQL(_sql, Host, DataBase, Password, Username);
        }

        public static MySqlDataReader RunSQL(string _sql, string phost, string pdb, string ppassword, string pusername)
        {
            return Exec(true, _sql, phost, db, ppassword, pusername);
        }

        public static void RunSQL_NoResult(string _sql)
        {
            RunSQL_NoResult(_sql, Host, DataBase, Password, Username);
        }

        public static void RunSQL_NoResult(string _sql, string phost, string pdb, string ppassword, string pusername)
        {
            Exec(false, _sql, phost, pdb, ppassword, pusername);
        }
        public static MySqlConnection ConnectSQLServer(string u, string db, string h, string p, int recall_level = 0)
        {
            DebugPush.Push("链接数据库:mysql://" + u + ":MASKED@" + h + "/" + db);
            MySqlConnection conn = new MySqlConnection("database=" + db + ";server=" + h + ";user id=" + u + ";password=" + p);
            try
            {
                conn.Open();
            }
            catch (Exception e)
            {
                if (recall_level > 3)
                    throw e;
                return ConnectSQLServer(u, db, h, p, recall_level++);
            }
            DebugPush.Push("数据库已连接!");
            return conn;
        }

        private static string PrefixSQL(string _sql)
        {
            string sql = _sql;
            if (sql.Last() != ';')
                sql += ";";
            return sql;
        }
        static string[] ForceUseNoResultExecute =
        {
            "INSERT INTO",
            "DELETE FROM",
            "UPDATE"
        };
        private static MySqlDataReader Exec(bool result, string _sql, string phost, string pdb, string ppassword, string pusername)
        {
            count++;
            string sql = PrefixSQL(_sql);
            if (!preprocess(sql))
                return null;
            DebugPush.Push("正在申请链接...");
            MySqlConnection conn = ConnectSQLServer(pusername, pdb, phost, ppassword);
            MySqlCommand cmd = new MySqlCommand(sql, conn);
            MySqlDataReader read = null;
            if (DebugSQL)
                DebugPush.Push("[" + result + "][" + count.ToString() + "]" + sql);
            if (result)
            {
                DebugPush.Push("执行安全检查并且运行....");
                foreach (string match in ForceUseNoResultExecute)
                    if (utils.PostVerifyNoBigLittle(sql, match))
                        throw new Exception("SQL必须由NoResult模式执行!\n" + _sql);
                read = cmd.ExecuteReader();
            }
            else
            {
                DebugPush.Push("执行无结果返回....");
                cmd.ExecuteNonQuery();
            }
            execute(result);
            return read;
        }

        public static UInt64 SqlNumberCollect(string db, string where)
        {
            MySqlDataReader read = RunSQL("SELECT count(*)number FROM " + db + " WHERE " + where);
            return read.GetUInt64("number");
        }
    }
}