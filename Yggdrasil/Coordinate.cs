using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Yggdrasil
{
    public class Coordinate
    {
        public double North = double.NaN;
        public double South = double.NaN;
        public double East = double.NaN;
        public double West = double.NaN;

        public Coordinate()
        {
            Clear();
        }

        public Coordinate(double north, double south, double west, double east)
        {
            North = north;
            South = south;
            West = west;
            East = east;
        }

        public bool IsValid()
        {
            if (double.IsNaN(North) == false && double.IsNaN(South) == false && double.IsNaN(East) == false && double.IsNaN(West) == false)
            {
                return (North >= South && West <= East);
            }

            return false;
        }

        public void Clear()
        {
            North = double.NaN;
            South = double.NaN;
            West = double.NaN;
            East = double.NaN;
        }
    }
}
