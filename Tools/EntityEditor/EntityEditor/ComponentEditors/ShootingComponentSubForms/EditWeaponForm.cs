using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EntityEditor.ComponentEditors.ShootingComponentSubForms
{
    public partial class EditWeaponForm : Form
    {
        private Entity.WeaponData myWeaponData;
        public EditWeaponForm()
        {
            InitializeComponent();
        }

        public EditWeaponForm(Form aParent)
        {
            InitializeComponent();

            this.Owner = aParent;
        }

        public void LoadSettings(Entity.WeaponData aWeaponData)
        {
            myWeaponData = aWeaponData;

            SC_EW_Text_Cooldown.Text = myWeaponData.myCooldown.ToString();
            SC_EW_Text_Spread.Text = myWeaponData.mySpread.ToString();
            SC_EW_Text_X.Text = myWeaponData.myPosition.myX.ToString();
            SC_EW_Text_Y.Text = myWeaponData.myPosition.myY.ToString();
            SC_EW_Text_Z.Text = myWeaponData.myPosition.myZ.ToString();

            SC_EW_BulletType.Items.Add(myWeaponData.myBulletType);
        }

        private void SC_EW_Btn_EditBulletType_Click(object sender, EventArgs e)
        {

        }

        private void SC_EW_Btn_Save_Click(object sender, EventArgs e)
        {

        }

        private void SC_EW_Btn_Cancel_Click(object sender, EventArgs e)
        {

        }
    }
}
