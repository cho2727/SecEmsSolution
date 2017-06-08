using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;
using System.Threading;

namespace CubeHelper
{
    [Serializable]
    public struct stLogData
    {
        public int      nLogType;
        public string   strMsg;
    }

    public class CubeLog : CubeTask
    {
        static object tempobj = new object();
        private const int nMaxLogFileSize = 5;

        public enum eLogType
        {
            nSystemLog = 1,
            nDataProcLog = 2
        }

        public enum eLogLevel
        {
            LOG_NONE,           // 0
            LOG_ERROR,
            LOG_INFO,
            LOG_DEBUG,
            LOG_SYSTEM
        }

        public const int nMaxCubeDataCount = 10000;
        public const int nLogThreadSleepTime = 2;

        private Thread thr_ = null;

        // 로그 레벨
        private eLogLevel LogLevel_;
        public eLogLevel LogLevel
        {
            get { return LogLevel_; }
            set { LogLevel_ = value; }
        }

        private int OfficeCode_;
        public int OfficeCode
        {
            get { return OfficeCode_; }
            set { OfficeCode_ = value; }
        }

        private string ProcName_;
        public string ProcName
        {
            get { return ProcName_; }
            set { ProcName_ = value; }
        }       
        

        public CubeBox<stLogData> CubeLogBox;
        private static CubeLog _instance;

        private CubeLog()
            :base()
        {
            OfficeCode_ = 0;
            ProcName = "";
            LogLevel = eLogLevel.LOG_INFO;
            CubeLogBox = new CubeBox<stLogData>(nMaxCubeDataCount);
            init();
        }

        ~CubeLog()
        {
            //fini();   // 메인에서 호출하는것으로 함.....
        }

        public static CubeLog Instance
        {
            get
            {
                if (_instance == null)
                    _instance = new CubeLog();

                return _instance;
            }
        }

        public override void svc()
        {
            while (true)
            {
                try
                {
                    if (Thr_Handle.WaitOne(2))
                        break;

                    LogWork();

                    // 큐에서 데이터 꺼내옴
                    Thread.Sleep(nLogThreadSleepTime);
                }
                catch (ThreadAbortException ex)
                {
                    break;
                }
                catch (System.Exception ex)
                {

                }
            }
          
        }

        public void LogWork()
        {
            try
            {
                if (CubeLogBox.Count > 0)
                {
                    stLogData pData;
                    lock (CubeLogBox)
                    {
                        pData = CubeLogBox.Dequeue();
                    }

                    // 해당 데이터 로그 출력
                    if (pData.nLogType >= (int)eLogType.nSystemLog)
                    {
                        LogDataWrite(pData.nLogType, pData.strMsg);
                    }
                }
            }
            catch (Exception)
            {
                throw;
            }
        }

        public void DirectLogWrite(eLogType nLogType, eLogLevel eLevel, string strMsg)
        {
            stLogData pData;

            string strTime = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff");

            pData.nLogType = (int)nLogType;
            pData.strMsg = strMsg;

            if (eLevel == eLogLevel.LOG_SYSTEM)
            {
                Console.WriteLine("{0} {1}", strTime, strMsg);
                eLevel = eLogLevel.LOG_INFO;
            }

            pData.strMsg = string.Format("{0} [{1}] {2}", strTime, Thread.CurrentThread.ManagedThreadId.ToString(), strMsg);


            if (LogLevel_ < eLevel || LogLevel_ == eLogLevel.LOG_NONE)
                return;

            LogDataWrite(pData.nLogType, pData.strMsg);
        }

        public void LogWrite(string strMsg)
        {
            LogWrite(CubeLog.eLogType.nSystemLog, CubeLog.eLogLevel.LOG_SYSTEM, strMsg);
        }

        public void LogWrite(eLogType nLogType, eLogLevel eLevel, string strMsg)
        {
            stLogData pData;            
            string strTime = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff");
            
            pData.nLogType = (int)nLogType;
            pData.strMsg = strMsg;

            if (eLevel == eLogLevel.LOG_SYSTEM)
            {
                Console.WriteLine("{0} {1}", strTime, strMsg);
                eLevel = eLogLevel.LOG_INFO;
            }

            pData.strMsg = string.Format("{0} [{1}] {2}", strTime, Thread.CurrentThread.ManagedThreadId.ToString(), strMsg);


            if (LogLevel_ < eLevel || LogLevel_ == eLogLevel.LOG_NONE)
                return;

            lock (CubeLogBox)
            {
                CubeLogBox.Enqueue(pData);
            }
        }

        private void LogDataWrite(int nLogType, string strMsg)
        {
            string strLogName = string.Empty;

            switch (nLogType)
            {
                case (int)eLogType.nSystemLog:
                    strLogName = "SystemLog";
                    break;

                case (int)eLogType.nDataProcLog:
                    strLogName = "DataProcLog";
                    break;
                
                default:
                    strLogName = "SystemLog";
                    break;
            }

            WriteText(strLogName, strMsg);
        }



        private void WriteText(string strLogName, string strMsg)
        {
            string strFileName = string.Empty;
            // 로그는 현재 실행 폴더아래 Log 폴더를 생성한다.
            // 파일 이름은 KpoaLog_yyyyMMdd.txt 형태로 생성한다.
            string strPath = string.Empty;

            if (OfficeCode > 0)
            {
                strPath = string.Format(@"{0}{1}\{2}", @System.AppDomain.CurrentDomain.BaseDirectory
                , "LOG", OfficeCode);
                
            }
            else
            {
                strPath = string.Format(@"{0}{1}", @System.AppDomain.CurrentDomain.BaseDirectory
                , "LOG");
            }

            DateTime now_tm = System.DateTime.Now;
            strFileName = string.Format("{0}_{1}_{2}.txt", ProcName, strLogName, now_tm.ToString("yyyyMMdd"));

            lock (tempobj)
            {
                DirectoryInfo dir = new DirectoryInfo(@strPath);

                if (!dir.Exists)
                {
                    dir.Create();
                }

                try
                {

                    MakeDivideLogFile(nMaxLogFileSize, @strPath + "\\" + strFileName);

                    StreamWriter w = File.AppendText(@strPath + "\\" + strFileName);
                    w.WriteLine("{0}", strMsg);

                    //Console.WriteLine("{0}", strMsg);

                    w.Flush();
                    w.Close();
                }
                catch
                {

                }
            }
        }


        private bool MakeDivideLogFile(int nDeviceMBSize, string strFilePath)
        {
            // 기존 날짜기준 로그파일명을 크기가 3MB 이상될때 현재시간을 파일명에 추가하여
            // 로그파일크기를 3MB단위로 저장한다.
            bool bCreate = false;
            string strRenameFilePath = "";

            FileInfo fi = new FileInfo(strFilePath);
            if (!fi.Exists)
                return false;

            if (fi.Length >= (1024 * 1024 * nDeviceMBSize))
            {
                strRenameFilePath = String.Format("{0}{1}.txt", strFilePath.Substring(0, strFilePath.LastIndexOf(".")), DateTime.Now.ToString("_HHmmss"));
                File.Move(strFilePath, strRenameFilePath);
                bCreate = true;
            }
            return bCreate;
        }


        //////////////////////////////////////////////////////////////////
        /// <summary>
        /// 로그 보관주기에 따라 로그 삭제
        /// </summary>
        /// <param name="strLogPath"></param>
        /// <param name="nDay"></param>
        public void DeleteLogFile(string strLogPath, int nDay)
        {
            string[] files = Directory.GetFiles(strLogPath);
            DateTime answer = DateTime.Now.AddDays(-nDay);

            int nDelDate = Convert.ToInt32(answer.ToString("yyyyMMdd"));

            foreach (string file in files)
            {
                try
                {
                    FileInfo file_info = new FileInfo(file);
                    DateTime CreateTime = file_info.CreationTime;
                    int nFileDate = Convert.ToInt32(CreateTime.ToString("yyyyMMdd"));
                    if (nFileDate < nDelDate)
                    {
                        file_info.Delete();
                    }
                }
                catch (System.Exception ex)
                {
                    continue;
                }
            }
        }
    }
}
