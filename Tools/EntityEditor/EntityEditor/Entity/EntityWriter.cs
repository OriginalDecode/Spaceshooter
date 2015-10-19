using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.IO;

namespace EntityEditor.Entity
{
    class EntityWriter
    {
        private String myFilePath = "";
        private Entity.EntityData myEntityData;
        private Entity.EntityListXML myEntityList;

        public void SaveFile(String aFilePath, Entity.EntityData aEntityData, Entity.EntityListXML aEntityList)
        {
            myFilePath = aFilePath;
            myEntityData = aEntityData;
            myEntityList = aEntityList;

            string entityListPath = "Data/Script/LI_list_entity.xml";

            XmlWriterSettings settings = new XmlWriterSettings();
            settings.OmitXmlDeclaration = true;
            settings.Indent = true;

            using (XmlWriter writer = XmlWriter.Create(myFilePath, settings))
            {
                WriteFile(writer);
            }

            using (XmlWriter writer = XmlWriter.Create(entityListPath, settings))
            {
                WriteEntityListFile(writer, myFilePath);
            }

        }

        private string ConvertPathToDataFolderPath(string aPath)
        {
            int dataTagIndex = aPath.IndexOf("Data");
            string convertedPath = aPath;
            convertedPath = aPath.Substring(dataTagIndex);
            convertedPath = convertedPath.Replace("\\", "/");

            return convertedPath;
        }

        private void WriteEntityListFile(XmlWriter aWriter, string aNewEntityPath)
        {
            aWriter.WriteStartElement("root");

            bool pathAlreadyExist = false;

            if (myEntityList.myPaths != null)
            {
                for (int i = 0; i < myEntityList.myPaths.Count; ++i)
                {
                    aWriter.WriteStartElement("path");
                    aWriter.WriteAttributeString("src", myEntityList.myPaths[i]);
                    aWriter.WriteEndElement();
                    if (myEntityList.myPaths[i] == ConvertPathToDataFolderPath(aNewEntityPath))
                    {
                        pathAlreadyExist = true;
                    }
                }
            }
            if(pathAlreadyExist == false)
            {
                aWriter.WriteStartElement("path");
                aWriter.WriteAttributeString("src", ConvertPathToDataFolderPath(aNewEntityPath));
                aWriter.WriteEndElement();
            }

            aWriter.WriteEndElement();
        }

        private void WriteFile(XmlWriter aWriter)
        {
            aWriter.WriteStartElement("Entity");
            aWriter.WriteAttributeString("name", myEntityData.myName);

            if (myEntityData.myGraphicsComponent.myIsActive == true)
            {
                WriteGraphicsComponent(aWriter);
            }
            if (myEntityData.myAIComponent.myIsActive == true)
            {
                WriteAIComponent(aWriter);
            }
            if (myEntityData.myShootingComponent.myIsActive == true)
            {
                WriteShootingComponent(aWriter);
            }
            if (myEntityData.myCollisionComponent.myIsActive == true)
            {
                WriteCollisionComponent(aWriter);
            }

            aWriter.WriteEndElement();
        }

        private void WriteGraphicsComponent(XmlWriter aWriter)
        {
            aWriter.WriteStartElement("GraphicsComponent");
            aWriter.WriteStartElement("Model");

            aWriter.WriteAttributeString("modelFile", myEntityData.myGraphicsComponent.myModelPath);
            aWriter.WriteAttributeString("effectFile", myEntityData.myGraphicsComponent.myEffectPath);

            aWriter.WriteEndElement();
            aWriter.WriteEndElement();
        }

        private void WriteAIComponent(XmlWriter aWriter)
        {
            aWriter.WriteStartElement("AIComponent");

            aWriter.WriteEndElement();
        }

        private void WriteShootingComponent(XmlWriter aWriter)
        {
            aWriter.WriteStartElement("ShootingComponent");
            aWriter.WriteStartElement("Weapon");

            aWriter.WriteAttributeString("type", myEntityData.myShootingComponent.myWeaponType);

            aWriter.WriteEndElement();
            aWriter.WriteEndElement();
        }

        private void WriteCollisionComponent(XmlWriter aWriter)
        {
            aWriter.WriteStartElement("CollisionComponent");

            if (myEntityData.myCollisionComponent.myHasSphere == true)
            {
                aWriter.WriteStartElement("CollisionSphere");
                aWriter.WriteAttributeString("radius", myEntityData.myCollisionComponent.myRadius.ToString());
                aWriter.WriteEndElement();
            }

            aWriter.WriteEndElement();
        }
    }
}
