namespace ModelViewer
{
    partial class ModelViewerWindow
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
            this.Menu_Panel = new System.Windows.Forms.Panel();
            this.Btn_OpenEffectFolder = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.EffectFilter = new System.Windows.Forms.ComboBox();
            this.Btn_OpenModel = new System.Windows.Forms.Button();
            this.ModelViewer = new System.Windows.Forms.Panel();
            this.ModelViewerMenu = new System.Windows.Forms.Panel();
            this.effectFolderBrowser = new System.Windows.Forms.FolderBrowserDialog();
            this.Menu_Panel.SuspendLayout();
            this.ModelViewer.SuspendLayout();
            this.SuspendLayout();
            // 
            // Menu_Panel
            // 
            this.Menu_Panel.Controls.Add(this.Btn_OpenEffectFolder);
            this.Menu_Panel.Controls.Add(this.label1);
            this.Menu_Panel.Controls.Add(this.EffectFilter);
            this.Menu_Panel.Controls.Add(this.Btn_OpenModel);
            this.Menu_Panel.Dock = System.Windows.Forms.DockStyle.Top;
            this.Menu_Panel.Location = new System.Drawing.Point(0, 0);
            this.Menu_Panel.Name = "Menu_Panel";
            this.Menu_Panel.Size = new System.Drawing.Size(505, 32);
            this.Menu_Panel.TabIndex = 0;
            // 
            // Btn_OpenEffectFolder
            // 
            this.Btn_OpenEffectFolder.Location = new System.Drawing.Point(85, 5);
            this.Btn_OpenEffectFolder.Name = "Btn_OpenEffectFolder";
            this.Btn_OpenEffectFolder.Size = new System.Drawing.Size(116, 23);
            this.Btn_OpenEffectFolder.TabIndex = 3;
            this.Btn_OpenEffectFolder.Text = "Open Effect Folder";
            this.Btn_OpenEffectFolder.UseVisualStyleBackColor = true;
            this.Btn_OpenEffectFolder.Click += new System.EventHandler(this.Btn_OpenEffectFolder_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(207, 10);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Effect File:";
            // 
            // EffectFilter
            // 
            this.EffectFilter.FormattingEnabled = true;
            this.EffectFilter.Location = new System.Drawing.Point(270, 7);
            this.EffectFilter.Name = "EffectFilter";
            this.EffectFilter.Size = new System.Drawing.Size(121, 21);
            this.EffectFilter.TabIndex = 1;
            // 
            // Btn_OpenModel
            // 
            this.Btn_OpenModel.Location = new System.Drawing.Point(4, 5);
            this.Btn_OpenModel.Name = "Btn_OpenModel";
            this.Btn_OpenModel.Size = new System.Drawing.Size(75, 23);
            this.Btn_OpenModel.TabIndex = 0;
            this.Btn_OpenModel.Text = "Open";
            this.Btn_OpenModel.UseVisualStyleBackColor = true;
            this.Btn_OpenModel.Click += new System.EventHandler(this.Btn_OpenModel_Click);
            // 
            // ModelViewer
            // 
            this.ModelViewer.Controls.Add(this.ModelViewerMenu);
            this.ModelViewer.Dock = System.Windows.Forms.DockStyle.Left;
            this.ModelViewer.Location = new System.Drawing.Point(0, 32);
            this.ModelViewer.Name = "ModelViewer";
            this.ModelViewer.Size = new System.Drawing.Size(502, 397);
            this.ModelViewer.TabIndex = 1;
            // 
            // ModelViewerMenu
            // 
            this.ModelViewerMenu.Dock = System.Windows.Forms.DockStyle.Right;
            this.ModelViewerMenu.Location = new System.Drawing.Point(363, 0);
            this.ModelViewerMenu.Name = "ModelViewerMenu";
            this.ModelViewerMenu.Size = new System.Drawing.Size(139, 397);
            this.ModelViewerMenu.TabIndex = 0;
            // 
            // ModelViewerWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(505, 429);
            this.Controls.Add(this.ModelViewer);
            this.Controls.Add(this.Menu_Panel);
            this.Name = "ModelViewerWindow";
            this.Text = "Prism Model Viewer";
            this.Menu_Panel.ResumeLayout(false);
            this.Menu_Panel.PerformLayout();
            this.ModelViewer.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel Menu_Panel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox EffectFilter;
        private System.Windows.Forms.Button Btn_OpenModel;
        private System.Windows.Forms.Panel ModelViewer;
        private System.Windows.Forms.Panel ModelViewerMenu;
        private System.Windows.Forms.Button Btn_OpenEffectFolder;
        private System.Windows.Forms.FolderBrowserDialog effectFolderBrowser;
    }
}

