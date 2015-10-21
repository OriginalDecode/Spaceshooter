using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CSharpUtilities;

namespace EntityEditor.Entity
{
    public struct EntityData
    {
        public string myName;

        public AIComponentData myAIComponent;
        public BulletComponentData myBulletComponent;
        public CollisionComponentData myCollisionComponent;
        public GraphicsComponentData myGraphicsComponent;
        public ShootingComponentData myShootingComponent;
        public HealthComponentData myHealthComponent;
        public PhysicsComponentData myPhysicsComponent;
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

        public Vector2<float> mySpeed;
        public Vector2<float> myTimeToNextDecision;
        public Vector3<float> myAvoidanceOffset;
        public float myAvoidanceDistance;
        public int myAIMode;
        public string myEntityToFollow;
    }

    public struct ShootingComponentData
    {
        public bool myIsActive;

        public string myWeaponType;
    }

    public struct HealthComponentData
    {
        public bool myIsActive;

        public int myHealth;
    }

    public struct PhysicsComponentData
    {
        public bool myIsActive;

        public float myWeight;
    }

    public struct BulletComponentData
    {
        public bool myIsActive;

        public float myLifeTime;
        public float myDamage;
    }

    public struct CollisionComponentData
    {
        public bool myIsActive;

        public bool myHasSphere;
        public float myRadius;
    }
}
