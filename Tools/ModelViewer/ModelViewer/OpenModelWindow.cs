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
    public partial class OpenModelWindow : Form
    {
        private string myModelFolder = "";
        private List<FileInfo> myModelFiles = new List<FileInfo>();
        public OpenModelWindow()
        {
            InitializeComponent();
        }

        public OpenModelWindow(Form aParentForm)
        {
            this.Owner = aParentForm;
            InitializeComponent();
        }

        private void Btn_CloseWindow_Click(object sender, EventArgs e)
        {
            ModelViewerWindow modelViewWindow = (ModelViewerWindow)this.Owner;
            this.Visible = false;
            this.Dispose();
        }

        private void Btn_OpenModelFolder_Click(object sender, EventArgs e)
        {
            modelFolderBroweser.ShowDialog();
            myModelFolder = modelFolderBroweser.SelectedPath;
            FillFileTree();
        }

        private void Btn_OpenFile_Click(object sender, EventArgs e)
        {
            ModelViewerWindow modelViewWindow = (ModelViewerWindow)this.Owner;
            modelViewWindow.SetModelFile(GetSelectedFile());
            this.Visible = false;
            this.Dispose();
        }

        private void FillFileTree()
        {
            DirectoryInfo currentDirectory = new DirectoryInfo(myModelFolder);

            RetriveAllModelFilesInDirectory(currentDirectory);

            for (int i = 0; i < myModelFiles.Count; ++i)
            {
                modelFileListWindow.Items.Add(myModelFiles[i].Name);
            }
        }

        private void RetriveAllModelFilesInDirectory(DirectoryInfo aDirectory)
        {
            FileInfo[] filesInDirectory = aDirectory.GetFiles();

            for (int i = 0; i < filesInDirectory.Length; ++i)
            {
                if (filesInDirectory[i].Extension == ".fbx")
                {
                    myModelFiles.Add(filesInDirectory[i]);
                }
            }
        }

        private FileInfo GetSelectedFile()
        {
            string filePath = "";

            string fileName = (string)modelFileListWindow.SelectedItem;

            for (int i = 0; i < myModelFiles.Count; ++i)
            {
                if (myModelFiles[i].Name == fileName)
                {
                    filePath = myModelFiles[i].FullName;
                    break;
                }
            }
            FileInfo selectedFile = new FileInfo(filePath);

            return selectedFile;
        }
    }
}
