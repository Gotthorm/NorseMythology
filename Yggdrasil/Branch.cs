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
	public class Branch : BranchInfo
	{
		public Branch()
		{
		}

		public Branch(Guid newGuid) : base(newGuid)
		{
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

        public override bool Load(string path)
        {
            try
            {
                string filePath = Path.Combine(path, m_GUID.ToString() + Branch_FileExtension);

                using (FileStream stream = new FileStream(filePath, FileMode.Open))
                {
                    using (BinaryReader reader = new BinaryReader(stream))
                    {
                        // Read the info
                        Read(reader);

                        // Read the image data
                        int imageSize = reader.ReadInt32();
                        m_ImageData = reader.ReadBytes(imageSize);

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

        private byte[] m_ImageData = null;
	}
}
