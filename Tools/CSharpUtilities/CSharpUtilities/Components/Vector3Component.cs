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

        public Vector3Component(Point aLocation, Size aSize, string aText)
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

        public override void Show()
        {
            throw new NotImplementedException();
        }

        public override void Hide()
        {
            throw new NotImplementedException();
        }

        public float GetX()
        {
            throw new NotImplementedException();
            return 0;
        }

        public TextBox GetXTextBox()
        {
            throw new NotImplementedException();
            return new TextBox();
        }

        public float GetY()
        {
            throw new NotImplementedException();
            return 0;
        }

        public TextBox GetYTextBox()
        {
            throw new NotImplementedException();
            return new TextBox();
        }

        public float GetZ()
        {
            throw new NotImplementedException();
            return 0;
        }

        public TextBox GetZTextBox()
        {
            throw new NotImplementedException();
            return new TextBox();
        }

        public void SetPosition(string aX, string aY, string aZ)
        {
            throw new NotImplementedException();
        }
    }
}
