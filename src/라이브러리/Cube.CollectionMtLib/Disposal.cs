using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Cube.CollectionMtLib
{
    public static class Disposal
    {
        public static void Dispose(object o)
        {
            IDisposable disposable = o as IDisposable;
            if (disposable != null)
            {
                disposable.Dispose();
            }
        }
    }
}
