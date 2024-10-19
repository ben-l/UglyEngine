#pragma once

#include "Entity.h"

namespace Ugly {
    class ScriptableEntity {
        public:
            virtual ~ScriptableEntity() {}
            template<typename T>
            T& GetComponent()
            {
                return m_Entity.GetComponent<T>();
            }
        protected:
            virtual void OnCreate() {}
            virtual void OnDestroy() {}
            virtual void OnUpdate(Timestep ts) {}
        private:
            Entity m_Entity;
            // allow Scene class to access private members of this class
            friend class Scene; 
    };
};
