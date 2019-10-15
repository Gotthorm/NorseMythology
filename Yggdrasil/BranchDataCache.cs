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
            float west = float.MaxValue;
            float east = float.MinValue;
            float north = float.MinValue;
            float south = float.MaxValue;

            foreach (BranchInfo branch in branchList)
            {
                // Update the bounds
                if (branch.GlobalCoordinateWest < west)
                {
                    west = branch.GlobalCoordinateWest;
                }
                if (branch.GlobalCoordinateEast > east)
                {
                    east = branch.GlobalCoordinateEast;
                }
                if (branch.GlobalCoordinateNorth > north)
                {
                    north = branch.GlobalCoordinateNorth;
                }
                if (branch.GlobalCoordinateSouth < south)
                {
                    south = branch.GlobalCoordinateSouth;
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
