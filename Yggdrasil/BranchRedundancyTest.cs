using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Yggdrasil
{
    class BranchRedundancyTest
    {
		// Add a new branch
		// Returns true if the added branch is not redundant and merges it to current set
		// Returns false if the added branch is redundant
		public bool Add(BranchInfo branch)
		{
			List<BranchBounds> newBranchList = new List<BranchBounds>();
			newBranchList.Add(new BranchBounds(branch));

			foreach(BranchBounds bounds in m_MergedBranches)
			{
				// Do an intersection test
				if(bounds.Merge(newBranchList))
				{
					// Nothing remains in our list, so branch was redundant
					return false;
				}
			}

			// Add what is left to the merged list
			m_MergedBranches.AddRange(newBranchList);

			return true;
		}

        //public uint Redundancy
        //{
        //    get
        //    {
        //        return 0;
        //    }
        //}

        //public void Reset( Branch startingBranch )
        //{
        //    m_RemainingCoverage = new List<BranchBounds>();
        //    m_RemainingCoverage.Add(new BranchBounds(startingBranch));
        //}

        //public bool Apply( Branch coverageBranch )
        //{
        //    BranchBounds newBounds = new BranchBounds(coverageBranch);

        //    // The count may change as we process the entries so we are only interested in the original count
        //    int count = m_RemainingCoverage.Count;

        //    while(count > 0)
        //    {
        //        BranchBounds oldBounds = m_RemainingCoverage[0];
        //        m_RemainingCoverage.RemoveAt(0);
        //    }

        //    return (m_RemainingCoverage.Count == 0);
        //}

        private struct BranchBounds
        {
            public BranchBounds(BranchInfo branch )
            {
                left = branch.GlobalCoordinate.West;
                right = branch.GlobalCoordinate.East;
                top = branch.GlobalCoordinate.North;
                bottom = branch.GlobalCoordinate.South;
            }

			// Returns true if given list was merged to a set of zero entries
			public bool Merge(List<BranchBounds> newBranch)
			{
				// Compare each entry and update the given list

				List<BranchBounds> updatedList = new List<BranchBounds>();

				foreach (BranchBounds branch in newBranch)
				{
					if(this.Intersects(branch))
					{
						AddHorizontalRemainders(branch, updatedList);
						AddVerticalRemainders(branch, updatedList);
					}
				}

				return (updatedList.Count == 0);
			}

			public bool Intersects( BranchBounds branch )
			{
				// Check for horizontal intersection
				if( branch.left < this.right && branch.right > this.left )
				{
					// Check for vertical intersection
					if (branch.top > this.bottom && branch.bottom < this.top)
					{
						return true;
					}
				}

				return false;
			}

			public void AddHorizontalRemainders(BranchBounds branch, List<BranchBounds> remainderList)
			{
				if(branch.left < this.left)
				{
					BranchBounds newBounds = new BranchBounds();
					newBounds.left = branch.left;
					newBounds.top = branch.top;
					newBounds.bottom = branch.bottom;
					newBounds.right = this.left;
					remainderList.Add(newBounds);
				}
				if(branch.right > this.right)
				{
					BranchBounds newBounds = new BranchBounds();
					newBounds.left = this.right;
					newBounds.top = branch.top;
					newBounds.bottom = branch.bottom;
					newBounds.right = branch.right;
					remainderList.Add(newBounds);
				}
			}

			public void AddVerticalRemainders(BranchBounds branch, List<BranchBounds> remainderList)
			{
				if (branch.top > this.top)
				{
					BranchBounds newBounds = new BranchBounds();
					newBounds.top = branch.top;
					newBounds.left = (branch.left > this.left) ? branch.left : this.left;
					newBounds.bottom = this.top;
					newBounds.right = (branch.right < this.right) ? branch.right : this.right;
					remainderList.Add(newBounds);
				}
				if (branch.bottom < this.bottom)
				{
					BranchBounds newBounds = new BranchBounds();
					newBounds.top = branch.bottom;
					newBounds.left = (branch.left > this.left) ? branch.left : this.left;
					newBounds.bottom = branch.bottom;
					newBounds.right = (branch.right < this.right) ? branch.right : this.right;
					remainderList.Add(newBounds);
				}
			}

			public double left;
            public double right;
            public double top;
            public double bottom;
        }

		List<BranchBounds> m_MergedBranches = new List<BranchBounds>();
	}
}
