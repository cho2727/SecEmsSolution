using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Collections;
using System.Configuration;
using System.Data;

using System.Data.SqlClient;
using System.IO;
using System.Xml;

namespace OleDBHelper
{
    public class DBWrapper
    {        
        public string strConn;      // DB 연결 스트링
        public string strConnName;  // DB 연결 스트링(Name)

        public string _ErrMsg;

        private string _ErrExecMsg;

        /// <summary>
        /// ErrMsg
        /// </summary>
        public string ErrMsg
        {
            get { return _ErrMsg; }
        }

        public string ErrExecMsg
        {
            get { return _ErrExecMsg; }
        }

        /// <summary>
        /// 생성자 Config에 해당 DatabaseConnectionString 이 없으면..예외 발생
        /// </summary>
        public DBWrapper()
        {
            strConnName = "DatabaseConnectionString";
            strConn = GetConnectionString("DatabaseConnectionString");
        }

        /// <summary>
        /// 생성자
        /// </summary>
        /// <param name="str"></param>
        public DBWrapper(string str)
        {
            try
            {
                strConnName = str;
                strConn = GetConnectionString(str);
            }
            catch 
            {
                strConn = string.Empty;
            }
        }

        public DBWrapper(string str, int type)
        {
            try
            {
                strConnName = str;
                strConn = GetConnectionString(str, type);
            }
            catch
            {
                strConn = string.Empty;
            }
        }

        public bool IS_DBConnection()
        {
            AdoDataAdapter db = new AdoDataAdapter(strConn);
            return db.IS_DBConnection();
        }

        public static string GetDataBaseName(string key)
        {
            ConfigXmlDocument doc = new ConfigXmlDocument();
            string strFileName = @System.AppDomain.CurrentDomain.BaseDirectory + "config" + "\\" + "db_config.xml";
            string strDBName = string.Empty;

            try
            {
                if (File.Exists(strFileName))
                {
                    doc.Load(strFileName);
                    XmlNodeList pNodeList = doc.SelectNodes("//upc_config/database");
                    if (pNodeList.Count > 0)
                    {
                        foreach (XmlNode pNode in pNodeList)
                        {
                            if (pNode.Attributes["type"].Value == key)
                            {
                                XmlNode pChildNode = pNode.SelectSingleNode("name");
                                strDBName = pChildNode.InnerText;
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {

            }

            return strDBName;
        }
 
#if false
        /// <summary>
        /// 테이블 반환(쿼리문 사용) 이 함수는 반드시 try ... catch문 안에서 호출해야함
        /// </summary>
        /// <param name="spName"></param>
        /// <param name="Param"></param>
        /// <param name="Value"></param>
        /// <returns></returns>
        public DataTable SelectQuery(string query)
        {
            AdoDataAdapter db = new AdoDataAdapter(strConn/*, strConn2*/);
            DataTable dt = new DataTable();

            db.SetQuery(query);

            try
            {
                DataSet ds = db.RunQuerySelect();
                if (ds != null && ds.Tables.Count > 0)
                    dt = ds.Tables[0];
                else
                {
                    throw (new Exception("NOT TABLE"));
                }
            }
            catch (Exception ex)
            {
                throw new Exception("mapper error", ex);
            }

            if (db.Ret == -1)
            {
                _ErrMsg = db.Msg;
                _ErrExecMsg = db.execMsg;
                throw (new Exception(db.Msg + "/" + db.execMsg));
            }

            return dt;
        }
#else
        /// <summary>
        /// 테이블 반환(쿼리문 사용) 이 함수는 반드시 try ... catch문 안에서 호출해야함
        /// </summary>
        /// <param name="spName"></param>
        /// <param name="Param"></param>
        /// <param name="Value"></param>
        /// <returns></returns>
        public DataTable SelectQuery(string query)
        {
            SqlData sql = new SqlData(query, "ReturnTable");
            AdoDataAdapter db = new AdoDataAdapter(strConn);
            DataTable dt = new DataTable();

            db.Add(sql);

            try
            {
                dt = db.RunSelect().Tables["ReturnTable"];
            }
            catch (Exception ex)
            {
                if (db.Ret != -1)
                    throw new Exception("mapper error", ex);
            }

            if (db.Ret == -1)
            {
                _ErrMsg = db.Msg;
                _ErrExecMsg = db.execMsg;
                throw (new Exception(strConnName + "/" + db.Msg + "/" + db.execMsg));
            }

            return dt;
        }

#endif

        /// <summary>
        /// 데이터베이스 업데이트(다중 쿼리 가능)
        /// </summary>
        /// <param name="sql"></param>
        /// <returns></returns>
        public bool RunUpdate(ArrayList sql)
        {
            AdoDataAdapter db = new AdoDataAdapter(strConn, sql);
            
            try
            {
                db.RunUpdate();
            }
            catch (Exception ex)
            {
                if (db.Ret != -1)
                    throw new Exception("mapper error", ex);
            }

            if (db.Ret == -1)
            {
                _ErrMsg = db.Msg;
                _ErrExecMsg = db.execMsg;
                throw (new Exception(strConnName + "/" + db.Msg + "/" + db.execMsg));
            }

            return true;
        }

        /// <summary>
        /// 데이터 베이스 업데이트(단일 쿼리문)
        /// </summary>
        /// <param name="query"></param>
        /// <returns></returns>
        public bool RunUpdate(string query)
        {
            SqlData sql = new SqlData(query, "ReturnTable");
            AdoDataAdapter db = new AdoDataAdapter(strConn);

            db.Add(sql);

            try
            {                
                db.RunUpdate();
            }
            catch (Exception ex)
            {
                if (db.Ret != -1)
                    throw new Exception("mapper error", ex);
            }

            if (db.Ret == -1)
            {
                _ErrMsg = db.Msg;
                _ErrExecMsg = db.execMsg;
                throw (new Exception(strConnName + "/" + db.Msg + "/" + db.execMsg));
            }
            return true;
        }

        /// <summary>
        /// PROC DATA 수행
        /// </summary>
        /// <param name="pd"></param>
        public void RunUpdate(SqlProcData pd)
        {
            AdoDataAdapter db = new AdoDataAdapter(strConn);

            db.Add(pd);

            try
            {
                db.RunUpdate();
            }
            catch (Exception ex)
            {
                if (db.Ret != -1)
                    throw new Exception("mapper error", ex);
            }

            if (db.Ret == -1)
            {
                _ErrMsg = db.Msg;
                _ErrExecMsg = db.execMsg;
                throw (new Exception(strConnName + "/" + db.Msg + "/" + db.execMsg));
            }	
        }

        /// <summary>
        /// Update Mapper
        /// </summary>
        /// <param name="spName"></param>
        /// <param name="Param"></param>
        /// <param name="Value"></param>
        public void RunQuery(string spName, string[] Param, string[] Value)
        {
            SqlProcData pd = new SqlProcData(spName, "ReturnTable");
            AdoDataAdapter db = new AdoDataAdapter(strConn);

            if (Param.Length != Value.Length)
                throw (new Exception("Array ERROR"));

            for (int i = 0; i < Param.Length; i++)
            {
                pd.AddParam(Param[i], Value[i]);
            }

            db.Add(pd);

            try
            {
                db.RunUpdate();
            }
            catch (Exception ex)
            {
                if (db.Ret != -1)
                    throw new Exception("mapper error", ex);
            }

            if (db.Ret == -1)
            {
                _ErrMsg = db.Msg;
                _ErrExecMsg = db.execMsg;
                throw (new Exception(strConnName + "/" + db.Msg + "/" + db.execMsg));
            }	
        }

        /// <summary>
        /// 프로시져 SEL 호출문
        /// </summary>
        /// <param name="spName"></param>
        /// <param name="Param"></param>
        /// <param name="Value"></param>
        /// <returns></returns>
        public DataTable SelectQuery(string spName, string[] Param, string[] Value)
        {
            SqlProcData dp = new SqlProcData(spName, "ReturnTable");
            AdoDataAdapter db = new AdoDataAdapter(strConn);
            DataTable dt = new DataTable();

            if (Param.Length != Value.Length)
                throw (new Exception("Param and Value Array Diff"));

            for (int i = 0; i < Param.Length; i++)
            {
                dp.AddParam(Param[i], Value[i]);
            }

            db.Add(dp);


            try
            {
                dt = db.RunSelect().Tables["ReturnTable"];
            }
            catch (Exception ex)
            {
                if (db.Ret != -1)
                    throw new Exception("mapper error", ex);
            }

            if (db.Ret == -1)
            {
                _ErrMsg = db.Msg;
                _ErrExecMsg = db.execMsg;
                throw (new Exception(strConnName + "/" + db.Msg + "/" + db.execMsg));
            }	

            return dt;
        }
        
        /// <summary>
        /// ConnectionString 셋팅
        /// </summary>
        /// <param name="str"></param>
        public void SetConnString(string str)
        {
            strConn = str;
        }


        //////////////////////////////////////////////-- STATIC 함수 --//////////////////////////////////////////
        /// <summary>
        /// Config 파일에서 ConnectionString를 얻어옴
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        public static string GetConnectionString(string key)
        {
            string strCurrentDir = @System.AppDomain.CurrentDomain.BaseDirectory;
            ExeConfigurationFileMap configFile = new ExeConfigurationFileMap();
            configFile.ExeConfigFilename = @strCurrentDir + @"\Config\Config.xml";
            Configuration config = ConfigurationManager.OpenMappedExeConfiguration(configFile, ConfigurationUserLevel.None);

            return config.ConnectionStrings.ConnectionStrings[key].ConnectionString;
        }

        public static string GetDatabaseName(string key)
        {
            string dbName = string.Empty;
            ConfigXmlDocument doc = new ConfigXmlDocument();
            string strFileName = @System.AppDomain.CurrentDomain.BaseDirectory + "config" + "\\" + "db_config.xml";

            try
            {
                if (File.Exists(strFileName))
                {
                    doc.Load(strFileName);
                    XmlNodeList pNodeList = doc.SelectNodes("//upc_config/database");
                    if (pNodeList.Count > 0)
                    {
                        foreach (XmlNode pNode in pNodeList)
                        {
                            if (pNode.Attributes["type"].Value == key)
                            {
                                XmlNode pChildNode = pNode.SelectSingleNode("source");
                                pChildNode = pNode.SelectSingleNode("name");
                                dbName = pChildNode.InnerText;
                                break;
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {

            }

            return dbName;
        }

        public static string GetConnectionString(string key, int type)
        {
            ConfigXmlDocument doc = new ConfigXmlDocument();
            string strFileName = @System.AppDomain.CurrentDomain.BaseDirectory + "config" + "\\" + "db_config.xml";
            //connectionString="Data Source=122.129.249.141,51433;Initial Catalog=center_das;User ID=sa;Password=;Failover Partner=122.129.249.141,51433;" providerName="System.Data.SqlClient"
            StringBuilder sbConnect = new StringBuilder();;

            try
            {
                if (File.Exists(strFileName))
                {
                    doc.Load(strFileName);
                    XmlNodeList pNodeList = doc.SelectNodes("//upc_config/database");
                    if (pNodeList.Count > 0)
                    {
                        foreach (XmlNode pNode in pNodeList)
                        {
                            if (pNode.Attributes["type"].Value == key)
                            {
                                XmlNode pChildNode = pNode.SelectSingleNode("source");
                                sbConnect.Append("Data Source=");
                                sbConnect.Append(pChildNode.InnerText);
                                sbConnect.Append(";");

                                pChildNode = pNode.SelectSingleNode("name");
                                sbConnect.Append("Initial Catalog=");
                                sbConnect.Append(pChildNode.InnerText);                                
                                sbConnect.Append(";");

                                pChildNode = pNode.SelectSingleNode("user");
                                sbConnect.Append("User ID=");
                                sbConnect.Append(pChildNode.InnerText);
                                sbConnect.Append(";");

                                pChildNode = pNode.SelectSingleNode("password");
                                sbConnect.Append("Password=");
                                sbConnect.Append(pChildNode.InnerText);
                                sbConnect.Append(";");

                                pChildNode = pNode.SelectSingleNode("failover");
                                sbConnect.Append("Failover Partner=");
                                sbConnect.Append(pChildNode.InnerText);
                                sbConnect.Append(";");

                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                
            }

            return sbConnect.ToString();
        }

        /// <summary>
        /// 문자열 쿼리 RUN
        /// </summary>
        /// <param name="strConnString"></param>
        /// <param name="strQuery"></param>
        /// <returns></returns>
        public static bool RunQuery(string strConnString, string strQuery)
        {
            DBWrapper db_wrapper = new DBWrapper(strConnString);

            try
            {
                db_wrapper.RunUpdate(strQuery);
            }
            catch
            {
                return false;
            }
            return true;
        }

        /// <summary>
        /// ArrayList 쿼리 RUN
        /// </summary>
        /// <param name="strConnString"></param>
        /// <param name="lstQuery"></param>
        /// <returns></returns>
        public static bool RunQuery(string strConnString, ArrayList lstQuery)
        {
            DBWrapper db_wrapper = new DBWrapper(strConnString);

            try
            {
                db_wrapper.RunUpdate(lstQuery);
            }
            catch
            {
                return false;
            }
            return true;
        }

        /// <summary>
        /// 문자열 쿼리 SELECT
        /// </summary>
        /// <param name="strConnString"></param>
        /// <param name="strQuery"></param>
        /// <returns></returns>
        public static DataTable SelectQuery(string strConnString, string strQuery)
        {
            DataTable dt = null;
            DBWrapper db_wrapper = new DBWrapper(strConnString);

            // 반드시 상위에서 try ... catch()를 해주어야함...
            dt = db_wrapper.SelectQuery(strQuery);

            return dt;
        }

        /// <summary>
        /// 문자열 쿼리(프로시져) RUN
        /// </summary>
        /// <param name="spName"></param>
        /// <param name="Param"></param>
        /// <param name="Value"></param>
        public static bool RunQuery(string strConnString, string spName, string[] Param, string[] Value)
        {
            DBWrapper db_wrapper = new DBWrapper(strConnString);
            bool retValue = false;
            try
            {
                db_wrapper.RunQuery(spName, Param, Value);
                retValue = true;
            }
            catch
            {
                try
                {
                    db_wrapper.RunQuery(spName, Param, Value);
                    retValue = true;
                }
                catch
                {

                }
            }
            return retValue;
        }

        /// <summary>
        /// 프로시져문 실행(한건)
        /// </summary>
        /// <param name="strConnString"></param>
        /// <param name="spName"></param>
        /// <param name="Param"></param>
        /// <param name="Value"></param>
        /// <returns></returns>
        public static DataTable SelectQuery(string strConnString, string spName, string[] Param, string[] Value)
        {
            DBWrapper db_wrapper = new DBWrapper(strConnString);
            DataTable dt = null;
            try
            {
                dt = db_wrapper.SelectQuery(spName, Param, Value);
            }
            catch(Exception ex)
            {
                throw ex;
            }

            return dt;
        }


        public static bool IsTableExist(string strConnString, string strDBName, string strSchemaName, string strTableName)
        {
            bool bRet = false;

            string strQuery = string.Format("SELECT count(*) as cnt FROM [{0}].[INFORMATION_SCHEMA].[TABLES] "
                + "WHERE TABLE_CATALOG = '{0}' and TABLE_SCHEMA = '{1}' and TABLE_NAME = '{2}';"
                , strDBName, strSchemaName, strTableName);
            try
            {
                DataTable dt = null;
                DBWrapper bizMapper = new DBWrapper(strConnString);
                dt = bizMapper.SelectQuery(strQuery);

                if (dt != null && dt.Rows.Count > 0)
                {
                    int iCount = Convert.ToInt32(dt.Rows[0]["cnt"].ToString());
                    if (iCount >= 1)
                        bRet = true;
                }
            }
            catch (Exception ex)
            {

            }

            return bRet;
        }
    }

#if false
    public class SmoBlukCopy
    {
        private string strConn; //DataBase Connection String

        private SqlBulkCopy mBluckCopy;
        public SmoBlukCopy(string szConn, string szDBName)
        {
            string strConnString = string.Empty;
            strConnString = DBWrapper.GetConnectionString(szConn);
            strConn = string.Format("{0};{1}{2};{3};{4};"
                , strConnString.Split(';')[0]
                , "Initial Catalog="
                , szDBName
                , strConnString.Split(';')[2]
                , strConnString.Split(';')[3]);
            //SqlConnection sql_conn = new SqlConnection(strConn);

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
                bRetValue = false;
            }

            return bRetValue;
        }

        public void Close()
        {
            mBluckCopy.Close();
        }
    }
#endif
}
