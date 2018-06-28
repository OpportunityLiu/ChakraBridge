using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Opportunity.ChakraBridge.UWP
{
    internal class JsFuncTable<T>
        where T : Delegate
    {
        public List<T> Table = new List<T>();

        public IntPtr GetNextPos()
        {
            var i = this.Table.IndexOf(null);
            if (i < 0)
                i = this.Table.Count;
            return new IntPtr(i);
        }

        public void Add(IntPtr pos, T value)
        {
            var i = pos.ToInt32();
            if (i == this.Table.Count)
                this.Table.Add(value);
            else
                this.Table[i] = value;
        }

        public T Get(IntPtr pos)
        {
            return this.Table[pos.ToInt32()];
        }

        public bool Remove(T value)
        {
            var i = this.Table.IndexOf(value);
            if (i < 0)
                return false;
            this.Table[i] = null;
            return true;
        }

        public T RemoveAt(IntPtr pos)
        {
            var i = pos.ToInt32();
            var r = this.Table[i];
            this.Table[i] = null;
            return r;
        }
    }
}
