namespace Yggdrasil
{
    partial class FormMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose( bool disposing )
        {
            if ( disposing && ( components != null ) )
            {
                components.Dispose();
            }
            base.Dispose( disposing );
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.newWorldToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.openWorldToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.importImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.optionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.settingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.tabControl_Main = new System.Windows.Forms.TabControl();
			this.tabPageView = new System.Windows.Forms.TabPage();
			this.tabPageBranches = new System.Windows.Forms.TabPage();
			this.dataGridViewBranches = new System.Windows.Forms.DataGridView();
			this.splitContainerView = new System.Windows.Forms.SplitContainer();
			this.pictureBoxMain = new System.Windows.Forms.PictureBox();
			this.numericUpDownZoom = new System.Windows.Forms.NumericUpDown();
			this.menuStrip1.SuspendLayout();
			this.tabControl_Main.SuspendLayout();
			this.tabPageView.SuspendLayout();
			this.tabPageBranches.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewBranches)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.splitContainerView)).BeginInit();
			this.splitContainerView.Panel1.SuspendLayout();
			this.splitContainerView.Panel2.SuspendLayout();
			this.splitContainerView.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.pictureBoxMain)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDownZoom)).BeginInit();
			this.SuspendLayout();
			// 
			// menuStrip1
			// 
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.optionsToolStripMenuItem,
            this.helpToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(1168, 24);
			this.menuStrip1.TabIndex = 0;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newWorldToolStripMenuItem,
            this.openWorldToolStripMenuItem,
            this.importImageToolStripMenuItem});
			this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
			this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
			this.fileToolStripMenuItem.Text = "File";
			// 
			// newWorldToolStripMenuItem
			// 
			this.newWorldToolStripMenuItem.Name = "newWorldToolStripMenuItem";
			this.newWorldToolStripMenuItem.Size = new System.Drawing.Size(146, 22);
			this.newWorldToolStripMenuItem.Text = "New World";
			this.newWorldToolStripMenuItem.Click += new System.EventHandler(this.NewWorldToolStripMenuItem_Click);
			// 
			// openWorldToolStripMenuItem
			// 
			this.openWorldToolStripMenuItem.Name = "openWorldToolStripMenuItem";
			this.openWorldToolStripMenuItem.Size = new System.Drawing.Size(146, 22);
			this.openWorldToolStripMenuItem.Text = "Open World";
			this.openWorldToolStripMenuItem.Click += new System.EventHandler(this.OpenWorldToolStripMenuItem_Click);
			// 
			// importImageToolStripMenuItem
			// 
			this.importImageToolStripMenuItem.Name = "importImageToolStripMenuItem";
			this.importImageToolStripMenuItem.Size = new System.Drawing.Size(146, 22);
			this.importImageToolStripMenuItem.Text = "Import Image";
			this.importImageToolStripMenuItem.Click += new System.EventHandler(this.ImportImageToolStripMenuItem_Click);
			// 
			// optionsToolStripMenuItem
			// 
			this.optionsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.settingsToolStripMenuItem});
			this.optionsToolStripMenuItem.Name = "optionsToolStripMenuItem";
			this.optionsToolStripMenuItem.Size = new System.Drawing.Size(61, 20);
			this.optionsToolStripMenuItem.Text = "Options";
			// 
			// settingsToolStripMenuItem
			// 
			this.settingsToolStripMenuItem.Name = "settingsToolStripMenuItem";
			this.settingsToolStripMenuItem.Size = new System.Drawing.Size(116, 22);
			this.settingsToolStripMenuItem.Text = "Settings";
			this.settingsToolStripMenuItem.Click += new System.EventHandler(this.settingsToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
			this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
			this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
			this.helpToolStripMenuItem.Text = "Help";
			// 
			// aboutToolStripMenuItem
			// 
			this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
			this.aboutToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
			this.aboutToolStripMenuItem.Text = "About";
			this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
			// 
			// tabControl_Main
			// 
			this.tabControl_Main.Alignment = System.Windows.Forms.TabAlignment.Bottom;
			this.tabControl_Main.Controls.Add(this.tabPageView);
			this.tabControl_Main.Controls.Add(this.tabPageBranches);
			this.tabControl_Main.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControl_Main.Location = new System.Drawing.Point(0, 24);
			this.tabControl_Main.Name = "tabControl_Main";
			this.tabControl_Main.SelectedIndex = 0;
			this.tabControl_Main.Size = new System.Drawing.Size(1168, 544);
			this.tabControl_Main.TabIndex = 1;
			// 
			// tabPageView
			// 
			this.tabPageView.Controls.Add(this.splitContainerView);
			this.tabPageView.Location = new System.Drawing.Point(4, 4);
			this.tabPageView.Name = "tabPageView";
			this.tabPageView.Padding = new System.Windows.Forms.Padding(3);
			this.tabPageView.Size = new System.Drawing.Size(1160, 518);
			this.tabPageView.TabIndex = 0;
			this.tabPageView.Text = "View";
			this.tabPageView.UseVisualStyleBackColor = true;
			// 
			// tabPageBranches
			// 
			this.tabPageBranches.Controls.Add(this.dataGridViewBranches);
			this.tabPageBranches.Location = new System.Drawing.Point(4, 4);
			this.tabPageBranches.Name = "tabPageBranches";
			this.tabPageBranches.Padding = new System.Windows.Forms.Padding(3);
			this.tabPageBranches.Size = new System.Drawing.Size(1160, 518);
			this.tabPageBranches.TabIndex = 1;
			this.tabPageBranches.Text = "Branches";
			this.tabPageBranches.UseVisualStyleBackColor = true;
			// 
			// dataGridViewBranches
			// 
			this.dataGridViewBranches.AllowUserToAddRows = false;
			this.dataGridViewBranches.AllowUserToDeleteRows = false;
			this.dataGridViewBranches.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridViewBranches.Dock = System.Windows.Forms.DockStyle.Fill;
			this.dataGridViewBranches.Location = new System.Drawing.Point(3, 3);
			this.dataGridViewBranches.Name = "dataGridViewBranches";
			this.dataGridViewBranches.ReadOnly = true;
			this.dataGridViewBranches.Size = new System.Drawing.Size(1154, 512);
			this.dataGridViewBranches.TabIndex = 0;
			// 
			// splitContainerView
			// 
			this.splitContainerView.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.splitContainerView.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainerView.Location = new System.Drawing.Point(3, 3);
			this.splitContainerView.Name = "splitContainerView";
			// 
			// splitContainerView.Panel1
			// 
			this.splitContainerView.Panel1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
			this.splitContainerView.Panel1.Controls.Add(this.pictureBoxMain);
			// 
			// splitContainerView.Panel2
			// 
			this.splitContainerView.Panel2.Controls.Add(this.numericUpDownZoom);
			this.splitContainerView.Size = new System.Drawing.Size(1154, 512);
			this.splitContainerView.SplitterDistance = 848;
			this.splitContainerView.TabIndex = 0;
			// 
			// pictureBoxMain
			// 
			this.pictureBoxMain.Location = new System.Drawing.Point(351, 281);
			this.pictureBoxMain.Margin = new System.Windows.Forms.Padding(0);
			this.pictureBoxMain.Name = "pictureBoxMain";
			this.pictureBoxMain.Size = new System.Drawing.Size(100, 50);
			this.pictureBoxMain.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
			this.pictureBoxMain.TabIndex = 0;
			this.pictureBoxMain.TabStop = false;
			// 
			// numericUpDownZoom
			// 
			this.numericUpDownZoom.Location = new System.Drawing.Point(105, 140);
			this.numericUpDownZoom.Maximum = new decimal(new int[] {
            800,
            0,
            0,
            0});
			this.numericUpDownZoom.Name = "numericUpDownZoom";
			this.numericUpDownZoom.Size = new System.Drawing.Size(120, 20);
			this.numericUpDownZoom.TabIndex = 0;
			this.numericUpDownZoom.Value = new decimal(new int[] {
            100,
            0,
            0,
            0});
			this.numericUpDownZoom.ValueChanged += new System.EventHandler(this.NumericUpDownZoom_ValueChanged);
			this.numericUpDownZoom.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.NumericUpDownZoom_KeyPress);
			// 
			// FormMain
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(1168, 568);
			this.Controls.Add(this.tabControl_Main);
			this.Controls.Add(this.menuStrip1);
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "FormMain";
			this.Text = "Yggdrasil";
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FormMain_FormClosed);
			this.Load += new System.EventHandler(this.FormMain_Load);
			this.DragDrop += new System.Windows.Forms.DragEventHandler(this.FormMain_DragDrop);
			this.DragEnter += new System.Windows.Forms.DragEventHandler(this.FormMain_DragEnter);
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.tabControl_Main.ResumeLayout(false);
			this.tabPageView.ResumeLayout(false);
			this.tabPageBranches.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.dataGridViewBranches)).EndInit();
			this.splitContainerView.Panel1.ResumeLayout(false);
			this.splitContainerView.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainerView)).EndInit();
			this.splitContainerView.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.pictureBoxMain)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDownZoom)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem optionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem settingsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openWorldToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem importImageToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem newWorldToolStripMenuItem;
		private System.Windows.Forms.TabControl tabControl_Main;
		private System.Windows.Forms.TabPage tabPageView;
		private System.Windows.Forms.TabPage tabPageBranches;
		private System.Windows.Forms.DataGridView dataGridViewBranches;
		private System.Windows.Forms.SplitContainer splitContainerView;
		private System.Windows.Forms.PictureBox pictureBoxMain;
		private System.Windows.Forms.NumericUpDown numericUpDownZoom;
	}
}

