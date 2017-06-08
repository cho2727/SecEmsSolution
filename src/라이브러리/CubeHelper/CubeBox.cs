using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CubeHelper
{
    public class CubeBox<T>
    {
        static object tempobj = new object();

        private T[] mQueue;
        private int EnIndex;
        private int DeIndex;
        private int cyclevalue;
        private int nBufferSize;
        public CubeBox(int capacity)
        {
            this.mQueue = new T[capacity];
            EnIndex = 0;
            DeIndex = 0;
            cyclevalue = 0;
            nBufferSize = capacity;
        }

        public CubeBox()
        {
            this.mQueue = new T[0];
            EnIndex = 0;
            DeIndex = 0;
            cyclevalue = 0;
        }

        public int Count
        {
            get
            {
                return (cyclevalue * nBufferSize) + EnIndex - DeIndex;
            }
        }

        public int EnqueueIndex
        {
            get
            {
                return EnIndex;
            }
        }

        public int DequeueIndex
        {
            get
            {
                return DeIndex;
            }
        }

        private void ArrayCopy(T[] from, T[] to, uint offset)
        {
            int toidx = 0;

            int len = from.Length;
            for (uint i = offset; i < len; i++)
            {
                to[toidx++] = from[i];
            }
        }

        public void CurrentEnque(T item)
        {
            this.mQueue[DeIndex] = item;
            if (DeIndex - 1 < 0)
            {
                DeIndex = nBufferSize - 1;
            }
            else
            {
                DeIndex--;
            }
        }

        public bool Enqueue(T item)
        {
            if (IsPull())
            {
                return false;
            }
            else
            {
                this.mQueue[EnIndex] = item;
                EnIndex++;

                if (EnIndex >= nBufferSize)
                {
                    EnIndex = 0;
                    //cyclevalue++;
                    IncCycleValue();
                }

            }
            return true;
        }

        public void IncCycleValue()
        {
            lock (tempobj)
            {
                cyclevalue++;
            }
        }

        public void DecCycleValue()
        {
            lock (tempobj)
            {
                cyclevalue--;
            }
        }

        public T Peek()
        {
            if (this.Count < 1) throw new InvalidOperationException("큐가 비어 있습니다.");
            return this.mQueue[0];
        }

        public T Dequeue()
        {
            T retVal;
            if (IsEmpty())
            {
                return default(T);
            }
            else
            {
                retVal = this.mQueue[DeIndex];
                this.mQueue[DeIndex] = default(T);
                DeIndex++;

                if (DeIndex >= nBufferSize)
                {
                    DeIndex = 0;
                    //cyclevalue--;
                    DecCycleValue();
                }
            }
            return retVal;
        }

        public void Clear()
        {
            DeIndex = 0;
            EnIndex = 0;
            cyclevalue = 0;
        }

        public bool IsEmpty()
        {
            if (DeIndex == EnIndex && cyclevalue == 0)
                return true;

            return false;
        }

        public bool IsPull()
        {
            if (DeIndex == EnIndex && cyclevalue == 1)
                return true;

            return false;
        }
    }
}
