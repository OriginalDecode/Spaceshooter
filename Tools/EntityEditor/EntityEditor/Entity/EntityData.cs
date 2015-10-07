using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EntityEditor.Entity
{
    public struct EntityData
    {
        public string myName;

        public AIComponentData myAIComponent;
        public CollisionComponentData myCollisionComponent;
        public GraphicsComponentData myGraphicsComponent;
        public ShootingComponentData myShootingComponent;
        
    }

    public struct EntityListXML
    {
        public List<string> myPaths;
    }

    public struct GraphicsComponentData
    {
        public bool myIsActive;

        public string myEffectPath;
        public string myModelPath;
    }

    public struct AIComponentData
    {
        public bool myIsActive;
    }

    public struct ShootingComponentData
    {
        public bool myIsActive;

        public string myWeaponType;
    }

    public struct CollisionComponentData
    {
        public bool myIsActive;

        public bool myHasSphere;
        public float myRadius;
    }
}
