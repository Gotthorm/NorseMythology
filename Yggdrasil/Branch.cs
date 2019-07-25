using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Yggdrasil
{
    // This represents a single imported data element
    class Branch
    {
        // This should end up in a more global location?
        public enum Type
        {
            Invalid,
            Elevation
        }

        private uint m_Version = 0;

        private string m_Remarks = "";
        private string m_SourceURL = "";

        private int m_ElevationMinimum = 0;
        private int m_ElevationMaximum = 0;
        private int m_ImageWidth = 0;
        private int m_ImageHeight = 0;

        
        private float m_GlobalCoordinateWest = 0.0f;
        private float m_GlobalCoordinateEast = 0.0f;
        private float m_GlobalCoordinateNorth = 0.0f;
        private float m_GlobalCoordinateSouth = 0.0f;

        private Type m_Type = Type.Invalid;

        private byte[] m_ImageData = null;
    }
}
