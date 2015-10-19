using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;
using CSharpUtilities.Components;
using CSharpUtilities;

namespace EntityEditor.Panels
{
    public enum eComponentType
    {
        AIComponent,
        CollisionComponent,
        GraphicsComponent,
        ShootingComponent
    };

    public class AddComponentPanel : BasePanel
    {
        private DropDownComponent myComponents;
        private Button myAddButton = new Button();
        private Entity.EntityData myEntity;

        public AddComponentPanel(Point aLocation, Size aSize, Form aParent)
            : base(aLocation, aSize, aParent)
        {
            myPropertyTitle.Text += "Add Components";
        }

        protected override void IntitalizeControls()
        {
            myComponents = new DropDownComponent(new Point(Location.X, Location.Y + 10), new Size(245, 13), "Component");
            myComponents.BindToPanel(this);
            myComponents.Show();

            myAddButton.Text = "Add";
            myAddButton.Location = new Point(Location.X + 150, Location.Y + 10);
            myAddButton.Size = new Size(40, 20);
            myAddButton.Click += new EventHandler(AddButton_Click);
            myAddButton.Show();

            this.Controls.Add(myAddButton);
        }

        public void Load(Entity.EntityData aEntity)
        {
            myEntity = aEntity;
            LoadSettings();
        }

        protected override void LoadSettings()
        {
            myComponents.GetDropDown().Items.Clear();
            if (myEntity.myAIComponent.myIsActive == false)
            {
                myComponents.AddItem(eComponentType.AIComponent);
            }
            if (myEntity.myCollisionComponent.myIsActive == false)
            {
                myComponents.AddItem(eComponentType.CollisionComponent);
            }
            if (myEntity.myGraphicsComponent.myIsActive == false)
            {
                myComponents.AddItem(eComponentType.GraphicsComponent);
            }
            if (myEntity.myShootingComponent.myIsActive == false)
            {
                myComponents.AddItem(eComponentType.ShootingComponent);
            }
        }

        protected override void SaveSettings()
        {
            EntityEditorForm eForm = (EntityEditorForm)myOwnerForm;
            if(myEntity.myAIComponent.myIsActive == true)
            {
                eForm.SetAIComponent(myEntity.myAIComponent);
            }
            if (myEntity.myCollisionComponent.myIsActive == true)
            {
                eForm.SetCollisionComponent(myEntity.myCollisionComponent);
            }
            if (myEntity.myGraphicsComponent.myIsActive == true)
            {
                eForm.SetGraphicsComponent(myEntity.myGraphicsComponent);
            }
            if (myEntity.myShootingComponent.myIsActive == true)
            {
                eForm.SetShootingComponent(myEntity.myShootingComponent);
            }
            eForm.DisplayEntityData();
        }

        private void AddButton_Click(object sender, EventArgs e)
        {
            if (myComponents.GetDropDown().SelectedItem == null) return;

            switch((eComponentType)myComponents.GetDropDown().SelectedItem)
            {
                case eComponentType.AIComponent:
                    myEntity.myAIComponent.myIsActive = true;
                    break;
                case eComponentType.CollisionComponent:
                    myEntity.myCollisionComponent.myIsActive = true;
                    break;
                case eComponentType.GraphicsComponent:
                    myEntity.myGraphicsComponent.myIsActive = true;
                    break;
                case eComponentType.ShootingComponent:
                    myEntity.myShootingComponent.myIsActive = true;
                    break;
                default:
                    DL_Debug.GetInstance.DL_ErrorMessage("The component " 
                        + (string)myComponents.GetDropDown().SelectedItem + " is not currently supported.");
                    break;
            }

            SaveSettings();
            LoadSettings();

            if (myComponents.GetDropDown().Items.Count > 0)
            {
                myComponents.GetDropDown().SelectedIndex = 0;
            }
        }
    }
}
