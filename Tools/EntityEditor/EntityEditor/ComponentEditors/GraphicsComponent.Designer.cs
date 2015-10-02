namespace EntityEditor.ComponentEditors
{
    partial class GraphicsComponent
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
            this.label1 = new System.Windows.Forms.Label();
            this.GC_Type = new System.Windows.Forms.ComboBox();
            this.GC_Active = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.GC_Text_ModelFile = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.GC_Text_EffectFile = new System.Windows.Forms.TextBox();
            this.GC_Btn_BrowseModelFile = new System.Windows.Forms.Button();
            this.GC_Btn_BrowseEffectFile = new System.Windows.Forms.Button();
            this.browseFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.GC_Btn_Save = new System.Windows.Forms.Button();
            this.GC_Btn_Cancel = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(94, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(37, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Type: ";
            // 
            // GC_Type
            // 
            this.GC_Type.FormattingEnabled = true;
            this.GC_Type.Location = new System.Drawing.Point(137, 6);
            this.GC_Type.Name = "GC_Type";
            this.GC_Type.Size = new System.Drawing.Size(99, 21);
            this.GC_Type.TabIndex = 1;
            // 
            // GC_Active
            // 
            this.GC_Active.AutoSize = true;
            this.GC_Active.Location = new System.Drawing.Point(15, 8);
            this.GC_Active.Name = "GC_Active";
            this.GC_Active.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.GC_Active.Size = new System.Drawing.Size(59, 17);
            this.GC_Active.TabIndex = 4;
            this.GC_Active.Text = ":Active";
            this.GC_Active.UseVisualStyleBackColor = true;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(15, 61);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(58, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Model file: ";
            // 
            // GC_Text_ModelFile
            // 
            this.GC_Text_ModelFile.Location = new System.Drawing.Point(69, 58);
            this.GC_Text_ModelFile.Name = "GC_Text_ModelFile";
            this.GC_Text_ModelFile.Size = new System.Drawing.Size(187, 20);
            this.GC_Text_ModelFile.TabIndex = 6;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(15, 87);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(57, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "Effect file: ";
            // 
            // GC_Text_EffectFile
            // 
            this.GC_Text_EffectFile.Location = new System.Drawing.Point(69, 84);
            this.GC_Text_EffectFile.Name = "GC_Text_EffectFile";
            this.GC_Text_EffectFile.Size = new System.Drawing.Size(187, 20);
            this.GC_Text_EffectFile.TabIndex = 8;
            // 
            // GC_Btn_BrowseModelFile
            // 
            this.GC_Btn_BrowseModelFile.Location = new System.Drawing.Point(262, 57);
            this.GC_Btn_BrowseModelFile.Name = "GC_Btn_BrowseModelFile";
            this.GC_Btn_BrowseModelFile.Size = new System.Drawing.Size(75, 21);
            this.GC_Btn_BrowseModelFile.TabIndex = 9;
            this.GC_Btn_BrowseModelFile.Text = "Browse";
            this.GC_Btn_BrowseModelFile.UseVisualStyleBackColor = true;
            this.GC_Btn_BrowseModelFile.Click += new System.EventHandler(this.GC_Btn_BrowseModelFile_Click);
            // 
            // GC_Btn_BrowseEffectFile
            // 
            this.GC_Btn_BrowseEffectFile.Location = new System.Drawing.Point(262, 83);
            this.GC_Btn_BrowseEffectFile.Name = "GC_Btn_BrowseEffectFile";
            this.GC_Btn_BrowseEffectFile.Size = new System.Drawing.Size(75, 21);
            this.GC_Btn_BrowseEffectFile.TabIndex = 10;
            this.GC_Btn_BrowseEffectFile.Text = "Browse";
            this.GC_Btn_BrowseEffectFile.UseVisualStyleBackColor = true;
            this.GC_Btn_BrowseEffectFile.Click += new System.EventHandler(this.GC_Btn_BrowseEffectFile_Click);
            // 
            // browseFileDialog
            // 
            this.browseFileDialog.FileName = "browseFileDialog";
            // 
            // GC_Btn_Save
            // 
            this.GC_Btn_Save.Location = new System.Drawing.Point(56, 127);
            this.GC_Btn_Save.Name = "GC_Btn_Save";
            this.GC_Btn_Save.Size = new System.Drawing.Size(75, 23);
            this.GC_Btn_Save.TabIndex = 11;
            this.GC_Btn_Save.Text = "Save";
            this.GC_Btn_Save.UseVisualStyleBackColor = true;
            this.GC_Btn_Save.Click += new System.EventHandler(this.GC_Btn_Save_Click);
            // 
            // GC_Btn_Cancel
            // 
            this.GC_Btn_Cancel.Location = new System.Drawing.Point(214, 127);
            this.GC_Btn_Cancel.Name = "GC_Btn_Cancel";
            this.GC_Btn_Cancel.Size = new System.Drawing.Size(75, 23);
            this.GC_Btn_Cancel.TabIndex = 12;
            this.GC_Btn_Cancel.Text = "Cancel";
            this.GC_Btn_Cancel.UseVisualStyleBackColor = true;
            this.GC_Btn_Cancel.Click += new System.EventHandler(this.GC_Btn_Cancel_Click);
            // 
            // GraphicsComponent
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(349, 162);
            this.Controls.Add(this.GC_Btn_Cancel);
            this.Controls.Add(this.GC_Btn_Save);
            this.Controls.Add(this.GC_Btn_BrowseEffectFile);
            this.Controls.Add(this.GC_Btn_BrowseModelFile);
            this.Controls.Add(this.GC_Text_EffectFile);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.GC_Text_ModelFile);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.GC_Active);
            this.Controls.Add(this.GC_Type);
            this.Controls.Add(this.label1);
            this.Name = "GraphicsComponent";
            this.Text = "GraphicsComponent";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox GC_Type;
        private System.Windows.Forms.CheckBox GC_Active;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox GC_Text_ModelFile;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox GC_Text_EffectFile;
        private System.Windows.Forms.Button GC_Btn_BrowseModelFile;
        private System.Windows.Forms.Button GC_Btn_BrowseEffectFile;
        private System.Windows.Forms.OpenFileDialog browseFileDialog;
        private System.Windows.Forms.Button GC_Btn_Save;
        private System.Windows.Forms.Button GC_Btn_Cancel;
    }
}