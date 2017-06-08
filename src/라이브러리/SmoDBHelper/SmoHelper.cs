using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.SqlServer.Management.Smo;
using Microsoft.SqlServer.Management.Common;
using System.Data.SqlClient;
using System.Collections.Specialized;
using System.Diagnostics;

namespace SmoDBHelper
{
    public struct stKeyValue
    {
        public string strKey;
        public string strValue;
    }

    public class SmoHelper
    {
        private string strConn; //DataBase Connection String
        private Server mSqlServer;

        public SmoHelper()
        {

        }

        public SmoHelper(string str)
        {
            try
            {
                strConn = str;
                mSqlServer = new Server(GetServerConn(str));
            }
            catch
            {

            }
        }

        public SmoHelper(string str, int type)
        {
            try
            {
                strConn = str;
                mSqlServer = new Server(GetServerConn(str, type));
            }
            catch
            {

            }
        }

        private ServerConnection GetServerConn(string str)
        {
            string strConnString = SmoMaker.GetConnectionString(str);
            SqlConnection sql_conn = new SqlConnection(strConnString);
            ServerConnection server_conn = new ServerConnection(sql_conn);
            
            return server_conn;
        }

        private ServerConnection GetServerConn(string str, int type)
        {
            string strConnString = SmoMaker.GetConnectionString(str, type);
            SqlConnection sql_conn = new SqlConnection(strConnString);
            ServerConnection server_conn = new ServerConnection(sql_conn);

            return server_conn;
        }

        public bool Connect()
        {
            bool ret_value = true;
            try
            {
                mSqlServer.ConnectionContext.Connect();
            }
            catch
            {
                ret_value = false;
                throw new Exception(string.Format("SMO CONN ERR(connstr:{0})", strConn));
            }
            finally
            {

            }

            return ret_value;
        }

        public void DisConnect()
        {
            if (mSqlServer.ConnectionContext.IsOpen)
                mSqlServer.ConnectionContext.Disconnect();
        }

        public bool IsConnect()
        {
            return mSqlServer.ConnectionContext.IsOpen;
        }

        public string GetTableQuery(string strDBName)
        {
            string strValue = string.Empty;
            Database db = mSqlServer.Databases[strDBName];
            ScriptingOptions so = new ScriptingOptions();
            so.DriAll = true;

            StringCollection cs = db.Script(so);

            foreach (string str in cs)
            {
                strValue = strValue + str + " ";
            }

            return strValue;
        }

        public string GetTableQuery(string strDBName, string strTable, string strSchema, bool UsageKey)
        {
            StringBuilder strValue = new StringBuilder();
            Database db = mSqlServer.Databases[strDBName];
            Table tb = db.Tables[strTable, strSchema];
            ScriptingOptions so = new ScriptingOptions();
            so.DriPrimaryKey = UsageKey;
            so.DriIndexes = UsageKey;
            so.DriUniqueKeys = UsageKey;
            so.NoIdentities = true; // 20150127 SPOA 파일 생성할시 IDENTITY 제거

            StringCollection cs = tb.Script(so);

            foreach (string str in cs)
            {
                strValue.Append(str);
                strValue.Append(" ");
            }

            return strValue.ToString();
        }

        public string GetViewQuery(string strDBName, string strSchema, string strView)
        {
            StringBuilder strValue = new StringBuilder();
            Database db = mSqlServer.Databases[strDBName];

            View vb = db.Views[strView, strSchema];

            ScriptingOptions so = new ScriptingOptions();
            so.ScriptSchema = true;
            so.ScriptDrops = false;
            so.ScriptData = false;
            so.IncludeIfNotExists = false;
            so.AnsiPadding = false;
// 
//             so.IncludeHeaders = true;
//             so.Permissions = true;
//             so.ExtendedProperties = true;
//             so.ScriptDrops = true;
//             so.IncludeDatabaseContext = true;
//             so.NoCollation = true;
//             so.NoFileGroup = true;
//             so.NoIdentities = true;

            StringCollection cs = vb.Script(so);
            foreach (string str in cs)
            {
                strValue.Append(str);
                //strValue.Append(" ");
                strValue.Append("\r\nGO\r\n");
            }



            return strValue.ToString();
        }

        public stKeyValue[] GetIndexColumnNames(string strDBName, string strSchema, string strTable)
        {
            stKeyValue[] strValues = null;

            try
            {
                Database db = mSqlServer.Databases[strDBName];
                Table tb = db.Tables[strTable, strSchema];
                IndexCollection idxs = tb.Indexes;

                if (idxs.Count > 0)
                {
                    /*Index idx = Idxs[0];*/
                    Index idx = idxs[0];

                    strValues = new stKeyValue[idx.IndexedColumns.Count];
                    int nIndex = 0;
                    foreach (IndexedColumn col in idx.IndexedColumns)
                    {
                        strValues[nIndex].strKey = col.Name;
                        strValues[nIndex].strValue = "";
                        nIndex++;
                    }
                }
            }
            catch
            {
                throw new Exception(string.Format("{0}.{1}.{2} 테이블의 존재하는지 확인하세요", strDBName, strSchema, strTable));
            }

            return strValues;
        }

        public bool IsView(string strDBName, string strSchema, string strTable)
        {
            Database db = mSqlServer.Databases[strDBName];
            return db.Views.Contains(strTable, strSchema);
        }

        public Table GetTable(string strDBName, string strTable, string strSchema)
        {
            Database db = mSqlServer.Databases[strDBName];
            Table tb = db.Tables[strTable, strSchema];
            return tb;
        }

        public Database GetDataBase(string strDBName)
        {
            Database db = mSqlServer.Databases[strDBName];
            return db;
        }

        public string GetTableQuery(string strDBName, string strTable, string strSchema)
        {
            string strValue = string.Empty;
            Database db = mSqlServer.Databases[strDBName];
            Table tb = db.Tables[strTable, strSchema];
            ScriptingOptions so = new ScriptingOptions();
            so.DriPrimaryKey = true;
            so.DriIndexes = true;
            so.DriUniqueKeys = true;
            so.NoIdentities = true; // 20150127 SPOA 파일 생성할시 IDENTITY 제거

            StringCollection cs = tb.Script(so);

            foreach (string str in cs)
            {
                strValue = strValue + str + " ";
            }

            return strValue;
        }

        public TableCollection GetDataTables(string strDBName)
        {
            return mSqlServer.Databases[strDBName].Tables;
        }

        public SchemaCollection GetDataSchema(string strDBName)
        {
            return mSqlServer.Databases[strDBName].Schemas;
        }

        public StoredProcedureCollection GetDataProcedure(string strDBName)
        {
            return mSqlServer.Databases[strDBName].StoredProcedures;
        }

        public DatabaseCollection GetAllDataBase()
        {
            return mSqlServer.Databases;
        }

        public bool CreateDataBase(string strDBName)
        {
            bool nRetVal = false;
            Database db1 = mSqlServer.Databases[strDBName];
            try
            {
                if (db1 == null)
                {
                    Database db = new Database(mSqlServer, strDBName);
                    db.Create();
                    nRetVal = true;
                }
                else
                {
                    nRetVal = false;
                }
            }
            catch
            {
                nRetVal = false;
            }

            return nRetVal;
        }

        public void CreateSchema(string strDBName, string strSchemaName)
        {
            if (mSqlServer.Databases[strDBName] != null)
            {
                Database db = mSqlServer.Databases[strDBName];
                if (db.Schemas[strSchemaName] == null)
                {
                    Schema sc = new Schema(db, strSchemaName);
                    sc.Create();
                }
            }
        }

        public void CreateTable(string strDBName, string strQuery)
        {
            try
            {
                Database db = new Database(mSqlServer, strDBName);

                db.ExecuteNonQuery(strQuery);
            }
            catch (FailedOperationException e)
            {
                SqlException innerException = (SqlException)e.InnerException;

                foreach (SqlError sqlerr in innerException.Errors)
                {
                    Debug.WriteLine(string.Format("Line number: {0} \n {1}", sqlerr.LineNumber, sqlerr.Message));
                }
            }
            catch (System.Exception ex)
            {
                Debug.WriteLine(string.Format("ex : {0}", ex.Message));
            }
        }

        public void RunQuery(string strDBName, string strQuery)
        {
            Database db = new Database(mSqlServer, strDBName);

            db.ExecuteNonQuery(strQuery);
        }

        public bool RunMultiQuery(string strDBName, string strQuery)
        {
            bool bRet = false;

            try
            {
                if (Connect())
                {
                    Database db = new Database(mSqlServer, strDBName);
                    db.ExecuteNonQuery(strQuery);
                    DisConnect();
                    bRet = true;
                }
            }
            catch (Exception ex)
            {
                DisConnect();
                throw;
            }

            return bRet;
        }

        
        public static stKeyValue[] GetTableKeyValue(string strConnName, string strDBName, string strSchema, string strTable)
        {
            //string strConnName = KpoaDataMana.GetDBConnName(strNodeCode, strDBName, nDBType);
            stKeyValue[] keyVal = null;
            SmoHelper smo = new SmoHelper(strConnName);
            try
            {
                smo.Connect();
            }
            catch (Exception ex)
            {
//                 CLogHelper.LogWrite(CLogHelper.eLogType.nSystemLog, CLogHelper.eLogLevel.LOG_ERROR
//                     , string.Format("[GET KEY] ({0}{1}) 사업소 SMO 연결중 예외 발생 (ex:{2})", strNodeCode, strDBName, ex.Message));
                smo.DisConnect();
                smo = null;
                return null;
            }

            try
            {
                keyVal = smo.GetIndexColumnNames(strDBName, strSchema, strTable);
            }
            catch (System.Exception ex)
            {
//                 CLogHelper.LogWrite(CLogHelper.eLogType.nSystemLog, CLogHelper.eLogLevel.LOG_ERROR
//                     , string.Format("[GET KEY] ({0}{1}) 사업소 SMO Key 데이터 추출 중 예외 발생 (ex:{2})", strNodeCode, strDBName, ex.Message));
            }

            smo.DisConnect();
            smo = null;
            return keyVal;
        }

    }
}
