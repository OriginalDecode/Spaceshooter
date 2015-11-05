using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using CSharpUtilities;

namespace ParticleEditor
{
    public partial class ParticleEditor : Form
    {
        private CSharpUtilities.Components.DLLPreviewComponent myPreviewWindow;
        public ParticleEditor()
        {
            InitializeComponent();
            this.Text += " v.0.0";

            myPreviewWindow = new CSharpUtilities.Components.DLLPreviewComponent(new Point(0, 0), new Size(500, 600), "Preview", true);
            myPreviewWindow.BindToPanel(myParticleWindow);
            myPreviewWindow.Show();

            CSharpUtilities.DLLImporter.NativeMethods.LoadParticle("Data/Resource/Particle/P_default_health.xml");

            UpdateTimer.Start();
        }

        private void UpdateTimer_Tick(object sender, EventArgs e)
        {
            myPreviewWindow.Update();
        }
    }
}
