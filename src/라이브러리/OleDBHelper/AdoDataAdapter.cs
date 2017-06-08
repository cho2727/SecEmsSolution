using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;

using System.Data.SqlClient;
using System.Collections;


//public DataSet RunListSelect() // List로 추가하여 여러개 수행
//public DataSet RunQuerySelect() // string 직접 실행
//public DataSet RunUpdateSingle(SqlProcData pdata) // 단건을 직접 실행

namespace OleDBHelper
{
    public class AdoDataAdapter
    {
        
        private int DBTimeOut = 1200;

        private string connectionString;
        private string strQuery;        
        private ArrayList dataList;// = new ArrayList();

        private int ret = 0;                      // 리턴값
        private string msg = "";                  // 메시지
        private string outputvalue = "";        // 메시지
        private string execmsg = "";           // 출력문자열


        public int Ret
        {
            get { return ret; }
        }

        public string OutputValue
        {
            get { return outputvalue; }
        }

        public string Msg
        {
            get { return msg; }
        }

        public AdoDataAdapter()
        {
            //connectionString = GetConnectionString("DatabaseConnectionString");
        }

        public AdoDataAdapter(string ConnectionString)
        {
            connectionString = ConnectionString;
            dataList = new ArrayList();
        }

        public AdoDataAdapter(string ConnectionString, ArrayList array)
        {
            connectionString = ConnectionString;
            dataList = array;
        }

        public string execMsg
        {
            get { return execmsg; }
        }

        /// <summary>
        /// 파라미터를 모두 초기화한다.
        /// </summary>
        public void ClearParameter()
        {
            //ret = 0;
            msg = "";
            dataList.Clear();
        }

        /// <summary>
        /// 현재 모듈에 몇개의 명령데이타가 포함되어 있는지 리턴한다.
        /// </summary>
        public int Count
        {
            get { return dataList.Count; }
        }

        /// <summary>
        /// 모듈에 새로운 명령데이타를 넣는다.
        /// </summary>
        /// <param name="odata"></param>
        public void Add(object odata)
        {
            dataList.Add(odata);
        }

        /// <summary>
        /// 쿼리문 셋팅
        /// </summary>
        /// <param name="query"></param>
        public void SetQuery(string query)
        {
            strQuery = query;
        }


        /// <summary>
        /// LIST로 쿼리문 실행(SELECT)
        /// </summary>
        /// <returns></returns>
        public DataSet RunSelect()
        {
            ret = 0; msg = "";
            SqlConnection conn = new SqlConnection(connectionString);

            try
            {
                if (connectionString == null || connectionString.Trim().Equals(""))
                    throw new Exception("NOT CONNECT STRING");

                if (dataList.Count <= 0)
                    throw new Exception("QUERY COUNT IS ZERO");

                conn.ConnectionString = connectionString;
                conn.Open();
            }
            catch (Exception se)
            {
                ret = -1;
                msg = se.Message;
                return null; 
            }            

            DataSet dataSet = new DataSet();
            SqlDataAdapter sqlDA = new SqlDataAdapter();
            try
            {
                for (int i = 0; i < dataList.Count; i++)
                {
                    Type a = dataList[i].GetType();
                    
                    // SqlProcData
                    // SqlData
                    if (a.Name == "SqlData")
                    {
                        SqlData sdata = (SqlData)dataList[i];
                        if (!sdata.checkData()) continue;

                        sqlDA.SelectCommand = new SqlCommand(sdata.Sql, conn);
                        sqlDA.SelectCommand.CommandTimeout = DBTimeOut;
                        sqlDA.Fill(dataSet, sdata.TblName);
                    }
                    else if(a.Name == "SqlProcData")
                    {
                        SqlProcData pdata = (SqlProcData)dataList[i];
                        
						SqlCommand cmd = new SqlCommand(pdata.ProcName, conn);
						cmd.CommandType = CommandType.StoredProcedure;
						cmd.UpdatedRowSource = UpdateRowSource.None;

						SqlCommandBuilder.DeriveParameters(cmd);

						for(int j=0; j<cmd.Parameters.Count; j++)
						{
							string key = cmd.Parameters[j].ParameterName; 
							object val = pdata.GetParam(key);
							if(val==null) continue;
							cmd.Parameters[j].Value=val;
						}
						sqlDA.SelectCommand = cmd;
						sqlDA.SelectCommand.CommandTimeout = DBTimeOut;
						sqlDA.Fill( dataSet, pdata.TblName);
                    }
                }
                if (dataSet == null)
                        throw new Exception("UNKNOWN_ERROR");
            }
            catch (Exception se)
            {
                ret = -1;
                msg = se.Message;
                conn.Close();
                return null;
            }
            finally
            {
                conn.Close();
            }
            return dataSet;
        }


        public bool IS_DBConnection()
        {
            bool retValue = false;
            SqlConnection conn = new SqlConnection();
            try
            {
                conn.ConnectionString = connectionString;
                conn.Open();
                retValue = true;
            }
            catch
            {
                //ret = -1;
                //msg = se.Message;
                retValue = false;
            }
            finally
            {
                conn.Close();
            }
            return retValue;
        }

        /// <summary>
        ///  조회 명령을 실행한다.(SQL 쿼리문 전용)
        /// </summary>
        /// <returns></returns>
        public DataSet RunQuerySelect()
        {
            ret = 0; msg = "";

            SqlConnection conn = new SqlConnection(connectionString);
            DataSet dataSet = new DataSet();
            try
            {
                if (connectionString == null || connectionString.Trim().Equals(""))
                    throw new Exception("NOT CONNECT STRING");

                if (strQuery == null || strQuery.Trim().Equals(""))
                    throw new Exception("NOT QUERY");

                conn.Open();

                SqlDataAdapter sqlDA = new SqlDataAdapter();

                sqlDA.SelectCommand = new SqlCommand(strQuery, conn);
                sqlDA.SelectCommand.CommandTimeout = DBTimeOut;
                sqlDA.Fill(dataSet);
            }
            catch (Exception ex)
            {
                ret = -1;
                msg = ex.Message;
                return null;
            }
            finally
            {
                if (conn != null)
                    conn.Close();
            }

            return dataSet;
        }

        
        /// <summary>
        /// 단건을 저장한다.
        /// </summary>
        /// <param name="pdata"></param>
        /// <returns></returns>
        public DataSet RunUpdateSingle(SqlProcData pdata)
        {
            ret = 0; msg = "";
            SqlConnection conn = new SqlConnection(connectionString);
            try
            {
                if (!pdata.checkData()) throw new Exception("Param(ProcData) IS Wrong");
                if (connectionString == null || connectionString.Trim().Equals(""))
                    throw new Exception("NOT CONNECT STRING");

                conn.ConnectionString = connectionString;
                conn.Open();
            }
            catch (Exception se)
            {               
                //ret = -1;
                msg = se.Message;
                return null;
            }

            DataSet dataSet = new DataSet();
            SqlDataAdapter sqlDA = new SqlDataAdapter();

            try
            {
                SqlCommand cmd = new SqlCommand(pdata.ProcName, conn);
                cmd.CommandType = CommandType.StoredProcedure;
                cmd.UpdatedRowSource = UpdateRowSource.None;
                SqlCommandBuilder.DeriveParameters(cmd);
                for (int j = 0; j < cmd.Parameters.Count; j++)
                {
                    string key = cmd.Parameters[j].ParameterName;
                    object val = pdata.GetParam(key);
                    if (val == null) continue;
                    cmd.Parameters[j].Value = val;
                }

                sqlDA.SelectCommand = cmd;
                sqlDA.SelectCommand.CommandTimeout = DBTimeOut;
                sqlDA.Fill(dataSet, pdata.TblName);

                if (dataSet == null) throw new Exception("UNKNOWN ERROR");

                DataTable rtn = dataSet.Tables[pdata.TblName];
                if (rtn == null || rtn.Rows.Count == 0) 
                    throw new Exception("NOT RETURN PROCEDURE :" + pdata.ProcName);

                int tret = int.Parse(rtn.Rows[0].ItemArray[0].ToString());
                string tmsg = /*rtn.Rows[0].ItemArray[1].ToString()*/"예외 발생";
                if (tret != 0) throw new Exception(tmsg);

                execmsg = cmd.CommandText;
                return dataSet;
            }
            catch (Exception se)
            {
                ret = -1;
                msg = se.Message;
                return null;
            }
            finally
            {
                conn.Close();
            }
        }


        /// <summary>
        /// 저장 명령을 실행한다.
        /// </summary>
        /// <returns></returns>
        public DataSet RunUpdate()
        {
            ret = 0; msg = "";
            SqlConnection conn = new SqlConnection(connectionString);
            SqlTransaction tran = null;
            DataSet dataSet = new DataSet();

            SqlDataAdapter sqlDA = new SqlDataAdapter();

            try
            {
                if (connectionString == null || connectionString.Trim().Equals(""))
                    throw new Exception("NOT CONNECT STRING");

                conn.ConnectionString = connectionString;
                conn.Open();
            }
            catch (Exception se)
            {
                ret = -1;
                msg = se.Message;
                return null;
            }


            try
            {

                for (int i = 0; i < dataList.Count; i++)
                {
                    Type a = dataList[i].GetType();

                    if (a.Name == "SqlData")
                    {
                        SqlData sdata = (SqlData)dataList[i];
                        if (!sdata.checkData()) continue;
                        sqlDA.SelectCommand = new SqlCommand(sdata.Sql, conn);
                        sqlDA.SelectCommand.CommandTimeout = DBTimeOut;
                        sqlDA.Fill(dataSet, sdata.TblName);
                    }
                    else if (a.Name == "SqlProcData")
                    {
                        SqlProcData pdata = (SqlProcData)dataList[i];
                        if (!pdata.checkData()) continue;

                        SqlCommand cmd = new SqlCommand(pdata.ProcName, conn);
                        cmd.CommandType = CommandType.StoredProcedure;
                        cmd.UpdatedRowSource = UpdateRowSource.None;
                        SqlCommandBuilder.DeriveParameters(cmd);
                        pdata.Cmd = cmd;
                    }
                }

                tran = conn.BeginTransaction();
                for (int i = 0; i < dataList.Count; i++)
                {
                    Type a = dataList[i].GetType();

                    if (a.Name == "SqlData") continue;

                    SqlProcData pdata = (SqlProcData)dataList[i];
                    if(pdata.Cmd == null) continue;

                    SqlCommand cmd = pdata.Cmd;
                    cmd.Transaction = tran;

                    if (pdata.HasMaster) //
                    {
                        DataTable mdt = dataSet.Tables[pdata.MasterTable];
                        if (mdt == null || mdt.Rows.Count == 0)
                        {
                            throw new Exception("NOT FOUND MASTER TABLE :" + pdata.ProcName);
                        }

                        IEnumerator ie = pdata.GetMasterKeyList();
                        while (ie.MoveNext())
                        {
                            string mkey = (string)ie.Current;
                            string mval = (string)pdata.GetMasterKey(mkey);
                            pdata.AddParam(mkey, mdt.DefaultView[0][mval]);
                        }
                    }

                    for (int j = 0; j < cmd.Parameters.Count; j++)
                    {
                        string key = cmd.Parameters[j].ParameterName;
                        object val = pdata.GetParam(key);
                        if (val == null) continue;
                        cmd.Parameters[j].Value = val;
                    }
                    sqlDA.SelectCommand = cmd;
                    sqlDA.SelectCommand.CommandTimeout = DBTimeOut;
                    sqlDA.Fill(dataSet, pdata.TblName);

                    if (dataSet == null) throw new Exception("UNKNOWN ERROR");
                    DataTable rtn = dataSet.Tables[pdata.TblName];
                    if (rtn == null || rtn.Rows.Count == 0) throw new Exception("NOT RETURN PROCEDURE :" + pdata.ProcName);
                    if (rtn.Rows[0].ItemArray[0] == null)
                        throw new Exception("NOT RETURN PROCEDURE :" + pdata.ProcName);
                    int tret = int.Parse(rtn.Rows[0].ItemArray[0].ToString());
                    string tmsg = (rtn.Rows[0].ItemArray[1] == null) ? "" : rtn.Rows[0].ItemArray[1].ToString();
                    if (tret != 0) throw new Exception(tmsg);
                }
                if (dataSet == null) throw new Exception("UNKNOWN ERROR");

                if (tran != null) tran.Commit();
                return dataSet;
            }
            catch (System.Exception ex)
            {
                if (tran != null) tran.Rollback();
                ret = -1;
                msg = ex.Message;
                return null;
            }
            finally
            {
                conn.Close();
            }

        }

    }
}
