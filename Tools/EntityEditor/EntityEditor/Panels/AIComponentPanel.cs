using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;

namespace EntityEditor.Panels
{
    public class AIComponentPanel : BasePanel
    {
        private Entity.AIComponentData myAIComponent;

        public AIComponentPanel(Point aLocation, Size aSize, Form aParent)
            : base(aLocation, aSize, aParent)
        {
            myPropertyTitle.Text += "AI Component";
        }

        protected override void IntitalizeControls()
        {
            
        }

        public void Load(Entity.AIComponentData aAIComponent)
        {
            myAIComponent = aAIComponent;
        }

        protected override void LoadSettings()
        {
            
        }

        protected override void SaveSettings()
        {

            EntityEditorForm eForm = (EntityEditorForm)myOwnerForm;
            eForm.SetAIComponent(myAIComponent);
        }
    }
}
