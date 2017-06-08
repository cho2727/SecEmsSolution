using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Security.Cryptography;
using System.Data;
using System.IO;

namespace CubeHelper
{
    public struct stPdbDataInfo
    {
        public string strKeyCode;
        public string strMd5hash;
        public string UpdateTime;
    }

    public class CubePdb
    {
        // public const int iPdbKeySize = 12;
        public const int iPdbKeySize = 32; // 기존 12 -> 32로 늘림
        public const int iPdbHashSize = 36;
        public const int iPdbTimeSize = 17;
        public const int iPdbRecordSize = iPdbKeySize + iPdbHashSize + iPdbTimeSize;

        /// <summary>
        /// MD5 검사 함수
        /// </summary>
        /// <param name="strInput"></param>
        /// <returns></returns>
        public static string MD5HashFunc(string strInput)
        {
            MD5CryptoServiceProvider x = new MD5CryptoServiceProvider();
            byte[] bs = System.Text.Encoding.UTF8.GetBytes(strInput);
            bs = x.ComputeHash(bs);
            StringBuilder s = new System.Text.StringBuilder();
            foreach (byte b in bs)
            {
                s.Append(b.ToString("x2").ToLower());
            }
            return s.ToString();
        }


        public static byte[] ReadPdbFile(string strDirName, string strPdbFileName)
        {
            string strFileName;
            FileStream fileStream;

            strFileName = strDirName + strPdbFileName;
            byte[] readData;
            try
            {
                FileInfo fInfo = new FileInfo(strFileName);
                readData = new byte[fInfo.Length];


                fileStream = new FileStream(strFileName, FileMode.Open);
                fileStream.Read(readData, 0, (int)fInfo.Length);

                fileStream.Close();
            }
            catch
            {
                return null;
            }

            return readData;

        }

        public static byte[] ReadPdbFile(string strPdbFileName)
        {
            FileStream fileStream;

            byte[] readData;
            try
            {
                FileInfo fInfo = new FileInfo(strPdbFileName);
                readData = new byte[fInfo.Length];


                fileStream = new FileStream(strPdbFileName, FileMode.Open);
                fileStream.Read(readData, 0, (int)fInfo.Length);

                fileStream.Close();
            }
            catch
            {
                return null;
            }

            return readData;

        }

        /// <summary>
        /// PDB 파일 쓰기
        /// </summary>
        /// <param name="pData"></param>
        /// <param name="nLength"></param>
        /// <returns></returns>
        public static int WritePdbFile(byte[] pData, int nLength, string strFileName)
        {
            FileStream fileStream;

            try
            {
                fileStream = new FileStream(strFileName, FileMode.Create);
                fileStream.Write(pData, 0, nLength);
                fileStream.Close();
            }
            catch
            {
                return -1;
            }
            return 0;
        }


        /// <summary>
        /// SLS로 부터 받은 String을 변환하고 Null까지 잘라낸다.
        /// </summary>
        /// <param name="szString"></param>
        /// <returns></returns>
        public static string ConvertString(byte[] szString)
        {
            return Encoding.Default.GetString(szString).Substring(0, Encoding.Default.GetString(szString).IndexOf('\0'));
        }

        /// <summary>
        /// Array to UINT32
        /// </summary>
        /// <param name="Data"></param>
        /// <param name="nIdx"></param>
        /// <returns></returns>
        public static UInt32 ConvertArrayToUint32(byte[] Data, int nIdx)
        {
            byte[] endian4B = new byte[4];
            Buffer.BlockCopy(Data, nIdx, endian4B, 0, 4);
            Array.Reverse(endian4B);
            return BitConverter.ToUInt32(endian4B, 0);
        }


        public static bool MoveFiles(string file, string strTarget)
        {
            DirectoryInfo dir = new DirectoryInfo(@strTarget);

            if (!dir.Exists)
            {
                dir.Create();
            }

            string strFileName = file.Substring(file.LastIndexOf('\\') + 1);

            FileInfo fi = new FileInfo(file);
            if (!fi.Exists)
            {
                return false;
            }

            try
            {
                if (File.Exists(strTarget + "\\" + strFileName))
                {
                    // 로그
                    return false;
                }
                else
                {
                    File.Move(file, strTarget + "\\" + strFileName);
                }
            }
            catch (System.Exception ex)
            {
                return false;
            }

            return true;
        }

    }
}
