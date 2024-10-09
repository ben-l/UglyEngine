#include <Sandbox2d.h>
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2d::Sandbox2d() : Layer("Sandbox2d"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
{
}

void Sandbox2d::OnAttach()
{
    UE_PROFILE_FUNCTION();
    m_CheckerboardTexture = Ugly::Texture2d::Create("assets/textures/Checkerboard.png");

    m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 5.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };

    m_CameraController.SetZoomLevel(5.0f);

    Ugly::FrameBufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_FrameBuffer = Ugly::FrameBuffer::Create(fbSpec);

}

void Sandbox2d::OnDetach(){
    UE_PROFILE_FUNCTION();
}

void Sandbox2d::OnUpdate(Ugly::Timestep ts){

    UE_PROFILE_FUNCTION();

    // Update
    m_CameraController.OnUpdate(ts);

    // Render
    Ugly::Renderer2d::ResetStats();
    {
        UE_PROFILE_SCOPE("Renderer_Prep");
        m_FrameBuffer->Bind();
	    Ugly::RenderCommand::SetClearColor({ 0.1f, 0.2f, 0.1f, 1 });
	    Ugly::RenderCommand::Clear();
    }
    {
        static float rotation = 0.0f;
        rotation += ts * 50.0f;

        UE_PROFILE_SCOPE("Renderer_Draw");
        Ugly::Renderer2d::BeginScene(m_CameraController.GetCamera());
		Ugly::Renderer2d::DrawRotatedQuad({ 1.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(-45.0f), {0.8f, 0.2f, 0.3f, 1.0f});
		Ugly::Renderer2d::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.0f, 0.0f, 1.0f, 1.0f });
		Ugly::Renderer2d::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
		//Ugly::Renderer2d::DrawQuad({ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture, 20.0f);
		Ugly::Renderer2d::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 1.0f);
		Ugly::Renderer2d::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_CheckerboardTexture, 1.0f);
        Ugly::Renderer2d::EndScene();

        Ugly::Renderer2d::BeginScene(m_CameraController.GetCamera());
        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f){
                glm::vec4 color = { (x + 0.5f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.9f };
                Ugly::Renderer2d::DrawQuad({x, y}, { 0.45f, 0.45f }, color);
            }
        }
        Ugly::Renderer2d::EndScene();
    }


    if (Ugly::Input::IsMouseButtonPressed(UE_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Ugly::Input::GetMousePosition();
		auto width = Ugly::Application::Get().GetWindow().GetWidth();
		auto height = Ugly::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
        // number of particles
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());

    Ugly::Renderer2d::EndScene();
    m_FrameBuffer->Unbind();

}

void Sandbox2d::OnImGuiRender()
{
    UE_PROFILE_FUNCTION();

    static bool dockingEnabled = true;
    if (dockingEnabled)
    {
        static bool dockspaceOpen = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        
        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }
        
        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;
        
        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();
        
        if (opt_fullscreen)
            ImGui::PopStyleVar(2);
        
        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                //ImGui::MenuItem("Padding", NULL, &opt_padding);
                //ImGui::Separator();
        
                if (ImGui::MenuItem("Exit")) Ugly::Application::Get().Close();
                ImGui::EndMenu();
            }
        
            ImGui::EndMenuBar();
        }
        
        ImGui::Begin("Settings");

        auto stats = Ugly::Renderer2d::GetStats();
        ImGui::Text("Renderer2d Stats:");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

        ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
        
        uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)textureID, ImVec2{ 1280.0f, 720.0f });
        ImGui::End();
        
        ImGui::End();
    } else {
        ImGui::Begin("Settings");
        
        auto stats = Ugly::Renderer2d::GetStats();
        ImGui::Text("Renderer2d Stats:");
        ImGui::Text("Draw Calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
        
        ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
        
        uint32_t textureID = m_CheckerboardTexture->GetRendererID();
        ImGui::Image((void*)textureID, ImVec2{ 256.0f, 256.0f });
        ImGui::End();
    }
}

void Sandbox2d::OnEvent(Ugly::Event& e){
    m_CameraController.OnEvent(e);
}
