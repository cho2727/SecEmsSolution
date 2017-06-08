using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

namespace Cube.CollectionMtLib
{
    public static class EnumerableExtension
    {
        // ******************************************************************
        public static bool Exists<T>(this IEnumerable<T> coll, Func<T, bool> condition)
        {
            foreach (T t in coll)
            {
                if (condition(t))
                {
                    return true;
                }
            }

            return false;
        }

        // ******************************************************************
        public static bool Exists<T>(this IEnumerable coll, Func<T, bool> condition)
        {
            foreach (T t in coll)
            {
                if (condition(t))
                {
                    return true;
                }
            }

            return false;
        }

        // ******************************************************************
        ///<summary>Finds the index of the first item matching an expression in an enumerable.</summary>
        ///<param name="items">The enumerable to search.</param>
        ///<param name="predicate">The expression to test the items against.</param>
        ///<returns>The index of the first matching item, or -1 if no items match.</returns>
        public static int FindIndex<T>(this IEnumerable<T> items, Func<T, bool> predicate)
        {
            if (items == null) throw new ArgumentNullException("items");
            if (predicate == null) throw new ArgumentNullException("predicate");

            int retVal = 0;
            foreach (var item in items)
            {
                if (predicate(item)) return retVal;
                retVal++;
            }
            return -1;
        }

        // ******************************************************************
        public static string ConcatItemsToString<T>(this IEnumerable<T> items, Func<T, string> elementStringToConcat, string separator = ", ")
        {
            StringBuilder sb = new StringBuilder();
            bool firstItem = true;
            foreach (T item in items)
            {
                if (firstItem)
                {
                    firstItem = false;
                }
                else
                {
                    if (separator != null)
                        sb.Append(separator);
                }

                sb.Append(elementStringToConcat(item));
            }

            return sb.ToString();
        }

        // ******************************************************************
        public static void ApplyForEachItem<T>(this IEnumerable<T> enumerable, Action<T> action)
        {
            foreach (T t in enumerable)
            {
                action(t);
            }
        }

        // ******************************************************************
        public static void ApplyForEachItemOnCopyForDeletion<T>(this IEnumerable<T> enumerable, Action<T> action)
        {
            var listCopy = new List<T>(enumerable);
            foreach (T t in listCopy)
            {
                action(t);
            }
        }

        // ******************************************************************

    }
}
