#include "Fire.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "../vendor/imgui-docking/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Fire::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_CameraPosition(0.0f,0.0f,5.0f)
	{
		m_Camera = Fire::Camera();
		//m_Camera.SetOrthographicCamera(-1.6f, 1.6f, -0.9f, 0.9f);
		m_Camera.SetPerspectiveCamera(60.0f, 1.0f, 0.1f, 100.0f);




		// Setup to draw a triangle on screen

		m_VertexArray.reset(Fire::VertexArray::Create());
		
		float vertices[3*7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Fire::Ref<Fire::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Fire::VertexBuffer::Create(vertices,sizeof(vertices)));

		Fire::BufferLayout layout = {
			{ Fire::ShaderDataType::Float3, "a_Position" },
			{ Fire::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Fire::Ref<Fire::IndexBuffer> indexBuffer;
		indexBuffer.reset(Fire::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);



		// Setup square to draw on screen

		m_SquareVA.reset(Fire::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Fire::Ref<Fire::VertexBuffer> squareVB;
		squareVB.reset(Fire::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Fire::ShaderDataType::Float3, "a_Position" },
			{ Fire::ShaderDataType::Float2, "a_TexCoord" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Fire::Ref<Fire::IndexBuffer> squareIB;
		squareIB.reset(Fire::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		// Load shaders
		
		//m_Shader = Fire::Shader::Create("/Users/jsb/Dropbox/Documents/Fire/Sandbox/assets/shaders/VertexPosColor.glsl");

		m_FlatColorShader = Fire::Shader::Create("/Users/jsb/Dropbox/Documents/Fire/Sandbox/assets/shaders/FlatColor.glsl");

		auto textureShader = m_ShaderLibrary.Load("/Users/jsb/Dropbox/Documents/Fire/Sandbox/assets/shaders/Texture.glsl");

		m_Texture = Fire::Texture2D::Create("/Users/jsb/Dropbox/Documents/Fire/Sandbox/assets/textures/Checkerboard.png");
		m_LogoTexture = Fire::Texture2D::Create("/Users/jsb/Dropbox/Documents/Fire/Sandbox/assets/textures/Logo.png");

		std::dynamic_pointer_cast<Fire::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Fire::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Fire::Timestep ts) override
	{
		//FR_TRACE("Delta time: {0} secs, {1} msecss",ts.GetSeconds(),ts.GetMilliseconds());

		// Move camera (respond to keypress)
		if (Fire::Input::IsKeyPressed(FR_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Fire::Input::IsKeyPressed(FR_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		
		if (Fire::Input::IsKeyPressed(FR_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		else if (Fire::Input::IsKeyPressed(FR_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;

		if (Fire::Input::IsKeyPressed(FR_KEY_W))
			m_CameraPosition.z += m_CameraMoveSpeed * ts;
		else if (Fire::Input::IsKeyPressed(FR_KEY_S))
			m_CameraPosition.z -= m_CameraMoveSpeed * ts;

		// Rotate camera (respond to keypress)
		if (Fire::Input::IsKeyPressed(FR_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		else if (Fire::Input::IsKeyPressed(FR_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		// Clear screen
		Fire::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Fire::RenderCommand::Clear();

		// Set camera
    	m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
		Fire::Renderer::BeginScene(m_Camera);

		// Submit objects/elements for rendering
    	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

    	std::dynamic_pointer_cast<Fire::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Fire::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

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

	void OnEvent(Fire::Event& event) override
	{
	}

private:
	Fire::ShaderLibrary m_ShaderLibrary;
	//Fire::Ref<Fire::Shader> m_Shader;
	Fire::Ref<Fire::VertexArray> m_VertexArray;

	Fire::Ref<Fire::Shader> m_FlatColorShader;
	Fire::Ref<Fire::VertexArray> m_SquareVA;

	Fire::Ref<Fire::Texture2D> m_Texture, m_LogoTexture;

	Fire::Camera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	float m_CameraMoveSpeed = 10.0f;
	float m_CameraRotationSpeed = 20.0f;

	float text_rotateAmount = 0.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Fire::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{}
};

Fire::Application* Fire::CreateApplication()
{
	return new Sandbox();
}
