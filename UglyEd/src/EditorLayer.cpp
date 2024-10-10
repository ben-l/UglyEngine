#include "EditorLayer.h"
#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Ugly {

    EditorLayer::EditorLayer() : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
    {
    }
    
    void EditorLayer::OnAttach()
    {
        UE_PROFILE_FUNCTION();
        m_CheckerboardTexture = Texture2d::Create("assets/textures/Checkerboard.png");
    
        m_CameraController.SetZoomLevel(5.0f);
    
        FrameBufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_FrameBuffer = FrameBuffer::Create(fbSpec);
    
    }
    
    void EditorLayer::OnDetach(){
        UE_PROFILE_FUNCTION();
    }
    
    void EditorLayer::OnUpdate(Timestep ts){
    
        UE_PROFILE_FUNCTION();
    
        // Update
        m_CameraController.OnUpdate(ts);
    
        // Render
        Renderer2d::ResetStats();
        {
            UE_PROFILE_SCOPE("Renderer_Prep");
            m_FrameBuffer->Bind();
    	    RenderCommand::SetClearColor({ 0.1f, 0.2f, 0.1f, 1 });
    	    RenderCommand::Clear();
        }
        {
            static float rotation = 0.0f;
            rotation += ts * 50.0f;
    
            UE_PROFILE_SCOPE("Renderer_Draw");
            Renderer2d::BeginScene(m_CameraController.GetCamera());
    		Renderer2d::DrawRotatedQuad({ 1.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(-45.0f), {0.8f, 0.2f, 0.3f, 1.0f});
    		Renderer2d::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.0f, 0.0f, 1.0f, 1.0f });
    		Renderer2d::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
    		//Renderer2d::DrawQuad({ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture, 20.0f);
    		Renderer2d::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, 1.0f);
    		Renderer2d::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_CheckerboardTexture, 1.0f);
            Renderer2d::EndScene();
    
            Renderer2d::BeginScene(m_CameraController.GetCamera());
            for (float y = -5.0f; y < 5.0f; y += 0.5f)
            {
                for (float x = -5.0f; x < 5.0f; x += 0.5f){
                    glm::vec4 color = { (x + 0.5f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.9f };
                    Renderer2d::DrawQuad({x, y}, { 0.45f, 0.45f }, color);
                }
            }
            Renderer2d::EndScene();
        }
    
    
        Renderer2d::EndScene();
        m_FrameBuffer->Unbind();
    
    }
    
    void EditorLayer::OnImGuiRender()
    {
        UE_PROFILE_FUNCTION();
    
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
            
                    if (ImGui::MenuItem("Exit")) Application::Get().Close();
                    ImGui::EndMenu();
                }
            
                ImGui::EndMenuBar();
            }
            
            ImGui::Begin("Settings");
    
            auto stats = Renderer2d::GetStats();
            ImGui::Text("Renderer2d Stats:");
            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quads: %d", stats.QuadCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    
            ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
            
            ImGui::End();


            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
            ImGui::Begin("ViewPort");

            m_ViewportFocused = ImGui::IsWindowFocused();
		    m_ViewportHovered = ImGui::IsWindowHovered();
		    Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
            /*
            if(m_ViewportSize != *((glm::vec2*)&viewportPanelSize)){
                m_FrameBuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
                m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);
            }
            */
            uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
            UE_WARN("Viewport size: {0}, {1}", viewportPanelSize.x, viewportPanelSize.y);

            ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{0,1},ImVec2{1, 0});

            ImGui::End();
            ImGui::PopStyleVar();

            ImGui::End();
        } 
    }
    
    void EditorLayer::OnEvent(Event& e){
        m_CameraController.OnEvent(e);
    }

};
