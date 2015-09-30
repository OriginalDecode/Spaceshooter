namespace EntityEditor.ComponentEditors
{
    partial class ShootingComponent
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
            this.SC_Active = new System.Windows.Forms.CheckBox();
            this.SC_Btn_Cancel = new System.Windows.Forms.Button();
            this.SC_Btn_Save = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // SC_Active
            // 
            this.SC_Active.AutoSize = true;
            this.SC_Active.Location = new System.Drawing.Point(12, 12);
            this.SC_Active.Name = "SC_Active";
            this.SC_Active.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.SC_Active.Size = new System.Drawing.Size(59, 17);
            this.SC_Active.TabIndex = 5;
            this.SC_Active.Text = ":Active";
            this.SC_Active.UseVisualStyleBackColor = true;
            // 
            // SC_Btn_Cancel
            // 
            this.SC_Btn_Cancel.Location = new System.Drawing.Point(148, 55);
            this.SC_Btn_Cancel.Name = "SC_Btn_Cancel";
            this.SC_Btn_Cancel.Size = new System.Drawing.Size(75, 23);
            this.SC_Btn_Cancel.TabIndex = 4;
            this.SC_Btn_Cancel.Text = "Cancel";
            this.SC_Btn_Cancel.UseVisualStyleBackColor = true;
            this.SC_Btn_Cancel.Click += new System.EventHandler(this.SC_Btn_Cancel_Click);
            // 
            // SC_Btn_Save
            // 
            this.SC_Btn_Save.Location = new System.Drawing.Point(11, 55);
            this.SC_Btn_Save.Name = "SC_Btn_Save";
            this.SC_Btn_Save.Size = new System.Drawing.Size(75, 23);
            this.SC_Btn_Save.TabIndex = 3;
            this.SC_Btn_Save.Text = "Save";
            this.SC_Btn_Save.UseVisualStyleBackColor = true;
            this.SC_Btn_Save.Click += new System.EventHandler(this.SC_Btn_Save_Click);
            // 
            // ShootingComponent
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(263, 101);
            this.Controls.Add(this.SC_Active);
            this.Controls.Add(this.SC_Btn_Cancel);
            this.Controls.Add(this.SC_Btn_Save);
            this.Name = "ShootingComponent";
            this.Text = "ShootingComponent";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox SC_Active;
        private System.Windows.Forms.Button SC_Btn_Cancel;
        private System.Windows.Forms.Button SC_Btn_Save;
    }
}