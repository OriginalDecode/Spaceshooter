﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;

namespace Launcher
{
    public partial class Form1 : Form
    {
        private string myDocumentFolder = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
        private string myConfigPath = "Data/Setting/SET_config.bin";
        private string[] myExePath = { "Application_Release.exe", "Application_Internal.exe", "Application_Debug.exe" };

        enum eResolutions
        {
            R800x600,
            R1280x720,
            R1650x1080,
            R1920x1080
        }

        enum eMSAA
        {
            x1,
            x2,
            x4
        }

        public Form1()
        {
            InitializeComponent();
            if (File.Exists(myExePath[0]) == true)
            {
                string releaseConfig = myDocumentFolder + "\\SpaceShooter\\" + myConfigPath;
                myConfigPath = releaseConfig;
                string subString = myDocumentFolder + "\\SpaceShooter";
                if (Directory.Exists(subString) == false)
                {
                    Directory.CreateDirectory(subString);
                }
                subString += "\\Data";
                if (Directory.Exists(subString) == false)
                {
                    Directory.CreateDirectory(subString);
                }
                subString += "\\Setting";
                if (Directory.Exists(subString) == false)
                {
                    Directory.CreateDirectory(subString);
                }
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            

            resolutionDropdown.Items.Add("800 x 600");
            resolutionDropdown.Items.Add("1280 x 720");
            resolutionDropdown.Items.Add("1650 x 1080");
            resolutionDropdown.Items.Add("1920 x 1080");
            resolutionDropdown.SelectedIndex = 3;

            aaDropdown.Items.Add("MSAA x1");
            aaDropdown.Items.Add("MSAA x2");
            aaDropdown.Items.Add("MSAA x4");
            aaDropdown.SelectedIndex = 2;

            windowedCheckbox.Checked = true;

            if (File.Exists(myConfigPath))
            {
                using (BinaryReader reader = new BinaryReader(File.Open(myConfigPath, FileMode.Open)))
                {
                    ReadResolutionFromFile(reader);
                    ReadMSAAFromFile(reader);
                    ReadWindowedFromFile(reader);
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            using (BinaryWriter writer = new BinaryWriter(File.Open(myConfigPath, FileMode.Create)))
            {
                WriteResolutionToFile(writer);
                WriteMSAAToFile(writer);
                WriterWindowedToFile(writer);
            }

            if (File.Exists(myExePath[0]) == true)
            {
                Process.Start(myExePath[0]);
                Application.Exit();
            }
            else if (File.Exists(myExePath[1]) == true)
            {
                Process.Start(myExePath[1]);
                Application.Exit();
            }
            else if (File.Exists(myExePath[2]) == true)
            {
                Process.Start(myExePath[2]);
                Application.Exit();
            }
            else
            {
                MessageBox.Show("Could not find a Release, Internal or Debug executable :(");
            }


        }

        void WriteResolutionToFile(BinaryWriter aWriter)
        {
            bool windowed = windowedCheckbox.Checked;
            Int32 width = 800;
            Int32 height = 600;

            if (windowed == true)
            {
                switch (resolutionDropdown.SelectedIndex)
                {
                    case (int)eResolutions.R800x600:
                        width = 800;
                        height = 600;
                        break;
                    case (int)eResolutions.R1280x720:
                        width = 1280;
                        height = 720;
                        break;
                    case (int)eResolutions.R1650x1080:
                        width = 1650;
                        height = 1080;
                        break;
                    case (int)eResolutions.R1920x1080:
                        width = 1920;
                        height = 1080;
                        break;
                }
            }
            else
            {
                Screen scr = Screen.PrimaryScreen;
                width = scr.Bounds.Width;
                height = scr.Bounds.Height;
            }

            aWriter.Write(width);
            aWriter.Write(height);
        }

        void WriteMSAAToFile(BinaryWriter aWriter)
        {
            Int32 MSAACount = 2;
            switch (aaDropdown.SelectedIndex)
            {
                case (int)eMSAA.x1:
                    MSAACount = 1;
                    break;
                case (int)eMSAA.x2:
                    MSAACount = 2;
                    break;
                case (int)eMSAA.x4:
                    MSAACount = 4;
                    break;
            }

            aWriter.Write(MSAACount);
        }

        void WriterWindowedToFile(BinaryWriter aWriter)
        {
            bool windowed = windowedCheckbox.Checked;

            if (windowed == true)
            {
                aWriter.Write((Int32)1);
            }
            else
            {
                aWriter.Write((Int32)0);
            }
        }

        void ReadResolutionFromFile(BinaryReader aReader)
        {
            int width = aReader.ReadInt32();
            int height = aReader.ReadInt32();

            if (width == 800 && height == 600)
            {
                resolutionDropdown.SelectedIndex = 0;
            }
            else if (width == 1280 && height == 720)
            {
                resolutionDropdown.SelectedIndex = 1;
            }
            else if (width == 1650 && height == 1080)
            {
                resolutionDropdown.SelectedIndex = 2;
            }
            else if (width == 1920 && height == 1080)
            {
                resolutionDropdown.SelectedIndex = 3;
            }
        }

        void ReadMSAAFromFile(BinaryReader aReader)
        {
            int msaa = aReader.ReadInt32();

            if (msaa == 1)
            {
                aaDropdown.SelectedIndex = 0;
            }
            else if (msaa == 2)
            {
                aaDropdown.SelectedIndex = 1;
            }
            else if (msaa == 4)
            {
                aaDropdown.SelectedIndex = 2;
            }
        }

        void ReadWindowedFromFile(BinaryReader aReader)
        {
            int windowed = aReader.ReadInt32();

            if (windowed == 1)
            {
                windowedCheckbox.Checked = true;
            }
            else
            {
                windowedCheckbox.Checked = false;
            }
        }

        private void windowedCheckbox_CheckedChanged(object sender, EventArgs e)
        {
            if (windowedCheckbox.Checked == true)
            {
                resolutionDropdown.Enabled = true;
            }
            else
            {
                resolutionDropdown.Enabled = false;
            }
        }
    }
}
