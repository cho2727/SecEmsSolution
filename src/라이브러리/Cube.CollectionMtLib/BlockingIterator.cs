using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

namespace Cube.CollectionMtLib
{

    public class BlockingIterator<T> : IDisposable, IEnumerator<T>
    {
        private IEnumerator<T> _iEnumerator = null;
        private object _objLock = null;

        public T Current
        {
            get { return _iEnumerator.Current; }
        }

        object IEnumerator.Current
        {
            get { return _iEnumerator.Current; }
        }

        public bool MoveNext()
        {
            return _iEnumerator.MoveNext();
        }

        public void Reset()
        {
            _iEnumerator.Reset();
        }

        private bool _disposed = false;

        // The stream passed to the constructor  
        // must be readable and not null. 
        public BlockingIterator(IEnumerator<T> iEnumerator, object objLock)
        {
            if (iEnumerator == null || objLock == null)
            {
                throw new ArgumentException();
            }

            _iEnumerator = iEnumerator;
            _objLock = objLock;
            System.Threading.Monitor.Enter(_objLock);
        }

        public void Dispose()
        {
            Dispose(true);

            // Use SupressFinalize in case a subclass 
            // of this type implements a finalizer.
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            // If you need thread safety, use a lock around these  
            // operations, as well as in your methods that use the resource. 
            if (!_disposed)
            {
                if (disposing)
                {
                    if (_objLock != null)
                    {
                        System.Threading.Monitor.Exit(_objLock);
                        _objLock = null;
                    }

                    var dispoable = _iEnumerator as IDisposable;
                    if (dispoable != null)
                    {
                        dispoable.Dispose();
                        _iEnumerator = null;
                    }
                }

                // Indicate that the instance has been disposed.
                _disposed = true;
            }
        }

    }
}
