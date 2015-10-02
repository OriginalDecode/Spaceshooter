namespace EntityEditor.ComponentEditors
{
    partial class AIComponent
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
            this.AC_Btn_Save = new System.Windows.Forms.Button();
            this.AC_Btn_Cancel = new System.Windows.Forms.Button();
            this.AC_Active = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // AC_Btn_Save
            // 
            this.AC_Btn_Save.Location = new System.Drawing.Point(12, 55);
            this.AC_Btn_Save.Name = "AC_Btn_Save";
            this.AC_Btn_Save.Size = new System.Drawing.Size(75, 23);
            this.AC_Btn_Save.TabIndex = 0;
            this.AC_Btn_Save.Text = "Save";
            this.AC_Btn_Save.UseVisualStyleBackColor = true;
            this.AC_Btn_Save.Click += new System.EventHandler(this.AC_Btn_Save_Click);
            // 
            // AC_Btn_Cancel
            // 
            this.AC_Btn_Cancel.Location = new System.Drawing.Point(149, 55);
            this.AC_Btn_Cancel.Name = "AC_Btn_Cancel";
            this.AC_Btn_Cancel.Size = new System.Drawing.Size(75, 23);
            this.AC_Btn_Cancel.TabIndex = 1;
            this.AC_Btn_Cancel.Text = "Cancel";
            this.AC_Btn_Cancel.UseVisualStyleBackColor = true;
            this.AC_Btn_Cancel.Click += new System.EventHandler(this.AC_Btn_Cancel_Click);
            // 
            // AC_Active
            // 
            this.AC_Active.AutoSize = true;
            this.AC_Active.Location = new System.Drawing.Point(13, 12);
            this.AC_Active.Name = "AC_Active";
            this.AC_Active.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.AC_Active.Size = new System.Drawing.Size(59, 17);
            this.AC_Active.TabIndex = 2;
            this.AC_Active.Text = ":Active";
            this.AC_Active.UseVisualStyleBackColor = true;
            // 
            // AIComponent
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(236, 90);
            this.Controls.Add(this.AC_Active);
            this.Controls.Add(this.AC_Btn_Cancel);
            this.Controls.Add(this.AC_Btn_Save);
            this.Name = "AIComponent";
            this.Text = "AIComponent";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button AC_Btn_Save;
        private System.Windows.Forms.Button AC_Btn_Cancel;
        private System.Windows.Forms.CheckBox AC_Active;
    }
}