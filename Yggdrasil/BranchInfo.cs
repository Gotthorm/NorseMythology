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
	public class BranchInfo
	{
		// This should end up in a more global location?
		public enum LayerType
		{
			Invalid,
			Elevation,
		};

		public enum LoadState
		{
			None,
			LoadFailure,
			Deprecated,
			Loaded,
		};

        protected BranchInfo()
		{
			m_GUID = Guid.NewGuid();
		}

		public BranchInfo(Guid newGuid)
		{
			m_GUID = newGuid;
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
				return Branch_Version;
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

        public Coordinate GlobalCoordinate
        {
            get
            {
                return m_GlobalCoordinate;
            }

            set
            {
                m_GlobalCoordinate = value;
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

		public Guid GUID
		{
			get
			{
				return m_GUID;
			}
		}

		public string LastModified
		{
			get
			{
				return m_LastModified.ToString();
			}
		}

		public string OriginalFileName
		{
			get
			{
				string fileName = "";
				try
				{
					fileName = Path.GetFileName(m_OriginalPath);
				}
				catch (Exception)
				{

				}
				return fileName;
			}
		}

        public float ResolutionX
        {
            get
            {
                return m_ResolutionX;
            }
        }

        public float ResolutionY
        {
            get
            {
                return m_ResolutionY;
            }
        }

        public LoadState State
        {
            get
            {
                return m_State;
            }
        }

        public virtual bool Load(string path)
		{
			try
			{
				string filePath = Path.Combine(path, m_GUID.ToString() + Branch_FileExtension);

				using (FileStream stream = new FileStream(filePath, FileMode.Open))
				{
					using (BinaryReader reader = new BinaryReader(stream))
					{
                        Read(reader);

						m_State = LoadState.Loaded;

						return true;
					}
				}
			}
			catch (Exception)
			{
			}

			m_State = LoadState.LoadFailure;

			return false;
		}

        public void Deprecate()
        {
            m_State = LoadState.Deprecated;
        }

		public void LoadFail()
		{
			m_State = LoadState.LoadFailure;
		}

        protected void Read(BinaryReader reader)
        {
            if (Branch_FileSignature != reader.ReadUInt32())
            {
                throw new Exception();
            }

            if (Version != reader.ReadUInt32())
            {
                throw new Exception();
            }

            m_LastModified = DateTime.Parse(reader.ReadString());

            uint layerType = reader.ReadUInt32();
            if (Enum.IsDefined(typeof(LayerType), (LayerType)layerType) == false)
            {
                throw new Exception();
            }
            m_Type = (LayerType)layerType;

            m_ElevationMinimum = reader.ReadInt32();
            m_ElevationMaximum = reader.ReadInt32();
            m_ImageWidth = reader.ReadInt32();
            m_ImageHeight = reader.ReadInt32();
            m_ImageBitDepth = reader.ReadInt32();

            m_GlobalCoordinate.North = reader.ReadDouble();
            m_GlobalCoordinate.South = reader.ReadDouble();
            m_GlobalCoordinate.West = reader.ReadDouble();
            m_GlobalCoordinate.East = reader.ReadDouble();

            m_OriginalPath = reader.ReadString();
            m_Remarks = reader.ReadString();

            // Determine the width of the map in meters
            float metersWidth = 0.0f;
            float metersHeight = 0.0f;
            Coordinate simple = new Coordinate();
            if( Utility.GlobalCoordinateToMeters(m_GlobalCoordinate, ref simple) )
            {
                metersWidth = Convert.ToSingle(Math.Abs(simple.East - simple.West));
                metersHeight = Convert.ToSingle(Math.Abs(simple.North - simple.South));
            }

            // Calculate the resolution of the data.  For now I am assuming the target will be 1 meter resolution
            m_ResolutionX = metersWidth / (m_ImageWidth * Utility.MetersPerPixel);
            m_ResolutionY = metersHeight / (m_ImageHeight * Utility.MetersPerPixel);
        }

        public static int CompareBranchesByResolution(BranchInfo alpha, BranchInfo beta)
		{
			// Not expecting a big discrepency between these two values

			if (alpha == null)
			{
				if (beta == null)
				{
					// If alpha is null and beta is null, they are equal. 
					return 0;
				}
				else
				{
					// If alpha is null and beta is not null, beta
					// is greater. 
					return 1;
				}
			}
			else
			{
				// If alpha is not null and beta is null, alpha is greater
				if (beta == null)
				{
					return -1;
				}
				else
				{
					// If alpha is not null and beta is not null, compare the resolutions of the two branches.
					if( alpha.ResolutionX == beta.ResolutionX )
					{
						if (alpha.ResolutionY == beta.ResolutionY)
						{
							return 0;
						}
						else
						{
							return (alpha.ResolutionY > beta.ResolutionY) ? -1 : 1;
						}
					}
					else
					{
						return (alpha.ResolutionX > beta.ResolutionX) ? -1 : 1;
					}
				}
			}
		}

        #region Non Serializable Data

        protected float m_ResolutionX = 0.0f;
        protected float m_ResolutionY = 0.0f;
        protected LoadState m_State = LoadState.None;

		#endregion

		#region Serializable Data

		// 19 90 19 90
		protected const UInt32 Branch_FileSignature = 0x012FAE26;
        protected const UInt32 Branch_Version = 0x00000001;
        protected const string Branch_FileExtension = ".branch";
        protected const string DateTimeOffsetFormatString = "yyyy-MM-ddTHH:mm:sszzz";

        protected Guid m_GUID;
        protected DateTime m_LastModified = new DateTime();

        protected int m_ImageWidth = 0;
        protected int m_ImageHeight = 0;
        protected int m_ImageBitDepth = 0;

        // We will use a default 60km area in the Mammoth Lakes, CA area
        protected string m_Remarks = "60km area in the Mammoth Lakes area";
        protected int m_ElevationMinimum = 1172;
        protected int m_ElevationMaximum = 3998;
        protected Coordinate m_GlobalCoordinate = new Coordinate(38.081909, 37.542925, -119.602474, -118.917722);

        protected LayerType m_Type = LayerType.Invalid;

        protected string m_OriginalPath = "";
        
		#endregion
	}
}
