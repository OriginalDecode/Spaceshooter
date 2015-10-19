﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.IO;
using System.Windows.Forms;

namespace EntityEditor
{
    class EntityReader
    {
        private String myFilePath = "";
        private Entity.EntityData myEntityData = new Entity.EntityData();
        private Entity.EntityListXML myEntityList = new Entity.EntityListXML();

        public Entity.EntityData LoadFile(String aFilePath, Form aParent)
        {
            if (aFilePath == "") return myEntityData;
            myFilePath = aFilePath;
            string entityListPath = "Data/Script/LI_list_entity.xml";

            if (myEntityList.myPaths == null)
            {
                myEntityList.myPaths = new List<string>();
            }

            using (XmlReader reader = XmlReader.Create(myFilePath))
            {
                while (reader.Read())
                {
                    if (reader.NodeType == XmlNodeType.Element)
                    {
                        ReadElement(reader);
                    }
                }
            }
            using (XmlReader reader = XmlReader.Create(entityListPath))
            {
                while (reader.Read())
                {
                    if (reader.NodeType == XmlNodeType.Element)
                    {
                        ReadEntityListFile(reader);
                    }
                }
            }

            EntityEditorForm entityForm = (EntityEditorForm)aParent;
            entityForm.SetEntityList(myEntityList);

            return myEntityData;
        }

        private void ReadEntityListFile(XmlReader aReader)
        {
            if (aReader.Name == "path")
            {
                aReader.MoveToAttribute("src");
                if (myEntityList.myPaths.Contains(aReader.Value) == false)
                {
                    myEntityList.myPaths.Add(aReader.Value);
                }
            }
        }

        private void ReadElement(XmlReader aReader)
        {
            if (aReader.Name == "Entity")
            {
                aReader.MoveToAttribute("name");
                myEntityData.myName = aReader.Value;
            }
            else if (aReader.Name == "GraphicsComponent")
            {
                myEntityData.myGraphicsComponent.myIsActive = true;
            }
            else if (aReader.Name == "Model")
            {
                aReader.MoveToAttribute("modelFile");
                myEntityData.myGraphicsComponent.myModelPath = aReader.Value;

                aReader.MoveToAttribute("effectFile");
                myEntityData.myGraphicsComponent.myEffectPath = aReader.Value;
            }
            else if (aReader.Name == "AIComponent")
            {
                myEntityData.myAIComponent.myIsActive = true;
            }
            else if (aReader.Name == "ShootingComponent")
            {
                myEntityData.myShootingComponent.myIsActive = true;
            }
            else if (aReader.Name == "Weapon")
            {
                aReader.MoveToAttribute("type");
                myEntityData.myShootingComponent.myWeaponType = aReader.Value;
            }
            else if (aReader.Name == "CollisionComponent")
            {
                myEntityData.myCollisionComponent.myIsActive = true;
            }
            else if (aReader.Name == "CollisionSphere")
            {
                aReader.MoveToAttribute("radius");
                string formatedValue = aReader.Value.Replace("f", "0");
                myEntityData.myCollisionComponent.myRadius = float.Parse(formatedValue);
                myEntityData.myCollisionComponent.myHasSphere = true;
            }
        }
    }
}
