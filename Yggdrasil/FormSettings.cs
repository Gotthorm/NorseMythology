using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Yggdrasil
{
    public partial class FormSettings : Form
    {
        public string FolderPath
        {
            get
            {
                return m_FolderPath;
            }
        }

        public FormSettings(string folderPath)
        {
            InitializeComponent();

            m_FolderPath = folderPath;

            this.textBoxWorldDataPath.Text = folderPath;
        }

        private void buttonBrowse_Click( object sender, EventArgs e )
        {
            using ( FolderBrowserDialog folderBrowserDialog = new FolderBrowserDialog() )
            {
                // Default to the currently set path
                folderBrowserDialog.SelectedPath = m_FolderPath;

                DialogResult dialogResult = folderBrowserDialog.ShowDialog();

                if ( DialogResult.OK == dialogResult )
                {
                    // Update the currently set path
                    m_FolderPath = folderBrowserDialog.SelectedPath;
                    this.textBoxWorldDataPath.Text = m_FolderPath;
                }
            }
        }

        private string m_FolderPath = "";
    }
}
