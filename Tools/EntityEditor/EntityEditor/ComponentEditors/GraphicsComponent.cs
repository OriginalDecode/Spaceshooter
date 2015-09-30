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

namespace EntityEditor.ComponentEditors
{
    public enum eGraphicsComponentType
    {
        Model,
        Cube
    };

    public partial class GraphicsComponent : Form
    {
        private Entity.GraphicsComponentData myGraphicsComponentData;
        private string myGraphicsFolder = Properties.Settings.Default.DefualtDataFolder;

        public GraphicsComponent()
        {
            InitializeComponent();
            myGraphicsComponentData = new Entity.GraphicsComponentData();
            myGraphicsComponentData.myIsActive = true;

            LoadGCType();
            LoadSettings();
        }

        public GraphicsComponent(Form aParent)
        {
            InitializeComponent();
            this.Owner = aParent;
            myGraphicsComponentData = new Entity.GraphicsComponentData();
            myGraphicsComponentData.myIsActive = true;

            LoadGCType();
            LoadSettings();
        }

        public GraphicsComponent(Form aParent, Entity.GraphicsComponentData aGraphicsComponentData)
        {
            InitializeComponent();
            this.Owner = aParent;
            myGraphicsComponentData = aGraphicsComponentData;
            LoadGCType();
            LoadSettings();
        }

        public void DestroyWindowForm(bool aSaveEntityFlag)
        {
            EntityEditorForm parentForm = (EntityEditorForm)this.Owner;

            if (aSaveEntityFlag == true)
            {
                SaveSettings();
                parentForm.SetGraphicsComponent(myGraphicsComponentData);
                parentForm.DisplayEntityData();
            }

            this.Visible = false;
            this.Dispose();

            parentForm.Visible = true;
            parentForm.Activate();
        }

        private void LoadGCType()
        {
            GC_Type.Items.Add(eGraphicsComponentType.Model);
            //GC_Type.Items.Add(eGraphicsComponentType.Cube);
        }

        private void LoadSettings()
        {
            GC_Active.Checked = myGraphicsComponentData.myIsActive;

            if (myGraphicsComponentData.myModelPath != "")
            {
                GC_Type.SelectedIndex = (int)eGraphicsComponentType.Model;

                GC_Text_ModelFile.Text = myGraphicsComponentData.myModelPath;
                GC_Text_EffectFile.Text = myGraphicsComponentData.myEffectPath;
            }
        }

        private void SaveSettings()
        {
            myGraphicsComponentData.myIsActive = GC_Active.Checked;

            myGraphicsComponentData.myModelPath = GC_Text_ModelFile.Text;
            myGraphicsComponentData.myEffectPath = GC_Text_EffectFile.Text;
        }

        private string GC_ConvertPathToOwnFolder(string aPath, string aFileName, string aTargetFolder)
        {
            string convertedPath = aPath.Replace(aFileName, "");
            if (convertedPath == "") return "";
            convertedPath = convertedPath.Replace("\\", "/");
            convertedPath = convertedPath.Remove(convertedPath.Length - 1);

            convertedPath = EntityEditorForm.Reverse(convertedPath);
            int firstSlash = convertedPath.IndexOf("/");

            convertedPath = convertedPath.Substring(0, firstSlash);
            convertedPath = EntityEditorForm.Reverse(convertedPath);

            convertedPath = aTargetFolder + "/" + convertedPath + "/" + aFileName;

            return convertedPath;
        }

        private void GC_Btn_BrowseModelFile_Click(object sender, EventArgs e)
        {
            browseFileDialog.Filter = "FBX files (*.fbx)|*.fbx";
            if (myGraphicsFolder != "")
            {
                browseFileDialog.InitialDirectory = myGraphicsFolder;
            }
            browseFileDialog.ShowDialog();

            if (browseFileDialog.FileName != null)
            {
                string filePath = GC_ConvertPathToOwnFolder(browseFileDialog.FileName, 
                        browseFileDialog.SafeFileName, "Data/rescources/model");

                GC_Text_ModelFile.Text = filePath;
            }

            myGraphicsFolder = browseFileDialog.FileName.Replace(browseFileDialog.SafeFileName, "");
            Properties.Settings.Default.DefualtDataFolder = myGraphicsFolder;
            Properties.Settings.Default.Save();
            browseFileDialog.Filter = "";
        }

        private void GC_Btn_BrowseEffectFile_Click(object sender, EventArgs e)
        {
            browseFileDialog.Filter = "Effect files (*.fx)|*.fx";
            if (myGraphicsFolder != "")
            {
                browseFileDialog.InitialDirectory = myGraphicsFolder;
            }
            browseFileDialog.ShowDialog();

            if (browseFileDialog.FileName != null)
            {
                string filePath = GC_ConvertPathToOwnFolder(browseFileDialog.FileName,
                        browseFileDialog.SafeFileName, "Data");

                GC_Text_EffectFile.Text = filePath;
            }

            myGraphicsFolder = browseFileDialog.FileName.Replace(browseFileDialog.SafeFileName, "");
            Properties.Settings.Default.DefualtDataFolder = myGraphicsFolder;
            Properties.Settings.Default.Save();
            browseFileDialog.Filter = "";
        }

        private void GC_Btn_Cancel_Click(object sender, EventArgs e)
        {
            DestroyWindowForm(false);
        }

        private void GC_Btn_Save_Click(object sender, EventArgs e)
        {
            DestroyWindowForm(true);
        }
    }
}
