using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace OleDBHelper
{
    public class SqlData
    {

        private string sql;
        private string tblName;

        public string Sql
        {
            get { return sql; }
        }
        public string TblName
        {
            get { return tblName; }
        }

        public SqlData()
        { }

        public SqlData(string sql, string tblName)
        {
            Set(sql, tblName);
        }

        private void Set(string sql, string tblName)
        {
            this.sql = sql;
            this.tblName = tblName;
        }
        public bool checkData()
        {
            if (sql == null || sql.Trim().Equals("") ||
                tblName == null || tblName.Trim().Equals(""))
                return false;
            else
                return true;
        }
    }
}
