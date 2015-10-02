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

namespace EntityEditor
{
    public enum eComponentType
    {
        AIComponent,
        CollisionComponent,
        GraphicsComponent,
        ShootingComponent
    };

    public partial class AddComponentForm : Form
    {

        private ComponentEditors.GraphicsComponent myGraphicsComponentSettingsForm = null;
        private ComponentEditors.AIComponent myAIComponentSettingsForm = null;
        private ComponentEditors.ShootingComponent myShootingComponentSettingsForm = null;
        private ComponentEditors.CollisionComponent myCollisionComponentSettingsForm = null;

        private Entity.EntityData myCurrentEntity;

        public AddComponentForm()
        {
            InitializeComponent();

            this.Visible = true;
            this.Activate();

            LoadSettings();
        }

        public AddComponentForm(Form aParent, Entity.EntityData aEntityData)
        {
            InitializeComponent();

            this.Owner = aParent;
            myCurrentEntity = aEntityData;

            this.Visible = true;
            this.Activate();

            LoadSettings();
        }

        private void DestroyWindowsForm()
        {
            this.Visible = false;
            this.Dispose();
        }

        private void LoadSettings()
        {
            if (myCurrentEntity.myAIComponent.myIsActive == false)
            {
                ACF_CB_ComponentType.Items.Add(eComponentType.AIComponent);
            }
            if (myCurrentEntity.myCollisionComponent.myIsActive == false)
            {
                ACF_CB_ComponentType.Items.Add(eComponentType.CollisionComponent);
            }
            if (myCurrentEntity.myGraphicsComponent.myIsActive == false)
            {
                ACF_CB_ComponentType.Items.Add(eComponentType.GraphicsComponent);
            }
            if (myCurrentEntity.myShootingComponent.myIsActive == false)
            {
                ACF_CB_ComponentType.Items.Add(eComponentType.ShootingComponent);
            }

            if (ACF_CB_ComponentType.Items.Count == 0)
            {
                DL_Debug.GetInstance.DL_MessageBox("You already have one of each component in the entity.", 
                    "Error: Could not Add Component", MessageBoxButtons.OK);
                DestroyWindowsForm();
            }
        }

        private void ACF_Btn_Add_Click(object sender, EventArgs e)
        {
            if ((eComponentType)ACF_CB_ComponentType.SelectedItem == eComponentType.AIComponent)
            {
                myAIComponentSettingsForm = new ComponentEditors.AIComponent(this.Owner);
                myAIComponentSettingsForm.Visible = true;
                myAIComponentSettingsForm.Activate();
            }
            else if ((eComponentType)ACF_CB_ComponentType.SelectedItem == eComponentType.CollisionComponent)
            {
                myCollisionComponentSettingsForm = new ComponentEditors.CollisionComponent(this.Owner);
                myCollisionComponentSettingsForm.Visible = true;
                myCollisionComponentSettingsForm.Activate();
            }
            else if ((eComponentType)ACF_CB_ComponentType.SelectedItem == eComponentType.GraphicsComponent)
            {
                myGraphicsComponentSettingsForm = new ComponentEditors.GraphicsComponent(this.Owner);
                myGraphicsComponentSettingsForm.Visible = true;
                myGraphicsComponentSettingsForm.Activate();
            }
            else if ((eComponentType)ACF_CB_ComponentType.SelectedItem == eComponentType.ShootingComponent)
            {
                myShootingComponentSettingsForm = new ComponentEditors.ShootingComponent(this.Owner);
                myShootingComponentSettingsForm.Visible = true;
                myShootingComponentSettingsForm.Activate();
            }
            DestroyWindowsForm();
        }

        private void ACF_Btn_Cancel_Click(object sender, EventArgs e)
        {
            DestroyWindowsForm();
        }
    }
}
