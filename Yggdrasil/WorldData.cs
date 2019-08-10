using ImageMagick;
using System;
using System.Collections.Generic;
using System.Data;
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

		public WorldData()
		{
			m_BranchDataTable.Columns.Add("Guid", typeof(string));
			m_BranchDataTable.Columns.Add("Last Modified", typeof(string));
			m_BranchDataTable.Columns.Add("Status", typeof(string));
			m_BranchDataTable.Columns.Add("Original File Name", typeof(string));
			m_BranchDataTable.Columns.Add("Remarks", typeof(string));

			Bitmap bmp = new Bitmap(1024, 1024);
			using (Graphics gr = Graphics.FromImage(bmp))
			{
				gr.Clear(Color.FromKnownColor(KnownColor.Window));
			}
			m_Image = bmp;
		}

		public int Width { get { return m_Width; } }

		public int Height { get { return m_Height; } }

		//public Image Image { get { return m_Image; } }

		// Load a yggdrasil file.
		public bool Load(string filePath)
		{
			// Erases all loaded data.

			// The data loaded will be a table of data elements that will be used 
			// to generate a live copy of world data

			m_Width = 0;
			m_Height = 0;
			//m_Image = null;
			m_FilePath = "";
			m_FileCreated = false;

			if (File.Exists(filePath))
			{
				//reading from the file
				try
				{
					using (FileStream stream = new FileStream(filePath, FileMode.Open))
					{
						using (BinaryReader reader = new BinaryReader(stream))
						{
							if (Yggdrasil_FileSignature != reader.ReadUInt32())
							{
								throw new Exception();
							}

							if (Yggdrasil_Version != reader.ReadUInt32())
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

					m_BranchDirectory = Path.Combine(Path.GetDirectoryName(m_FilePath), Path.GetFileNameWithoutExtension(m_FilePath));

					m_FileCreated = true;

					return Rebuild();
				}
				catch (Exception)
				{
					// Abort
				}
			}

			return false;
		}

		// Save a yggdrasil file
		public bool Save()
		{
			// If we have not created the save file, do it now
			if (m_FileCreated == false)
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

							string folderName = Path.GetFileNameWithoutExtension(m_FilePath);

							m_BranchDirectory = Path.Combine(Path.GetDirectoryName(m_FilePath), folderName);

							// Delete the existing branches
							if (Directory.Exists(m_BranchDirectory))
							{
								if (MessageBox.Show("Confirm deletion of folder: " + folderName, "Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation) == DialogResult.No)
								{
									// Abort the save
									return false;
								}

								// You cant delete and immediately create a directory with the same name
								// So we rename it first.
								string tempDirectoryName = m_BranchDirectory + "_temp";
								Directory.Move(m_BranchDirectory, tempDirectoryName);
								Directory.Delete(tempDirectoryName, true);
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
							writer.Write(Yggdrasil_FileSignature);
							writer.Write(Yggdrasil_Version);
							writer.Write(m_Branches.Count);

							foreach (Guid branchGUID in m_Branches)
							{
								writer.Write(branchGUID.ToByteArray());
							}

							writer.Close();
						}
					}
				}
				catch (Exception)
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

			if (newBranch.ImportHeightMapImage(filePath))
			{
				// Prompt user for additional information
				FormBranch branchDialogBox = new FormBranch(newBranch);

				if (branchDialogBox.ShowDialog() == DialogResult.OK)
				{
					// Create and save new branch file
					newBranch.Save(m_BranchDirectory);

					// Import branch data into current world data
					m_Branches.Add(newBranch.GUID);
					m_Dirty = true;

					// Update the yggdrasil file
					if (Save())
					{
						// Update the world data
						return Rebuild();
					}
				}
			}

			return false;
		}

		// Exports the world data in a binary format that contains no external data references
		public bool Export(string filePath)
		{
			return false;
		}

		public DataTable BranchDataTable
		{
			get
			{
				return m_BranchDataTable;
			}
		}

		public PictureBox ImageDisplayControl
		{
			set
			{
				m_ImageDisplay = value;
			}
		}

		// Rebuild data after initial load, and any time a branch is added, removed, or modified
		private bool Rebuild()
		{
			if (m_FileCreated)
			{
				// Reset or empty the branch status table
				m_BranchDataTable.Clear();

				// It would be optimal to merge the branches from the lowest resolution to the highest
				// This way as each branch is merged it simply overwrites any overlapping areas.
				// So we perform an initial pass where sort the branches.
				List<Branch> sortedBranchList = new List<Branch>();

				// If we have the branches sorted from lowest resolution to highest, we might also detect
				// branches that are completely covered by other branches of equal or higher resolution.
				// These branched could be flagged as "deprecated"

				// When merging two areas together, undefined areas will be set to 0 elevation (sea level)

				// Load each branch
				foreach (Guid branchGuid in m_Branches)
				{
					// Branches start in a "unloaded" state
					Branch branch = new Branch(branchGuid);

					// Lets delay populating the view table until the branch processing is done

					//DataRow newDataRow = m_BranchDataTable.NewRow();
					//newDataRow["Guid"] = branchGuid.ToString();

					if (branch.Load(m_BranchDirectory))
					{
						// Branch is now in a "loaded" state
						
						// Mark the status as loaded
						//newDataRow["Status"] = "Processing";

						// Extract any other info the branch status table requires (modification date)
						//newDataRow["Last Modified"] = branch.LastModified;

						//newDataRow["Original File Name"] = branch.OriginalFileName;

						//newDataRow["Remarks"] = branch.Remarks;

						sortedBranchList.Add(branch);

						// Use Image Magick to extract the payload and merge it into display
						//try
						//{
						//	using (MagickImage image = new MagickImage(branch.ImageData))
						//	{
						//		// Temp to display image
						//		m_ImageDisplay.Image = image.ToBitmap();
						//		m_ImageDisplay.Location = new Point();
						//		m_Width = image.Width;
						//		m_Height = image.Height;

						//		Console.WriteLine("I can break here!");
						//	}
						//}
						//catch(Exception)
						//{
						//	// Image data corrupted/invalid?
						//	newDataRow["Status"] = "Data Error";
						//}
					}
					else
					{
						// Branch is in a "load failure" state

						// Mark the status of the branch as load failure
						// TODO: If we want to have more details of the failure
						// we could add an error state to the branch that could be read here.
						// Mark the status as loaded

						//newDataRow["Status"] = "Load Failure";
					}

					//branch.TableIndex = m_BranchDataTable.Rows.Count;

					//m_BranchDataTable.Rows.Add(newDataRow);
				}

				// Sort the brances by resolution
				sortedBranchList.Sort(Branch.CompareBranchesByResolution);

				// Identify branches that can be deprecated
				// 

				// Populate the view with the branches
				foreach(Branch branch in sortedBranchList)
				{
					DataRow newDataRow = m_BranchDataTable.NewRow();
					newDataRow["Guid"] = branch.GUID.ToString();
					
					switch(branch.State)
					{
						case Branch.LoadState.Loaded:
						case Branch.LoadState.Deprecated:
							{
								newDataRow["Status"] = (branch.State == Branch.LoadState.Loaded) ? "Loaded" : "Deprecated";

								newDataRow["Last Modified"] = branch.LastModified;

								newDataRow["Original File Name"] = branch.OriginalFileName;

								newDataRow["Remarks"] = branch.Remarks;
							}
							break;
						case Branch.LoadState.LoadFailure:
						default:
							{
								newDataRow["Status"] = "Load Failure";
							}
							break;
					}

					m_BranchDataTable.Rows.Add(newDataRow);
				}

				return true;
			}

			return false;
		}

		// 19 66 19 68
		private const UInt32 Yggdrasil_FileSignature = 0x012C0490;
		private const UInt32 Yggdrasil_Version = 0x00000001;
		private bool m_FileCreated = false;
		private bool m_Dirty = true;

		// A list of project relative branch paths that are currently loaded
		private List<Guid> m_Branches = new List<Guid>();

		private DataTable m_BranchDataTable = new DataTable();

		private string m_FilePath = "";
		private string m_BranchDirectory = "";
        private int m_Width;
        private int m_Height;
		private Image m_Image = null;
		private PictureBox m_ImageDisplay = null;
    }
}
