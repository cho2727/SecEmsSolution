using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

namespace Cube.Infrastructure.BaseClasses
{
    public abstract class ModelBase : IDataErrorInfo
    {
        #region IDataErrorInfo Members

        /// <summary>
        /// Returns error against an object.
        /// </summary>
        public string Error
        {
            get { return null; }
        }

        /// <summary>
        /// Gets error against a property name.
        /// </summary>
        /// <param name="columnName"></param>
        /// <returns></returns>
        public string this[string columnName]
        {
            get { return GetErrorForProperty(columnName); }
        }

        #endregion

        #region Protected Members

        /// <summary>
        /// A virtual overridable method for returning an error against a property value.
        /// </summary>
        /// <param name="propertyName"></param>
        /// <returns></returns>
        protected virtual string GetErrorForProperty(string propertyName)
        {
            return null;
        }

        #endregion
    }
}
