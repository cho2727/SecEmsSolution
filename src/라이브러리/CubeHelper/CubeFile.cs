using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace CubeHelper
{
    public class CubeFile
    {
        public static string LoadTxtFile(string strDirName, string strFileName)
        {
            string strRetVal = string.Empty;
            DirectoryInfo di = new DirectoryInfo(strDirName);
            if (!di.Exists)
                return strRetVal;

            FileStream fileStream = new FileStream(strDirName + "\\" + strFileName, FileMode.Open);
            StreamReader Sr = new StreamReader(fileStream, Encoding.GetEncoding(/*"EUC-KR"*/"UTF-8"));

            string str;
            do
            {
                str = Sr.ReadLine();

                strRetVal += string.Format("{0}\n", str);
            } while (str != null);

            Sr.Close();
            fileStream.Close();

            return strRetVal;
        }

        public static string LoadTxtFile(string strFileName)
        {
            string str = string.Empty;
            try
            {
                using (FileStream fileStream = new FileStream(strFileName, FileMode.Open))
                {
                    using (StreamReader Sr = new StreamReader(fileStream, Encoding.GetEncoding("UTF-8")))
                    {
                        str = Sr.ReadToEnd();
                        Sr.Close();
                    }
                    fileStream.Close();
                }
            }
            catch (System.Exception e)
            {

            }

            return str;
        }
    }
}
