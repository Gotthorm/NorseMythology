namespace Yggdrasil
{
    partial class FormBranch
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
			this.components = new System.ComponentModel.Container();
			this.textBox_BranchVersion = new System.Windows.Forms.TextBox();
			this.label_BranchVersion = new System.Windows.Forms.Label();
			this.label_BranchRemarks = new System.Windows.Forms.Label();
			this.textBox_BranchRemarks = new System.Windows.Forms.TextBox();
			this.label_BranchElevationMinMax = new System.Windows.Forms.Label();
			this.textBox_BranchElevationMin = new System.Windows.Forms.TextBox();
			this.textBox_BranchElevationMax = new System.Windows.Forms.TextBox();
			this.textBox_BranchGlobalCoordinatesN = new System.Windows.Forms.TextBox();
			this.label_BranchGlobalCoordinates = new System.Windows.Forms.Label();
			this.textBox_BranchGlobalCoordinatesW = new System.Windows.Forms.TextBox();
			this.textBox_BranchGlobalCoordinatesS = new System.Windows.Forms.TextBox();
			this.textBox_BranchGlobalCoordinatesE = new System.Windows.Forms.TextBox();
			this.label_BranchImageDimensions = new System.Windows.Forms.Label();
			this.textBox_BranchImageDimensionsWidth = new System.Windows.Forms.TextBox();
			this.label_BranchImagePath = new System.Windows.Forms.Label();
			this.textBox_BranchImagePath = new System.Windows.Forms.TextBox();
			this.textBox_BranchImageDimensionsHeight = new System.Windows.Forms.TextBox();
			this.button_BranchOK = new System.Windows.Forms.Button();
			this.button_BranchCancel = new System.Windows.Forms.Button();
			this.errorProviderBranch = new System.Windows.Forms.ErrorProvider(this.components);
			this.label_BranchImageBitDepth = new System.Windows.Forms.Label();
			this.textBox_BranchImageBitDepth = new System.Windows.Forms.TextBox();
			this.textBox_ApproximateDimensionsHeight = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.textBox_ApproximateDimensionsWidth = new System.Windows.Forms.TextBox();
			((System.ComponentModel.ISupportInitialize)(this.errorProviderBranch)).BeginInit();
			this.SuspendLayout();
			// 
			// textBox_BranchVersion
			// 
			this.textBox_BranchVersion.Location = new System.Drawing.Point(174, 15);
			this.textBox_BranchVersion.Name = "textBox_BranchVersion";
			this.textBox_BranchVersion.ReadOnly = true;
			this.textBox_BranchVersion.Size = new System.Drawing.Size(100, 20);
			this.textBox_BranchVersion.TabIndex = 0;
			// 
			// label_BranchVersion
			// 
			this.label_BranchVersion.AutoSize = true;
			this.label_BranchVersion.Location = new System.Drawing.Point(12, 18);
			this.label_BranchVersion.Name = "label_BranchVersion";
			this.label_BranchVersion.Size = new System.Drawing.Size(79, 13);
			this.label_BranchVersion.TabIndex = 1;
			this.label_BranchVersion.Text = "Branch Version";
			// 
			// label_BranchRemarks
			// 
			this.label_BranchRemarks.AutoSize = true;
			this.label_BranchRemarks.Location = new System.Drawing.Point(12, 44);
			this.label_BranchRemarks.Name = "label_BranchRemarks";
			this.label_BranchRemarks.Size = new System.Drawing.Size(49, 13);
			this.label_BranchRemarks.TabIndex = 3;
			this.label_BranchRemarks.Text = "Remarks";
			// 
			// textBox_BranchRemarks
			// 
			this.textBox_BranchRemarks.AcceptsReturn = true;
			this.textBox_BranchRemarks.AcceptsTab = true;
			this.textBox_BranchRemarks.Location = new System.Drawing.Point(174, 41);
			this.textBox_BranchRemarks.Multiline = true;
			this.textBox_BranchRemarks.Name = "textBox_BranchRemarks";
			this.textBox_BranchRemarks.Size = new System.Drawing.Size(465, 68);
			this.textBox_BranchRemarks.TabIndex = 2;
			// 
			// label_BranchElevationMinMax
			// 
			this.label_BranchElevationMinMax.AutoSize = true;
			this.label_BranchElevationMinMax.Location = new System.Drawing.Point(12, 118);
			this.label_BranchElevationMinMax.Name = "label_BranchElevationMinMax";
			this.label_BranchElevationMinMax.Size = new System.Drawing.Size(136, 13);
			this.label_BranchElevationMinMax.TabIndex = 7;
			this.label_BranchElevationMinMax.Text = "Elevation Min/Max (meters)";
			// 
			// textBox_BranchElevationMin
			// 
			this.textBox_BranchElevationMin.Location = new System.Drawing.Point(174, 115);
			this.textBox_BranchElevationMin.Name = "textBox_BranchElevationMin";
			this.textBox_BranchElevationMin.Size = new System.Drawing.Size(100, 20);
			this.textBox_BranchElevationMin.TabIndex = 6;
			this.textBox_BranchElevationMin.Text = "0";
			this.textBox_BranchElevationMin.TextChanged += new System.EventHandler(this.TextBox_ValidatedField_TextChanged);
			this.textBox_BranchElevationMin.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.TextBox_BranchElevationMin_KeyPress);
			this.textBox_BranchElevationMin.Validating += new System.ComponentModel.CancelEventHandler(this.TextBox_BranchElevationMin_Validating);
			this.textBox_BranchElevationMin.Validated += new System.EventHandler(this.TextBox_BranchElevationMin_Validated);
			// 
			// textBox_BranchElevationMax
			// 
			this.textBox_BranchElevationMax.Location = new System.Drawing.Point(280, 115);
			this.textBox_BranchElevationMax.Name = "textBox_BranchElevationMax";
			this.textBox_BranchElevationMax.Size = new System.Drawing.Size(100, 20);
			this.textBox_BranchElevationMax.TabIndex = 8;
			this.textBox_BranchElevationMax.Text = "1000";
			this.textBox_BranchElevationMax.TextChanged += new System.EventHandler(this.TextBox_ValidatedField_TextChanged);
			this.textBox_BranchElevationMax.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.TextBox_BranchElevationMax_KeyPress);
			this.textBox_BranchElevationMax.Validating += new System.ComponentModel.CancelEventHandler(this.TextBox_BranchElevationMax_Validating);
			this.textBox_BranchElevationMax.Validated += new System.EventHandler(this.TextBox_BranchElevationMax_Validated);
			// 
			// textBox_BranchGlobalCoordinatesN
			// 
			this.textBox_BranchGlobalCoordinatesN.Location = new System.Drawing.Point(280, 141);
			this.textBox_BranchGlobalCoordinatesN.Name = "textBox_BranchGlobalCoordinatesN";
			this.textBox_BranchGlobalCoordinatesN.Size = new System.Drawing.Size(100, 20);
			this.textBox_BranchGlobalCoordinatesN.TabIndex = 11;
			this.textBox_BranchGlobalCoordinatesN.Text = "100.0";
			this.textBox_BranchGlobalCoordinatesN.TextChanged += new System.EventHandler(this.TextBox_ValidatedField_TextChanged);
			this.textBox_BranchGlobalCoordinatesN.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.TextBox_BranchGlobalCoordinatesN_KeyPress);
			this.textBox_BranchGlobalCoordinatesN.Validating += new System.ComponentModel.CancelEventHandler(this.TextBox_BranchGlobalCoordinatesN_Validating);
			this.textBox_BranchGlobalCoordinatesN.Validated += new System.EventHandler(this.TextBox_BranchGlobalCoordinatesN_Validated);
			// 
			// label_BranchGlobalCoordinates
			// 
			this.label_BranchGlobalCoordinates.AutoSize = true;
			this.label_BranchGlobalCoordinates.Location = new System.Drawing.Point(12, 144);
			this.label_BranchGlobalCoordinates.Name = "label_BranchGlobalCoordinates";
			this.label_BranchGlobalCoordinates.Size = new System.Drawing.Size(147, 13);
			this.label_BranchGlobalCoordinates.TabIndex = 10;
			this.label_BranchGlobalCoordinates.Text = "Global Coordinates (W,N,E,S)";
			// 
			// textBox_BranchGlobalCoordinatesW
			// 
			this.textBox_BranchGlobalCoordinatesW.Location = new System.Drawing.Point(174, 141);
			this.textBox_BranchGlobalCoordinatesW.Name = "textBox_BranchGlobalCoordinatesW";
			this.textBox_BranchGlobalCoordinatesW.Size = new System.Drawing.Size(100, 20);
			this.textBox_BranchGlobalCoordinatesW.TabIndex = 9;
			this.textBox_BranchGlobalCoordinatesW.Text = "-100.0";
			this.textBox_BranchGlobalCoordinatesW.TextChanged += new System.EventHandler(this.TextBox_ValidatedField_TextChanged);
			this.textBox_BranchGlobalCoordinatesW.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.TextBox_BranchGlobalCoordinatesW_KeyPress);
			this.textBox_BranchGlobalCoordinatesW.Validating += new System.ComponentModel.CancelEventHandler(this.TextBox_BranchGlobalCoordinatesW_Validating);
			this.textBox_BranchGlobalCoordinatesW.Validated += new System.EventHandler(this.TextBox_BranchGlobalCoordinatesW_Validated);
			// 
			// textBox_BranchGlobalCoordinatesS
			// 
			this.textBox_BranchGlobalCoordinatesS.Location = new System.Drawing.Point(492, 141);
			this.textBox_BranchGlobalCoordinatesS.Name = "textBox_BranchGlobalCoordinatesS";
			this.textBox_BranchGlobalCoordinatesS.Size = new System.Drawing.Size(100, 20);
			this.textBox_BranchGlobalCoordinatesS.TabIndex = 13;
			this.textBox_BranchGlobalCoordinatesS.Text = "99.0";
			this.textBox_BranchGlobalCoordinatesS.TextChanged += new System.EventHandler(this.TextBox_ValidatedField_TextChanged);
			this.textBox_BranchGlobalCoordinatesS.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.TextBox_BranchGlobalCoordinatesS_KeyPress);
			this.textBox_BranchGlobalCoordinatesS.Validating += new System.ComponentModel.CancelEventHandler(this.TextBox_BranchGlobalCoordinatesS_Validating);
			this.textBox_BranchGlobalCoordinatesS.Validated += new System.EventHandler(this.TextBox_BranchGlobalCoordinatesS_Validated);
			// 
			// textBox_BranchGlobalCoordinatesE
			// 
			this.textBox_BranchGlobalCoordinatesE.Location = new System.Drawing.Point(386, 141);
			this.textBox_BranchGlobalCoordinatesE.Name = "textBox_BranchGlobalCoordinatesE";
			this.textBox_BranchGlobalCoordinatesE.Size = new System.Drawing.Size(100, 20);
			this.textBox_BranchGlobalCoordinatesE.TabIndex = 12;
			this.textBox_BranchGlobalCoordinatesE.Text = "-99.0";
			this.textBox_BranchGlobalCoordinatesE.TextChanged += new System.EventHandler(this.TextBox_ValidatedField_TextChanged);
			this.textBox_BranchGlobalCoordinatesE.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.TextBox_BranchGlobalCoordinatesE_KeyPress);
			this.textBox_BranchGlobalCoordinatesE.Validating += new System.ComponentModel.CancelEventHandler(this.TextBox_BranchGlobalCoordinatesE_Validating);
			this.textBox_BranchGlobalCoordinatesE.Validated += new System.EventHandler(this.TextBox_BranchGlobalCoordinatesE_Validated);
			// 
			// label_BranchImageDimensions
			// 
			this.label_BranchImageDimensions.AutoSize = true;
			this.label_BranchImageDimensions.Location = new System.Drawing.Point(12, 196);
			this.label_BranchImageDimensions.Name = "label_BranchImageDimensions";
			this.label_BranchImageDimensions.Size = new System.Drawing.Size(124, 13);
			this.label_BranchImageDimensions.TabIndex = 15;
			this.label_BranchImageDimensions.Text = "Image Dimensions (W,H)";
			// 
			// textBox_BranchImageDimensionsWidth
			// 
			this.textBox_BranchImageDimensionsWidth.Location = new System.Drawing.Point(174, 193);
			this.textBox_BranchImageDimensionsWidth.Name = "textBox_BranchImageDimensionsWidth";
			this.textBox_BranchImageDimensionsWidth.ReadOnly = true;
			this.textBox_BranchImageDimensionsWidth.Size = new System.Drawing.Size(100, 20);
			this.textBox_BranchImageDimensionsWidth.TabIndex = 14;
			// 
			// label_BranchImagePath
			// 
			this.label_BranchImagePath.AutoSize = true;
			this.label_BranchImagePath.Location = new System.Drawing.Point(12, 248);
			this.label_BranchImagePath.Name = "label_BranchImagePath";
			this.label_BranchImagePath.Size = new System.Drawing.Size(61, 13);
			this.label_BranchImagePath.TabIndex = 17;
			this.label_BranchImagePath.Text = "Image Path";
			// 
			// textBox_BranchImagePath
			// 
			this.textBox_BranchImagePath.Location = new System.Drawing.Point(174, 245);
			this.textBox_BranchImagePath.Name = "textBox_BranchImagePath";
			this.textBox_BranchImagePath.ReadOnly = true;
			this.textBox_BranchImagePath.Size = new System.Drawing.Size(465, 20);
			this.textBox_BranchImagePath.TabIndex = 16;
			// 
			// textBox_BranchImageDimensionsHeight
			// 
			this.textBox_BranchImageDimensionsHeight.Location = new System.Drawing.Point(280, 193);
			this.textBox_BranchImageDimensionsHeight.Name = "textBox_BranchImageDimensionsHeight";
			this.textBox_BranchImageDimensionsHeight.ReadOnly = true;
			this.textBox_BranchImageDimensionsHeight.Size = new System.Drawing.Size(100, 20);
			this.textBox_BranchImageDimensionsHeight.TabIndex = 18;
			// 
			// button_BranchOK
			// 
			this.button_BranchOK.Location = new System.Drawing.Point(564, 271);
			this.button_BranchOK.Name = "button_BranchOK";
			this.button_BranchOK.Size = new System.Drawing.Size(75, 23);
			this.button_BranchOK.TabIndex = 19;
			this.button_BranchOK.Text = "OK";
			this.button_BranchOK.UseVisualStyleBackColor = true;
			this.button_BranchOK.Click += new System.EventHandler(this.Button_BranchOK_Click);
			// 
			// button_BranchCancel
			// 
			this.button_BranchCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.button_BranchCancel.Location = new System.Drawing.Point(480, 271);
			this.button_BranchCancel.Name = "button_BranchCancel";
			this.button_BranchCancel.Size = new System.Drawing.Size(75, 23);
			this.button_BranchCancel.TabIndex = 20;
			this.button_BranchCancel.Text = "Cancel";
			this.button_BranchCancel.UseVisualStyleBackColor = true;
			// 
			// errorProviderBranch
			// 
			this.errorProviderBranch.ContainerControl = this;
			// 
			// label_BranchImageBitDepth
			// 
			this.label_BranchImageBitDepth.AutoSize = true;
			this.label_BranchImageBitDepth.Location = new System.Drawing.Point(12, 222);
			this.label_BranchImageBitDepth.Name = "label_BranchImageBitDepth";
			this.label_BranchImageBitDepth.Size = new System.Drawing.Size(83, 13);
			this.label_BranchImageBitDepth.TabIndex = 22;
			this.label_BranchImageBitDepth.Text = "Image Bit Depth";
			// 
			// textBox_BranchImageBitDepth
			// 
			this.textBox_BranchImageBitDepth.Location = new System.Drawing.Point(174, 219);
			this.textBox_BranchImageBitDepth.Name = "textBox_BranchImageBitDepth";
			this.textBox_BranchImageBitDepth.ReadOnly = true;
			this.textBox_BranchImageBitDepth.Size = new System.Drawing.Size(100, 20);
			this.textBox_BranchImageBitDepth.TabIndex = 21;
			// 
			// textBox_ApproximateDimensionsHeight
			// 
			this.textBox_ApproximateDimensionsHeight.Location = new System.Drawing.Point(280, 167);
			this.textBox_ApproximateDimensionsHeight.Name = "textBox_ApproximateDimensionsHeight";
			this.textBox_ApproximateDimensionsHeight.ReadOnly = true;
			this.textBox_ApproximateDimensionsHeight.Size = new System.Drawing.Size(100, 20);
			this.textBox_ApproximateDimensionsHeight.TabIndex = 25;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 170);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(147, 13);
			this.label1.TabIndex = 24;
			this.label1.Text = "Approximate Dimensions (KM)";
			// 
			// textBox_ApproximateDimensionsWidth
			// 
			this.textBox_ApproximateDimensionsWidth.Location = new System.Drawing.Point(174, 167);
			this.textBox_ApproximateDimensionsWidth.Name = "textBox_ApproximateDimensionsWidth";
			this.textBox_ApproximateDimensionsWidth.ReadOnly = true;
			this.textBox_ApproximateDimensionsWidth.Size = new System.Drawing.Size(100, 20);
			this.textBox_ApproximateDimensionsWidth.TabIndex = 23;
			// 
			// FormBranch
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(651, 305);
			this.Controls.Add(this.textBox_ApproximateDimensionsHeight);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.textBox_ApproximateDimensionsWidth);
			this.Controls.Add(this.label_BranchImageBitDepth);
			this.Controls.Add(this.textBox_BranchImageBitDepth);
			this.Controls.Add(this.button_BranchCancel);
			this.Controls.Add(this.button_BranchOK);
			this.Controls.Add(this.textBox_BranchImageDimensionsHeight);
			this.Controls.Add(this.label_BranchImagePath);
			this.Controls.Add(this.textBox_BranchImagePath);
			this.Controls.Add(this.label_BranchImageDimensions);
			this.Controls.Add(this.textBox_BranchImageDimensionsWidth);
			this.Controls.Add(this.textBox_BranchGlobalCoordinatesS);
			this.Controls.Add(this.textBox_BranchGlobalCoordinatesE);
			this.Controls.Add(this.textBox_BranchGlobalCoordinatesN);
			this.Controls.Add(this.label_BranchGlobalCoordinates);
			this.Controls.Add(this.textBox_BranchGlobalCoordinatesW);
			this.Controls.Add(this.textBox_BranchElevationMax);
			this.Controls.Add(this.label_BranchElevationMinMax);
			this.Controls.Add(this.textBox_BranchElevationMin);
			this.Controls.Add(this.label_BranchRemarks);
			this.Controls.Add(this.textBox_BranchRemarks);
			this.Controls.Add(this.label_BranchVersion);
			this.Controls.Add(this.textBox_BranchVersion);
			this.Name = "FormBranch";
			this.Text = "FormBranch";
			((System.ComponentModel.ISupportInitialize)(this.errorProviderBranch)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox_BranchVersion;
        private System.Windows.Forms.Label label_BranchVersion;
        private System.Windows.Forms.Label label_BranchRemarks;
        private System.Windows.Forms.TextBox textBox_BranchRemarks;
        private System.Windows.Forms.Label label_BranchElevationMinMax;
        private System.Windows.Forms.TextBox textBox_BranchElevationMin;
        private System.Windows.Forms.TextBox textBox_BranchElevationMax;
        private System.Windows.Forms.TextBox textBox_BranchGlobalCoordinatesN;
        private System.Windows.Forms.Label label_BranchGlobalCoordinates;
        private System.Windows.Forms.TextBox textBox_BranchGlobalCoordinatesW;
        private System.Windows.Forms.TextBox textBox_BranchGlobalCoordinatesS;
        private System.Windows.Forms.TextBox textBox_BranchGlobalCoordinatesE;
        private System.Windows.Forms.Label label_BranchImageDimensions;
        private System.Windows.Forms.TextBox textBox_BranchImageDimensionsWidth;
        private System.Windows.Forms.Label label_BranchImagePath;
        private System.Windows.Forms.TextBox textBox_BranchImagePath;
        private System.Windows.Forms.TextBox textBox_BranchImageDimensionsHeight;
        private System.Windows.Forms.Button button_BranchOK;
        private System.Windows.Forms.Button button_BranchCancel;
        private System.Windows.Forms.ErrorProvider errorProviderBranch;
        private System.Windows.Forms.Label label_BranchImageBitDepth;
        private System.Windows.Forms.TextBox textBox_BranchImageBitDepth;
		private System.Windows.Forms.TextBox textBox_ApproximateDimensionsHeight;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox textBox_ApproximateDimensionsWidth;
	}
}