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

namespace EntityEditor
{
    public partial class EntityEditorForm : Form
    {
        private Entity.EntityData myCurrentEntity = new Entity.EntityData();
        private EntityReader myEntityReader = new EntityReader();
        private Entity.EntityWriter myEntityWriter = new Entity.EntityWriter();
        private Entity.EntityListXML myEntityList;

        private RenameEntity myRenameEntityForm = null;

        private string myCurrentEntityFolderPath = Properties.Settings.Default.DefaultEntityFolderPath;
        private string myCurrentEntityFilePath = Properties.Settings.Default.DefaultEntityFileName;

        private AddComponentForm myAddComponentForm = null;

        private ComponentEditors.GraphicsComponent myGraphicsComponentSettingsForm = null;
        private ComponentEditors.AIComponent myAIComponentSettingsForm = null;
        private ComponentEditors.ShootingComponent myShootingComponentSettingsForm = null;
        private ComponentEditors.CollisionComponent myCollisionComponentSettingsForm = null;

        public EntityEditorForm()
        {
            InitializeComponent();

            openEntityFile.InitialDirectory = myCurrentEntityFolderPath;
        }

        public static string Reverse(string s)
        {
            char[] charArray = s.ToCharArray();
            Array.Reverse(charArray);
            return new string(charArray);
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
            myRenameEntityForm = new RenameEntity(this);
            myRenameEntityForm.Activate();
            myRenameEntityForm.Visible = true;
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

        private void EditSelectedComponent(string aComponentName)
        {
            if (aComponentName.StartsWith("Graphics") == true)
            {
                myGraphicsComponentSettingsForm = new ComponentEditors.GraphicsComponent(this, myCurrentEntity.myGraphicsComponent);
                myGraphicsComponentSettingsForm.Visible = true;
                myGraphicsComponentSettingsForm.Activate();
                return;
            }
            if (aComponentName.StartsWith("AI") == true)
            {
                myAIComponentSettingsForm = new ComponentEditors.AIComponent(this, myCurrentEntity.myAIComponent);
                myAIComponentSettingsForm.Visible = true;
                myAIComponentSettingsForm.Activate();
                return;
            }
            if (aComponentName.StartsWith("Shooting") == true)
            {
                myShootingComponentSettingsForm = new ComponentEditors.ShootingComponent(this, myCurrentEntity.myShootingComponent);
                myShootingComponentSettingsForm.Visible = true;
                myShootingComponentSettingsForm.Activate();
                return;
            }
            if (aComponentName.StartsWith("Collision") == true)
            {
                myCollisionComponentSettingsForm = new ComponentEditors.CollisionComponent(this, myCurrentEntity.myCollisionComponent);
                myCollisionComponentSettingsForm.Visible = true;
                myCollisionComponentSettingsForm.Activate();
                return;
            }
        }
        //----- Edit Entity Buttons Section Start -----
        private void EntityContentList_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            int index = this.EntityContentList.IndexFromPoint(e.Location);
            if (CheckIfItemStartsWith("Name: ", index))
            {
                OpenRenameEntityWindow();
            }
            if (CheckIfItemEndsWith("Component", index))
            {
                EditSelectedComponent((string)EntityContentList.Items[index]);
            }
        }
        private void renameEntityToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenRenameEntityWindow();
        }

        private void editComponentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string selectedName = (string)EntityContentList.SelectedItem;
            if (selectedName != null && selectedName.EndsWith("Component"))
            {
                EditSelectedComponent(selectedName);
            }
        }

        private void Btn_Edit_Click(object sender, EventArgs e)
        {
            string selectedName = (string)EntityContentList.SelectedItem;
            if (selectedName != null && selectedName.StartsWith("Name: "))
            {
                OpenRenameEntityWindow();
            }
            if (selectedName != null && selectedName.EndsWith("Component"))
            {
                EditSelectedComponent(selectedName);
            }
        }
        //----- Edit Entity Buttons Section Ends -----

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
            MessageBox.Show("Save entity " + myCurrentEntity.myName + " at\n" + myCurrentEntityFilePath,
                    "Save Successfull!", MessageBoxButtons.OK);
        }

        //----- Add Component Button Start -----
        private void addComponentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            myAddComponentForm = new AddComponentForm(this, myCurrentEntity);
        }

        private void Btn_Add_Click(object sender, EventArgs e)
        {
            myAddComponentForm = new AddComponentForm(this, myCurrentEntity);
        }

        //----- Add Component Button Ends -----

        //----- Remove Component Button Start -----
        private void Btn_Remove_Click(object sender, EventArgs e)
        {
            string selectedName = (string)EntityContentList.SelectedItem;
            if (selectedName != null && selectedName.EndsWith("Component"))
            {
                RemoveSelectedComponent(selectedName);
                DisplayEntityData();
            }
        }

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
            if (myCurrentEntityFolderPath != "")
            {
                saveEntityFile.InitialDirectory = myCurrentEntityFolderPath;
            }
            saveEntityFile.ShowDialog();

            myCurrentEntityFilePath = saveEntityFile.FileName;
            Properties.Settings.Default.DefaultEntityFileName = myCurrentEntityFilePath;
            Properties.Settings.Default.Save();

            OpenRenameEntityWindow();

            myEntityReader.LoadEntityList(myCurrentEntityFilePath, this);
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (myCurrentEntityFolderPath != "")
            {
                saveEntityFile.InitialDirectory = myCurrentEntityFolderPath;
            }
            saveEntityFile.ShowDialog();

            myCurrentEntityFilePath = saveEntityFile.FileName;
            Properties.Settings.Default.DefaultEntityFileName = myCurrentEntityFilePath;
            Properties.Settings.Default.Save();

            myEntityWriter.SaveFile(myCurrentEntityFilePath, myCurrentEntity, myEntityList);
            MessageBox.Show("Save entity " + myCurrentEntity.myName + " at\n" + myCurrentEntityFilePath,
                    "Save Successfull!", MessageBoxButtons.OK);
        }
    }
}
