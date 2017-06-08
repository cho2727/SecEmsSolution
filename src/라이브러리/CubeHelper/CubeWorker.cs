using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Collections;

namespace CubeHelper
{
    public class CubeWorker // 쓰레드 풀용.
    {
        private Thread thr_ = null;
        public string Name
        {
            get
            {
                return thr_.Name;
            }
            set
            {
                thr_.Name = value;
            }
        }

        public CubeWorker(Thread thr)
        {
            thr_ = thr;
        }

        public void init()
        {
            thr_.IsBackground = true;
            thr_.Start();
        }

        public void fini()
        {
            if (thr_ != null)
            {
                //thr_.Abort();
                thr_.Join(1000);
            }
      
//             CubeLog.Instance.DirectLogWrite(CubeLog.eLogType.nSystemLog, CubeLog.eLogLevel.LOG_SYSTEM
//                 , string.Format("P({0}:{1}) Thread({2}) Pool Exit", CubeLog.Instance.OfficeCode, CubeLog.Instance.ProcName, thr_.Name));
        }
    }

    public abstract class CubeThreadPool<T>
    {
        public CubeBox<T> WorkBox_;
        public Hashtable ThreadList = null;
        private EventWaitHandle thr_handle_ = null;

        public EventWaitHandle Thr_Handle
        {
            get
            {
                return thr_handle_;
            }
        }

        public CubeThreadPool(int iCount)
        {
            WorkBox_ = new CubeBox<T>(iCount);
            ThreadList = Hashtable.Synchronized(new Hashtable());
            thr_handle_ = new EventWaitHandle(false, EventResetMode.ManualReset);
        }

        public void init(int thr_cnt)
        {
            for (int i = 0; i < thr_cnt; i++)
            {
                CubeWorker proc_thr = new CubeWorker(new Thread(new ThreadStart(svc)));

                proc_thr.init();
                string key = (i + 1).ToString();
                proc_thr.Name = key;

                lock (ThreadList)
                {
                    ThreadList.Add(key, proc_thr);
                }
            }
        }

        public void fini()
        {
            thr_handle_.Set();
            if (ThreadList.Count > 0)
            {
                lock (ThreadList)
                {
                    foreach (DictionaryEntry clientEntry in ThreadList)
                    {
                        CubeWorker proc_thr = (CubeWorker)clientEntry.Value;
                        proc_thr.fini();                        
                    }

                    ThreadList.Clear();
                }
            }
        }

        public void EnqueueData(T pData)
        {
            lock (WorkBox_)
            {
                WorkBox_.Enqueue(pData);
            }
        }

        public abstract void svc();
    }

#if false
    public abstract class CubeThread
    {
        private Thread thr_ = null;
        public string Name
        {
            get
            {
                return thr_.Name;
            }
            set
            {
                thr_.Name = value;
            }
        }

        public CubeThread()
        {
            thr_ = new Thread(new ThreadStart(svc));
        }

        public void init()
        {
            thr_.IsBackground = true;
            thr_.Start();
        }

        public void fini()
        {
            if (thr_ != null)
                thr_.Abort();
        }

        public abstract void svc();
    }
#endif
    public abstract class CubeTask
    {
        private Thread thr_ = null;
        private  EventWaitHandle thr_handle_ = null;
        public string Name
        {
            get
            {
                return thr_.Name;
            }
            set
            {
                thr_.Name = value;
            }
        }

        public EventWaitHandle Thr_Handle 
        {
            get
            {
                return thr_handle_;
            }
        }

        public CubeTask()
        {
            thr_ = new Thread(new ThreadStart(svc));
            thr_handle_ = new EventWaitHandle(false, EventResetMode.ManualReset);
        }

        public void init()
        {
            thr_.IsBackground = true;
            thr_.Start();
        }

        public void fini()
        {
            thr_handle_.Set();
            thr_.Abort();
            //thr_.Join(1000);
        }

        protected int GetElapseTime(int iStartTime) // 경과 시간
        {
            int iCurrentCount = Environment.TickCount & Int32.MaxValue;
            int iDiffCount = 0;
            if (iStartTime <= iCurrentCount) // 시작시간보다 현재시간이 크면 현재시간에서 시작시간을 뺌
                iDiffCount = iCurrentCount - iStartTime;
            else
                iDiffCount = (Int32.MaxValue - iStartTime) + iCurrentCount;

            return iDiffCount / 1000;
        }

        protected int GetCurrentTick() // 현재 TICK COUNT 리턴
        {
            return Environment.TickCount & Int32.MaxValue;
        }

        public abstract void svc();
    }



    public abstract class CubeBoxTask<T> : CubeTask
    {
        public CubeBox<T> WorkBox_;

        public CubeBoxTask(int iCount)
            : base()
        {
            WorkBox_ = new CubeBox<T>(iCount);
        }

        public void EnqueueData(T pData)
        {
            lock (WorkBox_)
            {
                WorkBox_.Enqueue(pData);
            }
        }
    }
}
