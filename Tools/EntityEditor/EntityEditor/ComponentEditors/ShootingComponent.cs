using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EntityEditor.ComponentEditors
{
    public partial class ShootingComponent : Form
    {
        private Entity.ShootingComponentData myShootingComponent;

        public ShootingComponent()
        {
            InitializeComponent();

            myShootingComponent = new Entity.ShootingComponentData();
            myShootingComponent.myIsActive = true;

            LoadSetting();
        }

        public ShootingComponent(Form aParent)
        {
            InitializeComponent();

            this.Owner = aParent;
            myShootingComponent = new Entity.ShootingComponentData();
            myShootingComponent.myIsActive = true;

            LoadSetting();
        }

        public ShootingComponent(Form aParent, Entity.ShootingComponentData aShootingComponent)
        {
            InitializeComponent();

            this.Owner = aParent;
            myShootingComponent = aShootingComponent;

            LoadSetting();
        }

         private void DestroyWindowForm(bool aSaveEntity)
        {
            EntityEditorForm parentForm = (EntityEditorForm)this.Owner;

            if (aSaveEntity == true)
            {
                SaveSetting();
                parentForm.SetShootingComponent(myShootingComponent);
                parentForm.DisplayEntityData();
            }

            this.Visible = false;
            this.Dispose();

            parentForm.Visible = true;
            parentForm.Activate();
        }
        //Save Load Block Start
        private void LoadSetting()
        {
            SC_Active.Checked = myShootingComponent.myIsActive;
        }

        private void SaveSetting()
        {
            myShootingComponent.myIsActive = SC_Active.Checked;
        }

        private void SC_Btn_Save_Click(object sender, EventArgs e)
        {
            DestroyWindowForm(true);
        }

        private void SC_Btn_Cancel_Click(object sender, EventArgs e)
        {
            DestroyWindowForm(false);
        }
    }
}