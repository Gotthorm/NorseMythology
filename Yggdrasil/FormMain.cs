using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace Yggdrasil
{
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();
            this.AllowDrop = true;

            this.m_DataFolderPath = Properties.Settings.Default.WorldDataFolder;

            // Cache a reference to the picture box control that we will display the loaded data with
            foreach ( Control mainFormControl in this.Controls )
            {
                if( mainFormControl is SplitContainer )
                {
                    SplitContainer splitContainer = mainFormControl as SplitContainer;

                    // Set the parent panel to the picturebox as being scrollable
                    splitContainer.Panel1.AutoScroll = true;

                    foreach ( Control splitContainerControl in splitContainer.Panel1.Controls )
                    {
                        m_PictureBox = splitContainerControl as PictureBox;

                        if ( null != m_PictureBox )
                        {
                            break;
                        }
                    }

                    // Abort from main form children control loop
                    break;
                }
            }
        }

        private void settingsToolStripMenuItem_Click( object sender, EventArgs e )
        {
            FormSettings formSettings = new FormSettings( m_DataFolderPath );

            DialogResult dialogresult = formSettings.ShowDialog();

            if ( dialogresult == DialogResult.OK )
            {
                // Validate new settings?

                this.m_DataFolderPath = formSettings.FolderPath;

                Properties.Settings.Default.WorldDataFolder = m_DataFolderPath;
            }
            // We currently do not care
            //else if ( dialogresult == DialogResult.Cancel )
            //{

            //}

            formSettings.Dispose();
        }

        private void aboutToolStripMenuItem_Click( object sender, EventArgs e )
        {
            AboutBox aboutBox = new AboutBox();

            DialogResult dialogresult = aboutBox.ShowDialog();

            // We currently do not care
            //if ( dialogresult == DialogResult.OK )
            //{
            //    Console.WriteLine( "You clicked OK" );
            //}

            aboutBox.Dispose();
        }

        private string m_DataFolderPath;
        private WorldData m_Data = null;
        private PictureBox m_PictureBox = null;

        private void FormMain_FormClosed( object sender, FormClosedEventArgs e )
        {
            Properties.Settings.Default.Save();
        }

        private void FormMain_Load( object sender, EventArgs e )
        {
        }

        private void FormMain_DragEnter( object sender, DragEventArgs e )
        {
            if ( e.Data.GetDataPresent( DataFormats.FileDrop ) )
            {
                e.Effect = DragDropEffects.Copy;
            }
        }

        private void FormMain_DragDrop( object sender, DragEventArgs e )
        {
            string[] files = (string[])e.Data.GetData( DataFormats.FileDrop );

            // We only support dragging a single data file as input
            if ( files.Length == 1 )
            {
                LoadWorld( files[ 0 ] );
            }
        }

        private void LoadWorld( string filePath )
        {
            if ( null != m_Data )
            {
                // Unload old world
                m_Data = null;
            }

            // Load new world
            m_Data = new WorldData();

            if( false == m_Data.Load( filePath ) )
            {
                // We report an error and leave the default empty world as being loaded
                string message = "Failed to load world";
                string title = "Failure";
                MessageBoxButtons buttons = MessageBoxButtons.OK;
                DialogResult result = MessageBox.Show( message, title, buttons, MessageBoxIcon.Warning );
            }

            m_PictureBox.Image = m_Data.Image;

            // Based on the current zoom value
            m_PictureBox.Width = m_Data.Width;
            m_PictureBox.Height = m_Data.Height;
        }

        private void ImportImage(string filePath)
        {
            // Create a default empty world if one isnt already loaded?
            if (null == m_Data)
            {
                // Load a new world
                m_Data = new WorldData();

				// Save the default world data to generate the location
				if(m_Data.Save() == false)
				{
					// Something went wrong so we will not proceed
					// The world data should report its own error
					return;
				}
            }

            // Create a GUID

            // Create a new Branch instance
            Branch newBranch = new Branch();

            if(newBranch.LoadImage(filePath))
            {
                // Prompt user for additional information
                FormBranch branchDialogBox = new FormBranch(newBranch);

                if (branchDialogBox.ShowDialog() == DialogResult.OK)
                {
                    // Create and save new branch file

                    // Import branch data into current world data
                }
            }

        }

        private void numericUpDownZoom_KeyPress( object sender, KeyPressEventArgs e )
        {
            if ( !char.IsControl( e.KeyChar ) && !char.IsDigit( e.KeyChar ) )
            {
                e.Handled = true;
            }
        }

        private void numericUpDownZoom_ValueChanged( object sender, EventArgs e )
        {
            NumericUpDown numericControl = sender as NumericUpDown;

            if( null != numericControl )
            {
                decimal zoomPercent = numericControl.Value / 100.0m;
                decimal width = m_Data.Width * zoomPercent;
                decimal height = m_Data.Height * zoomPercent;

                m_PictureBox.Width = Decimal.ToInt32(width);
                m_PictureBox.Height = Decimal.ToInt32(height);
            }
        }

        private void ImportImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.InitialDirectory = m_DataFolderPath;
                openFileDialog.Filter = "image files (*.png)|*.png|All files (*.*)|*.*";
                openFileDialog.FilterIndex = 1;
                openFileDialog.RestoreDirectory = true;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    ImportImage(openFileDialog.FileName);
                }
            }
        }

        private void OpenWorldToolStripMenuItem_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.InitialDirectory = m_DataFolderPath;
                openFileDialog.Filter = "txt files (*.yggdrasil)|*.yggdrasil|All files (*.*)|*.*";
                openFileDialog.FilterIndex = 1;
                openFileDialog.RestoreDirectory = true;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    LoadWorld(openFileDialog.FileName);
                }
            }
        }
    }
}
