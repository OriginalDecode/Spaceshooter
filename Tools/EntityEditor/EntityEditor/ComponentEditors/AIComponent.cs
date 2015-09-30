using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EntityEditor.ComponentEditors
{
    public partial class AIComponent : Form
    {
        private Entity.AIComponentData myAIComponent;
        public AIComponent()
        {
            InitializeComponent();

            myAIComponent = new Entity.AIComponentData();
            myAIComponent.myIsActive = true;

            LoadSetting();
        }

        public AIComponent(Form aParent)
        {
            InitializeComponent();

            this.Owner = aParent;
            myAIComponent = new Entity.AIComponentData();
            myAIComponent.myIsActive = true;

            LoadSetting();
        }

        public AIComponent(Form aParent, Entity.AIComponentData aAIComponent)
        {
            InitializeComponent();

            this.Owner = aParent;
            myAIComponent = aAIComponent;

            LoadSetting();
        }

        private void DestroyWindowForm(bool aSaveEntity)
        {
            EntityEditorForm parentForm = (EntityEditorForm)this.Owner;

            if (aSaveEntity == true)
            {
                SaveSetting();
                parentForm.SetAIComponent(myAIComponent);
                parentForm.DisplayEntityData();
            }

            this.Visible = false;
            this.Dispose();

            parentForm.Visible = true;
            parentForm.Activate();
        }
        //Save Load Block Start
        private void LoadSetting()
        {
            AC_Active.Checked = myAIComponent.myIsActive;
        }

        private void SaveSetting()
        {
            myAIComponent.myIsActive = AC_Active.Checked;
        }
        //Save Load Block End
        private void AC_Btn_Save_Click(object sender, EventArgs e)
        {
            DestroyWindowForm(true);
        }

        private void AC_Btn_Cancel_Click(object sender, EventArgs e)
        {
            DestroyWindowForm(false);
        }
    }
}
