using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Yggdrasil
{
    class BranchRedundancyTest
    {
        public uint Redundancy
        {
            get
            {
                return 0;
            }
        }

        public void Reset( Branch startingBranch )
        {
            m_RemainingCoverage = new List<BranchBounds>();
            m_RemainingCoverage.Add(new BranchBounds(startingBranch));
        }

        public bool Apply( Branch coverageBranch )
        {
            BranchBounds newBounds = new BranchBounds(coverageBranch);

            // The count may change as we process the entries so we are only interested in the original count
            int count = m_RemainingCoverage.Count;

            while(count > 0)
            {
                BranchBounds oldBounds = m_RemainingCoverage[0];
                m_RemainingCoverage.RemoveAt(0);
            }

            return (m_RemainingCoverage.Count == 0);
        }

        private struct BranchBounds
        {
            public BranchBounds( Branch branch )
            {
                left = branch.GlobalCoordinateWest;
                right = branch.GlobalCoordinateEast;
                top = branch.GlobalCoordinateNorth;
                bottom = branch.GlobalCoordinateSouth;
            }

            public float left;
            public float right;
            public float top;
            public float bottom;
        }

        List<BranchBounds> m_RemainingCoverage;
    }
}
