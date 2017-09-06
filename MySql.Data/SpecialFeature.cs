﻿using System;
using System.Linq;
using CSemphella;
namespace MySql.Data.MySqlClient
{
    public static class DB
    {
        private static string host="";
        private static string db="";
        private static string password="";
        private static string username="";

        public static MySqlDataReader RunSQL(string _sql)
        {
            return RunSQL(_sql, host, db, password, username);
        }

        public static MySqlDataReader RunSQL(string _sql, string phost, string pdb, string ppassword, string pusername)
        {
            return Exec(true, _sql, phost, db, ppassword, pusername);
        }

        public static void RunSQL_NoResult(string _sql)
        {
            RunSQL_NoResult(_sql, host, db, password, username);
        }

        public static void RunSQL_NoResult(string _sql, string phost, string pdb, string ppassword, string pusername)
        {
            Exec(false, _sql, phost, pdb, ppassword, pusername);
        }

        public static MySqlConnection ConnectSQLServer(string u, string db, string h, string p, int recall_level = 0)
        {
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
            string sql = PrefixSQL(_sql);
            MySqlConnection conn = ConnectSQLServer(pusername, pdb, phost, ppassword);
            MySqlCommand cmd = new MySqlCommand(sql, conn);
            if (result)
            {
                foreach (string match in ForceUseNoResultExecute)
                {
                    if (!utils.PostVerifyNoBigLittle(sql, match))
                        throw new Exception("THIS_SQL_MUST_USE_NO_QUERY");
                }
                return cmd.ExecuteReader();
            }
            cmd.ExecuteNonQuery();
            return null;
        }

        public static UInt64 SqlNumberCollect(string db,string where)
        {
            MySqlDataReader read = RunSQL("SELECT count(*)number FROM " + db + " WHERE " + where);
            return read.GetUInt64("number");
        }
    }
}