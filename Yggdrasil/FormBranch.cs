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
    public partial class FormBranch : Form
    {
        public FormBranch(Branch branchData)
        {
            InitializeComponent();

            // Populate the read only text boxes
            textBox_BranchImagePath.Text = branchData.SourcePath;
            textBox_BranchVersion.Text = branchData.Version.ToString();
            textBox_BranchImageDimensionsWidth.Text = branchData.ImageWidth.ToString();
            textBox_BranchImageDimensionsHeight.Text = branchData.ImageHeight.ToString();
            textBox_BranchImageBitDepth.Text = branchData.ImageBitDepth.ToString();

            m_BranchData = branchData;
        }

        private bool m_UserModifiedElevation = false;
        private bool m_UserModifiedGlobalCoordinates = false;
        private Branch m_BranchData = null;

        private bool ElevationInputIsValid(char newChar, string currentText)
        {
            // Only allow digits
            if (!char.IsControl(newChar) && !char.IsDigit(newChar) && (newChar != '-'))
            {
                return false;
            }

            // Only allow one negative
            if ((newChar == '-') && (currentText.IndexOf('-') > -1))
            {
                return false;
            }

            return true;
        }

        private bool ElevationTextIsValid(string elevationText)
        {
            // If there is a negative it must be in the initial position only
            if (elevationText.IndexOf('-') <= 0)
            {
                return true;
            }

            return false;
        }

        private bool ElevationsAreValid()
        {
            try
            {
                // Convert min and max to integers and ensure min is less than max
                int min = Convert.ToInt32(textBox_BranchElevationMin.Text);
                int max = Convert.ToInt32(textBox_BranchElevationMax.Text);

                // Add some range boundary tests also?

                return min < max;
            }
            catch (Exception)
            {
                // Catch everything
            }

            return false;
        }

        private bool GlobalCoordinateIsValid(string coordinateText)
        {
            // If there is a negative it must be in the initial position only
            if (coordinateText.IndexOf('-') <= 0)
            {
                return true;
            }

            return false;
        }

        private bool GlobalCoordinatesAreValid()
        {
            try
            {
                // Convert global coordinates to floats and ensure thwy are valid
                float north = Convert.ToSingle(textBox_BranchGlobalCoordinatesN.Text);
                float south = Convert.ToSingle(textBox_BranchGlobalCoordinatesS.Text);
                float west = Convert.ToSingle(textBox_BranchGlobalCoordinatesW.Text);
                float east = Convert.ToSingle(textBox_BranchGlobalCoordinatesE.Text);

                // Add some range boundary tests also?

                return (south < north) && (west < east);
            }
            catch (Exception)
            {
                // Catch everything
            }

            return false;
        }

        private bool GlobalCoordinateTextInputIsValid(char newChar, string currentText)
        {
            // Only allow digits
            if (!char.IsControl(newChar) && !char.IsDigit(newChar) && (newChar != '-') && (newChar != '.'))
            {
                return false;
            }

            // Only allow one negative
            if ((newChar == '-') && (currentText.IndexOf('-') > -1))
            {
                return false;
            }

            // only allow one decimal point
            if ((newChar == '.') && (currentText.IndexOf('.') > -1))
            {
                return false;
            }

            return true;
        }


        private void Button_BranchOK_Click(object sender, EventArgs e)
        {
            // Elevation range and global coordinates are mandatory to proceed.
            // so generate a warning if either of them have not been edited.
            if (m_UserModifiedElevation == false)
            {
                if (MessageBox.Show("Did you remember to set the elevation extents?", "Important Question", MessageBoxButtons.YesNo) == DialogResult.No)
                {
                    return;
                }
            }

            if (m_UserModifiedGlobalCoordinates == false)
            {
                if (MessageBox.Show("Did you remember to set the global coordinates?", "Important Question", MessageBoxButtons.YesNo) == DialogResult.No)
                {
                    return;
                }
            }

            // Accept the OK
            this.DialogResult = DialogResult.OK;

            try
            {
                // Update the data
                m_BranchData.ElevationMin = Convert.ToInt32(textBox_BranchElevationMin.Text);
                m_BranchData.ElevationMax = Convert.ToInt32(textBox_BranchElevationMax.Text);
                m_BranchData.GlobalCoordinateEast = Convert.ToSingle(textBox_BranchGlobalCoordinatesE.Text);
                m_BranchData.GlobalCoordinateWest = Convert.ToSingle(textBox_BranchGlobalCoordinatesW.Text);
                m_BranchData.GlobalCoordinateNorth = Convert.ToSingle(textBox_BranchGlobalCoordinatesN.Text);
                m_BranchData.GlobalCoordinateSouth = Convert.ToSingle(textBox_BranchGlobalCoordinatesS.Text);
                m_BranchData.Remarks = textBox_BranchRemarks.Text;
                m_BranchData.Type = Branch.LayerType.Elevation;
            }
            catch (Exception)
            {
                MessageBox.Show("Invalid data read from the Branch form", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void TextBox_BranchElevationMin_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (ElevationInputIsValid(e.KeyChar, (sender as TextBox).Text) == false)
            {
                e.Handled = true;
            }
        }

        private void TextBox_BranchElevationMin_Validating(object sender, CancelEventArgs e)
        {
            if (ElevationTextIsValid((sender as TextBox).Text) == false)
            {
                // Elevation text appears to be invalid
                e.Cancel = true;

                this.errorProviderBranch.SetError(textBox_BranchElevationMin, "Min elevation must be less than max!");
            }
        }

        private void TextBox_BranchElevationMax_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (ElevationInputIsValid(e.KeyChar, (sender as TextBox).Text) == false)
            {
                e.Handled = true;
            }
        }

        private void TextBox_BranchElevationMax_Validating(object sender, CancelEventArgs e)
        {
            if (ElevationTextIsValid((sender as TextBox).Text) == false)
            {
                // Elevation text appears to be invalid
                e.Cancel = true;

                this.errorProviderBranch.SetError(textBox_BranchElevationMax, "Max elevation must be greater than min!");
            }
        }

        private void TextBox_BranchGlobalCoordinatesW_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (GlobalCoordinateTextInputIsValid(e.KeyChar, (sender as TextBox).Text) == false)
            {
                e.Handled = true;
            }
        }

        private void TextBox_BranchGlobalCoordinatesN_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (GlobalCoordinateTextInputIsValid(e.KeyChar, (sender as TextBox).Text) == false)
            {
                e.Handled = true;
            }
        }

        private void TextBox_BranchGlobalCoordinatesE_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (GlobalCoordinateTextInputIsValid(e.KeyChar, (sender as TextBox).Text) == false)
            {
                e.Handled = true;
            }
        }

        private void TextBox_BranchGlobalCoordinatesS_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (GlobalCoordinateTextInputIsValid(e.KeyChar, (sender as TextBox).Text) == false)
            {
                e.Handled = true;
            }
        }

        private void TextBox_BranchElevationMin_Validated(object sender, EventArgs e)
        {
            errorProviderBranch.SetError(textBox_BranchElevationMin, "");
            m_UserModifiedElevation = true;
        }

        private void TextBox_BranchElevationMax_Validated(object sender, EventArgs e)
        {
            errorProviderBranch.SetError(textBox_BranchElevationMax, "");
            m_UserModifiedElevation = true;
        }

        private void TextBox_BranchGlobalCoordinatesW_Validated(object sender, EventArgs e)
        {
            errorProviderBranch.SetError(textBox_BranchGlobalCoordinatesW, "");
            m_UserModifiedGlobalCoordinates = true;
        }

        private void TextBox_BranchGlobalCoordinatesW_Validating(object sender, CancelEventArgs e)
        {
            if (GlobalCoordinateIsValid((sender as TextBox).Text) == false)
            {
                // Global coordinate text appears to be invalid
                e.Cancel = true;

                this.errorProviderBranch.SetError(textBox_BranchGlobalCoordinatesW, "West coordinate must be less than East coordinate!");
            }
        }

        private void TextBox_BranchGlobalCoordinatesN_Validating(object sender, CancelEventArgs e)
        {
            if (GlobalCoordinateIsValid((sender as TextBox).Text) == false)
            {
                // Global coordinate text appears to be invalid
                e.Cancel = true;

                this.errorProviderBranch.SetError(textBox_BranchGlobalCoordinatesN, "North coordinate must be greater than South coordinate!");
            }
        }

        private void TextBox_BranchGlobalCoordinatesN_Validated(object sender, EventArgs e)
        {
            errorProviderBranch.SetError(textBox_BranchGlobalCoordinatesN, "");
            m_UserModifiedGlobalCoordinates = true;
        }

        private void TextBox_BranchGlobalCoordinatesE_Validated(object sender, EventArgs e)
        {
            errorProviderBranch.SetError(textBox_BranchGlobalCoordinatesE, "");
            m_UserModifiedGlobalCoordinates = true;
        }

        private void TextBox_BranchGlobalCoordinatesE_Validating(object sender, CancelEventArgs e)
        {
            if (GlobalCoordinateIsValid((sender as TextBox).Text) == false)
            {
                // Global coordinate text appears to be invalid
                e.Cancel = true;

                this.errorProviderBranch.SetError(textBox_BranchGlobalCoordinatesE, "East coordinate must be greater than West coordinate!");
            }
        }

        private void TextBox_BranchGlobalCoordinatesS_Validated(object sender, EventArgs e)
        {
            errorProviderBranch.SetError(textBox_BranchGlobalCoordinatesS, "");
            m_UserModifiedGlobalCoordinates = true;
        }

        private void TextBox_BranchGlobalCoordinatesS_Validating(object sender, CancelEventArgs e)
        {
            if (GlobalCoordinateIsValid((sender as TextBox).Text) == false)
            {
                // Global coordinate text appears to be invalid
                e.Cancel = true;

                this.errorProviderBranch.SetError(textBox_BranchGlobalCoordinatesS, "South coordinate must be less than North coordinate!");
            }
        }

        private void TextBox_ValidatedField_TextChanged(object sender, EventArgs e)
        {
            // Enable or disable the OK button depending on if input fields are all valid
            button_BranchOK.Enabled = (GlobalCoordinatesAreValid() && ElevationsAreValid());
        }
    }
}
