using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;
using System.Data;
using System.Xml;

using System.Configuration;

namespace CubeHelper
{
    public class CubeXml
    {
        /// <summary>
        /// 데이터테이블을 XML 파일로 출력
        /// </summary>
        /// <param name="dt"></param>
        /// <param name="fileName"></param>
        public static void WritingXMLData(DataTable dt, string strDirName, string strFileName)
        {
            try
            {
                if (dt != null)
                {
                    DataSet dsTemp = new DataSet();
                    dsTemp.Tables.Add(dt.Copy());

                    DirectoryInfo di = new DirectoryInfo(strDirName);
                    if (!di.Exists)
                        di.Create();

                    FileInfo fi = new FileInfo(strDirName + strFileName);
                    if (fi.Exists)
                        fi.Delete();

                    dsTemp.WriteXml((strDirName + strFileName));
                }
            }
            catch (Exception ex)
            {

            }

        }

        public static void WritingXMLData(DataTable dt, string strFullFileName)
        {
            if (dt != null)
            {
                DataSet dsTemp = new DataSet();
                dsTemp.Tables.Add(dt.Copy());

                FileInfo fi = new FileInfo(strFullFileName);

                if (fi.Exists)
                    fi.Delete();

                dsTemp.WriteXml((strFullFileName));
            }
        }

        /// <summary>
        /// XML 파일을 데이터테이블로 로딩
        /// </summary>
        /// <param name="fileName"></param>
        /// <returns></returns>
        public static DataTable LoadingXMLData(string strDirName, string strFileName)
        {
            DataTable dt = null;
            try
            {
                if (File.Exists(strDirName + strFileName))
                {
                    DataSet dsTemp = new DataSet();
                    dsTemp.ReadXml(strDirName + strFileName);
                    dt = dsTemp.Tables[0].Copy();
                }
                else
                {
                    throw new Exception("NOT FILE");
                    dt = null;
                }
            }
            catch(Exception ex)
            {
                throw ex;
            }
            return dt;
        }

        public static string LoadingXmlAttribute(string strDirName, string strFileName, string strNodePath, string strAttribute)
        {
            string strRet = string.Empty;
            XmlDocument doc = new XmlDocument();

            try
            {
                if (File.Exists(strDirName + strFileName))
                {
                    doc.Load(strDirName + strFileName);

                    XmlNode node = doc.SelectSingleNode(strNodePath);

                    strRet = node.Attributes[strAttribute].Value;

                }
            }
            catch (System.Exception e)
            {

            }

            return strRet;
        }

        public static string LoadingXmlAttribute(string strFilePath, string strNodePath, string strAttribute)
        {
            string strRet = string.Empty;
            XmlDocument doc = new XmlDocument();

            try
            {
                if (File.Exists(strFilePath))
                {
                    doc.Load(strFilePath);
                    XmlNode node = doc.SelectSingleNode(strNodePath);
                    strRet = node.Attributes[strAttribute].Value;
                }
            }
            catch (System.Exception e)
            {
            }
            return strRet;
        }

        public static bool GetServerValue(string strFilePath, string strNodePath, string strServiceCode, string strKey, ref string strServerCode, ref string strAdrress)
        {
            ConfigXmlDocument doc = new ConfigXmlDocument();
            try
            {
                if (File.Exists(strFilePath))
                {
                    doc.Load(strFilePath);

                    XmlNodeList pNodeList = doc.SelectNodes(strNodePath);
                    if (pNodeList.Count > 0)
                    {
                        bool bFind = false;
                        XmlNode pFind = null;
                        foreach (XmlNode pNode in pNodeList)
                        {
                            string code = pNode.Attributes[strKey].Value;
                            if (strServiceCode == code)
                            {
                                pFind = pNode;
                                bFind = true;
                            }
                        }

                        if (bFind)
                        {
                            XmlNodeList pChildList = pFind.ChildNodes;
                            if (pChildList.Count > 0)
                            {
                                foreach (XmlNode pChile in pChildList)
                                {
                                    strServerCode = strServiceCode;
                                    strAdrress = pChile.InnerText;
                                    return true;
                                }
                            }
                        }
                        else
                        {
                            foreach (XmlNode pNode in pNodeList)
                            {
                                strServerCode = pNode.Attributes[strKey].Value;
                                XmlNodeList pChildList = pNode.ChildNodes;
                                if (pChildList.Count > 0)
                                {
                                    foreach (XmlNode pChile in pChildList)
                                    {
                                        strAdrress = pChile.InnerText;
                                        return false;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {

            }
            return false;
        }

        /// <summary>
        /// config 파일에서 해당 key 값 얻어오기
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        public static string GetCofigValue(string key)
        {
            try
            {
                string strCurrentDir = @System.AppDomain.CurrentDomain.BaseDirectory;
                ExeConfigurationFileMap configFile = new ExeConfigurationFileMap();
                configFile.ExeConfigFilename = @strCurrentDir + @"\Config\Config.xml";
                Configuration config = ConfigurationManager.OpenMappedExeConfiguration(configFile, ConfigurationUserLevel.None);

                return config.AppSettings.Settings[key].Value;
            }
            catch (NullReferenceException ex)
            {
                throw new Exception(string.Format("GetConfigValue KEY:{0} fail", key));
            }
        }

        /// <summary>
        /// 해당 Key값을 value 값으로 변경 및 SAVE
        /// </summary>
        /// <param name="key"></param>
        /// <param name="value"></param>
        public static void SetConfigValue(string key, string value)
        {
            try
            {
                string strCurrentDir = @System.AppDomain.CurrentDomain.BaseDirectory;
                ExeConfigurationFileMap configFile = new ExeConfigurationFileMap();
                configFile.ExeConfigFilename = @strCurrentDir + @"\Config\Config.xml";
                Configuration config = ConfigurationManager.OpenMappedExeConfiguration(configFile, ConfigurationUserLevel.None);
                config.AppSettings.Settings[key].Value = value;
                config.Save(ConfigurationSaveMode.Minimal);
            }
            catch (System.Exception ex)
            {

            }
        }

        /// <summary>
        /// Config 파일에서 ConnectionString 변경된 부분 저장
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        public static void SaveConnectionString(string key, string strConnectionString)
        {
            string strCurrentDir = @System.AppDomain.CurrentDomain.BaseDirectory;
            ExeConfigurationFileMap configFile = new ExeConfigurationFileMap();
            configFile.ExeConfigFilename = @strCurrentDir + @"\Config\Config.xml";
            Configuration config = ConfigurationManager.OpenMappedExeConfiguration(configFile, ConfigurationUserLevel.None);
            config.ConnectionStrings.ConnectionStrings[key].ConnectionString = strConnectionString;
            config.Save(ConfigurationSaveMode.Modified);
        }


        /// <summary>
        /// 초를 날짜/시간으로 변환
        /// </summary>
        /// <param name="time_t"></param>
        /// <returns></returns>
        public static DateTime TimeTToDateTime(uint time_t)
        {
            long win32FileTime = 10000000 * (long)time_t + 116444736000000000;
            //return DateTime.FromFileTimeUtc(win32FileTime);
            return DateTime.FromFileTime(win32FileTime);
        }

    }
}
