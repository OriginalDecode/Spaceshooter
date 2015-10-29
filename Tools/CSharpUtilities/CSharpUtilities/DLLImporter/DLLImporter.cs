using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace CSharpUtilities.DLLImporter
{
    public static class NativeMethods
    {
        const string EngineDLLName = "DLLExporter_Release.dll";

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void StartEngine(IntPtr aHwnd);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void SetupWindow(Int32 aWidth, Int32 aHeight);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void Render();

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void Update();

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void UpdateFilewatcher();

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void LoadModel(string aModelFile, string aEffectFile);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void SetEffect(string aEffectFile);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void SetClearColor(float aRChannel, float aGChannel, float aBChannel, float aAChannel);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void DirectionaLightRotateX(float aXValue);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void DirectionaLightRotateY(float aYValue);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void DirectionaLightRotateZ(float aZValue);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe float GetDirectionaLightXRotation();

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe float GetDirectionaLightYRotation();

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe float GetDirectionaLightZRotation();

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void RotateObjectAtX(float aSpeed);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void RotateObjectAtY(float aSpeed);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void RotateObjectAtZ(float aSpeed);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void SetRotateObjectAtX(float aAngle);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void SetRotateObjectAtY(float aAngle);

        [DllImport(EngineDLLName, CallingConvention = CallingConvention.Cdecl)]
        public extern static unsafe void SetRotateObjectAtZ(float aAngle);
    }
}
