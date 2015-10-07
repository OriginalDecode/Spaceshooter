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
        private ComponentEditors.ShootingComponentSubForms.EditWeaponForm myEditWeaponForm = null;
        private Entity.ShootingComponentData myShootingComponent;
        private Entity.WeaponData myCurrentWeaponData;
        private Entity.WeaponReader myWeaponReader = new Entity.WeaponReader();
        private List<Entity.WeaponData> myLoadedWeaponTypes;

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
            myWeaponReader.LoadWeapons("Data/weapons/WeaponList.xml");
            myWeaponReader.LoadBullets("Data/weapons/projectiles/ProjectileList.xml");

            SC_Active.Checked = myShootingComponent.myIsActive;

            myLoadedWeaponTypes = myWeaponReader.GetWeaponData();
            for (int i = 0; i < myLoadedWeaponTypes.Count; ++i)
            {
                SC_WeaponType.Items.Add(myLoadedWeaponTypes[i].myType);
            }

            for(int i = 0; i < SC_WeaponType.Items.Count; ++i)
            {
                if((string)SC_WeaponType.Items[i] == myShootingComponent.myWeaponType)
                {
                    
                }
            }
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

        private void SC_Btn_Edit_WeaponType_Click(object sender, EventArgs e)
        {
            myEditWeaponForm = new ShootingComponentSubForms.EditWeaponForm(this);
            myEditWeaponForm.LoadSettings(myCurrentWeaponData);

            myEditWeaponForm.Visible = true;
            myEditWeaponForm.Activate();
        }

        private void SC_WeaponType_SelectedIndexChanged(object sender, EventArgs e)
        {
            for (int i = 0; i < myLoadedWeaponTypes.Count; ++i)
            {
                if(myLoadedWeaponTypes[i].myType == (string)SC_WeaponType.SelectedItem)
                {
                    myCurrentWeaponData = myLoadedWeaponTypes[i];
                    break;
                }
            }
        }
    }
}