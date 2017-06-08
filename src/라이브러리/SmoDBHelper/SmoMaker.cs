using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.Configuration;
using System.IO;
using System.Xml;

namespace SmoDBHelper
{
    public class SmoMaker
    {
        public static string GetConnectionString(string key)
        {
            string strCurrentDir = @System.AppDomain.CurrentDomain.BaseDirectory;
            ExeConfigurationFileMap configFile = new ExeConfigurationFileMap();
            configFile.ExeConfigFilename = @strCurrentDir + @"\Config\Config.xml";
            Configuration config = ConfigurationManager.OpenMappedExeConfiguration(configFile, ConfigurationUserLevel.None);

            return config.ConnectionStrings.ConnectionStrings[key].ConnectionString;
        }


        public static string GetConnectionString(string key, int type)
        {
            ConfigXmlDocument doc = new ConfigXmlDocument();
            string strFileName = @System.AppDomain.CurrentDomain.BaseDirectory + "config" + "\\" + "db_config.xml";
            StringBuilder sbConnect = new StringBuilder(); ;

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


        //// 기존 DeleteQuery 사용 불가능 : 맵에 있는 데이터만 delete 쿼리 생성
        public static string GetDeleteQuery(string strDataBase
            , string strSchema
            , string strTable
            , stKeyValue[] KeyVal)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(string.Format("DELETE FROM [{0}].{1}.{2} WHERE ", strDataBase, strSchema, strTable));
            for (int i = 0; i < KeyVal.Length; i++)
            {
                sb.Append(string.Format("{0} = '{1}' ", KeyVal[i].strKey, KeyVal[i].strValue.ToString().Replace("'", "''")));// ' -> ''로 바꾸면 됨....
                if (KeyVal.Length > i + 1)
                {
                    sb.Append(" AND ");
                }
            }
            sb.Append(";");
            //return str + ";\r\nGO";
            return sb.ToString();
        }


        public static string GetDeletedDasInsertQuery(string strSchema
            , string strTable
            , stKeyValue[] KeyVal
            , string value
            , string strOfficeCode
            , string strDBName
            , string strDelDBName)
        {
            string str = string.Empty;
            string[] strValue = value.Split('!');

            if (strValue.Length == KeyVal.Length)
            {
                str = string.Format("INSERT INTO [{0}].{2}.{3} SELECT * FROM [{1}].{2}.{3} WHERE ", strDelDBName, strDBName, strSchema, strTable);
                for (int i = 0; i < KeyVal.Length; i++)
                {
                    str = str + string.Format("{0} = '{1}'", KeyVal[i].strKey, strValue[i]);
                    if (KeyVal.Length > i + 1)
                    {
                        str = str + " AND ";
                    }
                }
                str = str + ";\r\nGO";
            }

            return str;
        }

        public static string GetDeletedDasUpdateQuery(string strSchema
            , string strTable
            , stKeyValue[] KeyVal
            , string value
            , string strOfficeCode
            , string strDBName
            , string strDelDBName)
        {
            string str = string.Empty;
            string[] strValue = value.Split('!');

            if (strValue.Length == KeyVal.Length)
            {
                str = string.Format("UPDATE [{0}].{1}.{2} set actiontype = 3, at_updatetime = getdate() WHERE ", strDelDBName, strSchema, strTable);

                for (int i = 0; i < KeyVal.Length; i++)
                {
                    str = str + string.Format("{0} = '{1}'", KeyVal[i].strKey, strValue[i]);
                    if (KeyVal.Length > i + 1)
                    {
                        str = str + " AND ";
                    }
                }
                str = str + ";\r\nGO";
            }

            return str;
        }

        public static string GetDeleteQuery(string strDataBase
            , string strSchema
            , string strTable
            , stKeyValue[] KeyVal
            , string value)
        {
            string str = string.Empty;
            string[] strValue = value.Split('!');

            if (strValue.Length == KeyVal.Length)
            {
                str = string.Format("DELETE FROM [{0}].{1}.{2} WHERE ", strDataBase, strSchema, strTable);
                for (int i = 0; i < KeyVal.Length; i++)
                {
                    str = str + string.Format("{0} = '{1}'", KeyVal[i].strKey, strValue[i]);
                    if (KeyVal.Length > i + 1)
                    {
                        str = str + " AND ";
                    }
                }
                //str = str + ";\r\nGO";
                str += ";";
            }

            return str;
        }

        public static string GetDeleteQuery(string strDataBase
             , string strSchema
             , string strTable
             , string strKey
             , string strKeyValue)
        {
            string str = string.Empty;

            str = string.Format("DELETE FROM [{0}].{1}.{2} WHERE ", strDataBase, strSchema, strTable);
            str = str + string.Format("{0} = '{1}'", strKey, strKeyValue);
            return str + ";";
        }

        public static string GetUpdateQuery(string strDataBase
            , string strSchema
            , string strTable
            , DataColumnCollection DataColumns
            , DataRow dr
            //, stKeyValue[] KeyVal)
            , string strKey
            , string strKeyValue)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(string.Format("UPDATE [{0}].{1}.{2} SET ", strDataBase, strSchema, strTable));

            foreach (DataColumn col in DataColumns)
            {
                try
                {
                    if (dr[col.ColumnName] == DBNull.Value)
                    {
                        sb.Append(string.Format("{0} = NULL, ", col.ColumnName));
                    }
                    else
                    {
                        if (col.DataType == typeof(System.DateTime))
                        {
                            sb.Append(string.Format("{0} = '{1}', ", col.ColumnName, Convert.ToDateTime(dr[col.ColumnName].ToString()).ToString("yyyy-MM-dd HH:mm:ss.fff")));
                        }
                        else if (col.DataType == typeof(System.String))
                        {
                            sb.Append(string.Format("{0} = '{1}', ", col.ColumnName, dr[col.ColumnName].ToString().Replace("'", "''")));// ' -> ''로 바꾸면 됨....
                        }
                        else if (col.DataType == typeof(System.Boolean))
                        {
                            sb.Append(string.Format("{0} = {1}, ", col.ColumnName, Convert.ToBoolean(dr[col.ColumnName]) ? 1 : 0));
                        }
                        else if (col.DataType == typeof(System.Byte[]))
                        {
                            sb.Append(string.Format("{0} = {1}, ", col.ColumnName, ByteArrayToString((byte[])dr[col.ColumnName])));
                        }
                        else
                        {
                            sb.Append(string.Format("{0} = {1}, ", col.ColumnName, dr[col.ColumnName]));
                        }
                    }
                }
                catch
                {
                    continue;
                }
            }

            sb.Remove(sb.Length - 2, 2);
            sb.Append(string.Format(" WHERE {0} = '{1}';", strKey, strKeyValue.ToString().Replace("'", "''")));

            return sb.ToString();
        }

        public static string GetUpdateQuery(string strDataBase
            , string strSchema
            , string strTable
            , DataColumnCollection DataColumns
            , DataRow dr
            , stKeyValue[] KeyVal)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(string.Format("UPDATE [{0}].{1}.{2} SET ", strDataBase, strSchema, strTable));

            foreach (DataColumn col in DataColumns)
            {
                try
                {
                    if (dr[col.ColumnName] == DBNull.Value)
                    {
                        sb.Append(string.Format("{0} = NULL, ", col.ColumnName));
                    }
                    else
                    {
                        if (col.DataType == typeof(System.DateTime))
                        {
                            sb.Append(string.Format("{0} = '{1}', ", col.ColumnName, Convert.ToDateTime(dr[col.ColumnName].ToString()).ToString("yyyy-MM-dd HH:mm:ss.fff")));
                        }
                        else if (col.DataType == typeof(System.String))
                        {
                            sb.Append(string.Format("{0} = '{1}', ", col.ColumnName, dr[col.ColumnName].ToString().Replace("'", "''")));// ' -> ''로 바꾸면 됨....
                        }
                        else if (col.DataType == typeof(System.Boolean))
                        {
                            sb.Append(string.Format("{0} = {1}, ", col.ColumnName, Convert.ToBoolean(dr[col.ColumnName]) ? 1 : 0));
                        }
                        else if (col.DataType == typeof(System.Byte[]))
                        {
                            sb.Append(string.Format("{0} = {1}, ", col.ColumnName, ByteArrayToString((byte[])dr[col.ColumnName])));
                        }
                        else
                        {
                            sb.Append(string.Format("{0} = {1}, ", col.ColumnName, dr[col.ColumnName]));
                        }
                    }
                }
                catch
                {
                    continue;
                }
            }

            sb.Remove(sb.Length - 2, 2);

            //str = str.Remove(str.LastIndexOf(','), str.Length - str.LastIndexOf(','));
            sb.Append(" WHERE ");
            for (int i = 0; i < KeyVal.Length; i++)
            {
                sb.Append(string.Format("{0} = '{1}' ", KeyVal[i].strKey, KeyVal[i].strValue.ToString().Replace("'", "''")));// ' -> ''로 바꾸면 됨....
                if (KeyVal.Length > i + 1)
                {
                    sb.Append("AND ");
                }
            }
            sb.Append(";");
            //return str + ";\r\nGO";
            return sb.ToString();
        }

        //public static string GetUpdateQuery(stKeyValue[] KeyVal)
        public static string GetWhereQuery(stKeyValue[] KeyVal)
        {
            string str = string.Empty;

            str = "WHERE ";
            for (int i = 0; i < KeyVal.Length; i++)
            {
                str = str + string.Format("{0} = '{1}' ", KeyVal[i].strKey, KeyVal[i].strValue.ToString().Replace("'", "''"));// ' -> ''로 바꾸면 됨....
                if (KeyVal.Length > i + 1)
                {
                    str = str + "AND ";
                }
            }
            return str + ";";
        }

        public static string GetSelectQuery(string strDataBase
            , string strSchema
            , string strTable
            , DataColumnCollection DataColumns
            , DataRow dr
            , stKeyValue[] KeyVal)
        {
            string str = string.Format("SELECT * from [{0}].{1}.{2}", strDataBase, strSchema, strTable);

            str = str + " WHERE ";
            for (int i = 0; i < KeyVal.Length; i++)
            {
                str = str + string.Format("{0} = '{1}' ", KeyVal[i].strKey, KeyVal[i].strValue);
                if (KeyVal.Length > i + 1)
                {
                    str = str + "AND ";
                }
            }
            return str;
        }

        public static string GetInsertQuery(string strDataBase
            , string strSchema
            , string strTable
            , DataColumnCollection DataColumns
            , DataRow dr)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(string.Format("INSERT INTO [{0}].[{1}].[{2}] (", strDataBase, strSchema, strTable));
            foreach (DataColumn col in DataColumns)
            {
                sb.Append(string.Format("[{0}], ", col.ColumnName));
            }

            sb.Remove(sb.Length - 2, 2);
            sb.Append(") VALUES (");

            foreach (DataColumn col in DataColumns)
            {
                try
                {
                    if (dr[col.ColumnName] == DBNull.Value)
                    {
                        sb.Append(string.Format("NULL, "));
                    }
                    else
                    {
                        if (col.DataType == typeof(System.DateTime))
                        {
                            sb.Append(string.Format("'{0}', ", Convert.ToDateTime(dr[col.ColumnName].ToString()).ToString("yyyy-MM-dd HH:mm:ss.fff")));
                        }
                        else if (col.DataType == typeof(System.String))
                        {
                            sb.Append(string.Format("'{0}', ", dr[col.ColumnName].ToString().Replace("'", "''"))); // ' -> ''로 바꾸면 됨....
                        }
                        else if (col.DataType == typeof(System.Boolean))
                        {
                            sb.Append(string.Format("{0}, ", Convert.ToBoolean(dr[col.ColumnName]) ? 1 : 0));
                        }
                        else if (col.DataType == typeof(System.Byte[]))
                        {
                            sb.Append(string.Format("0x{0}, ", ByteArrayToString((byte[])dr[col.ColumnName]))); // ' -> ''로 바꾸면 됨....
                        }
                        else
                        {
                            sb.Append(string.Format("{0}, ", dr[col.ColumnName]));
                        }
                    }
                }
                catch
                {
                    continue;
                }
            }

            sb.Remove(sb.Length - 2, 2);
            sb.Append(");");

            return sb.ToString();
        }


        public static void WriteSql(string strPathName, string strFileName, string strMsg)
        {
            DirectoryInfo dir = new DirectoryInfo(@strPathName);

            if (!dir.Exists)
            {
                dir.Create();
            }

            try
            {
                using (StreamWriter w = File.AppendText(@strPathName + "\\" + strFileName))
                {
                    w.WriteLine("{0}", strMsg);
                    w.Flush();
                    w.Close();
                    w.Dispose();
                }
            }
            catch (System.Exception ex)
            {
                // 로그 출력 
            }
        }

        public static string ByteArrayToString(byte[] ba)
        {
            StringBuilder hex = new StringBuilder(ba.Length * 2);
            foreach (byte b in ba)
                hex.AppendFormat("{0:X2}", b);
            return hex.ToString();
        }
    }
}
