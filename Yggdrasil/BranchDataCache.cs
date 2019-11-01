using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Yggdrasil
{
    class BranchDataCache
    {
        public BranchDataCache()
        {
        }

        // Returns true if a non-zero area is generated
        public bool Initialize(List<BranchInfo> branchList)
        {
            // Generate the global extents of the combined branches
            double west = double.MaxValue;
            double east = double.MinValue;
            double north = double.MinValue;
            double south = double.MaxValue;

            foreach (BranchInfo branch in branchList)
            {
                // Update the bounds
                if (branch.GlobalCoordinate.West < west)
                {
                    west = branch.GlobalCoordinate.West;
                }
                if (branch.GlobalCoordinate.East > east)
                {
                    east = branch.GlobalCoordinate.East;
                }
                if (branch.GlobalCoordinate.North > north)
                {
                    north = branch.GlobalCoordinate.North;
                }
                if (branch.GlobalCoordinate.South < south)
                {
                    south = branch.GlobalCoordinate.South;
                }
            }

            // 
            // Populate some attributes with data what was just set up
            return true;

            //return false;
        }

        // Calling this clears any previously generated cache
        public bool GenerateCache(uint horizontalOffset, uint verticalOffset)
        {
            return true;
        }
    }
}
