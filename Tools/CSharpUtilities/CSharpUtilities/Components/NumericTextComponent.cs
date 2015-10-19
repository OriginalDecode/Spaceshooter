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

        public NumericTextComponent(Point aLocation, Size aSize, string aText, bool aDigitOnlyFlag = true)
            : base(aLocation, aSize, aText)
        {

        }

        protected override void InitializeComponents(string aText)
        {
            throw new NotImplementedException();
        }

        public override void BindToPanel(Panel aPanel)
        {
            throw new NotImplementedException();
        }

        public override void Hide()
        {
            throw new NotImplementedException();
        }

        public override void Show()
        {
            throw new NotImplementedException();
        }

        public TextBox GetTextBox()
        {
            throw new NotImplementedException();
            return new TextBox();
        }
    }
}
