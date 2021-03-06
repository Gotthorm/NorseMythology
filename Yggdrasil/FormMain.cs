﻿using System;
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

            // Cache references to any controls that we will need
            foreach ( Control mainFormControl in this.Controls )
            {
				// We expect to find a tab control in the root
                if( mainFormControl is TabControl )
                {
					TabControl tabControl = (mainFormControl as TabControl);

					foreach(TabPage tabPage in tabControl.TabPages)
					{
						if(m_BranchGridView == null && tabPage.Name == "tabPageBranches")
						{
							foreach (Control tabPageChildControl in tabPage.Controls)
							{
								// We expect to find a DataGridView
								if(tabPageChildControl is DataGridView)
								{
									m_BranchGridView = (tabPageChildControl as DataGridView);

									// Abort from tab page children control loop
									break;
								}
							}
						}

						if(m_PictureBox == null && tabPage.Name == "tabPageView")
						{
							foreach (Control tabPageChildControl in tabPage.Controls)
							{
								// We expect to find a SplitControl
								if (tabPageChildControl is SplitContainer)
								{
									SplitContainer splitContainer = tabPageChildControl as SplitContainer;

									// Set the parent panel to the picturebox as being scrollable
									splitContainer.Panel1.AutoScroll = true;

									foreach (Control splitContainerControl in splitContainer.Panel1.Controls)
									{
										m_PictureBox = splitContainerControl as PictureBox;

										if (null != m_PictureBox)
										{
											break;
										}
									}

									// Abort from tab page children control loop
									break;
								}
							}
						}
					}
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
		private DataGridView m_BranchGridView = null;

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

		private void LoadWorld(string filePath)
		{
			// Load new world
			NewWorld();

			if (false == m_Data.Load(filePath))
			{
				// We report an error and leave the default empty world as being loaded
				string message = "Failed to load world";
				string title = "Failure";
				MessageBoxButtons buttons = MessageBoxButtons.OK;
				MessageBox.Show(message, title, buttons, MessageBoxIcon.Warning);
			}
		}

		private void NewWorld()
		{
			// Load new world
			m_Data = new WorldData();

			m_BranchGridView.DataSource = m_Data.BranchDataTable;
			m_Data.ImageDisplayControl = m_PictureBox;
			//m_PictureBox.Image = m_Data.Image;

			// Based on the current zoom value
			m_PictureBox.Width = m_Data.Width;
			m_PictureBox.Height = m_Data.Height;

			// We cache this to allow us to handle the last column differently
			int columnLastIndex = m_BranchGridView.Columns.Count - 1;
			if (columnLastIndex >= 0)
			{
				for (int columnIndex = 0; columnIndex < columnLastIndex; ++columnIndex)
				{
					m_BranchGridView.Columns[columnIndex].AutoSizeMode = DataGridViewAutoSizeColumnMode.DisplayedCells;
				}
				// Last entry
				m_BranchGridView.Columns[columnLastIndex].AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
			}
		}

		private void ImportImage(string filePath)
        {
            // Create a default empty world if one isnt already loaded?
            if (null == m_Data)
            {
                // Load a new world
                m_Data = new WorldData();

				m_BranchGridView.DataSource = m_Data.BranchDataTable;

				// Save the default world data to generate the location
				if (m_Data.Save() == false)
				{
					// Something went wrong so we will not proceed
					// The world data should report its own error
					return;
				}
			}

			m_Data.ImportImage(filePath);
        }

        //private void numericUpDownZoom_KeyPress( object sender, KeyPressEventArgs e )
        //{
        //    if ( !char.IsControl( e.KeyChar ) && !char.IsDigit( e.KeyChar ) )
        //    {
        //        e.Handled = true;
        //    }
        //}

        //private void numericUpDownZoom_ValueChanged( object sender, EventArgs e )
        //{
        //    NumericUpDown numericControl = sender as NumericUpDown;

        //    if( null != numericControl )
        //    {
        //        decimal zoomPercent = numericControl.Value / 100.0m;
        //        decimal width = m_Data.Width * zoomPercent;
        //        decimal height = m_Data.Height * zoomPercent;

        //        m_PictureBox.Width = Decimal.ToInt32(width);
        //        m_PictureBox.Height = Decimal.ToInt32(height);
        //    }
        //}

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

		private void NewWorldToolStripMenuItem_Click(object sender, EventArgs e)
		{
			// Load new world
			NewWorld();

			// Write to disk
			m_Data.Save();
		}

		private void NumericUpDownZoom_ValueChanged(object sender, EventArgs e)
		{
			NumericUpDown numericControl = sender as NumericUpDown;

			if (null != numericControl)
			{
				decimal zoomPercent = numericControl.Value / 100.0m;
				decimal width = m_Data.Width * zoomPercent;
				decimal height = m_Data.Height * zoomPercent;

				m_PictureBox.Width = Decimal.ToInt32(width);
				m_PictureBox.Height = Decimal.ToInt32(height);
			}
		}

		private void NumericUpDownZoom_KeyPress(object sender, KeyPressEventArgs e)
		{
			if (!char.IsControl(e.KeyChar) && !char.IsDigit(e.KeyChar))
			{
				e.Handled = true;
			}
		}
	}
}
