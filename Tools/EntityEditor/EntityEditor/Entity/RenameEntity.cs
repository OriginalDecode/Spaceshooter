using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EntityEditor
{
    public partial class RenameEntity : Form
    {
        public RenameEntity()
        {
            InitializeComponent();
        }

        public RenameEntity(Form aParent)
        {
            InitializeComponent();
            this.Owner = aParent;

            EntityEditorForm parentForm = (EntityEditorForm)this.Owner;

            Text_NewEntityName.Text = parentForm.GetEntityName();
        }

        private void Btn_RenameEntity_Ok_Click(object sender, EventArgs e)
        {
            EntityEditorForm parentForm = (EntityEditorForm)this.Owner;

            parentForm.SetEntityName(Text_NewEntityName.Text);

            this.Visible = false;
            this.Dispose();
        }

        private void Btn_RenameEntity_Cancel_Click(object sender, EventArgs e)
        {
            this.Visible = false;
            this.Dispose();
        }
    }
}
