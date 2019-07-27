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
            textBox_BranchImagePath.Text = branchData.SourcePath;
            textBox_BranchVersion.Text = branchData.Version.ToString();
            textBox_BranchImageDimensionsWidth.Text = branchData.ImageWidth.ToString();
            textBox_BranchImageDimensionsHeight.Text = branchData.ImageHeight.ToString();
            textBox_BranchImageBitDepth.Text = branchData.ImageBitDepth.ToString();
        }

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
            catch(Exception)
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
            if (!char.IsControl(newChar) && !char.IsDigit(newChar) && (newChar != '-'))
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
            // We will validate entries before allowing this to proceed?
            // Elevation range and global coordinates are mandatory to proceed.
        }

        private void TextBox_BranchElevationMin_KeyPress(object sender, KeyPressEventArgs e)
        {
            if(ElevationInputIsValid(e.KeyChar, (sender as TextBox).Text) == false)
            {
                e.Handled = true;
            }
        }

        private void TextBox_BranchElevationMin_Validating(object sender, CancelEventArgs e)
        {
            if (ElevationTextIsValid((sender as TextBox).Text) == false || ElevationsAreValid() == false)
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
            if (ElevationTextIsValid((sender as TextBox).Text) == false || ElevationsAreValid() == false)
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
        }

        private void TextBox_BranchElevationMax_Validated(object sender, EventArgs e)
        {
            errorProviderBranch.SetError(textBox_BranchElevationMax, "");
        }

        private void TextBox_BranchGlobalCoordinatesW_Validated(object sender, EventArgs e)
        {
            errorProviderBranch.SetError(textBox_BranchGlobalCoordinatesW, "");
        }

        private void TextBox_BranchGlobalCoordinatesW_Validating(object sender, CancelEventArgs e)
        {
            if (GlobalCoordinateIsValid((sender as TextBox).Text) == false || GlobalCoordinatesAreValid() == false)
            {
                // Global coordinate text appears to be invalid
                e.Cancel = true;

                this.errorProviderBranch.SetError(textBox_BranchGlobalCoordinatesW, "West coordinate must be less than East coordinate!");
            }
        }

        private void TextBox_BranchGlobalCoordinatesN_Validating(object sender, CancelEventArgs e)
        {
            if (GlobalCoordinateIsValid((sender as TextBox).Text) == false || GlobalCoordinatesAreValid() == false)
            {
                // Global coordinate text appears to be invalid
                e.Cancel = true;

                this.errorProviderBranch.SetError(textBox_BranchGlobalCoordinatesN, "North coordinate must be greater than South coordinate!");
            }
        }

        private void TextBox_BranchGlobalCoordinatesN_Validated(object sender, EventArgs e)
        {
            errorProviderBranch.SetError(textBox_BranchGlobalCoordinatesN, "");
        }

        private void TextBox_BranchGlobalCoordinatesE_Validated(object sender, EventArgs e)
        {
            errorProviderBranch.SetError(textBox_BranchGlobalCoordinatesE, "");
        }

        private void TextBox_BranchGlobalCoordinatesE_Validating(object sender, CancelEventArgs e)
        {
            if (GlobalCoordinateIsValid((sender as TextBox).Text) == false || GlobalCoordinatesAreValid() == false)
            {
                // Global coordinate text appears to be invalid
                e.Cancel = true;

                this.errorProviderBranch.SetError(textBox_BranchGlobalCoordinatesE, "East coordinate must be greater than West coordinate!");
            }
        }

        private void TextBox_BranchGlobalCoordinatesS_Validated(object sender, EventArgs e)
        {
            errorProviderBranch.SetError(textBox_BranchGlobalCoordinatesS, "");
        }

        private void TextBox_BranchGlobalCoordinatesS_Validating(object sender, CancelEventArgs e)
        {
            if (GlobalCoordinateIsValid((sender as TextBox).Text) == false || GlobalCoordinatesAreValid() == false)
            {
                // Global coordinate text appears to be invalid
                e.Cancel = true;

                this.errorProviderBranch.SetError(textBox_BranchGlobalCoordinatesS, "South coordinate must be less than North coordinate!");
            }
        }
    }
}
