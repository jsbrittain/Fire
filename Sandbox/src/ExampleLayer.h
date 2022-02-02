#include <../vendor/imgui-docking/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Fire::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_CameraController(1280.0f/720.0f)
	{
		m_VertexArray = Fire::VertexArray::Create();
		float vertices[3*7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Fire::Ref<Fire::VertexBuffer> vertexBuffer = Fire::VertexBuffer::Create(vertices,sizeof(vertices));

		Fire::BufferLayout layout = {
			{ Fire::ShaderDataType::Float3, "a_Position" },
			{ Fire::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Fire::Ref<Fire::IndexBuffer> indexBuffer = Fire::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);



		// Setup square to draw on screen

		m_SquareVA = Fire::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Fire::Ref<Fire::VertexBuffer> squareVB = Fire::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ Fire::ShaderDataType::Float3, "a_Position" },
			{ Fire::ShaderDataType::Float2, "a_TexCoord" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Fire::Ref<Fire::IndexBuffer> squareIB = Fire::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);

		// Load shaders
		
		//m_Shader = Fire::Shader::Create("/Users/jsb/Dropbox/Documents/Fire/Sandbox/assets/shaders/VertexPosColor.glsl");

		m_FlatColorShader = Fire::Shader::Create("/Users/jsb/Dropbox/Documents/Fire/Sandbox/assets/shaders/FlatColor3.glsl");

		auto textureShader = m_ShaderLibrary.Load("/Users/jsb/Dropbox/Documents/Fire/Sandbox/assets/shaders/Texture.glsl");

		m_Texture = Fire::Texture2D::Create("/Users/jsb/Dropbox/Documents/Fire/Sandbox/assets/textures/Checkerboard.png");
		m_LogoTexture = Fire::Texture2D::Create("/Users/jsb/Dropbox/Documents/Fire/Sandbox/assets/textures/Logo.png");

		textureShader->Bind();
		textureShader->SetInt("u_Texture", 0);
	}

	void OnUpdate(Fire::Timestep ts) override
	{
		// Update
		m_CameraController.OnUpdate(ts);

		// Render
		Fire::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Fire::RenderCommand::Clear();
		Fire::Renderer::BeginScene(m_CameraController.GetCamera());

    	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

    	m_FlatColorShader->Bind();
		m_FlatColorShader->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Fire::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

    	m_Texture->Bind();
    	text_rotateAmount += 2.0f*ts;		// Rotating
    	glm::mat4 transform = glm::rotate(glm::mat4(1.0f), text_rotateAmount, glm::vec3(0.0f,1.0f,0.0f));
    	transform = glm::scale(transform, glm::vec3(1.5f));
		Fire::Renderer::Submit(textureShader, m_SquareVA, transform);

		m_LogoTexture->Bind();
		Fire::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    	// End scene
    	Fire::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Fire::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

private:
	Fire::ShaderLibrary m_ShaderLibrary;
	Fire::Ref<Fire::VertexArray> m_VertexArray;

	Fire::Ref<Fire::Shader> m_FlatColorShader;
	Fire::Ref<Fire::VertexArray> m_SquareVA;

	Fire::Ref<Fire::Texture2D> m_Texture, m_LogoTexture;

	Fire::OrthographicCameraController m_CameraController;
	float text_rotateAmount = 0.0f;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};