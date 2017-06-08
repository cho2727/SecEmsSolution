using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data.SqlClient;
using System.Data;

namespace SmoDBHelper
{
    public class SmoBlukCopy
    {
        private string strConn; //DataBase Connection String

        private SqlBulkCopy mBluckCopy;
        public SmoBlukCopy(string szConn, string szDBName)
        {
            string strConnString = string.Empty;
            strConnString = SmoMaker.GetConnectionString(szConn);
            strConn = string.Format("{0};{1}{2};{3};{4};"
                , strConnString.Split(';')[0]
                , "Initial Catalog="
                , szDBName
                , strConnString.Split(';')[2]
                , strConnString.Split(';')[3]);
            //SqlConnection sql_conn = new SqlConnection(strConn);

            mBluckCopy = new SqlBulkCopy(strConn);
        }

        public SmoBlukCopy(string szConn)
        {
            strConn = SmoMaker.GetConnectionString(szConn);
            mBluckCopy = new SqlBulkCopy(strConn);
        }

        public bool RunBlockCopy(DataTable dt, string strTableName, int nTimeOut)
        {
            bool bRetValue = true;
            mBluckCopy.DestinationTableName = strTableName;
            mBluckCopy.BulkCopyTimeout = nTimeOut;

            try
            {
                mBluckCopy.WriteToServer(dt);
            }
            catch
            {
                throw;
            }

            return bRetValue;
        }

        public bool RunBlockCopy(DataTable dt, string strTableName, int nTimeOut, DataRowState rowState)
        {
            bool bRetValue = true;
            mBluckCopy.DestinationTableName = strTableName;
            mBluckCopy.BulkCopyTimeout = nTimeOut;

            try
            {
                mBluckCopy.WriteToServer(dt, rowState);
            }
            catch
            {
                throw;
            }

            return bRetValue;
        }

        public void Close()
        {
            mBluckCopy.Close();
        }
    }
}
