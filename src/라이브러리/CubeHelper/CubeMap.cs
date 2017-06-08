using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CubeHelper
{
    public class CubeMap<stkey, stValue>
    {
        public SortedDictionary<stkey, stValue> dic_ = null;

        public CubeMap()
        {
            dic_ = new SortedDictionary<stkey, stValue>();
        }

        public CubeMap(IComparer<stkey> comparer)
        {
            dic_ = new SortedDictionary<stkey, stValue>(comparer);
        }

        public void Add(stkey key, stValue value)
        {
            stValue oValue;

            if (dic_.TryGetValue(key, out oValue))
            {
                dic_[key] = value;
            }
            else
            {
                dic_.Add(key, value);
            }
        }

        public bool ContainsKey(stkey key)
        {
            return dic_.ContainsKey(key);
        }

        public stValue this[stkey key]
        {
            get
            {
                try
                {
                    return dic_[key];
                }
                catch
                {
                    throw new Exception(string.Format("dic_[{0}] is null", key));
                }
            }
        }

        public IEnumerable<stkey> keys
        {
            get
            {
                return dic_.Keys;
            }
        }

        public IEnumerable<stValue> values
        {
            get
            {
                return dic_.Values;
            }
        }

        public stValue[] GetArrayList()
        {
            stValue[] pData = new stValue[dic_.Count];
            dic_.Values.CopyTo(pData, 0);
            return pData;
        }

        public int Count()
        {
            return dic_.Count;


        }

        public void Clear()
        {
            if (dic_ != null)
                dic_.Clear();
        }

        public void Remove(stkey key)
        {
            dic_.Remove(key);
        }

        public bool GetValue(stkey key, out stValue val)
        {
            return dic_.TryGetValue(key, out val);
        }

    }

    public class CubeMultiMap<stkey, stValue>
    {
        private SortedDictionary<stkey, List<stValue>> dic_ = null;

        public CubeMultiMap()
        {
            dic_ = new SortedDictionary<stkey, List<stValue>>();
        }

        public CubeMultiMap(IComparer<stkey> comparer)
        {
            dic_ = new SortedDictionary<stkey, List<stValue>>(comparer);
        }

        public void Add(stkey key, stValue value)
        {
            List<stValue> list = null;

            if (dic_.TryGetValue(key, out list))
            {
                if(!list.Contains(value))
                list.Add(value);
            }
            else
            {
                list = new List<stValue>();
                list.Add(value);
                dic_.Add(key, list);
            }
        }

        public void Add(stkey key, List<stValue> value)
        {
            List<stValue> list = null;
            if (dic_.TryGetValue(key, out list))
            {
                dic_.Remove(key);
            }

            dic_.Add(key, value);
        }

        public void Remove(stkey key)
        {
            dic_.Remove(key);
        }

        public bool ContainsKey(stkey key)
        {
            return dic_.ContainsKey(key);
        }

        public List<stValue> this[stkey key]
        {
            get
            {
                List<stValue> list = null;
                if (!dic_.TryGetValue(key, out list))
                {
                    list = new List<stValue>();
                    dic_.Add(key, list);
                }

                return list;
            }
        }

        public IEnumerable<stkey> keys
        {
            get
            {
                return dic_.Keys;
            }
        }

        public SortedDictionary<stkey, List<stValue>>.ValueCollection values
        {
            get
            {
                return dic_.Values;
            }
        }

        public void Clear()
        {
            dic_.Clear();
        }

        public int Count()
        {
            int nCount = 0;
            IEnumerator<stkey> iter = dic_.Keys.GetEnumerator();
            iter.Reset();
            while (iter.MoveNext())
            {
                List<stValue> list = null;
                dic_.TryGetValue(iter.Current, out list);
                nCount += list.Count;
            }

            return nCount;
        }

        public bool GetValue(stkey key, out List<stValue> list)
        {
            return dic_.TryGetValue(key, out list);
        }

    }
}
