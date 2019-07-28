using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ImageMagick;

namespace Yggdrasil
{
    // This represents a single imported data element
    public class Branch
    {
        // This should end up in a more global location?
        public enum LayerType
        {
            Invalid,
            Elevation
        };

        public Branch()
        {

        }

        public int ImageWidth
        {
            get
            {
                return m_ImageWidth;
            }
        }

        public int ImageHeight
        {
            get
            {
                return m_ImageHeight;
            }
        }

        public int ImageBitDepth
        {
            get
            {
                return m_ImageBitDepth;
            }
        }

        public uint Version
        {
            get
            {
                return m_Version;
            }
        }

        public string SourcePath
        {
            get
            {
                return m_OriginalPath;
            }
        }

        public int ElevationMin
        {
            get
            {
                return m_ElevationMinimum;
            }

            set
            {
                m_ElevationMinimum = value;
            }
        }

        public int ElevationMax
        {
            get
            {
                return m_ElevationMaximum;
            }

            set
            {
                m_ElevationMaximum = value;
            }
        }

        public string Remarks
        {
            get
            {
                return m_Remarks;
            }

            set
            {
                m_Remarks = value;
            }
        }

        public float GlobalCoordinateWest
        {
            get
            {
                return m_GlobalCoordinateWest;
            }

            set
            {
                m_GlobalCoordinateWest = value;
            }
        }

        public float GlobalCoordinateEast
        {
            get
            {
                return m_GlobalCoordinateEast;
            }

            set
            {
                m_GlobalCoordinateEast = value;
            }
        }

        public float GlobalCoordinateNorth
        {
            get
            {
                return m_GlobalCoordinateNorth;
            }

            set
            {
                m_GlobalCoordinateNorth = value;
            }
        }

        public float GlobalCoordinateSouth
        {
            get
            {
                return m_GlobalCoordinateSouth;
            }

            set
            {
                m_GlobalCoordinateSouth = value;
            }
        }

		public LayerType Type
		{
			get
			{
				return m_Type;
			}

			set
			{
				m_Type = value;
			}
		}

        public bool LoadImage(string filePath)
        {
            // Load the image "as is"
            if (File.Exists(filePath))
            {
                m_OriginalPath = filePath;

                try
                {
                    using (FileStream fileStream = new FileStream(filePath, FileMode.Open))
                    {
                        int fileLength = (int)fileStream.Length;
                        m_ImageData = new byte[fileLength];
                        fileStream.Read(m_ImageData, 0, fileLength);

                        // Load the image into ImageMagick to extract some basic information
                        using (MagickImage image = new MagickImage(m_ImageData))
                        {
                            m_ImageBitDepth = image.BitDepth();

                            m_ImageWidth = image.Width;
                            m_ImageHeight = image.Height;
                        }

                        return true;
                    }
                }
                catch (Exception)
                {

                }
            }

            return false;
        }

        private uint m_Version = 1;
 
        private string m_Remarks = "";

        private int m_ElevationMinimum = 0;
        private int m_ElevationMaximum = 0;
        private int m_ImageWidth = 0;
        private int m_ImageHeight = 0;
        private int m_ImageBitDepth = 0;
        
        private float m_GlobalCoordinateWest = 0.0f;
        private float m_GlobalCoordinateEast = 0.0f;
        private float m_GlobalCoordinateNorth = 0.0f;
        private float m_GlobalCoordinateSouth = 0.0f;

        private LayerType m_Type = LayerType.Invalid;

        private string m_OriginalPath = "";

        // m_GUID?
        
        private byte[] m_ImageData = null;
    }
}
