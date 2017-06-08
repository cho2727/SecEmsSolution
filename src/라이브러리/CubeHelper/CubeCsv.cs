using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Data;
using System.Runtime.Serialization;

namespace CubeHelper
{
    public static class CubeCsv
    {
        [System.Security.SecuritySafeCritical]  // auto-generated 
        public unsafe static int MakeSeparatorListT(this string str, char[] separator, char[] skipsep, ref int[] sepList)
        {
            int foundCount = 0;
            char[] array = str.ToCharArray();
            bool skipyn = false;

            if (separator == null || separator.Length == 0)
            {
                fixed (char* pwzChars = &array[0])
                {
                    //If they passed null or an empty string, look for whitespace.
                    for (int i = 0; i < str.Length && foundCount < sepList.Length; i++)
                    {
                        if (Char.IsWhiteSpace(pwzChars[i]))
                        {
                            sepList[foundCount++] = i;
                        }
                    }
                }
            }
            else
            {
                int sepListCount = sepList.Length;
                int sepCount = separator.Length;
                //If they passed in a string of chars, actually look for those chars.
                fixed (char* pwzChars = &array[0], pSepChars = separator, pSkipChars = skipsep)
                {
                    for (int i = 0; i < str.Length && foundCount < sepListCount; i++)
                    {
                        char* pSep = pSepChars;
                        char* pSkip = pSkipChars;
                        for (int j = 0; j < sepCount; j++, pSep++)
                        {
                            if (pwzChars[i] == *pSkip)
                            {
                                skipyn = skipyn ? false : true;
                            }

                            if (!skipyn)    // SKIPYN이 FALSE이면
                            {
                                if (pwzChars[i] == *pSep)
                                {
                                    sepList[foundCount++] = i;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            return foundCount;
        }


        public static String[] InternalSplitOmitEmptyEntriesT(this string str, Int32[] sepList, Int32[] lengthList, Int32 numReplaces, int count, bool check)
        {
            // Allocate array to hold items. This array may not be
            // filled completely in this function, we will create a 
            // new array and copy string references to that new array.
            int maxItems = (numReplaces < count) ? (numReplaces + 1) : count;
            String[] splitStrings = new String[maxItems];

            int currIndex = 0;
            int arrIndex = 0;

            for (int i = 0; i < numReplaces && currIndex < str.Length; i++)
            {
                if (sepList[i] - currIndex > 0)
                {
                    splitStrings[arrIndex++] = str.Substring(currIndex, sepList[i] - currIndex);
                }
                else if (sepList[i] - currIndex == 0)
                {
                    arrIndex++;
                }

                currIndex = sepList[i] + ((lengthList == null) ? 1 : lengthList[i]);
                if (arrIndex == count - 1)
                {
                    // If all the remaining entries at the end are empty, skip them
                    while (i < numReplaces - 1 && currIndex == sepList[++i])
                    {
                        currIndex += ((lengthList == null) ? 1 : lengthList[i]);
                    }
                    break;
                }
            }

            //Handle the last string at the end of the array if there is one.
            if (currIndex < str.Length)
            {
                splitStrings[arrIndex++] = str.Substring(currIndex);
            }

            String[] stringArray = splitStrings;
            if (arrIndex == maxItems && check)
            {
                stringArray = new String[arrIndex];
                for (int j = 0; j < arrIndex; j++)
                {
                    string temp = splitStrings[j];
                    if (string.IsNullOrEmpty(temp) == false)
                    {
                        if (temp.StartsWith("\"") && temp.EndsWith("\""))
                            temp = temp.Substring(1, temp.Length - 2);
                        if (string.IsNullOrEmpty(temp) == false)
                            temp = temp.Replace("\"\"", "\"");
                        stringArray[j] = temp;
                    }
                    //stringArray[j] = splitStrings[j];
                }
            }
            return stringArray;
        }

        public static String[] SplitT(this string str, char separator, char skipchar, bool check = true)
        {
            int[] sepList = new int[str.Length];
            char[] separators = { separator };
            char[] skipchars = { skipchar };
            int numReplaces = str.MakeSeparatorListT(separators, skipchars, ref sepList);
            return str.InternalSplitOmitEmptyEntriesT(sepList, null, numReplaces, Int32.MaxValue, check);
        }

        public static DataTable DataTableFromTextFile(string location, char delimiter = ',')
        {
            DataTable result;
            string[] LineArray = File.ReadAllLines(location, Encoding.Default);
            result = FormDataTable(LineArray, delimiter);
            return result;
        }

        private static DataTable FormDataTable(string[] LineArray, char delimiter)
        {
            DataTable dt = new DataTable();
            AddColumnToTable(LineArray, delimiter, ref dt);
            AddRowToTable(LineArray, delimiter, ref dt);
            return dt;
        }

        private static void AddRowToTable(string[] valueCollection, char delimiter, ref DataTable dt)
        {
            //string[] columns = valueCollection[1].Split(delimiter);
            for (int i = 2; i < valueCollection.Length; i++)
            {
                if (valueCollection[i] == "" || valueCollection[i] == string.Empty)
                    continue;

                string[] values = valueCollection[i].SplitT(delimiter, '\"');
                DataRow dr = dt.NewRow();
                for (int j = 0; j < values.Length; j++)
                {
                    dr[j] = values[j]; 
                    dr[j] = dr[j].ToString().Replace("\"", "");
                }
                
                dt.Rows.Add(dr);
            }
        }

        private static void AddColumnToTable(string[] columnCollection, char delimiter, ref DataTable dt)
        {
            string[] caption = columnCollection[0].Split(delimiter);
            string[] columns = columnCollection[1].Split(delimiter);
            for (int i = 0; i < columns.Length; i++)
            {
                DataColumn dc = new DataColumn(columns[i], typeof(string));
                dc.Caption = caption[i];
                dt.Columns.Add(dc);
            }
        }

    }
}
