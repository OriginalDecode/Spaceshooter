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

namespace ModelViewer
{
    public partial class ModelViewerWindow : Form
    {
        private string myEffectFolderPath = "";
        private List<FileInfo> myEffectFiles = null;
        private OpenModelWindow myOpenModelWindowForm;
        private FileInfo myCurrentModelFile = null;

        public ModelViewerWindow()
        {
            InitializeComponent();
        }

        private void Btn_OpenModel_Click(object sender, EventArgs e)
        {
            myOpenModelWindowForm = new OpenModelWindow(this);
            myOpenModelWindowForm.Activate();
            myOpenModelWindowForm.Visible = true;
        }

        private void Btn_OpenEffectFolder_Click(object sender, EventArgs e)
        {
            effectFolderBrowser.ShowDialog();
            myEffectFolderPath = effectFolderBrowser.SelectedPath;
            if (myEffectFolderPath != "")
            {
                FillEffectList();
            }
        }

        private void FillEffectList()
        {
            DirectoryInfo currentDirectory = new DirectoryInfo(myEffectFolderPath);

            FileInfo[] filesInDirectory = currentDirectory.GetFiles();

            myEffectFiles = SortOutEffectFiles(filesInDirectory);

            for (int i = 0; i < myEffectFiles.Count; ++i)
            {
                EffectFilter.Items.Add(myEffectFiles[i].Name);
            }
        }

        private List<FileInfo> SortOutEffectFiles(FileInfo[] someFiles)
        {
            List<FileInfo> effectFile = new List<FileInfo>();

            for (int i = 0; i < someFiles.Length; ++i)
            {
                if (someFiles[i].Extension == ".fx")
                {
                    effectFile.Add(someFiles[i]);
                }
            }
            return effectFile;
        }

        public void SetModelFile(FileInfo aModelFile)
        {
            myCurrentModelFile = aModelFile;
        }
    }
}
