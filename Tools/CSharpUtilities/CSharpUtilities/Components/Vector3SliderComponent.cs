using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Windows.Forms;

namespace CSharpUtilities.Components
{
    public class Vector3SliderComponent : BaseComponent
    {
        private Label myLabel = new Label();

        private SliderComponent myXSlider;
        private SliderComponent myYSlider;
        private SliderComponent myZSlider;

        public Vector3SliderComponent(Point aLocation, Size aSize, string aText, 
            float aMinValue, float aMaxValue, float aStartValue, bool aOneToOneScaleFlag = false)
            : base(aLocation, aSize, aText)
        {
            InitializeComponents(aText, aMinValue, aMaxValue, aStartValue, aOneToOneScaleFlag);
        }

        protected override void InitializeComponents(string aText)
        {
            int textSize = aText.Length * 10;
            if (textSize > mySize.Width) textSize = mySize.Width;
            myLabel.Text = aText;
            myLabel.Location = new Point(myLocation.X + ((mySize.Width - (textSize / 2)) / 2), myLocation.Y);
            myLabel.Size = new Size(textSize, 13);
        }

        protected void InitializeComponents(string aText, float aMinValue, float aMaxValue, float aStartValue, bool aOneToOneScaleFlag)
        {
            myXSlider = new SliderComponent(new Point(myLocation.X, myLocation.Y + 15),
                new Size(175, 13), "X: ", aMinValue, aMaxValue, aStartValue, aOneToOneScaleFlag);
            myYSlider = new SliderComponent(new Point(myLocation.X, myLocation.Y + 30),
                new Size(175, 13), "Y: ", aMinValue, aMaxValue, aStartValue, aOneToOneScaleFlag);
            myZSlider = new SliderComponent(new Point(myLocation.X, myLocation.Y + 45),
                new Size(175, 13), "Z: ", aMinValue, aMaxValue, aStartValue, aOneToOneScaleFlag);
        }

        public void AddSelectedValueChangedEvent(EventHandler aEvent)
        {
            myXSlider.AddSelectedValueChangedEvent(aEvent);
            myYSlider.AddSelectedValueChangedEvent(aEvent);
            myZSlider.AddSelectedValueChangedEvent(aEvent);
        }

        public void AddSelectedXValueChangedEvent(EventHandler aEvent)
        {
            myXSlider.AddSelectedValueChangedEvent(aEvent);
        }

        public void AddSelectedYValueChangedEvent(EventHandler aEvent)
        {
            myYSlider.AddSelectedValueChangedEvent(aEvent);
        }

        public void AddSelectedZValueChangedEvent(EventHandler aEvent)
        {
            myZSlider.AddSelectedValueChangedEvent(aEvent);
        }

        public override void BindToPanel(Panel aPanel)
        {
            aPanel.Controls.Add(myLabel);
            myXSlider.BindToPanel(aPanel);
            myYSlider.BindToPanel(aPanel);
            myZSlider.BindToPanel(aPanel);
        }

        public override void Hide()
        {
            myLabel.Hide();
            myXSlider.Hide();
            myYSlider.Hide();
            myZSlider.Hide();
        }

        public override void Show()
        {
            myLabel.Show();
            myXSlider.Show();
            myYSlider.Show();
            myZSlider.Show();
        }

        public float GetXValue()
        {
            return myXSlider.GetValue();
        }

        public void SetXValue(float aValue)
        {
            myXSlider.SetValue(aValue);
        }

        public float GetYValue()
        {
            return myYSlider.GetValue();
        }

        public void SetYValue(float aValue)
        {
            myYSlider.SetValue(aValue);
        }

        public float GetZValue()
        {
            return myZSlider.GetValue();
        }

        public void SetZValue(float aValue)
        {
            myZSlider.SetValue(aValue);
        }
    }
}
