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

        private string myCurrentEffectFilePath = "";

        public ModelViewerWindow()
        {
            InitializeComponent();

            IntPtr windowHandle = ModelViewer.Handle;

            ModelViewer.Invalidate();
            //Action<Message> windowProc = WndProc;

            Int32 width = ModelViewer.Width;
            Int32 height = ModelViewer.Height;

            NativeMethods.SetupWindow(width, height);
            NativeMethods.StartEngine(windowHandle);
            NativeMethods.Render();

            UpdateTimer.Start();
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

        private void ModelViewer_Paint(object sender, PaintEventArgs e)
        {
            NativeMethods.Update();
            NativeMethods.Render();
        }

        private void EffectFilter_SelectedIndexChanged(object sender, EventArgs e)
        {
            for (int i = 0; i < myEffectFiles.Count; ++i)
            {
                if (myEffectFiles[i].Name == EffectFilter.SelectedItem)
                {
                    myCurrentEffectFilePath = myEffectFiles[i].FullName;
                    NativeMethods.SetEffect(myCurrentEffectFilePath);
                }
            }
        }

        private void UpdateTimer_Tick(object sender, EventArgs e)
        {
            ModelViewer.Invalidate();
        }

        private void Btn_LoadModel_Click(object sender, EventArgs e)
        {
            NativeMethods.LoadModel(myCurrentModelFile.FullName, myCurrentEffectFilePath);
        }
    }
}
