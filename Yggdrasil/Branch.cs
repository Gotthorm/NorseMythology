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
			Elevation,
		};

		public enum LoadState
		{
			None,
			LoadFailure,
			Deprecated,
			Loaded,
		};

		public Branch()
		{
			m_GUID = Guid.NewGuid();
		}

		public Branch(Guid newGuid)
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

		public byte[] ImageData
		{
			get
			{
				return m_ImageData;
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

		public bool ImportHeightMapImage(string filePath)
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

		public bool Save(string path)
		{
			try
			{
				string filePath = Path.Combine(path, m_GUID.ToString() + Branch_FileExtension);

				using (FileStream stream = new FileStream(filePath, FileMode.Create))
				{
					using (BinaryWriter writer = new BinaryWriter(stream))
					{
						m_LastModified = DateTime.Now;

						writer.Write(Branch_FileSignature);
						writer.Write(Version);
						writer.Write(m_LastModified.ToString(DateTimeOffsetFormatString));
						writer.Write((uint)m_Type);
						writer.Write(m_ElevationMinimum);
						writer.Write(m_ElevationMaximum);
						writer.Write(m_ImageWidth);
						writer.Write(m_ImageHeight);
						writer.Write(m_ImageBitDepth);
						writer.Write(m_GlobalCoordinateWest);
						writer.Write(m_GlobalCoordinateEast);
						writer.Write(m_GlobalCoordinateNorth);
						writer.Write(m_GlobalCoordinateSouth);
						writer.Write(m_OriginalPath);
						writer.Write(m_Remarks);
						writer.Write(m_ImageData.Length);
						writer.Write(m_ImageData);

						writer.Close();
					}
				}
			}
			catch (Exception)
			{
				// Abort
				return false;
			}

			return true;
		}

		public bool Load(string path)
		{
			try
			{
				string filePath = Path.Combine(path, m_GUID.ToString() + Branch_FileExtension);

				using (FileStream stream = new FileStream(filePath, FileMode.Open))
				{
					using (BinaryReader reader = new BinaryReader(stream))
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
						if(Enum.IsDefined(typeof(LayerType), (LayerType)layerType) == false)
						{
							throw new Exception();
						}
						m_Type = (LayerType)layerType;

						m_ElevationMinimum = reader.ReadInt32();
						m_ElevationMaximum = reader.ReadInt32();
						m_ImageWidth = reader.ReadInt32();
						m_ImageHeight = reader.ReadInt32();
						m_ImageBitDepth = reader.ReadInt32();

						m_GlobalCoordinateWest = reader.ReadSingle();
						m_GlobalCoordinateEast = reader.ReadSingle();
						m_GlobalCoordinateNorth = reader.ReadSingle();
						m_GlobalCoordinateSouth = reader.ReadSingle();

						m_OriginalPath = reader.ReadString();
						m_Remarks = reader.ReadString();
						int imageSize = reader.ReadInt32();
						m_ImageData = reader.ReadBytes(imageSize);

						// Determine the width of the map in meters
						double metersWidth = Utility.GlobalCoordinateToMeters(m_GlobalCoordinateNorth,
																				m_GlobalCoordinateWest,
																				m_GlobalCoordinateNorth,
																				m_GlobalCoordinateEast);

						// Determine the width of the map in meters
						double metersHeight = Utility.GlobalCoordinateToMeters(m_GlobalCoordinateNorth,
																				m_GlobalCoordinateWest,
																				m_GlobalCoordinateSouth,
																				m_GlobalCoordinateWest);

						// Calculate the resolution of the data.  For now I am assuming the target will be 1 meter resolution
						m_ResolutionX = (float)metersWidth / m_ImageWidth;
						m_ResolutionY = (float)metersHeight / m_ImageHeight;

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

		public static int CompareBranchesByResolution(Branch alpha, Branch beta)
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

		private float m_ResolutionX = 0.0f;
		private float m_ResolutionY = 0.0f;
		private LoadState m_State = LoadState.None;

		#endregion

		#region Serializable Data

		// 19 90 19 90
		private const UInt32 Branch_FileSignature = 0x012FAE26;
		private const UInt32 Branch_Version = 0x00000001;
		private const string Branch_FileExtension = ".branch";
		private const string DateTimeOffsetFormatString = "yyyy-MM-ddTHH:mm:sszzz";

		private Guid m_GUID;
		private DateTime m_LastModified = new DateTime();

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
        
        private byte[] m_ImageData = null;

		#endregion
	}
}
