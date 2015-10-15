using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Windows.Forms;
using CSharpUtilities.Components;

namespace EntityEditor.ComponentEditors
{
    public class ShootingComponentPanel : BasePanel
    {
        private Entity.ShootingComponentData myShootingComponent;

        private DropDownEditComponent myWeaponTypes;

        public ShootingComponentPanel(Point aLocation, Size aSize, Form aParent) : base(aLocation, aSize, aParent)
        {

        }

        public void Load(Entity.ShootingComponentData aShootingComponentData)
        {

        }

        protected override void IntitalizeControls()
        {
            myWeaponTypes = new DropDownEditComponent(new Point(Location.X, Location.Y + 30), new Size(Size.Width, 10), "Weapon Type");
            myWeaponTypes.BindToPanel(this);
            myWeaponTypes.Show();
        }

        protected override void LoadSettings()
        {

        }

        protected override void SaveSettings()
        {

        }
    }
}
