#include <UglyEngine.h>
#include <EntryPoint.h>

#include <EditorLayer.h>

namespace Ugly {

    class UglyEd : public Application
    {
    public:
        UglyEd() : Application("UglyEd")
        {
            //PushLayer(new ExampleLayer());
            PushLayer(new EditorLayer());
        }
        ~UglyEd()
        {
        }
    };
    
    Application* CreateApplication()
    {
        return new UglyEd();
    }
};
