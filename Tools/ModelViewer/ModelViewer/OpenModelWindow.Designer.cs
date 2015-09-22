namespace ModelViewer
{
    partial class OpenModelWindow
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
            this.OpenModelMenu = new System.Windows.Forms.Panel();
            this.Btn_OpenFile = new System.Windows.Forms.Button();
            this.Btn_CloseWindow = new System.Windows.Forms.Button();
            this.Btn_OpenModelFolder = new System.Windows.Forms.Button();
            this.OpenModelView = new System.Windows.Forms.Panel();
            this.modelFileListWindow = new System.Windows.Forms.ListBox();
            this.modelFolderBroweser = new System.Windows.Forms.FolderBrowserDialog();
            this.OpenModelMenu.SuspendLayout();
            this.OpenModelView.SuspendLayout();
            this.SuspendLayout();
            // 
            // OpenModelMenu
            // 
            this.OpenModelMenu.Controls.Add(this.Btn_OpenFile);
            this.OpenModelMenu.Controls.Add(this.Btn_CloseWindow);
            this.OpenModelMenu.Controls.Add(this.Btn_OpenModelFolder);
            this.OpenModelMenu.Dock = System.Windows.Forms.DockStyle.Top;
            this.OpenModelMenu.Location = new System.Drawing.Point(0, 0);
            this.OpenModelMenu.Name = "OpenModelMenu";
            this.OpenModelMenu.Size = new System.Drawing.Size(269, 35);
            this.OpenModelMenu.TabIndex = 0;
            // 
            // Btn_OpenFile
            // 
            this.Btn_OpenFile.Location = new System.Drawing.Point(85, 4);
            this.Btn_OpenFile.Name = "Btn_OpenFile";
            this.Btn_OpenFile.Size = new System.Drawing.Size(75, 23);
            this.Btn_OpenFile.TabIndex = 2;
            this.Btn_OpenFile.Text = "Open File";
            this.Btn_OpenFile.UseVisualStyleBackColor = true;
            this.Btn_OpenFile.Click += new System.EventHandler(this.Btn_OpenFile_Click);
            // 
            // Btn_CloseWindow
            // 
            this.Btn_CloseWindow.Location = new System.Drawing.Point(166, 4);
            this.Btn_CloseWindow.Name = "Btn_CloseWindow";
            this.Btn_CloseWindow.Size = new System.Drawing.Size(75, 23);
            this.Btn_CloseWindow.TabIndex = 1;
            this.Btn_CloseWindow.Text = "Close";
            this.Btn_CloseWindow.UseVisualStyleBackColor = true;
            this.Btn_CloseWindow.Click += new System.EventHandler(this.Btn_CloseWindow_Click);
            // 
            // Btn_OpenModelFolder
            // 
            this.Btn_OpenModelFolder.Location = new System.Drawing.Point(4, 4);
            this.Btn_OpenModelFolder.Name = "Btn_OpenModelFolder";
            this.Btn_OpenModelFolder.Size = new System.Drawing.Size(75, 23);
            this.Btn_OpenModelFolder.TabIndex = 0;
            this.Btn_OpenModelFolder.Text = "Open";
            this.Btn_OpenModelFolder.UseVisualStyleBackColor = true;
            this.Btn_OpenModelFolder.Click += new System.EventHandler(this.Btn_OpenModelFolder_Click);
            // 
            // OpenModelView
            // 
            this.OpenModelView.Controls.Add(this.modelFileListWindow);
            this.OpenModelView.Dock = System.Windows.Forms.DockStyle.Top;
            this.OpenModelView.Location = new System.Drawing.Point(0, 35);
            this.OpenModelView.Name = "OpenModelView";
            this.OpenModelView.Size = new System.Drawing.Size(269, 370);
            this.OpenModelView.TabIndex = 1;
            // 
            // modelFileListWindow
            // 
            this.modelFileListWindow.FormattingEnabled = true;
            this.modelFileListWindow.Location = new System.Drawing.Point(4, 7);
            this.modelFileListWindow.Name = "modelFileListWindow";
            this.modelFileListWindow.Size = new System.Drawing.Size(253, 355);
            this.modelFileListWindow.TabIndex = 0;
            // 
            // OpenModelWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(269, 409);
            this.Controls.Add(this.OpenModelView);
            this.Controls.Add(this.OpenModelMenu);
            this.Name = "OpenModelWindow";
            this.Text = "Open Model Window";
            this.OpenModelMenu.ResumeLayout(false);
            this.OpenModelView.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel OpenModelMenu;
        private System.Windows.Forms.Button Btn_CloseWindow;
        private System.Windows.Forms.Button Btn_OpenModelFolder;
        private System.Windows.Forms.Panel OpenModelView;
        private System.Windows.Forms.Button Btn_OpenFile;
        private System.Windows.Forms.FolderBrowserDialog modelFolderBroweser;
        private System.Windows.Forms.ListBox modelFileListWindow;
    }
}