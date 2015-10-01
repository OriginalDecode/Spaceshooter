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
    public partial class CollisionComponent : Form
    {
        private Entity.CollisionComponentData myCollisionComponent;

        public CollisionComponent()
        {
            InitializeComponent();

            myCollisionComponent = new Entity.CollisionComponentData();
            myCollisionComponent.myIsActive = true;

            LoadSetting();
        }

        public CollisionComponent(Form aParent)
        {
            InitializeComponent();

            this.Owner = aParent;
            myCollisionComponent = new Entity.CollisionComponentData();
            myCollisionComponent.myIsActive = true;

            LoadSetting();
        }

        public CollisionComponent(Form aParent, Entity.CollisionComponentData aCollisionComponent)
        {
            InitializeComponent();

            this.Owner = aParent;
            myCollisionComponent = aCollisionComponent;

            LoadSetting();
        }

         private void DestroyWindowForm(bool aSaveEntity)
        {
            EntityEditorForm parentForm = (EntityEditorForm)this.Owner;

            if (aSaveEntity == true)
            {
                SaveSetting();
                parentForm.SetCollisionComponent(myCollisionComponent);
                parentForm.DisplayEntityData();
            }

            this.Visible = false;
            this.Dispose();

            parentForm.Visible = true;
            parentForm.Activate();
        }

        private void LoadSetting()
        {
            CC_Active.Checked = myCollisionComponent.myIsActive;

            CC_Sphere_Active.Checked = myCollisionComponent.myHasSphere;
            CC_Sphere_Radius.ReadOnly = !CC_Sphere_Active.Checked;
            if (myCollisionComponent.myHasSphere == true)
            {
                CC_Sphere_Radius.Value = (decimal)myCollisionComponent.myRadius;
            }
        }

        private void SaveSetting()
        {
            myCollisionComponent.myIsActive = CC_Active.Checked;

            myCollisionComponent.myHasSphere = CC_Sphere_Active.Checked;
            myCollisionComponent.myRadius = (float)CC_Sphere_Radius.Value;
        }
        
        private void CC_Btn_Save_Click(object sender, EventArgs e)
        {
            DestroyWindowForm(true);
        }

        private void CC_Btn_Cancel_Click(object sender, EventArgs e)
        {
            DestroyWindowForm(false);
        }

        private void CC_Sphere_Active_CheckedChanged(object sender, EventArgs e)
        {
            CC_Sphere_Radius.ReadOnly = !CC_Sphere_Active.Checked;
        }
    }
}
