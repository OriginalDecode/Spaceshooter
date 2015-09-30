namespace EntityEditor.ComponentEditors
{
    partial class CollisionComponent
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
            this.CC_Active = new System.Windows.Forms.CheckBox();
            this.CC_Btn_Cancel = new System.Windows.Forms.Button();
            this.CC_Btn_Save = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.CC_Sphere_Radius = new System.Windows.Forms.NumericUpDown();
            this.CC_Sphere_Active = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.CC_Sphere_Radius)).BeginInit();
            this.SuspendLayout();
            // 
            // CC_Active
            // 
            this.CC_Active.AutoSize = true;
            this.CC_Active.Location = new System.Drawing.Point(12, 12);
            this.CC_Active.Name = "CC_Active";
            this.CC_Active.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.CC_Active.Size = new System.Drawing.Size(59, 17);
            this.CC_Active.TabIndex = 8;
            this.CC_Active.Text = ":Active";
            this.CC_Active.UseVisualStyleBackColor = true;
            // 
            // CC_Btn_Cancel
            // 
            this.CC_Btn_Cancel.Location = new System.Drawing.Point(149, 86);
            this.CC_Btn_Cancel.Name = "CC_Btn_Cancel";
            this.CC_Btn_Cancel.Size = new System.Drawing.Size(75, 23);
            this.CC_Btn_Cancel.TabIndex = 7;
            this.CC_Btn_Cancel.Text = "Cancel";
            this.CC_Btn_Cancel.UseVisualStyleBackColor = true;
            this.CC_Btn_Cancel.Click += new System.EventHandler(this.CC_Btn_Cancel_Click);
            // 
            // CC_Btn_Save
            // 
            this.CC_Btn_Save.Location = new System.Drawing.Point(12, 86);
            this.CC_Btn_Save.Name = "CC_Btn_Save";
            this.CC_Btn_Save.Size = new System.Drawing.Size(75, 23);
            this.CC_Btn_Save.TabIndex = 6;
            this.CC_Btn_Save.Text = "Save";
            this.CC_Btn_Save.UseVisualStyleBackColor = true;
            this.CC_Btn_Save.Click += new System.EventHandler(this.CC_Btn_Save_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 60);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(80, 13);
            this.label1.TabIndex = 9;
            this.label1.Text = "Sphere Radius:";
            // 
            // CC_Sphere_Radius
            // 
            this.CC_Sphere_Radius.DecimalPlaces = 2;
            this.CC_Sphere_Radius.Location = new System.Drawing.Point(99, 58);
            this.CC_Sphere_Radius.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.CC_Sphere_Radius.Name = "CC_Sphere_Radius";
            this.CC_Sphere_Radius.Size = new System.Drawing.Size(120, 20);
            this.CC_Sphere_Radius.TabIndex = 10;
            // 
            // CC_Sphere_Active
            // 
            this.CC_Sphere_Active.AutoSize = true;
            this.CC_Sphere_Active.Location = new System.Drawing.Point(12, 35);
            this.CC_Sphere_Active.Name = "CC_Sphere_Active";
            this.CC_Sphere_Active.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.CC_Sphere_Active.Size = new System.Drawing.Size(96, 17);
            this.CC_Sphere_Active.TabIndex = 11;
            this.CC_Sphere_Active.Text = ":Sphere Active";
            this.CC_Sphere_Active.UseVisualStyleBackColor = true;
            this.CC_Sphere_Active.CheckedChanged += new System.EventHandler(this.CC_Sphere_Active_CheckedChanged);
            // 
            // CollisionComponent
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(240, 121);
            this.Controls.Add(this.CC_Sphere_Active);
            this.Controls.Add(this.CC_Sphere_Radius);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.CC_Active);
            this.Controls.Add(this.CC_Btn_Cancel);
            this.Controls.Add(this.CC_Btn_Save);
            this.Name = "CollisionComponent";
            this.Text = "CollisionComponent";
            ((System.ComponentModel.ISupportInitialize)(this.CC_Sphere_Radius)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox CC_Active;
        private System.Windows.Forms.Button CC_Btn_Cancel;
        private System.Windows.Forms.Button CC_Btn_Save;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown CC_Sphere_Radius;
        private System.Windows.Forms.CheckBox CC_Sphere_Active;
    }
}