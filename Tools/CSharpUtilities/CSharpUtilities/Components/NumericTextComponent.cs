using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;

namespace CSharpUtilities.Components
{
    public class NumericTextComponent : BaseComponent
    {
        private Label myLabel = new Label();
        private TextBox myTextBox = new TextBox();

        private bool myDigitOnlyFlag;

        public NumericTextComponent(Point aLocation, Size aSize, string aText, bool aDigitOnlyFlag = true)
            : base(aLocation, aSize, aText)
        {
            myDigitOnlyFlag = aDigitOnlyFlag;
        }

        protected override void InitializeComponents(string aText)
        {
            int textLength = aText.Length;
            if (textLength > 6) textLength = 6;

            myLabel.Text = aText;
            myLabel.Location = new Point(myLocation.X, myLocation.Y);
            myLabel.Size = new Size(textLength * 10, mySize.Height);


            myTextBox.Location = new Point(myLocation.X + textLength * 10 + 10, myLocation.Y - 3);
            myTextBox.Size = new Size(mySize.Width - 50, mySize.Height);
            myTextBox.KeyPress += new KeyPressEventHandler(TextBox_KeyPress);
        }

        public override void BindToPanel(Panel aPanel)
        {
            aPanel.Controls.Add(myLabel);
            aPanel.Controls.Add(myTextBox);
        }

        public override void Hide()
        {
            myLabel.Hide();
            myTextBox.Hide();
        }

        public override void Show()
        {
            myLabel.Show();
            myTextBox.Show();
        }

        public TextBox GetTextBox()
        {
            return myTextBox;
        }

        private void TextBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (myDigitOnlyFlag == true)
            {
                if (char.IsControl(e.KeyChar) == false && char.IsDigit(e.KeyChar) == false 
                    && e.KeyChar != '.' && e.KeyChar != '-')
                {
                    e.Handled = true;
                }
                if (e.KeyChar == '.' && (sender as TextBox).Text.IndexOf('.') > -1)
                {
                    e.Handled = true;
                }
                if (e.KeyChar == '-' && (sender as TextBox).Text.IndexOf('-') > -1)
                {
                    e.Handled = true;
                }
            }
        }
    }
}
