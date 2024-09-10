#include <Sandbox2d.h>
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

template<typename Fn>
class Timer {
    public:
    	Timer(const char* name, Fn&& func)
    		: m_Name(name), m_Func(func), m_Stopped(false)
    	{
            // TODO implement macro for std::chrono::high_resolution_clock and steady_clock. Issue may be OS related
            // https://github.com/jmp75/jsonserial/issues/1
    		m_StartTimepoint = std::chrono::steady_clock::now();
    	}
    
    	~Timer()
    	{
    		if (!m_Stopped)
    			Stop();
    	}
    
    	void Stop()
    	{
            // TODO implement macro for std::chrono::high_resolution_clock and steady_clock. Issue may be OS related
    		auto endTimepoint = std::chrono::steady_clock::now();
    
    		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
    		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
    
    		m_Stopped = true;

    		float duration = (end - start) * 0.001f;
    		m_Func({ m_Name, duration });
    	}
    private:
	    const char* m_Name;
	    Fn m_Func;
	    std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	    bool m_Stopped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

Sandbox2d::Sandbox2d() : Layer("Sandbox2d"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2d::OnAttach(){
    m_CheckerboardTexture = Ugly::Texture2d::Create("assets/textures/Checkerboard.jpg");

}

void Sandbox2d::OnDetach(){
}

void Sandbox2d::OnUpdate(Ugly::Timestep ts){

    PROFILE_SCOPE("Sandbox2d::OnUpdate");

    // Update
    {
        PROFILE_SCOPE("CameraController::OnUpdate");
        m_CameraController.OnUpdate(ts);
    }

    // Render
    {
        PROFILE_SCOPE("Renderer_Prep");
	    Ugly::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	    Ugly::RenderCommand::Clear();
    }

    {
        PROFILE_SCOPE("Renderer_Draw");
        Ugly::Renderer2d::BeginScene(m_CameraController.GetCamera());
        Ugly::Renderer2d::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        Ugly::Renderer2d::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
        Ugly::Renderer2d::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
        Ugly::Renderer2d::EndScene();
    }
}

void Sandbox2d::OnImGuiRender(){
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

    for (auto& result : m_ProfileResults)
    {
        char label[50];
        strcpy(label, "%.3fms  ");
        strcat(label, result.Name);
        ImGui::Text(label, result.Time);
    }
    m_ProfileResults.clear();

    ImGui::End();
}

void Sandbox2d::OnEvent(Ugly::Event& e){
    m_CameraController.OnEvent(e);
}
