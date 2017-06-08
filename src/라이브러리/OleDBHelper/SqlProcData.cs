using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Data.SqlClient;
using System.Collections;

namespace OleDBHelper
{
    public class SqlProcData
    {
        private string procName;
        private string tblName;
        private Hashtable paramList = new Hashtable();
        private bool hasMaster = false;
        private string masterTable;
        private Hashtable masterKeyList = new Hashtable();
        private SqlCommand cmd = null;

        /// <summary>
        /// DAS-SLS를 하기위해(사업소 코드를 집어넣음)
        /// </summary>
        private int nOfficeCode;
        public int OfficeCode
        {
            get { return nOfficeCode; }
            set { nOfficeCode = value; }
        }

        public SqlProcData()
        { }

        /// <summary>
        /// SqlProcData
        /// </summary>
        /// <param name="procName"></param>
        /// <param name="tblName"></param>
        public SqlProcData(string procName, string tblName)
        {
            Set(procName, tblName);
        }

        /// <summary>
        /// Cmd
        /// </summary>
        public SqlCommand Cmd
        {
            set { this.cmd = value; }
            get { return this.cmd; }
        }

        /// <summary>
        /// HasMaster
        /// </summary>
        public bool HasMaster
        {
            get { return hasMaster; }
        }

        /// <summary>
        /// MasterTable
        /// </summary>
        public string MasterTable
        {
            set
            {
                hasMaster = true;
                this.masterTable = value;
            }
            get { return masterTable; }
        }

        /// <summary>
        /// ProcName
        /// </summary>
        public string ProcName
        {
            set { procName = value; }
            get { return procName; }
        }

        /// <summary>
        /// TblName
        /// </summary>
        public string TblName
        {
            set { tblName = value; }
            get { return tblName; }
        }

        /// <summary>
        /// Set
        /// </summary>
        /// <param name="procName"></param>
        /// <param name="tblName"></param>
        private void Set(string procName, string tblName)
        {
            this.procName = procName;
            this.tblName = tblName;
        }

        /// <summary>
        /// ClearParam
        /// </summary>
        /// <returns></returns>
        public bool ClearParam()
        {
            paramList.Clear();
            return true;
        }

        /// <summary>
        /// AddParam
        /// </summary>
        /// <param name="key"></param>
        /// <param name="val"></param>
        /// <returns></returns>
        public bool AddParam(string key, object val)
        {
            if (paramList.Contains(key))
            {
                return false;
            }
            else
            {
                paramList.Add(key, val);
                return true;
            }
        }

        /// <summary>
        /// AddMasterKey
        /// </summary>
        /// <param name="key"></param>
        /// <param name="val"></param>
        /// <returns></returns>
        public bool AddMasterKey(string key, string val)
        {
            if (masterKeyList.Contains(key))
            {
                return false;
            }
            else
            {
                masterKeyList.Add(key, val);
                return true;
            }
        }

        /// <summary>
        /// GetMasterKeyList
        /// </summary>
        /// <returns></returns>
        public IEnumerator GetMasterKeyList()
        {
            return masterKeyList.Keys.GetEnumerator();
        }


        public string GetMasterKey(string key)
        {
            if (!masterKeyList.Contains(key))
                return null;
            else
                return (string)masterKeyList[key];
        }

        /// <summary>
        /// checkData
        /// </summary>
        /// <returns></returns>
        public bool checkData()
        {
            if (procName == null || procName.Trim().Equals("") ||
                tblName == null || tblName.Trim().Equals(""))
                return false;
            else
                return true;
        }

        /// <summary>
        /// GetParam
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        public object GetParam(string key)
        {
            if (!paramList.Contains(key))
                return null;
            else
                return paramList[key];
        }

        /// <summary>
        /// ToString
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append("exec ").Append(procName).Append(" ");
            if (HasMaster)
            {
                IEnumerator iem = GetMasterKeyList();
                while (iem.MoveNext())
                {
                    string mkey = (string)iem.Current;
                    string mval = "'동적바인딩'";
                    sb.Append(mkey).Append("=").Append(mval).Append(",");
                }
            }
            IEnumerator iep = paramList.Keys.GetEnumerator();
            while (iep.MoveNext())
            {
                string mkey = (string)iep.Current;
                string mval = "";
                if (paramList[mkey] != null)
                {
                    Type a = paramList[mkey].GetType();
                    if (a.Name == "String")
                        mval = "'" + (string)paramList[mkey] + "'";
                    else
                        mval = paramList[mkey].ToString();
                }
                else
                {
                    mval = "null";
                }
                sb.Append(mkey).Append("=").Append(mval).Append(",");
            }

            return sb.ToString(0, sb.Length - 1);
        }
    }
}
