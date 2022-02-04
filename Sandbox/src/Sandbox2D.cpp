#include "Sandbox2D.h"
#include <../vendor/imgui-docking/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const uint32_t s_MapWidth = 24;
static const char* s_MapTiles =
"WWWWWWWWbbbbbbbbbWWWWWWW"
"WWWWWWWWGGGGGGGGGWWWWWWW"
"WWWWWWGGGGGGGGGGGGGGWWWW"
"WWWWWWWGGGGGGGGGGGGGWWWW"
"WWWWWWGGGGGGGGGGGGGWWWWW"
"WWWWWGGGGGttttGGGGGGWWWW"
"WWWWWGGGGWWWWWWGGGGWWWWW"
"WWWWWWGGGGGWWGGGGGWWWWWW"
"WWWWWGGGGGGGGGGGGGWWWWWW"
"WWWWWcGGGGGGGGGGGWWWWWWW"
"WWWWWWtttttttttttWWWWWWW"
;

static const char* s_ObjectTiles =
"........................"
"........---------......."
"......---B----------...."
".......------B--B---...."
"......-------------....."
".....-----....--B---...."
".....---T......----....."
"......-TT-T..-----......"
".....--TTTBB------......"
"......-B-BB------......."
"........................"
;

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f/720.0f)
{
}

void Sandbox2D::OnAttach()
{
	FR_PROFILE_FUNCTION();

	m_CheckerboardTexture = Fire::Texture2D::Create("assets/textures/Checkerboard.png");
	
	m_SpriteSheet   = Fire::Texture2D::Create("assets/spritesheets/RPGpack_sheet_2X.png");
	m_TextureStairs = Fire::SubTexture2D::CreateFromCoords(m_SpriteSheet, {7,6}, {128,128});
	m_TextureBarrel = Fire::SubTexture2D::CreateFromCoords(m_SpriteSheet, {8,2}, {128,128});
	m_TextureTree   = Fire::SubTexture2D::CreateFromCoords(m_SpriteSheet, {2,1}, {128,128}, {1,2});	// Retrive 2-cell sprite

	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles)/m_MapWidth;

	char key;
	key = 'G'; s_TextureMapSize[key] = {1,1}; s_TextureMap[key] = Fire::SubTexture2D::CreateFromCoords(m_SpriteSheet, {1,11},  {128,128}, s_TextureMapSize[key]);	// [G]rass
	key = 'D'; s_TextureMapSize[key] = {1,1}; s_TextureMap[key] = Fire::SubTexture2D::CreateFromCoords(m_SpriteSheet, {6,11},  {128,128}, s_TextureMapSize[key]);	// [D]irt
	key = 'W'; s_TextureMapSize[key] = {1,1}; s_TextureMap[key] = Fire::SubTexture2D::CreateFromCoords(m_SpriteSheet, {11,11}, {128,128}, s_TextureMapSize[key]);	// [W]ater
	key = 't'; s_TextureMapSize[key] = {1,1}; s_TextureMap[key] = Fire::SubTexture2D::CreateFromCoords(m_SpriteSheet, {11,12}, {128,128}, s_TextureMapSize[key]);	//
	key = 'b'; s_TextureMapSize[key] = {1,1}; s_TextureMap[key] = Fire::SubTexture2D::CreateFromCoords(m_SpriteSheet, {11,10}, {128,128}, s_TextureMapSize[key]);	//
	key = 'c'; s_TextureMapSize[key] = {1,1}; s_TextureMap[key] = Fire::SubTexture2D::CreateFromCoords(m_SpriteSheet, {12,12}, {128,128}, s_TextureMapSize[key]);	//
	key = 'T'; s_TextureMapSize[key] = {1,2}; s_TextureMap[key] = Fire::SubTexture2D::CreateFromCoords(m_SpriteSheet, {2,1},   {128,128}, s_TextureMapSize[key]);	// [T]ree, 2-cell sprite
	key = 'B'; s_TextureMapSize[key] = {1,1}; s_TextureMap[key] = Fire::SubTexture2D::CreateFromCoords(m_SpriteSheet, {3,3},   {128,128}, s_TextureMapSize[key]);	// [B]ush

	Fire::FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = Fire::Framebuffer::Create(fbSpec);

	m_CameraController.SetZoomLevel(5.0f);
}

void Sandbox2D::OnDetach()
{
	FR_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Fire::Timestep ts)
{
	FR_PROFILE_FUNCTION();

	// Check state changes due to ImGui
	if ( m_VsyncState != m_VsyncGuiState )
	{
		m_VsyncState = m_VsyncGuiState;
		Fire::Application::Get().GetWindow().SetVSync( m_VsyncState );
	}

	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Fire::Renderer2D::ResetStats();
	{
		FR_PROFILE_SCOPE("Renderer Prep");
		Fire::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Fire::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		FR_PROFILE_SCOPE("Renderer Draw");
		switch ( 3 )
		{
		case 1:
			// Test case - textures, subtextures, backgrounds grid, animation
			Fire::Renderer2D::BeginScene(m_CameraController.GetCamera());
			Fire::Renderer2D::DrawQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f }, glm::radians(-45.0f));
			Fire::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
			Fire::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
			Fire::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_CheckerboardTexture, glm::radians(0.0f), 5.0f);
			Fire::Renderer2D::DrawQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, m_CheckerboardTexture, glm::radians(rotation), 1.0f);

			Fire::Renderer2D::DrawQuad({ -1.0f, -1.0f, 0.1f }, { 1.0f, 1.0f }, m_TextureStairs);
			Fire::Renderer2D::DrawQuad({ 0.0f, -1.0f, 0.1f }, { 1.0f, 1.0f },  m_TextureBarrel);
			Fire::Renderer2D::DrawQuad({ 1.0f, -1.0f }, { 1.0f, 2.0f },  m_TextureTree);

			Fire::Renderer2D::DrawQuad({ 0.0f, -3.0f }, { 1.0f, 1.0f },  s_TextureMap['D']);
			Fire::Renderer2D::DrawQuad({ 1.0f, -3.0f }, { 1.0f, 1.0f },  s_TextureMap['W']);

			Fire::Renderer2D::EndScene();
			break;
		case 2:
			// Translucent squares grid
	 		Fire::Renderer2D::BeginScene(m_CameraController.GetCamera());
	 		for (float y = -5.0f; y < 5.0f; y += 0.5f )
	 		{
	 			for (float x = -5.0f; x < 5.0f; x += 0.5f )
	 			{
	 				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
	 				Fire::Renderer2D::DrawQuad({x,y},{0.45f, 0.45f}, color);
	 			}
	 		}
	 		Fire::Renderer2D::EndScene();
	 		break;
	 	case 3:
	 		Fire::Renderer2D::BeginScene(m_CameraController.GetCamera());
	 		// Map tiles --- background
	 		for ( uint32_t y = 0; y < m_MapHeight; y++ )
	 		{
	 			for ( uint32_t x = 0; x < m_MapWidth; x++ )
	 			{
	 				// Background set to z=-100.0
	 				char tileIndex = s_MapTiles[x+y*m_MapWidth];
	 				Fire::Renderer2D::DrawQuad({ (float)x-(float)m_MapWidth/2, (float)m_MapHeight/2-(float)y, -99.0f/100.0f }, s_TextureMapSize[tileIndex], s_TextureMap[tileIndex]);
	 			}
	 		}
	 		// Map tiles --- objects
	 		for ( uint32_t y = 0; y < m_MapHeight; y++ )
	 		{
	 			for ( uint32_t x = 0; x < m_MapWidth; x++ )
	 			{
	 				// Background overlap stacking only works when z is set with lower rows set closer
	 				char tileIndex = s_ObjectTiles[x+y*m_MapWidth];
	 				if (( tileIndex != '.' ) && ( tileIndex != '-' ))
		 				Fire::Renderer2D::DrawQuad({ (float)x-(float)m_MapWidth/2, (float)m_MapHeight/2-(float)y, (float)y/100.0f }, s_TextureMapSize[tileIndex], s_TextureMap[tileIndex]);
	 			}
	 		}
	 		Fire::Renderer2D::EndScene();
	 		break;
	 	default:	;
	 	}
	}

	m_fps = 0.9f*m_fps + 0.1f/ts;		// Running average

	// TODO: Add these functions - Shader::SetMat4, Shader::SetFloat4
	// std::dynamic_pointer_cast<Fire::OpenGLShader>(m_FlatColorShader)->Bind();
	// std::dynamic_pointer_cast<Fire::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
}

void Sandbox2D::OnImGuiRender()
{
	FR_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = Fire::Renderer2D::GetStats();
 	ImGui::Text("Renderer2D Stats:");
 	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
 	ImGui::Text("Quads: %d", stats.QuadCount);
 	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
 	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
 	ImGui::Checkbox("VSync: ", &m_VsyncGuiState);
 	ImGui::Text("Frame timing: %.1f msecs (%.1f fps)", 1000/m_fps, m_fps);
 	
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	
	uint32_t textureID = m_CheckerboardTexture->GetRendererID();
	ImGui::Image((void*)(intptr_t)textureID, ImVec2{ 1280, 720 }, ImVec2{0,1}, ImVec2{1,0});
	ImGui::End();
}

void Sandbox2D::OnEvent(Fire::Event& e)
{
	m_CameraController.OnEvent(e);
}
