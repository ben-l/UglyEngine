#include "EditorLayer.h"
#include "Input.h"
#include "Maths.h"
#include "SceneSerializer.h"
#include "PlatformUtils.h"


#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ImGuizmo.h>


namespace Ugly {

    EditorLayer::EditorLayer() : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
    {
    }
    
    void EditorLayer::OnAttach()
    {
        UE_PROFILE_FUNCTION();
    
        FrameBufferSpecification fbSpec;
        fbSpec.Attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth };
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_FrameBuffer = FrameBuffer::Create(fbSpec);

        m_ActiveScene = CreateRef<Scene>();
        m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

#if 0

        //auto m_SquareEntity = m_ActiveScene->CreateEntity("Green Square Entity");
        //m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

        //auto redSquare = m_ActiveScene->CreateEntity("Red Square Entity");
        //redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});

        m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
        m_CameraEntity.AddComponent<CameraComponent>();
    
        m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
        auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
        cc.Primary = false;

        class CameraController : public ScriptableEntity {
            public:
                void OnCreate(){
                    auto& translation = GetComponent<TransformComponent>().Translation;
                    translation.x = rand() % 10 - 5.0f;
                }
                void OnDestroy(){
                }

                void OnUpdate(Timestep ts){
                    float speed = 5.0f;
                    auto& translation = GetComponent<TransformComponent>().Translation;

                    if (Input::IsKeyPressed(UE_KEY_S)){
                        translation.x -= speed * ts;
                    }
                    if (Input::IsKeyPressed(UE_KEY_F)){
                        translation.x += speed * ts;
                    }
                    if (Input::IsKeyPressed(UE_KEY_E)){
                        translation.y += speed * ts;
                    }
                    if (Input::IsKeyPressed(UE_KEY_D)){
                        translation.y -= speed * ts;
                    }
                }
        };
        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
        m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

        m_Panel.SetContext(m_ActiveScene);

    }
    
    void EditorLayer::OnDetach(){
        UE_PROFILE_FUNCTION();
    }
    
    void EditorLayer::OnUpdate(Timestep ts){
    
        UE_PROFILE_FUNCTION();

        // Resize
		if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

        // Update
        if (m_ViewportFocused){
            m_CameraController.OnUpdate(ts);
        }
        m_EditorCamera.OnUpdate(ts);

    
        // Render
        Renderer2d::ResetStats();
        m_FrameBuffer->Bind();
    	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    	RenderCommand::Clear();

        // Clear entity ID attachment to -1
        m_FrameBuffer->ClearAttachment(1, -1);


        // Update Scene
        m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0]; 
        my = viewportSize.y - my;
        int mouseX = (int)mx;
        int mouseY = (int)my;

        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
            int pixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
            UE_CORE_WARN("pixel data = {0}", pixelData);
        }




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
            ImGuiStyle& style = ImGui::GetStyle();
            float minWinSizeX = style.WindowMinSize.x;
            style.WindowMinSize.x = 370.0f;
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }
            style.WindowMinSize.x = minWinSizeX;
            
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    // Disabling fullscreen would allow the window to be moved to the front of other windows,
                    // which we can't undo at the moment without finer window depth/z control.
                    //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                    //ImGui::MenuItem("Padding", NULL, &opt_padding);
                    //ImGui::Separator();
                    //
                    if (ImGui::MenuItem("New", "Ctrl+Shift+N")){
                        NewScene();
                    }

                    if (ImGui::MenuItem("Open...", "Ctrl+0")){
                        OpenScene();
                    }
            
                    if (ImGui::MenuItem("Save As...", "Ctrl+S")){
                        SaveSceneAs();
                    }

                    if (ImGui::MenuItem("Exit")) Application::Get().Close();
                    ImGui::EndMenu();
                }
            
                ImGui::EndMenuBar();
            }

            m_Panel.OnImGuiRender();
            
            ImGui::Begin("Stats");
    
            auto stats = Renderer2d::GetStats();
            ImGui::Text("Renderer2d Stats:");
            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quads: %d", stats.QuadCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    
            ImGui::End();


            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
            ImGui::Begin("ViewPort");
            auto viewportOffset = ImGui::GetCursorPos(); // Includes tab bar

            m_ViewportFocused = ImGui::IsWindowFocused();
		    m_ViewportHovered = ImGui::IsWindowHovered();
		    Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};

            uint64_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
            //UE_WARN("Viewport size: {0}, {1}", viewportPanelSize.x, viewportPanelSize.y);

            ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{0,1},ImVec2{1, 0});

            auto windowSize = ImGui::GetWindowSize();
            ImVec2 minBound = ImGui::GetWindowPos();
            minBound.x += viewportOffset.x;
            minBound.y += viewportOffset.y;

            ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
            m_ViewportBounds[0] = { minBound.x, minBound.y };
            m_ViewportBounds[1] = { maxBound.x, maxBound.y };



            // Gizmos
            Entity selectedEntity = m_Panel.GetSelectedEntity();
            // Make sure selectedEntity has a Transform Component
            // otherwise creating and selecting new entity will cause a crash
            if (selectedEntity && m_GizmoType != -1) {
                ImGuizmo::SetOrthographic(false);
                ImGuizmo::SetDrawlist();

                float windowWidth = (float)ImGui::GetWindowWidth();
                float windowHeight = (float)ImGui::GetWindowHeight();
                ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);


                // Gizmo Camera Stuff

                /*
                auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
                const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
                const glm::mat4& cameraProjection = camera.GetProjection();
                glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());
                */

                // Editor camera
                const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
                glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

                // Entity transform
                auto& tc = selectedEntity.GetComponent<TransformComponent>();
                glm::mat4 transform = tc.GetTransform();

                // Snapping
                bool snap = Input::IsKeyPressed(UE_KEY_LEFT_CONTROL);
                float snapValue = 0.5f; // Snap to 0.5 for translation / scale
                // Snap to 45 degrees for rotation;
                if (m_GizmoType == ImGuizmo::OPERATION::ROTATE){
                    snapValue = 45.0f;
                }

                float snapValues[3] = {snapValue, snapValue, snapValue};

                ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr, nullptr, nullptr);

                if (ImGuizmo::IsUsing()){
                    glm::vec3 translation, scale, rotation;
                    Maths::DecomposeTransform(transform, translation, rotation, scale);
                    glm::vec3 deltaRotation = rotation - tc.Rotation;
                    tc.Translation = translation;
                    tc.Rotation += deltaRotation;
                    tc.Scale = scale;

                }

            }

            ImGui::End();
            ImGui::PopStyleVar();

            ImGui::End();
        } 
    }
    
    void EditorLayer::OnEvent(Event& e){
        m_CameraController.OnEvent(e);
        m_EditorCamera.OnEvent(e);

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<Ugly::KeyPressedEvent>(UE_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    }


    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e){
        if (e.GetRepeatCount() > 0){
            return false;
        }
        bool control = Input::IsKeyPressed(UE_KEY_LEFT_CONTROL) || Input::IsKeyPressed(UE_KEY_RIGHT_CONTROL);
        bool shift = Input::IsKeyPressed(UE_KEY_LEFT_SHIFT) || Input::IsKeyPressed(UE_KEY_RIGHT_SHIFT);
        switch(e.GetKeyCode()){
            case UE_KEY_N:
            {
                if (control && shift){
                    NewScene();
                    break;
                }
            }
            case UE_KEY_O:
            {
                if (control){
                    OpenScene();
                    break;
                }
            }
            case UE_KEY_S:
            {
                if (control){
                    SaveSceneAs();
                    break;
                }
            }

            // Gizmo shortcuts
            // gets rid of the Gizmo tool onscreen
            case UE_KEY_Q:
            {
                m_GizmoType = -1;
                break;
            }
            // triggers the gizmo translate tool
            case UE_KEY_W:
            {
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                break;
            }
            // triggers the gizmo rotate tool
            case UE_KEY_E:
            {
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
                break;
            }
            // triggers the gizmo scale tool
            case UE_KEY_R:
            {
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
                break;
            }
        }
        return true;
    }

    void EditorLayer::NewScene(){
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_Panel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OpenScene(){
        std::string filepath = FileDialogs::OpenFile("Ugly Scene (*.ugly)\0*.ugly\0");
        if (!filepath.empty()){
            m_ActiveScene = CreateRef<Scene>();
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_Panel.SetContext(m_ActiveScene);
            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(filepath);
        }
    }

    void EditorLayer::SaveSceneAs(){
        std::string filepath = FileDialogs::SaveFile("Ugly Scene (*.ugly)\0*.ugly\0");
        if (!filepath.empty()){
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(filepath);
        }
    }

};
