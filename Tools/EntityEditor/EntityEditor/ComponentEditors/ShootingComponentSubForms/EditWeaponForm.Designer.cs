namespace EntityEditor.ComponentEditors.ShootingComponentSubForms
{
    partial class EditWeaponForm
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
            this.SC_EW_Btn_Save = new System.Windows.Forms.Button();
            this.SC_EW_Btn_Cancel = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.SC_EW_BulletType = new System.Windows.Forms.ComboBox();
            this.SC_EW_Btn_EditBulletType = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.SC_EW_Text_X = new System.Windows.Forms.TextBox();
            this.SC_EW_Text_Y = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.SC_EW_Text_Z = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.SC_EW_Text_Spread = new System.Windows.Forms.TextBox();
            this.SC_EW_Text_Cooldown = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // SC_EW_Btn_Save
            // 
            this.SC_EW_Btn_Save.Location = new System.Drawing.Point(12, 116);
            this.SC_EW_Btn_Save.Name = "SC_EW_Btn_Save";
            this.SC_EW_Btn_Save.Size = new System.Drawing.Size(75, 23);
            this.SC_EW_Btn_Save.TabIndex = 0;
            this.SC_EW_Btn_Save.Text = "Save";
            this.SC_EW_Btn_Save.UseVisualStyleBackColor = true;
            this.SC_EW_Btn_Save.Click += new System.EventHandler(this.SC_EW_Btn_Save_Click);
            // 
            // SC_EW_Btn_Cancel
            // 
            this.SC_EW_Btn_Cancel.Location = new System.Drawing.Point(199, 116);
            this.SC_EW_Btn_Cancel.Name = "SC_EW_Btn_Cancel";
            this.SC_EW_Btn_Cancel.Size = new System.Drawing.Size(75, 23);
            this.SC_EW_Btn_Cancel.TabIndex = 1;
            this.SC_EW_Btn_Cancel.Text = "Cancel";
            this.SC_EW_Btn_Cancel.UseVisualStyleBackColor = true;
            this.SC_EW_Btn_Cancel.Click += new System.EventHandler(this.SC_EW_Btn_Cancel_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(14, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(54, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Cooldown";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(14, 41);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Spread";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(14, 66);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(44, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Position";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(14, 93);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(60, 13);
            this.label4.TabIndex = 5;
            this.label4.Text = "Bullet Type";
            // 
            // SC_EW_BulletType
            // 
            this.SC_EW_BulletType.FormattingEnabled = true;
            this.SC_EW_BulletType.Location = new System.Drawing.Point(81, 89);
            this.SC_EW_BulletType.Name = "SC_EW_BulletType";
            this.SC_EW_BulletType.Size = new System.Drawing.Size(121, 21);
            this.SC_EW_BulletType.TabIndex = 6;
            // 
            // SC_EW_Btn_EditBulletType
            // 
            this.SC_EW_Btn_EditBulletType.Location = new System.Drawing.Point(208, 89);
            this.SC_EW_Btn_EditBulletType.Name = "SC_EW_Btn_EditBulletType";
            this.SC_EW_Btn_EditBulletType.Size = new System.Drawing.Size(66, 21);
            this.SC_EW_Btn_EditBulletType.TabIndex = 7;
            this.SC_EW_Btn_EditBulletType.Text = "Edit";
            this.SC_EW_Btn_EditBulletType.UseVisualStyleBackColor = true;
            this.SC_EW_Btn_EditBulletType.Click += new System.EventHandler(this.SC_EW_Btn_EditBulletType_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(55, 66);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(20, 13);
            this.label5.TabIndex = 8;
            this.label5.Text = "X: ";
            // 
            // SC_EW_Text_X
            // 
            this.SC_EW_Text_X.Location = new System.Drawing.Point(72, 63);
            this.SC_EW_Text_X.Name = "SC_EW_Text_X";
            this.SC_EW_Text_X.Size = new System.Drawing.Size(52, 20);
            this.SC_EW_Text_X.TabIndex = 9;
            // 
            // SC_EW_Text_Y
            // 
            this.SC_EW_Text_Y.Location = new System.Drawing.Point(143, 63);
            this.SC_EW_Text_Y.Name = "SC_EW_Text_Y";
            this.SC_EW_Text_Y.Size = new System.Drawing.Size(52, 20);
            this.SC_EW_Text_Y.TabIndex = 11;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(126, 66);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(20, 13);
            this.label6.TabIndex = 10;
            this.label6.Text = "Y: ";
            // 
            // SC_EW_Text_Z
            // 
            this.SC_EW_Text_Z.Location = new System.Drawing.Point(213, 63);
            this.SC_EW_Text_Z.Name = "SC_EW_Text_Z";
            this.SC_EW_Text_Z.Size = new System.Drawing.Size(52, 20);
            this.SC_EW_Text_Z.TabIndex = 13;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(196, 66);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(20, 13);
            this.label7.TabIndex = 12;
            this.label7.Text = "Z: ";
            // 
            // SC_EW_Text_Spread
            // 
            this.SC_EW_Text_Spread.Location = new System.Drawing.Point(72, 38);
            this.SC_EW_Text_Spread.Name = "SC_EW_Text_Spread";
            this.SC_EW_Text_Spread.Size = new System.Drawing.Size(100, 20);
            this.SC_EW_Text_Spread.TabIndex = 14;
            // 
            // SC_EW_Text_Cooldown
            // 
            this.SC_EW_Text_Cooldown.Location = new System.Drawing.Point(72, 12);
            this.SC_EW_Text_Cooldown.Name = "SC_EW_Text_Cooldown";
            this.SC_EW_Text_Cooldown.Size = new System.Drawing.Size(100, 20);
            this.SC_EW_Text_Cooldown.TabIndex = 15;
            // 
            // EditWeaponForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(283, 154);
            this.Controls.Add(this.SC_EW_Text_Cooldown);
            this.Controls.Add(this.SC_EW_Text_Spread);
            this.Controls.Add(this.SC_EW_Text_Z);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.SC_EW_Text_Y);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.SC_EW_Text_X);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.SC_EW_Btn_EditBulletType);
            this.Controls.Add(this.SC_EW_BulletType);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.SC_EW_Btn_Cancel);
            this.Controls.Add(this.SC_EW_Btn_Save);
            this.Name = "EditWeaponForm";
            this.Text = "EditWeaponForm";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button SC_EW_Btn_Save;
        private System.Windows.Forms.Button SC_EW_Btn_Cancel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox SC_EW_BulletType;
        private System.Windows.Forms.Button SC_EW_Btn_EditBulletType;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox SC_EW_Text_X;
        private System.Windows.Forms.TextBox SC_EW_Text_Y;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox SC_EW_Text_Z;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox SC_EW_Text_Spread;
        private System.Windows.Forms.TextBox SC_EW_Text_Cooldown;
    }
}