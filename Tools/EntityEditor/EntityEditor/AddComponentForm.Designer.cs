namespace EntityEditor
{
    partial class AddComponentForm
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
            this.ACF_Btn_Add = new System.Windows.Forms.Button();
            this.ACF_Btn_Cancel = new System.Windows.Forms.Button();
            this.ACF_CB_ComponentType = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // ACF_Btn_Add
            // 
            this.ACF_Btn_Add.Location = new System.Drawing.Point(16, 49);
            this.ACF_Btn_Add.Name = "ACF_Btn_Add";
            this.ACF_Btn_Add.Size = new System.Drawing.Size(75, 23);
            this.ACF_Btn_Add.TabIndex = 0;
            this.ACF_Btn_Add.Text = "Add";
            this.ACF_Btn_Add.UseVisualStyleBackColor = true;
            this.ACF_Btn_Add.Click += new System.EventHandler(this.ACF_Btn_Add_Click);
            // 
            // ACF_Btn_Cancel
            // 
            this.ACF_Btn_Cancel.Location = new System.Drawing.Point(156, 49);
            this.ACF_Btn_Cancel.Name = "ACF_Btn_Cancel";
            this.ACF_Btn_Cancel.Size = new System.Drawing.Size(75, 23);
            this.ACF_Btn_Cancel.TabIndex = 1;
            this.ACF_Btn_Cancel.Text = "Cancel";
            this.ACF_Btn_Cancel.UseVisualStyleBackColor = true;
            this.ACF_Btn_Cancel.Click += new System.EventHandler(this.ACF_Btn_Cancel_Click);
            // 
            // ACF_CB_ComponentType
            // 
            this.ACF_CB_ComponentType.FormattingEnabled = true;
            this.ACF_CB_ComponentType.Location = new System.Drawing.Point(110, 10);
            this.ACF_CB_ComponentType.Name = "ACF_CB_ComponentType";
            this.ACF_CB_ComponentType.Size = new System.Drawing.Size(121, 21);
            this.ACF_CB_ComponentType.TabIndex = 2;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(91, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Component Type:";
            // 
            // AddComponentForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(253, 85);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.ACF_CB_ComponentType);
            this.Controls.Add(this.ACF_Btn_Cancel);
            this.Controls.Add(this.ACF_Btn_Add);
            this.Name = "AddComponentForm";
            this.Text = "AddComponentForm";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button ACF_Btn_Add;
        private System.Windows.Forms.Button ACF_Btn_Cancel;
        private System.Windows.Forms.ComboBox ACF_CB_ComponentType;
        private System.Windows.Forms.Label label1;
    }
}