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

namespace EntityEditor
{
    public partial class EntityEditorForm : Form
    {
        private Entity.EntityData myCurrentEntity = new Entity.EntityData();
        private EntityReader myEntityReader = new EntityReader();
        private Entity.EntityWriter myEntityWriter = new Entity.EntityWriter();
        private Entity.EntityListXML myEntityList;

        private string myCurrentEntityFolderPath = Properties.Settings.Default.DefaultEntityFolderPath;
        private string myCurrentEntityFilePath = Properties.Settings.Default.DefaultEntityFileName;
        private string myDataFolderPath = "";

        private Panels.AddComponentPanel myAddComponentPanel = null;
        private Panels.AIComponentPanel myAIComponentPanel = null;
        private Panels.CollisionComponentPanel myCollisionComponentPanel = null;
        private Panels.GraphicsComponentPanel myGraphicsComponentPanel = null;
        private Panels.ShootingComponentPanel myShootingComponentPanel = null;
        private Panels.RenamePanel myRenameEntityPanel = null;

        public EntityEditorForm()
        {
            InitializeComponent();

            this.Text = "Entity Editor v.1.0";

            openEntityFile.InitialDirectory = myCurrentEntityFolderPath;
            DL_Debug.GetInstance.Init("EntityEditorLog");

            myDataFolderPath = StringUtilities.ConvertPathToDataFolderPath(myCurrentEntityFolderPath);

            if (myCurrentEntityFolderPath == "")
            {
                myCurrentEntityFolderPath = Directory.GetCurrentDirectory();
                Properties.Settings.Default.DefaultEntityFolderPath = myCurrentEntityFolderPath;
                Properties.Settings.Default.Save();
            }

            Point panelLocation = new Point(10, 10);
            Size panelSize = new Size(500, 800);

            myAddComponentPanel = new Panels.AddComponentPanel(panelLocation, panelSize, this);
            myAIComponentPanel = new Panels.AIComponentPanel(panelLocation, panelSize, this);
            myCollisionComponentPanel = new Panels.CollisionComponentPanel(panelLocation, panelSize, this);
            myGraphicsComponentPanel = new Panels.GraphicsComponentPanel(panelLocation, panelSize, this);
            myShootingComponentPanel = new Panels.ShootingComponentPanel(panelLocation, panelSize, this);
            myRenameEntityPanel = new Panels.RenamePanel(panelLocation, panelSize, this);

            PropertyPanel.Controls.Add(myAddComponentPanel);
            PropertyPanel.Controls.Add(myAIComponentPanel);
            PropertyPanel.Controls.Add(myCollisionComponentPanel);
            PropertyPanel.Controls.Add(myGraphicsComponentPanel);
            PropertyPanel.Controls.Add(myShootingComponentPanel);
            PropertyPanel.Controls.Add(myRenameEntityPanel);

            HidePanels();
        }

        public void DisplayEntityData()
        {
            EntityContentList.Items.Clear();

            EntityContentList.Items.Add("Name: " + myCurrentEntity.myName);
            EntityContentList.Items.Add("Components:");
            if (myCurrentEntity.myGraphicsComponent.myIsActive)
            {
                EntityContentList.Items.Add("GraphicsComponent");
            }
            if (myCurrentEntity.myShootingComponent.myIsActive)
            {
                EntityContentList.Items.Add("ShootingComponent");
            }
            if (myCurrentEntity.myAIComponent.myIsActive)
            {
                EntityContentList.Items.Add("AIComponent");
            }
            if (myCurrentEntity.myCollisionComponent.myIsActive)
            {
                EntityContentList.Items.Add("CollisionComponent");
            }
        }

        public void SetEntityList(Entity.EntityListXML aEntityList)
        {
            myEntityList = aEntityList;
        }

        public void SetEntityName(string aNewEntityName)
        {
            myCurrentEntity.myName = aNewEntityName;
            DisplayEntityData();
        }

        public string GetEntityName()
        {
            return myCurrentEntity.myName;
        }

        //Set Components To EntityData Start
        public void SetGraphicsComponent(Entity.GraphicsComponentData aGraphicsComponent)
        {
            myCurrentEntity.myGraphicsComponent = aGraphicsComponent;
        }
        public void SetAIComponent(Entity.AIComponentData aAIComponent)
        {
            myCurrentEntity.myAIComponent = aAIComponent;
        }
        public void SetShootingComponent(Entity.ShootingComponentData aShootingComponent)
        {
            myCurrentEntity.myShootingComponent = aShootingComponent;
        }
        public void SetCollisionComponent(Entity.CollisionComponentData aCollisionComponent)
        {
            myCurrentEntity.myCollisionComponent = aCollisionComponent;
        }
        //Set Components To EntityData End

        private bool CheckIfItemStartsWith(String aStartWith, int aIndex)
        {
            if (aIndex != System.Windows.Forms.ListBox.NoMatches)
            {
                string selectedName = (string)EntityContentList.Items[aIndex];
                if (selectedName.StartsWith(aStartWith))
                {
                    return true;
                }
            }
            return false;
        }

        private bool CheckIfItemEndsWith(String aEndWith, int aIndex)
        {
            if (aIndex != System.Windows.Forms.ListBox.NoMatches)
            {
                string selectedName = (string)EntityContentList.Items[aIndex];
                if (selectedName.EndsWith(aEndWith))
                {
                    return true;
                }
            }
            return false;
        }

        private void OpenRenameEntityWindow()
        {
            HidePanels();

            myRenameEntityPanel.Show();
            myRenameEntityPanel.Load(myCurrentEntity.myName);
        }

        private void RemoveSelectedComponent(string aComponentName)
        {
            if (aComponentName.StartsWith("Graphics") == true)
            {
                myCurrentEntity.myGraphicsComponent.myIsActive = false;
                myCurrentEntity.myGraphicsComponent.myModelPath = "";
                myCurrentEntity.myGraphicsComponent.myEffectPath = "";
                return;
            }
            if (aComponentName.StartsWith("AI") == true)
            {
                myCurrentEntity.myAIComponent.myIsActive = false;
                return;
            }
            if (aComponentName.StartsWith("Shooting") == true)
            {
                myCurrentEntity.myShootingComponent.myIsActive = false;
                myCurrentEntity.myShootingComponent.myWeaponType = "";
                return;
            }
            if (aComponentName.StartsWith("Collision") == true)
            {
                myCurrentEntity.myCollisionComponent.myIsActive = false;
                myCurrentEntity.myCollisionComponent.myHasSphere = false;
                myCurrentEntity.myCollisionComponent.myRadius = 0;
                return;
            }
        }

        private void HidePanels()
        {
            myAddComponentPanel.Hide();
            myAIComponentPanel.Hide();
            myCollisionComponentPanel.Hide();
            myGraphicsComponentPanel.Hide();
            myShootingComponentPanel.Hide();
            myRenameEntityPanel.Hide();
        }

        private void EditSelectedComponent(string aComponentName)
        {
            HidePanels();
            if (aComponentName.StartsWith("Graphics") == true)
            {
                myGraphicsComponentPanel.Show();
                myGraphicsComponentPanel.Load(myCurrentEntity.myGraphicsComponent);
                return;
            }
            if (aComponentName.StartsWith("AI") == true)
            {
                myAIComponentPanel.Show();
                myAIComponentPanel.Load(myCurrentEntity.myAIComponent);
                return;
            }
            if (aComponentName.StartsWith("Shooting") == true)
            {
                myShootingComponentPanel.Show();
                myShootingComponentPanel.Load(myCurrentEntity.myShootingComponent);
                return;
            }
            if (aComponentName.StartsWith("Collision") == true)
            {
                myCollisionComponentPanel.Show();
                myCollisionComponentPanel.Load(myCurrentEntity.myCollisionComponent);
                return;
            }
        }

        //----- Open Entity Buttons Section Start -----
        private void openEntityToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            openEntityFile.InitialDirectory = myCurrentEntityFolderPath;
            if (openEntityFile.InitialDirectory == "")
            {
                openEntityFile.InitialDirectory = Directory.GetCurrentDirectory();
            }

            openEntityFile.ShowDialog();
            if (openEntityFile.SafeFileName != "")
            {
                myCurrentEntity = myEntityReader.LoadFile(openEntityFile.FileName, this);

                myCurrentEntityFilePath = openEntityFile.FileName;
                myCurrentEntityFolderPath = myCurrentEntityFilePath.Replace(openEntityFile.SafeFileName, "");
                myDataFolderPath = StringUtilities.ConvertPathToDataFolderPath(myCurrentEntityFolderPath);
                myShootingComponentPanel.ReloadXML(myDataFolderPath);
                Properties.Settings.Default.DefaultEntityFolderPath = myCurrentEntityFolderPath;
                Properties.Settings.Default.DefaultEntityFileName = myCurrentEntityFilePath;
                Properties.Settings.Default.Save();

                DisplayEntityData();
            }
        }

        //----- Open Entity Buttons Section Ends -----
        private void saveEntityToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            myEntityWriter.SaveFile(myCurrentEntityFilePath, myCurrentEntity, myEntityList);
            DL_Debug.GetInstance.DL_MessageBox("Save entity " + myCurrentEntity.myName + " at\n" + myCurrentEntityFilePath,
                    "Save Successfull!", MessageBoxButtons.OK);
        }

        //----- Add Component Button Start -----
        private void addComponentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            myAddComponentPanel.Show();
            myAddComponentPanel.Load(myCurrentEntity);
        }

        //----- Add Component Button Ends -----

        //----- Remove Component Button Start -----
        private void removeComponentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string selectedName = (string)EntityContentList.SelectedItem;
            if (selectedName != null && selectedName.EndsWith("Component"))
            {
                RemoveSelectedComponent(selectedName);
                DisplayEntityData();
            }
        }
        //----- Remove Component Button Ends -----

        private void newEntityToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SetEntityName("");
            OpenRenameEntityWindow();
            myShootingComponentPanel.ReloadXML(myDataFolderPath);
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (myCurrentEntityFolderPath != "")
            {
                saveEntityFile.InitialDirectory = myCurrentEntityFolderPath;
            }
            saveEntityFile.AddExtension = true;
            saveEntityFile.DefaultExt = ".xml";
            saveEntityFile.Filter = "XML file (*.xml)|*.xml";
            saveEntityFile.ShowDialog();

            myCurrentEntityFilePath = saveEntityFile.FileName;
            Properties.Settings.Default.DefaultEntityFileName = myCurrentEntityFilePath;
            Properties.Settings.Default.Save();

            if (myCurrentEntityFilePath != "")
            {
                myEntityWriter.SaveFile(myCurrentEntityFilePath, myCurrentEntity, myEntityList);
                DL_Debug.GetInstance.DL_MessageBox("Save entity " + myCurrentEntity.myName + " at\n" + myCurrentEntityFilePath,
                        "Save Successfull!", MessageBoxButtons.OK);
            }
            else
            {
                DL_Debug.GetInstance.DL_ErrorMessage("Could not save the file, because it missing a name.");
            }
        }

        private void EntityContentList_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (CheckIfItemStartsWith("Name: ", EntityContentList.SelectedIndex))
            {
                OpenRenameEntityWindow();
            }
            if (CheckIfItemEndsWith("Component", EntityContentList.SelectedIndex))
            {
                EditSelectedComponent((string)EntityContentList.Items[EntityContentList.SelectedIndex]);
            }
        }
    }
}
