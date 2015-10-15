using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;

namespace CSharpUtilities.Components
{
    public class DropDownEditComponent : BaseComponent
    {
        private Label myLabel = new Label();
        private ComboBox myDropDown = new ComboBox();
        private Button myButton = new Button();

        public DropDownEditComponent(Point aLocation, Size aSize, string aText) : base(aLocation,aSize,aText)
        {

        }

        protected override void InitializeComponents(string aText)
        {
            myLabel.Text = aText;
            myLabel.Location = new Point(myLocation.X, myLocation.Y + 3);
            myLabel.Size = new Size(50, mySize.Height + 2);

            myDropDown.Location = new Point(myLocation.X + 50, myLocation.Y);
            myDropDown.Size = new Size(100, mySize.Height);

            myButton.Text = "Edit";
            myButton.Location = new Point(myLocation.X + 200, myLocation.Y);
            myButton.Size = new Size(50, mySize.Height + 10);
        }

        public override void Show()
        {
            myLabel.Show();
            myDropDown.Show();
            myButton.Show();
        }

        public override void Hide()
        {
            myLabel.Hide();
            myDropDown.Hide();
            myButton.Hide();
        }

        public override void BindToPanel(Panel aPanel)
        {
            aPanel.Controls.Add(myLabel);
            aPanel.Controls.Add(myDropDown);
            aPanel.Controls.Add(myButton);
        }
    }
}
