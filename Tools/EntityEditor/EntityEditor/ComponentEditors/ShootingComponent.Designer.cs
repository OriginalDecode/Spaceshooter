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
            this.label1 = new System.Windows.Forms.Label();
            this.SC_WeaponType = new System.Windows.Forms.ComboBox();
            this.SC_Btn_Edit_WeaponType = new System.Windows.Forms.Button();
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
            this.SC_Btn_Cancel.Location = new System.Drawing.Point(176, 66);
            this.SC_Btn_Cancel.Name = "SC_Btn_Cancel";
            this.SC_Btn_Cancel.Size = new System.Drawing.Size(75, 23);
            this.SC_Btn_Cancel.TabIndex = 4;
            this.SC_Btn_Cancel.Text = "Cancel";
            this.SC_Btn_Cancel.UseVisualStyleBackColor = true;
            this.SC_Btn_Cancel.Click += new System.EventHandler(this.SC_Btn_Cancel_Click);
            // 
            // SC_Btn_Save
            // 
            this.SC_Btn_Save.Location = new System.Drawing.Point(12, 66);
            this.SC_Btn_Save.Name = "SC_Btn_Save";
            this.SC_Btn_Save.Size = new System.Drawing.Size(75, 23);
            this.SC_Btn_Save.TabIndex = 3;
            this.SC_Btn_Save.Text = "Save";
            this.SC_Btn_Save.UseVisualStyleBackColor = true;
            this.SC_Btn_Save.Click += new System.EventHandler(this.SC_Btn_Save_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 36);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(81, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "Weapon Type: ";
            // 
            // SC_WeaponType
            // 
            this.SC_WeaponType.FormattingEnabled = true;
            this.SC_WeaponType.Location = new System.Drawing.Point(91, 33);
            this.SC_WeaponType.Name = "SC_WeaponType";
            this.SC_WeaponType.Size = new System.Drawing.Size(99, 21);
            this.SC_WeaponType.TabIndex = 7;
            this.SC_WeaponType.SelectedIndexChanged += new System.EventHandler(this.SC_WeaponType_SelectedIndexChanged);
            // 
            // SC_Btn_Edit_WeaponType
            // 
            this.SC_Btn_Edit_WeaponType.Location = new System.Drawing.Point(197, 33);
            this.SC_Btn_Edit_WeaponType.Name = "SC_Btn_Edit_WeaponType";
            this.SC_Btn_Edit_WeaponType.Size = new System.Drawing.Size(54, 23);
            this.SC_Btn_Edit_WeaponType.TabIndex = 8;
            this.SC_Btn_Edit_WeaponType.Text = "Edit";
            this.SC_Btn_Edit_WeaponType.UseVisualStyleBackColor = true;
            this.SC_Btn_Edit_WeaponType.Click += new System.EventHandler(this.SC_Btn_Edit_WeaponType_Click);
            // 
            // ShootingComponent
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(263, 101);
            this.Controls.Add(this.SC_Btn_Edit_WeaponType);
            this.Controls.Add(this.SC_WeaponType);
            this.Controls.Add(this.label1);
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
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox SC_WeaponType;
        private System.Windows.Forms.Button SC_Btn_Edit_WeaponType;
    }
}