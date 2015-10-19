using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CSharpUtilities
{
   public class StringUtilities
    {

        public static void PrintString(string aString)
        {
            MessageBox.Show(aString, "Test");
        }

        public static bool StringHasExtension(string aString)
        {
            int index = 0;
            index = aString.IndexOf(".");

            return index == 0;
        }

        public static byte[] GetBytes(string str)
        {
            byte[] bytes = new byte[str.Length * sizeof(char)];
            System.Buffer.BlockCopy(str.ToCharArray(), 0, bytes, 0, bytes.Length);
            return bytes;
        }

        public static string GetString(byte[] bytes)
        {
            char[] chars = new char[bytes.Length / sizeof(char)];
            System.Buffer.BlockCopy(bytes, 0, chars, 0, bytes.Length);
            return new string(chars);
        }

        public static string Reverse(string s)
        {
            char[] charArray = s.ToCharArray();
            Array.Reverse(charArray);
            return new string(charArray);
        }

        public static string ConvertPathToOwnFolder(string aPath, string aFileName, string aTargetFolder)
        {
            string convertedPath = aPath.Replace(aFileName, "");
            if (convertedPath == "") return "";
            convertedPath = convertedPath.Replace("\\", "/");
            convertedPath = convertedPath.Remove(convertedPath.Length - 1);

            convertedPath = Reverse(convertedPath);
            int firstSlash = convertedPath.IndexOf("/");

            convertedPath = convertedPath.Substring(0, firstSlash);
            convertedPath = Reverse(convertedPath);

            convertedPath = aTargetFolder + "/" + convertedPath + "/" + aFileName;

            return convertedPath;
        }

        public static string ConvertPathToDataFolderPath(string aPath)
        {
            int dataTagIndex = aPath.IndexOf("Data");
            if (dataTagIndex <= 0) return "Data/";
            string convertedPath = aPath;
            convertedPath = aPath.Substring(0, dataTagIndex);
            convertedPath = convertedPath.Replace("\\", "/");
            convertedPath += "Data/";

            return convertedPath;
        }
    }
}
