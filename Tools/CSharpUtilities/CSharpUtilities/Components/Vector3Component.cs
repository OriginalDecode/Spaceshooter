using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;

namespace CSharpUtilities.Components
{
    public class Vector3Component : BaseComponent
    {
        private Label myLabel = new Label();

        private NumericTextComponent myXText;
        private NumericTextComponent myYText;
        private NumericTextComponent myZText;

        public Vector3Component(Point aLocation, Size aSize, string aText)
            : base(aLocation, aSize, aText)
        {

        }

        protected override void InitializeComponents(string aText)
        {
            myLabel.Text = aText;
            myLabel.Location = new Point(myLocation.X, myLocation.Y);
            myLabel.Size = new Size(50, mySize.Height);

            myXText = new NumericTextComponent(new Point(myLocation.X + 50, myLocation.Y), new Size(90, mySize.Height), "X");
            myYText = new NumericTextComponent(new Point(myLocation.X + 110, myLocation.Y), new Size(90, mySize.Height), "Y");
            myZText = new NumericTextComponent(new Point(myLocation.X + 170, myLocation.Y), new Size(90, mySize.Height), "Z");

        }

        public override void BindToPanel(Panel aPanel)
        {
            aPanel.Controls.Add(myLabel);

            myXText.BindToPanel(aPanel);
            myYText.BindToPanel(aPanel);
            myZText.BindToPanel(aPanel);

        }

        public override void Show()
        {
            myLabel.Show();
            myXText.Show();
            myYText.Show();
            myZText.Show();
        }

        public override void Hide()
        {
            myLabel.Hide();
            myXText.Hide();
            myYText.Hide();
            myZText.Hide();
        }

        public float GetX()
        {
            return float.Parse(myXText.GetTextBox().Text);
        }

        public TextBox GetXTextBox()
        {
            return myXText.GetTextBox();
        }

        public float GetY()
        {
            return float.Parse(myYText.GetTextBox().Text);
        }

        public TextBox GetYTextBox()
        {
            return myYText.GetTextBox();
        }

        public float GetZ()
        {
            return float.Parse(myZText.GetTextBox().Text);
        }

        public TextBox GetZTextBox()
        {
            return myZText.GetTextBox();
        }

        public void SetPosition(string aX, string aY, string aZ)
        {
            myXText.GetTextBox().Text = aX;
            myYText.GetTextBox().Text = aY;
            myZText.GetTextBox().Text = aZ;
        }
    }
}
