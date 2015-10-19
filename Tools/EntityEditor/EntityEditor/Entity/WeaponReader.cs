using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.IO;
using System.Windows.Forms;

namespace EntityEditor.Entity
{
    public class WeaponReader
    {
        private List<WeaponData> myWeaponData = new List<WeaponData>();
        private List<BulletData> myBulletData = new List<BulletData>();
        private WeaponListXml myWeaponPaths;
        private BulletListXml myBulletPaths;

        private WeaponData myNewWeaponData;
        private BulletData myNewBulletData;

        public void LoadWeapons(string aWeaponListPath)
        {
            if (aWeaponListPath == "") return;

            if (File.Exists(aWeaponListPath) == false) MessageBox.Show("Error file not found.");

            if (myWeaponPaths.myPaths == null)
            {
                myWeaponPaths.myPaths = new List<string>();
            }

            myWeaponData.Clear();

            using (XmlReader reader = XmlReader.Create(aWeaponListPath))
            {
                while(reader.Read())
                {
                    if(reader.NodeType == XmlNodeType.Element)
                    {
                        ReadWeaponList(reader);
                    }
                }
            }

            for(int i = 0; i < myWeaponPaths.myPaths.Count; ++i)
            {
                using(XmlReader reader = XmlReader.Create(myWeaponPaths.myPaths[i]))
                {
                    while(reader.Read())
                    {
                        if(reader.NodeType == XmlNodeType.Element)
                        {
                            ReadWeaponData(reader);
                        }
                    }
                    if(myNewWeaponData.myNumberOfBulletsPerShot == 0)
                    {
                        myNewWeaponData.myNumberOfBulletsPerShot = 1;
                    }
                    myNewWeaponData.myFilePath = myWeaponPaths.myPaths[i];
                    myWeaponData.Add(myNewWeaponData);
                }
            }
        }

        private void ReadWeaponList(XmlReader aReader)
        {
            if(aReader.Name == "path")
            {
                aReader.MoveToAttribute("src");
                if(myWeaponPaths.myPaths.Contains(aReader.Value) == false)
                {
                    myWeaponPaths.myPaths.Add(aReader.Value);
                }
            }
        }

        private void ReadWeaponData(XmlReader aReader)
        {
            if(aReader.Name == "Weapon")
            {
                aReader.MoveToAttribute("name");
                myNewWeaponData.myType = aReader.Value;
            }
            else if(aReader.Name == "cooldown")
            {
                aReader.MoveToAttribute("value");
                myNewWeaponData.myCooldown = float.Parse(aReader.Value);
            }
            else if (aReader.Name == "spread")
            {
                aReader.MoveToAttribute("value");
                myNewWeaponData.mySpread = int.Parse(aReader.Value);
            }
            else if (aReader.Name == "bulletsPerShot")
            {
                aReader.MoveToAttribute("value");
                myNewWeaponData.myNumberOfBulletsPerShot = int.Parse(aReader.Value);
            }
            else if (aReader.Name == "position")
            {
                aReader.MoveToAttribute("x");
                myNewWeaponData.myPosition.myX = float.Parse(aReader.Value);

                aReader.MoveToAttribute("y");
                myNewWeaponData.myPosition.myY = float.Parse(aReader.Value);

                aReader.MoveToAttribute("z");
                myNewWeaponData.myPosition.myZ = float.Parse(aReader.Value);
            }
            else if (aReader.Name == "bullet")
            {
                aReader.MoveToAttribute("type");
                myNewWeaponData.myBulletType = aReader.Value;
            }
        }

        public List<WeaponData> GetWeaponData()
        {
            return myWeaponData;
        }

        public WeaponListXml GetWeaponListXml()
        {
            return myWeaponPaths;
        }

        public void LoadBullets(string aBulletListPath)
        {
            if (aBulletListPath == "") return;

            if (myBulletPaths.myPaths == null)
            {
                myBulletPaths.myPaths = new List<string>();
            }

            myBulletData.Clear();

            using (XmlReader reader = XmlReader.Create(aBulletListPath))
            {
                while (reader.Read())
                {
                    if (reader.NodeType == XmlNodeType.Element)
                    {
                        ReadBulletList(reader);
                    }
                }
            }

            for (int i = 0; i < myBulletPaths.myPaths.Count; ++i)
            {
                using (XmlReader reader = XmlReader.Create(myBulletPaths.myPaths[i]))
                {
                    while (reader.Read())
                    {
                        if (reader.NodeType == XmlNodeType.Element)
                        {
                            ReadBulletData(reader);
                        }
                    }
                    myNewBulletData.myFilePath = myBulletPaths.myPaths[i];
                    myBulletData.Add(myNewBulletData);
                }
            }
        }

        private void ReadBulletList(XmlReader aReader)
        {
            if (aReader.Name == "path")
            {
                aReader.MoveToAttribute("src");
                if (myBulletPaths.myPaths.Contains(aReader.Value) == false)
                {
                    myBulletPaths.myPaths.Add(aReader.Value);
                }
            }
        }

        private void ReadBulletData(XmlReader aReader)
        {
            if (aReader.Name == "Projectile")
            {
                aReader.MoveToAttribute("type");
                myNewBulletData.myType = aReader.Value;
            }
            else if (aReader.Name == "Entity")
            {
                aReader.MoveToAttribute("type");
                myNewBulletData.myEntityType = aReader.Value;
            }
            else if (aReader.Name == "maxAmount")
            {
                aReader.MoveToAttribute("value");
                myNewBulletData.myMaxAmount = int.Parse(aReader.Value);
            }
            else if (aReader.Name == "speed")
            {
                aReader.MoveToAttribute("value");
                string formatedValue = aReader.Value.Replace("f", "0");
                float floatValue = float.Parse(formatedValue, System.Globalization.CultureInfo.InvariantCulture);
                myNewBulletData.mySpeed = floatValue;
            }
        }

        public List<BulletData> GetBulletData()
        {
            return myBulletData;
        }

        public BulletListXml GetBulletListXml()
        {
            return myBulletPaths;
        }
    }
}
