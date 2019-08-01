using ImageMagick;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Yggdrasil
{
    class WorldData
    {
        //public string FilePath { get { return m_FilePath; } }

		//public string BranchDirectory { get { return m_BranchDirectory; } }

        public int Width { get { return m_Width; } }

        public int Height { get { return m_Height; } }

        public Image Image { get { return m_Image; } }

        // Load a yggdrasil file.
        public bool Load(string filePath)
        {
            // Erases all loaded data.

            // The data loaded will be a table of data elements that will be used 
            // to generate a live copy of world data

            m_Width = 0;
            m_Height = 0;
            m_Image = null;
            m_FilePath = "";

            if ( File.Exists( filePath ) )
            {
				//reading from the file
				try
				{
					using (FileStream stream = new FileStream(filePath, FileMode.Open))
					{
						using (BinaryReader reader = new BinaryReader(stream))
						{
							if(m_FileSignature != reader.ReadUInt32())
							{
								throw new Exception();
							}

							if(m_Version != reader.ReadUInt32())
							{
								throw new Exception();
							}

							int branchCount = reader.ReadInt32();

							for (int count = 0; count < branchCount; ++count)
							{
								m_Branches.Add(new Guid(reader.ReadBytes(16)));
							}

							reader.Close();

							m_FilePath = filePath;
						}
					}
				}
				catch (Exception)
				{
					// Abort
					return false;
				}
			}

			return true;
        }

        // Save a yggdrasil file
        public bool Save()
        {
			// If we have not created the save file, do it now
			if(m_FileCreated == false)
			{
				using (SaveFileDialog openFileDialog = new SaveFileDialog())
				{
					openFileDialog.InitialDirectory = Properties.Settings.Default.WorldDataFolder;
					openFileDialog.Filter = "txt files (*.yggdrasil)|*.yggdrasil|All files (*.*)|*.*";
					openFileDialog.FilterIndex = 1;
					openFileDialog.RestoreDirectory = true;

					try
					{
						if (openFileDialog.ShowDialog() == DialogResult.OK)
						{
							m_FilePath = openFileDialog.FileName;

							// If the file already exists we will prompt the user
							// This is important because we are going to destroy all existing branches
							if (File.Exists(m_FilePath))
							{
								if (MessageBox.Show("Are you sure? This will delete branch files.", "Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation) == DialogResult.No)
								{
									// Abort the save
									return false;
								}

								m_BranchDirectory = Path.Combine(Path.GetDirectoryName(m_FilePath), Path.GetFileNameWithoutExtension(m_FilePath));

								// Delete the existing branches
								if (Directory.Exists(m_BranchDirectory))
								{
									// You cant delete and immediately create a directory with the same name
									// So we rename it first.
									string tempDirectoryName = m_BranchDirectory + "_temp";
									Directory.Move(m_BranchDirectory, tempDirectoryName);
									Directory.Delete(tempDirectoryName, true);
								}
							}
						}

						Directory.CreateDirectory(m_BranchDirectory);
					}
					catch (Exception)
					{
						// Abort
						return false;
					}
				}
				m_FileCreated = true;
			}

			if (m_Dirty)
			{
				try
				{
					using (FileStream stream = new FileStream(m_FilePath, FileMode.Create))
					{
						using (BinaryWriter writer = new BinaryWriter(stream))
						{
							writer.Write(m_FileSignature);
							writer.Write(m_Version);
							writer.Write(m_Branches.Count);

							foreach (Guid branchGUID in m_Branches)
							{
								writer.Write(branchGUID.ToByteArray());
							}

							writer.Close();
						}
					}
				}
				catch(Exception)
				{
					// Abort
					return false;
				}

				m_Dirty = false;
			}

			return true;
        }

        // Import a single image, merging it into the current world data
        public bool ImportImage(string filePath)
        {
			// Create a new Branch instance
			Branch newBranch = new Branch();

			if (newBranch.LoadImage(filePath))
			{
				// Prompt user for additional information
				FormBranch branchDialogBox = new FormBranch(newBranch);

				if (branchDialogBox.ShowDialog() == DialogResult.OK)
				{
					// Create and save new branch file
					newBranch.Save(m_BranchDirectory);

					// Import branch data into current world data
					m_Branches.Add(newBranch.GUID);
				}
			}

			if (File.Exists(filePath))
            {
                // Attempt to import the given file as image data
                try
                {
                    using (MagickImage image = new MagickImage(filePath))
                    {
                        // Perform any extra verification here?

                        // The image has been loaded

                        // Extract the needed information

                        // Add the image to our yggdrasil data

                        int bitDepth = image.BitDepth();

                        m_Width = image.Width;
                        m_Height = image.Height;

                        m_Image = image.ToBitmap();

                        if (bitDepth == 8)
                        {
                            Console.WriteLine("8 Bit");
                        }
                    }

                    return true;
                }
                catch (Exception)
                {
                    // Catch everything and ignore
                }
            }

            return false;
        }

        // Exports the world data in a binary format that contains no external data references
        public bool Export(string filePath)
        {
            return false;
        }

		// 19 66 19 68
		private UInt32 m_FileSignature = 0x012C0490;
		private UInt32 m_Version = 0x00000001;
		private bool m_FileCreated = false;
		private bool m_Dirty = true;

		// A list of project relative branch paths that are currently loaded
		private List<Guid> m_Branches = new List<Guid>();

		private string m_FilePath = "";
		private string m_BranchDirectory = "";
        private int m_Width;
        private int m_Height;
        private Image m_Image = null;
    }
}
