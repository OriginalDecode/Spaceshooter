using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using CSharpUtilities;

namespace ModelViewer
{
    public partial class ModelViewerWindow : Form
    {
        private List<string> myModelFiles = null;
        private List<string> myShaderFiles = null;

        private string myCurrentShaderFilePath = Properties.Settings.Default.DefaultEffectFilePath;
        private string myCurrentModelFilePath = Properties.Settings.Default.DefaultModelFilePath;

        private string myDataFolderPath = Properties.Settings.Default.DefaultDataFolderPath;

        private Point myPreviousMousePosition;

        private Button myChooseDataFolderButton = new Button();

        private CSharpUtilities.Components.DropDownComponent myModelList;
        private CSharpUtilities.Components.DropDownComponent myShaderList;

        private CSharpUtilities.Components.DLLPreviewComponent myPreviewWindow;

        public ModelViewerWindow()
        {
            InitializeComponent();

            if (myDataFolderPath == "")
            {
                Properties.Settings.Default.DefaultDataFolderPath = Directory.GetCurrentDirectory();
                myDataFolderPath = Properties.Settings.Default.DefaultDataFolderPath;
                Properties.Settings.Default.Save();
            }

            DataFolderBrowser.SelectedPath = myDataFolderPath;

            myPreviousMousePosition = MousePosition;

            myChooseDataFolderButton.Text = "Open";
            myChooseDataFolderButton.Location = new Point(Location.X + 5, Location.Y+10);
            myChooseDataFolderButton.Size = new Size(50, 20);
            myChooseDataFolderButton.Show();
            myChooseDataFolderButton.Click += new EventHandler(this.Btn_OpenDataFolder_Click);
            Menu_Panel.Controls.Add(myChooseDataFolderButton);

            myModelList = new CSharpUtilities.Components.DropDownComponent(new Point(Location.X + 60, Location.Y+10), new Size(250, 13), "Model: ");
            myModelList.AddSelectedIndexChangeEvent(this.ModelList_SelectedIndexChanged);
            myModelList.BindToPanel(Menu_Panel);
            myModelList.Show();

            myShaderList = new CSharpUtilities.Components.DropDownComponent(new Point(Location.X + 310, Location.Y+10), new Size(250, 13), "Shader: ");
            myShaderList.AddSelectedIndexChangeEvent(this.ShaderList_SelectedIndexChanged);
            myShaderList.BindToPanel(Menu_Panel);
            myShaderList.Show();

            myPreviewWindow = new CSharpUtilities.Components.DLLPreviewComponent(new Point(ModelViewer.Location.X, ModelViewer.Location.Y - 20), ModelViewer.Size, "Preview", true);
            myPreviewWindow.BindToPanel(ModelViewer);
            myPreviewWindow.Show();

            FillModelList();
            FillShaderList();

            UpdateTimer.Start();
        }

        private void Btn_OpenDataFolder_Click(object sender, EventArgs e)
        {
            if (Directory.Exists(myDataFolderPath) == false)
            {
                myDataFolderPath = Directory.GetCurrentDirectory();
            }
            DataFolderBrowser.SelectedPath = myDataFolderPath;
            DataFolderBrowser.ShowDialog();
            myDataFolderPath = DataFolderBrowser.SelectedPath;

            if (myDataFolderPath != "")
            {
                FillModelList();
                FillShaderList();
            }

            Properties.Settings.Default.DefaultDataFolderPath = myDataFolderPath;
            Properties.Settings.Default.Save();
        }

        void FillModelList()
        {
            myModelList.GetDropDown().Items.Clear();
            myModelFiles = Directory.GetFiles(myDataFolderPath, "*.fbx", SearchOption.AllDirectories).ToList();
            for (int i = 0; i < myModelFiles.Count; ++i)
            {
                string file = StringUtilities.ConvertPathToRelativePath(myModelFiles[i], "Model\\");
                myModelList.AddItem(file);
            }
        }

        private void FillShaderList()
        {
            myShaderList.GetDropDown().Items.Clear();
            myShaderFiles = Directory.GetFiles(myDataFolderPath, "*.fx", SearchOption.AllDirectories).ToList();
            for (int i = 0; i < myShaderFiles.Count; ++i)
            {
                string file = StringUtilities.ConvertPathToRelativePath(myShaderFiles[i], "Shader\\");
                if (file.StartsWith("S_effect") == true && VerifyShader(file) == true)
                {
                    myShaderList.AddItem(file);
                }
            }
        }

        private bool VerifyShader(string aFileName)
        {
            if (aFileName.EndsWith("pbl.fx")
                || aFileName.EndsWith("font.fx")
                || aFileName.EndsWith("sprite.fx")
                || aFileName.EndsWith("graph.fx")
                || aFileName.EndsWith("debug.fx")
                || aFileName.EndsWith("skybox.fx")
                || aFileName.EndsWith("basic.fx"))
            {
                return true;
            }
            return false;
        }

        private void ModelList_SelectedIndexChanged(object sender, EventArgs e)
        {
            string selectedItem = (string)myModelList.GetDropDown().SelectedItem;
            selectedItem = selectedItem.Replace("/", "\\");
            for (int i = 0; i < myModelFiles.Count; ++i)
            {
                if (myModelFiles[i].EndsWith(selectedItem) == true)
                {
                    myCurrentModelFilePath = myModelFiles[i];
                    break;
                }
            }
            if (myCurrentShaderFilePath != "" && myCurrentModelFilePath != "")
            {
                Btn_LoadModel_Click(sender, e);
            }
        }

        private void ShaderList_SelectedIndexChanged(object sender, EventArgs e)
        {
            string selectedItem = (string)myShaderList.GetDropDown().SelectedItem;
            selectedItem = selectedItem.Replace("/", "\\");
            for (int i = 0; i < myShaderFiles.Count; ++i)
            {
                if (myShaderFiles[i].EndsWith(selectedItem) == true)
                {
                    myCurrentShaderFilePath = myShaderFiles[i];
                    break;
                }
            }
            if (myCurrentShaderFilePath != "" && myCurrentModelFilePath != "")
            {
                Btn_LoadModel_Click(sender, e);
            }
        }

        private void UpdateTimer_Tick(object sender, EventArgs e)
        {
            myPreviewWindow.Update();
        }

        private void Btn_LoadModel_Click(object sender, EventArgs e)
        {
            if (myCurrentModelFilePath == "")
            {
                MessageBox.Show("Error: No model file is selected");
                return;
            }
            if (myCurrentShaderFilePath == "")
            {
                MessageBox.Show("Error: No effect file is selected");
                return;
            }
            if (myCurrentModelFilePath != "" && myCurrentShaderFilePath != "")
            {
                if (Path.GetExtension(myCurrentModelFilePath) == ".fbx")
                {
                    CSharpUtilities.DLLImporter.NativeMethods.LoadModel(myCurrentModelFilePath, myCurrentShaderFilePath);
                }
                else
                {
                    MessageBox.Show("Error: " + Path.GetExtension(myCurrentModelFilePath) + " is not compatible. \nTry using a .fbx file instead.");
                }
            }
        }

        private void Btn_BackgroundColor_Click(object sender, EventArgs e)
        {
            BackgroundColorDialog.ShowDialog();
            Btn_BackgroundColor.BackColor = BackgroundColorDialog.Color;

            float redChannel =   (BackgroundColorDialog.Color.R)/ 255.0f;
            float greenChannel = (BackgroundColorDialog.Color.G)/ 255.0f;
            float blueChannel =  (BackgroundColorDialog.Color.B)/ 255.0f;
            float alphaChannel = (BackgroundColorDialog.Color.A)/ 255.0f;

            CSharpUtilities.DLLImporter.NativeMethods.SetClearColor(redChannel, greenChannel, blueChannel, alphaChannel);
        }

        private void DirectionalLightX_Scroll(object sender, ScrollEventArgs e)
        {
            float xValue = DirectionLightX.Value / (360.0f / 2) - 1;
            float xTruncatedValue = (float)(Math.Truncate((double)xValue * 100.0) / 100.0);
            DirectionLightXValue.Text = "X: " + xTruncatedValue.ToString();

            CSharpUtilities.DLLImporter.NativeMethods.DirectionaLightRotateX(xTruncatedValue);
        }

        private void DirectionalLightY_Scroll(object sender, ScrollEventArgs e)
        {
            float yValue = DirectionalLightY.Value / (360.0f /2) - 1;
            float yTruncatedValue = (float)(Math.Truncate((double)yValue * 100.0) / 100.0);
            DirectionalLightYValue.Text = "Y: " + yTruncatedValue.ToString();

            CSharpUtilities.DLLImporter.NativeMethods.DirectionaLightRotateY(yTruncatedValue);
        }

        private void DirectionalLightZ_Scroll(object sender, ScrollEventArgs e)
        {
            float zValue = DirectionalLightZ.Value / (360.0f / 2) - 1;
            float zTruncatedValue = (float)(Math.Truncate((double)zValue * 100.0) / 100.0);
            DirectionalLightZValue.Text = "Z: " + zTruncatedValue.ToString();

            CSharpUtilities.DLLImporter.NativeMethods.DirectionaLightRotateZ(zTruncatedValue);
        }

    }
}
