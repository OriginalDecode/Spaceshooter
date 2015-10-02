namespace EntityEditor
{
    partial class RenameEntity
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
            this.Text_NewEntityName = new System.Windows.Forms.TextBox();
            this.Btn_RenameEntity_Ok = new System.Windows.Forms.Button();
            this.Btn_RenameEntity_Cancel = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(19, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(41, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Name: ";
            // 
            // Text_NewEntityName
            // 
            this.Text_NewEntityName.Location = new System.Drawing.Point(66, 12);
            this.Text_NewEntityName.Name = "Text_NewEntityName";
            this.Text_NewEntityName.Size = new System.Drawing.Size(168, 20);
            this.Text_NewEntityName.TabIndex = 1;
            // 
            // Btn_RenameEntity_Ok
            // 
            this.Btn_RenameEntity_Ok.Location = new System.Drawing.Point(12, 51);
            this.Btn_RenameEntity_Ok.Name = "Btn_RenameEntity_Ok";
            this.Btn_RenameEntity_Ok.Size = new System.Drawing.Size(75, 23);
            this.Btn_RenameEntity_Ok.TabIndex = 2;
            this.Btn_RenameEntity_Ok.Text = "OK";
            this.Btn_RenameEntity_Ok.UseVisualStyleBackColor = true;
            this.Btn_RenameEntity_Ok.Click += new System.EventHandler(this.Btn_RenameEntity_Ok_Click);
            // 
            // Btn_RenameEntity_Cancel
            // 
            this.Btn_RenameEntity_Cancel.Location = new System.Drawing.Point(159, 51);
            this.Btn_RenameEntity_Cancel.Name = "Btn_RenameEntity_Cancel";
            this.Btn_RenameEntity_Cancel.Size = new System.Drawing.Size(75, 23);
            this.Btn_RenameEntity_Cancel.TabIndex = 3;
            this.Btn_RenameEntity_Cancel.Text = "Cancel";
            this.Btn_RenameEntity_Cancel.UseVisualStyleBackColor = true;
            this.Btn_RenameEntity_Cancel.Click += new System.EventHandler(this.Btn_RenameEntity_Cancel_Click);
            // 
            // RenameEntity
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(244, 85);
            this.Controls.Add(this.Btn_RenameEntity_Cancel);
            this.Controls.Add(this.Btn_RenameEntity_Ok);
            this.Controls.Add(this.Text_NewEntityName);
            this.Controls.Add(this.label1);
            this.Name = "RenameEntity";
            this.Text = "RenameEntity";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox Text_NewEntityName;
        private System.Windows.Forms.Button Btn_RenameEntity_Ok;
        private System.Windows.Forms.Button Btn_RenameEntity_Cancel;
    }
}