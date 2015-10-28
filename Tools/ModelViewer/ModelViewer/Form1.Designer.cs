
﻿namespace ModelViewer
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
            this.components = new System.ComponentModel.Container();
            this.Menu_Panel = new System.Windows.Forms.Panel();
            this.Btn_BackgroundColor = new System.Windows.Forms.Button();
            this.Btn_LoadModel = new System.Windows.Forms.Button();
            this.ModelViewer = new System.Windows.Forms.Panel();
            this.ModelViewerMenu = new System.Windows.Forms.Panel();
            this.DirectionalLightZValue = new System.Windows.Forms.Label();
            this.DirectionalLightZ = new System.Windows.Forms.HScrollBar();
            this.DirectionalLightYValue = new System.Windows.Forms.Label();
            this.DirectionalLightY = new System.Windows.Forms.HScrollBar();
            this.label2 = new System.Windows.Forms.Label();
            this.DirectionLightXValue = new System.Windows.Forms.Label();
            this.DirectionLightX = new System.Windows.Forms.HScrollBar();
            this.UpdateTimer = new System.Windows.Forms.Timer(this.components);
            this.BackgroundColorDialog = new System.Windows.Forms.ColorDialog();
            this.DataFolderBrowser = new System.Windows.Forms.FolderBrowserDialog();
            this.Menu_Panel.SuspendLayout();
            this.ModelViewerMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // Menu_Panel
            // 
            this.Menu_Panel.Controls.Add(this.Btn_BackgroundColor);
            this.Menu_Panel.Controls.Add(this.Btn_LoadModel);
            this.Menu_Panel.Dock = System.Windows.Forms.DockStyle.Top;
            this.Menu_Panel.Location = new System.Drawing.Point(0, 0);
            this.Menu_Panel.Name = "Menu_Panel";
            this.Menu_Panel.Size = new System.Drawing.Size(1085, 32);
            this.Menu_Panel.TabIndex = 0;
            // 
            // Btn_BackgroundColor
            // 
            this.Btn_BackgroundColor.BackColor = System.Drawing.SystemColors.Control;
            this.Btn_BackgroundColor.Location = new System.Drawing.Point(672, 10);
            this.Btn_BackgroundColor.Name = "Btn_BackgroundColor";
            this.Btn_BackgroundColor.Size = new System.Drawing.Size(20, 20);
            this.Btn_BackgroundColor.TabIndex = 5;
            this.Btn_BackgroundColor.UseVisualStyleBackColor = false;
            this.Btn_BackgroundColor.Click += new System.EventHandler(this.Btn_BackgroundColor_Click);
            // 
            // Btn_LoadModel
            // 
            this.Btn_LoadModel.Location = new System.Drawing.Point(591, 10);
            this.Btn_LoadModel.Name = "Btn_LoadModel";
            this.Btn_LoadModel.Size = new System.Drawing.Size(75, 20);
            this.Btn_LoadModel.TabIndex = 4;
            this.Btn_LoadModel.Text = "Load Model";
            this.Btn_LoadModel.UseVisualStyleBackColor = true;
            this.Btn_LoadModel.Click += new System.EventHandler(this.Btn_LoadModel_Click);
            // 
            // ModelViewer
            // 
            this.ModelViewer.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.ModelViewer.Location = new System.Drawing.Point(0, 32);
            this.ModelViewer.Name = "ModelViewer";
            this.ModelViewer.Size = new System.Drawing.Size(890, 720);
            this.ModelViewer.TabIndex = 1;
            // 
            // ModelViewerMenu
            // 
            this.ModelViewerMenu.Controls.Add(this.DirectionalLightZValue);
            this.ModelViewerMenu.Controls.Add(this.DirectionalLightZ);
            this.ModelViewerMenu.Controls.Add(this.DirectionalLightYValue);
            this.ModelViewerMenu.Controls.Add(this.DirectionalLightY);
            this.ModelViewerMenu.Controls.Add(this.label2);
            this.ModelViewerMenu.Controls.Add(this.DirectionLightXValue);
            this.ModelViewerMenu.Controls.Add(this.DirectionLightX);
            this.ModelViewerMenu.Dock = System.Windows.Forms.DockStyle.Right;
            this.ModelViewerMenu.Location = new System.Drawing.Point(887, 32);
            this.ModelViewerMenu.Name = "ModelViewerMenu";
            this.ModelViewerMenu.Size = new System.Drawing.Size(198, 720);
            this.ModelViewerMenu.TabIndex = 0;
            // 
            // DirectionalLightZValue
            // 
            this.DirectionalLightZValue.AutoSize = true;
            this.DirectionalLightZValue.Location = new System.Drawing.Point(7, 50);
            this.DirectionalLightZValue.Name = "DirectionalLightZValue";
            this.DirectionalLightZValue.Size = new System.Drawing.Size(26, 13);
            this.DirectionalLightZValue.TabIndex = 7;
            this.DirectionalLightZValue.Text = "Z: 0";
            // 
            // DirectionalLightZ
            // 
            this.DirectionalLightZ.LargeChange = 1;
            this.DirectionalLightZ.Location = new System.Drawing.Point(45, 50);
            this.DirectionalLightZ.Maximum = 360;
            this.DirectionalLightZ.Name = "DirectionalLightZ";
            this.DirectionalLightZ.Size = new System.Drawing.Size(120, 13);
            this.DirectionalLightZ.TabIndex = 6;
            this.DirectionalLightZ.Scroll += new System.Windows.Forms.ScrollEventHandler(this.DirectionalLightZ_Scroll);
            // 
            // DirectionalLightYValue
            // 
            this.DirectionalLightYValue.AutoSize = true;
            this.DirectionalLightYValue.Location = new System.Drawing.Point(7, 36);
            this.DirectionalLightYValue.Name = "DirectionalLightYValue";
            this.DirectionalLightYValue.Size = new System.Drawing.Size(26, 13);
            this.DirectionalLightYValue.TabIndex = 5;
            this.DirectionalLightYValue.Text = "Y: 0";
            // 
            // DirectionalLightY
            // 
            this.DirectionalLightY.LargeChange = 1;
            this.DirectionalLightY.Location = new System.Drawing.Point(45, 36);
            this.DirectionalLightY.Maximum = 360;
            this.DirectionalLightY.Name = "DirectionalLightY";
            this.DirectionalLightY.Size = new System.Drawing.Size(120, 13);
            this.DirectionalLightY.TabIndex = 4;
            this.DirectionalLightY.Scroll += new System.Windows.Forms.ScrollEventHandler(this.DirectionalLightY_Scroll);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(24, 3);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(126, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Directional Light Rotation";
            // 
            // DirectionLightXValue
            // 
            this.DirectionLightXValue.AutoSize = true;
            this.DirectionLightXValue.Location = new System.Drawing.Point(7, 21);
            this.DirectionLightXValue.Name = "DirectionLightXValue";
            this.DirectionLightXValue.Size = new System.Drawing.Size(26, 13);
            this.DirectionLightXValue.TabIndex = 2;
            this.DirectionLightXValue.Text = "X: 0";
            // 
            // DirectionLightX
            // 
            this.DirectionLightX.LargeChange = 1;
            this.DirectionLightX.Location = new System.Drawing.Point(45, 21);
            this.DirectionLightX.Maximum = 360;
            this.DirectionLightX.Name = "DirectionLightX";
            this.DirectionLightX.Size = new System.Drawing.Size(120, 13);
            this.DirectionLightX.TabIndex = 1;
            this.DirectionLightX.Scroll += new System.Windows.Forms.ScrollEventHandler(this.DirectionalLightX_Scroll);
            // 
            // UpdateTimer
            // 
            this.UpdateTimer.Interval = 1;
            this.UpdateTimer.Tick += new System.EventHandler(this.UpdateTimer_Tick);
            // 
            // ModelViewerWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1085, 752);
            this.Controls.Add(this.ModelViewerMenu);
            this.Controls.Add(this.ModelViewer);
            this.Controls.Add(this.Menu_Panel);
            this.Name = "ModelViewerWindow";
            this.Text = "Prism Model Viewer";
            this.Menu_Panel.ResumeLayout(false);
            this.ModelViewerMenu.ResumeLayout(false);
            this.ModelViewerMenu.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel Menu_Panel;
        private System.Windows.Forms.Panel ModelViewer;
        private System.Windows.Forms.Panel ModelViewerMenu;
        private System.Windows.Forms.Timer UpdateTimer;
        private System.Windows.Forms.Button Btn_LoadModel;
        private System.Windows.Forms.Button Btn_BackgroundColor;
        private System.Windows.Forms.ColorDialog BackgroundColorDialog;
        private System.Windows.Forms.HScrollBar DirectionLightX;
        private System.Windows.Forms.Label DirectionLightXValue;
        private System.Windows.Forms.Label DirectionalLightYValue;
        private System.Windows.Forms.HScrollBar DirectionalLightY;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label DirectionalLightZValue;
        private System.Windows.Forms.HScrollBar DirectionalLightZ;
        private System.Windows.Forms.FolderBrowserDialog DataFolderBrowser;
    }
}