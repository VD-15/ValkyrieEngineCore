#include "GLRenderer.h"

#define GLEW_STATIC
#include "GLEW/glew.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#include "ValkyrieEngine/graphics/DrawTextureComponent.h"
#include "ValkyrieEngine/graphics/CameraComponent.h"
#include "ValkyrieEngine/common/Matrix.h"
#include "ValkyrieEngine/window/Window.h"

using namespace vlk;

GLBuffer::GLBuffer(UInt size, UInt usage) :
	handle(GenBuffer()),
	size(size)
{
	UInt currentBinding = GetBoundBuffer(GL_ARRAY_BUFFER);
	
	glBindBuffer(GL_ARRAY_BUFFER, this->handle);
	glBufferData(GL_ARRAY_BUFFER, this->size, nullptr, usage);
	glBindBuffer(GL_ARRAY_BUFFER, currentBinding);

	this->target = 0;
}

GLBuffer::~GLBuffer()
{
	glDeleteBuffers(1, &handle);
}

void GLBuffer::Bind(UInt _target)
{
	if (this->target != 0)
	{
		throw std::exception("This buffer is already bound to a different target.");
	}

	//Insert or access pointer
	UInt boundBuffer = GetBoundBuffer(_target);

	if (boundBuffer != 0)
	{
		throw std::exception("A different buffer is already bound to the target.");
	}

	glBindBuffer(_target, this->handle);
	this->target = _target;
}

void GLBuffer::Unbind()
{
	if (this->target == 0)
	{
		throw std::exception("This buffer is not bound to a target.");
	}

	UInt currentBinding = GetBoundBuffer(this->target);

	if (currentBinding != 0)
	{
		throw std::exception("This buffer is not bound to its target.");
	}

	glBindBuffer(this->target, 0);
	this->target = 0;
}

void GLBuffer::Fill(const ByteBuffer& data)
{
	if (this->target == 0)
	{
		throw std::exception("This buffer is not bound to a target.");
	}

	if (data.size > this->size)
	{
		throw std::exception("Buffer data is too big.");
	}

	if (GetBoundBuffer(this->target) != this->handle)
	{
		throw std::exception("A different buffer is bound to the target.");
	}

	glBufferSubData(this->target, 0, data.size, data.data);
}

UInt GLBuffer::GenBuffer()
{
	UInt handle = 0;
	glGenBuffers(1, &handle);
	return handle;
}

GLRenderer::GLRenderer(const GLRendererArgs& args)
{

}
/*
namespace
{
	std::map<const Texture2D*, UInt> textureMap;

	struct GLBuffer
	{
		UInt id;
		UInt size;
	};

	struct Primitive2DVertexArray
	{
		UInt id;
		GLBuffer vertexBuffer;
		GLBuffer elementBuffer;

		struct Program
		{
			UInt id;
			UInt vertexPositionBinding;
			UInt vertexColorBinding;
			UInt vertexDepthBinding;
			UInt viewportBinding;

		} program;

	} primitiveVertexArray;

	struct Texture2DVertexArray
	{
		UInt id;
		GLBuffer vertexBuffer;
		GLBuffer elementBuffer;

		struct Texture2DProgram
		{
			UInt id;
			UInt vertexPositionBinding;
			UInt vertexDepthBinding;
			UInt vertexColorBinding;
			UInt vertexUVBinding;
			UInt textureBinding;
			UInt viewportBinding;

		} program;

	} texture2DVertexArray;

	UInt CreateShader(UInt type, const std::string& path)
	{
		UInt shader = glCreateShader(type);
		StringCollection src = IO::ReadFile(path);
		glShaderSource(shader, src.Count(), src.Data(), src.Lengths());
		glCompileShader(shader);
		return shader;
	}

	void CheckShaderCompile(UInt shader)
	{
		Int success = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			Int logLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

			char* infoLog = new char[logLength];
			glGetShaderInfoLog(shader, logLength, 0, infoLog);
			LogError("GLRenderer", "Shader failed to compile:\n" + std::string(infoLog));
			delete[] infoLog;
		}
		else
		{
			LogVerbose("GLRenderer", "Shader compiled successfully");
		}
	}

	void CheckProgramCompile(UInt program)
	{
		Int success = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &success);

		if (!success)
		{
			Int logLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

			char* infoLog = new char[logLength];
			glGetProgramInfoLog(program, logLength, 0, infoLog);
			LogError("GLRenderer", "Shader program failed to link:\n" + std::string(infoLog));
			delete[] infoLog;
		}
		else
		{
			LogVerbose("GLRenderer", "Shader program linked successfully");
		}
	}
	
	void OnResize(vlk::WindowFramebufferEvent& ev)
	{
		glViewport(0, 0, ev.width, ev.height);
	}

	void OnTextureAtlasLoad(const ContentLoadedEvent<Texture> ev)
	{
		LogInfo("GLRenderer", "Loading texture atlas: " + ev.name);

		UInt textureID = 0;
		glGenTextures(1, &textureID);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, ev.atlas->Width(), ev.atlas->Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, ev.atlas->Data());

		Float col[4] { 1.0f, 0.0f, 1.0f, 1.0f };

		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, col);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glGenerateMipmap(GL_TEXTURE_2D);

		textureMap.insert({ ev.atlas, textureID });
	}

	void OnTextureAtlasUnload(TextureAtlasUnloadedEvent& ev)
	{
		LogInfo("GLRenderer", "Unloading texture atlas: " + ev.name);

		auto it = textureMap.find(ev.atlas);

		if (it != textureMap.end())
		{
			UInt textureID = textureMap[ev.atlas];
			glDeleteTextures(1, &textureID);

			textureMap.erase(it);
		}
	}

	void InitPrimitive2DVertexArray()
	{
		//Create vertex array
		UInt vertexArray = 0;
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		//Create vertex shader
		UInt vertShader = CreateShader(GL_VERTEX_SHADER, "shaders/primitiveShader.vert");
		CheckShaderCompile(vertShader);

		//Create fragment shader
		UInt fragShader = CreateShader(GL_FRAGMENT_SHADER, "shaders/primitiveShader.frag");
		CheckShaderCompile(fragShader);

		//Create shader program
		UInt shaderProgram = glCreateProgram();

		//Attach shaders
		glAttachShader(shaderProgram, vertShader);
		glAttachShader(shaderProgram, fragShader);

		//Link shader program
		glLinkProgram(shaderProgram);
		CheckProgramCompile(shaderProgram);

		//Detach shaders
		glDetachShader(shaderProgram, vertShader);
		glDetachShader(shaderProgram, fragShader);

		//Delete shaders
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		//Use program
		glUseProgram(shaderProgram);

		//Create buffers
		UInt buffers[2]{ 0, 0 };
		glGenBuffers(2, buffers);

		//Create vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, DEFAULT_VERTEX_BUFFER_SIZE, nullptr, GL_STREAM_DRAW);

		//Create element buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, DEFAULT_ELEMENT_BUFFER_SIZE, nullptr, GL_STREAM_DRAW);

		//Get vertex attribute bindings
		UInt vertexPosBinding = glGetAttribLocation(shaderProgram, "inPosition");
		UInt vertexDepthBinding = glGetAttribLocation(shaderProgram, "inDepth");
		UInt vertexColBinding = glGetAttribLocation(shaderProgram, "inColor");

		UInt viewportBinding = glGetUniformLocation(shaderProgram, "uViewport");

		//Enable vertex attributes
		glEnableVertexArrayAttrib(vertexArray, vertexPosBinding);
		glEnableVertexArrayAttrib(vertexArray, vertexDepthBinding);
		glEnableVertexArrayAttrib(vertexArray, vertexColBinding);

		//Create vertex attribute
		glVertexAttribPointer(vertexPosBinding, 2, GL_FLOAT, GL_FALSE, 28, (void*)0);
		glVertexAttribPointer(vertexDepthBinding, 1, GL_FLOAT, GL_FALSE, 28, (void*)8);
		glVertexAttribPointer(vertexColBinding, 4, GL_FLOAT, GL_FALSE, 28, (void*)12);

		GLBuffer vertexBuffer { buffers[0], DEFAULT_VERTEX_BUFFER_SIZE };
		GLBuffer elementBuffer { buffers[1], DEFAULT_ELEMENT_BUFFER_SIZE };

		Primitive2DVertexArray::Program program;
		program.id = shaderProgram;
		program.vertexPositionBinding = vertexPosBinding;
		program.vertexDepthBinding = vertexDepthBinding;
		program.vertexColorBinding = vertexColBinding;
		program.viewportBinding = viewportBinding;
		
		primitiveVertexArray.id = vertexArray;
		primitiveVertexArray.vertexBuffer = vertexBuffer;
		primitiveVertexArray.elementBuffer = elementBuffer;
		primitiveVertexArray.program = program;

		glBindVertexArray(0);
	}

	void DrawPrimitives2D()
	{
		std::vector<DrawRectangleComponent2D*> rectangles;
		std::vector<DrawTriangleComponent2D*> triangles;
		std::vector<DrawElipseComponent2D*> elipses;

		rectangles.reserve(DrawRectangleComponent2D::GetCount());
		triangles.reserve(DrawTriangleComponent2D::GetCount());
		elipses.reserve(DrawElipseComponent2D::GetCount());

		auto getRect = DrawRectangleComponent2D::ForEach([&rectangles](DrawRectangleComponent2D* c)
		{
			rectangles.push_back(c);
		});

		auto getTri = DrawTriangleComponent2D::ForEach([&triangles](DrawTriangleComponent2D* c)
		{
			triangles.push_back(c);
		});

		auto getElip = DrawElipseComponent2D::ForEach([&elipses](DrawElipseComponent2D* c)
		{
			elipses.push_back(c);
		});

		if (rectangles.empty() && triangles.empty() && elipses.empty()) return;

		//vec2 inPosition, vec4 inColor, float inDepth
		UInt vertexSize = (2 * 4) + (4 * 4) + (1 * 4);
		UInt elementSize = 4;

		UInt numVerticies = static_cast<UInt>(rectangles.size() * 4 + triangles.size() * 3 + elipses.size() * (vlk::ElipseResolution + 1));
		UInt numElements = static_cast<UInt>(rectangles.size() * 6 + triangles.size() * 3 + elipses.size() * vlk::ElipseResolution * 3);

		UInt vertexBufferSize = numVerticies * vertexSize;
		UInt elementBufferSize = numElements * elementSize;

		ByteBuffer vertexBuffer;
		ByteBuffer elementBuffer;

		vertexBuffer.Allocate(vertexBufferSize);
		elementBuffer.Allocate(elementBufferSize);

		UInt elementOffset = 0;

		for (UInt i = 0; i < rectangles.size(); i++)
		{
			const DrawRectangleComponent2D* r = rectangles.at(i);

			Vector2 vertpos[4]
			{
				//Bottom left
				Vector2(r->transform->location.x - (r->transform->scale.x * r->size.x), 
						r->transform->location.y - (r->transform->scale.y * r->size.y)),
												 				 	
				//Bottom right										
				Vector2(r->transform->location.x + (r->transform->scale.x * r->size.x),
						r->transform->location.y - (r->transform->scale.y * r->size.y)),
																		
				//Top right						 					
				Vector2(r->transform->location.x + (r->transform->scale.x * r->size.x),
						r->transform->location.y + (r->transform->scale.y * r->size.y)),
																		  
				//Top left												 
				Vector2(r->transform->location.x - (r->transform->scale.x * r->size.x),
						r->transform->location.y + (r->transform->scale.y * r->size.y))
			};

			for (UInt v = 0; v < 4; v++)
			{
				vertpos[v].RotateAround(r->transform->location, r->transform->rotation);
			}

			if (r->flags & VLK_SCREEN_RELATIVE_BIT)
			{
				for (UInt v = 0; v < 4; v++)
				{
					vertpos[v] += CameraComponent2D::ACTIVE->transform->location;
				}
			}

			for (UInt v = 0; v < 4; v++)
			{
				Vector2& vec = vertpos[v];

				vertexBuffer.Put<Float>(vec.x);
				vertexBuffer.Put<Float>(vec.y);
				vertexBuffer.Put<Float>(r->depth);
				vertexBuffer.Put<Float>(r->color.r);
				vertexBuffer.Put<Float>(r->color.g);
				vertexBuffer.Put<Float>(r->color.b);
				vertexBuffer.Put<Float>(r->color.a);
			}

			UInt offset = i * 4;

			elementBuffer.Put<UInt>(offset + 0 + elementOffset);
			elementBuffer.Put<UInt>(offset + 1 + elementOffset);
			elementBuffer.Put<UInt>(offset + 2 + elementOffset);
			elementBuffer.Put<UInt>(offset + 0 + elementOffset);
			elementBuffer.Put<UInt>(offset + 3 + elementOffset);
			elementBuffer.Put<UInt>(offset + 2 + elementOffset);
		}

		elementOffset += static_cast<UInt>(rectangles.size() * 4);

		for (UInt i = 0; i < triangles.size(); i++)
		{
			const DrawTriangleComponent2D* t = triangles.at(i);

			Vector2 vertpos[3]
			{
				//Point 1
				Vector2(t->transform->location.x + (t->p1.x * t->transform->scale.x),
						t->transform->location.y + (t->p1.y * t->transform->scale.y)),
												   								   
				//Point 2						   								   
				Vector2(t->transform->location.x + (t->p2.x * t->transform->scale.x),
						t->transform->location.y + (t->p2.y * t->transform->scale.y)),
												   								   
				//Point 3						   								   
				Vector2(t->transform->location.x + (t->p3.x * t->transform->scale.x),
						t->transform->location.y + (t->p2.y * t->transform->scale.y))
			};

			for (UInt v = 0; v < 3; v++)
			{
				vertpos[v].RotateAround(t->transform->location, t->transform->rotation);
			}

			if (t->flags & VLK_SCREEN_RELATIVE_BIT)
			{
				for (UInt v = 0; v < 3; v++)
				{
					vertpos[v] += CameraComponent2D::ACTIVE->transform->location;
				}
			}

			for (UInt v = 0; v < 3; v++)
			{
				Vector2& vec = vertpos[v];

				vertexBuffer.Put<Float>(vec.x);
				vertexBuffer.Put<Float>(vec.y);
				vertexBuffer.Put<Float>(t->depth);
				vertexBuffer.Put<Float>(t->color.r);
				vertexBuffer.Put<Float>(t->color.g);
				vertexBuffer.Put<Float>(t->color.b);
				vertexBuffer.Put<Float>(t->color.a);
			}

			UInt offset = i * 3;

			elementBuffer.Put<UInt>(offset + 0 + elementOffset);
			elementBuffer.Put<UInt>(offset + 1 + elementOffset);
			elementBuffer.Put<UInt>(offset + 2 + elementOffset);
		}

		elementOffset += static_cast<UInt>(triangles.size() * 3);

		for (UInt i = 0; i < elipses.size(); i++)
		{
			const DrawElipseComponent2D* e = elipses.at(i);

			//Origin
			vertexBuffer.Put<Float>(e->transform->location.x);
			vertexBuffer.Put<Float>(e->transform->location.y);
			vertexBuffer.Put<Float>(e->depth);
			vertexBuffer.Put<Float>(e->color.r);
			vertexBuffer.Put<Float>(e->color.g);
			vertexBuffer.Put<Float>(e->color.b);
			vertexBuffer.Put<Float>(e->color.a);

			for (UInt t = 0; t < vlk::ElipseResolution; t++)
			{
				Float theta = (vlk::TwoPi / vlk::ElipseResolution) * t;

				//x = cos, y = sin
				Vector2 vertPos(e->transform->location.x + e->radii.x * e->transform->scale.x * cosf(theta), 
								e->transform->location.y + e->radii.y * e->transform->scale.y * sinf(theta));

				if (e->flags & VLK_SCREEN_RELATIVE_BIT)
				{
					vertPos += CameraComponent2D::ACTIVE->transform->location;
				}

				vertPos.RotateAround(e->transform->location, e->transform->rotation);

				vertexBuffer.Put<Float>(vertPos.x);
				vertexBuffer.Put<Float>(vertPos.y);
				vertexBuffer.Put<Float>(e->depth);
				vertexBuffer.Put<Float>(e->color.r);
				vertexBuffer.Put<Float>(e->color.g);
				vertexBuffer.Put<Float>(e->color.b);
				vertexBuffer.Put<Float>(e->color.a);
			}

			UInt offset = i * (vlk::ElipseResolution + 1);

			for (UInt j = 1; j < vlk::ElipseResolution; j++)
			{
				elementBuffer.Put<UInt>(offset + elementOffset);			//origin
				elementBuffer.Put<UInt>(offset + j + 0 + elementOffset);	//This element
				elementBuffer.Put<UInt>(offset + j + 1 + elementOffset);	//next element
			}

			elementBuffer.Put<UInt>(offset + elementOffset);							//origin
			elementBuffer.Put<UInt>(offset + 1 + elementOffset);						//First element
			elementBuffer.Put<UInt>(offset + vlk::ElipseResolution + elementOffset);	//Last element
		}

		glBindVertexArray(primitiveVertexArray.id);
		glUseProgram(primitiveVertexArray.program.id);
		glBindBuffer(GL_ARRAY_BUFFER, primitiveVertexArray.vertexBuffer.id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, primitiveVertexArray.elementBuffer.id);

		if (vertexBufferSize > primitiveVertexArray.vertexBuffer.size)
		{
			glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertexBuffer.Data(), GL_STREAM_DRAW);
			primitiveVertexArray.vertexBuffer.size = vertexBufferSize;
		}
		else
		{
			glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBufferSize, vertexBuffer.Data());
		}

		if (elementBufferSize > primitiveVertexArray.elementBuffer.size)
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementBufferSize, elementBuffer.Data(), GL_STREAM_DRAW);
			primitiveVertexArray.elementBuffer.size = elementBufferSize;
		}
		else
		{
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, elementBufferSize, elementBuffer.Data());
		}

		Matrix4 viewport(CameraComponent2D::ACTIVE->GetView() * CameraComponent2D::ACTIVE->GetProjection());
		glUniformMatrix4fv(primitiveVertexArray.program.viewportBinding, 1, false, viewport.Data());
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}

	void DestroyPrimitive2DVertexArray()
	{
		UInt buffers[2] { primitiveVertexArray.vertexBuffer.id, primitiveVertexArray.elementBuffer.id };

		glDeleteBuffers(2, buffers);
		glDeleteVertexArrays(1, &primitiveVertexArray.id);
		glDeleteProgram(primitiveVertexArray.program.id);
	}

	void InitTexture2DVertexArray()
	{
		//Create vertex array
		UInt vertexArray = 0;
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		//Create vertex shader
		UInt vertShader = CreateShader(GL_VERTEX_SHADER, "shaders/texture2DShader.vert");
		CheckShaderCompile(vertShader);

		//Create fragment shader
		UInt fragShader = CreateShader(GL_FRAGMENT_SHADER, "shaders/texture2DShader.frag");
		CheckShaderCompile(fragShader);

		//Create shader program
		UInt shaderProgram = glCreateProgram();

		//Attach shaders
		glAttachShader(shaderProgram, vertShader);
		glAttachShader(shaderProgram, fragShader);

		//Link shader program
		glLinkProgram(shaderProgram);
		CheckProgramCompile(shaderProgram);

		//Detach shaders
		glDetachShader(shaderProgram, vertShader);
		glDetachShader(shaderProgram, fragShader);

		//Delete shaders
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		//Use program
		glUseProgram(shaderProgram);

		//Create buffers
		UInt buffers[2]{ 0, 0 };
		glGenBuffers(2, buffers);

		//Create vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, DEFAULT_VERTEX_BUFFER_SIZE, nullptr, GL_STREAM_DRAW);

		//Create element buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, DEFAULT_ELEMENT_BUFFER_SIZE, nullptr, GL_STREAM_DRAW);

		//Get vertex attribute bindings
		UInt vertexPosBinding = glGetAttribLocation(shaderProgram, "inPosition");
		UInt vertexDepthBinding = glGetAttribLocation(shaderProgram, "inDepth");
		UInt vertexColBinding = glGetAttribLocation(shaderProgram, "inColor");
		UInt vertexUVBinding = glGetAttribLocation(shaderProgram, "inTextureUV");

		UInt viewportBinding = glGetUniformLocation(shaderProgram, "uViewport");

		UInt textureBinding = glGetUniformLocation(shaderProgram, "uTexture");

		//Enable vertex attributes
		glEnableVertexArrayAttrib(vertexArray, vertexPosBinding);
		glEnableVertexArrayAttrib(vertexArray, vertexDepthBinding);	
		glEnableVertexArrayAttrib(vertexArray, vertexColBinding);
		glEnableVertexArrayAttrib(vertexArray, vertexUVBinding);	

		//Create vertex attribute
		glVertexAttribPointer(vertexPosBinding,	  2, GL_FLOAT, GL_FALSE, 36, reinterpret_cast<void*>(0));
		glVertexAttribPointer(vertexDepthBinding, 1, GL_FLOAT, GL_FALSE, 36, reinterpret_cast<void*>(8));
		glVertexAttribPointer(vertexColBinding,	  4, GL_FLOAT, GL_FALSE, 36, reinterpret_cast<void*>(12));
		glVertexAttribPointer(vertexUVBinding,	  2, GL_FLOAT, GL_FALSE, 36, reinterpret_cast<void*>(28));

		GLBuffer vertexBuffer;
		vertexBuffer.id = buffers[0];
		vertexBuffer.size = DEFAULT_VERTEX_BUFFER_SIZE;

		GLBuffer elementBuffer;
		elementBuffer.id = buffers[1];
		elementBuffer.size = DEFAULT_ELEMENT_BUFFER_SIZE;

		Texture2DVertexArray::Texture2DProgram program;
		program.id = shaderProgram;
		program.vertexPositionBinding = vertexPosBinding;
		program.vertexColorBinding = vertexColBinding;
		program.vertexUVBinding = vertexUVBinding;
		program.vertexDepthBinding = vertexDepthBinding;
		program.textureBinding = textureBinding;
		program.viewportBinding = viewportBinding;
		
		texture2DVertexArray.id = vertexArray;
		texture2DVertexArray.vertexBuffer = vertexBuffer;
		texture2DVertexArray.elementBuffer = elementBuffer;
		texture2DVertexArray.program = program;

		glBindVertexArray(0);
	}

	void DrawTextures2D()
	{
		std::vector<DrawTextureComponent2D*> textures;

		textures.reserve(DrawTextureComponent2D::GetCount());

		auto getTex = DrawTextureComponent2D::ForEach([&textures](DrawTextureComponent2D* c)
		{
			textures.push_back(c);
		});

		if (textures.empty()) return;

		//Send translucent draws to end
		auto translucentPivot = std::stable_partition(textures.begin(), textures.end(), [](DrawTextureComponent2D* a)
		{
			return !(a->flags & VLK_TRANSLUCENT_BIT);
		});

		struct
		{
			Boolean operator()(DrawTextureComponent2D* a, DrawTextureComponent2D* b)
			{
				return (a->texture->atlas > b->texture->atlas);
			}
		} sorter;

		std::sort(textures.begin(), translucentPivot, sorter);
		std::sort(translucentPivot, textures.end(), sorter);

		UInt vertexSize = (2 * 4) + (2 * 4) + (4 * 4) + (1 * 4);
		UInt elementSize = 4;

		UInt numVerticies = (UInt)textures.size() * 4;
		UInt numElements = (UInt)textures.size() * 6;

		UInt vertexBufferSize = numVerticies * vertexSize;
		UInt elementBufferSize = numElements * elementSize;

		ByteBuffer vertexBuffer;
		ByteBuffer elementBuffer;

		vertexBuffer.Allocate(vertexBufferSize);
		elementBuffer.Allocate(elementBufferSize);

		Vector2 zoomOffset = Vector2((1.0f / CameraComponent2D::ACTIVE->zoom.x) / 2.0f,
									 (1.0f / CameraComponent2D::ACTIVE->zoom.y) / 2.0f);

		for (UInt i = 0; i < textures.size(); i++)
		{
			const DrawTextureComponent2D* t = textures.at(i);

			//Y-Coord is flipped when going from image space to world space, so flip the orientation of the y-origin
			Vector2 texSizePos((t->texture->origin.x) * t->transform->scale.x * t->texture->atlas->Width(),
							   (t->texture->size.y - t->texture->origin.y) * t->transform->scale.y * t->texture->atlas->Height());

			Vector2 texSizeNeg((t->texture->size.x - t->texture->origin.x) * t->transform->scale.x * t->texture->atlas->Width(),
							   (t->texture->origin.y) * t->transform->scale.y * t->texture->atlas->Height());

			//Textures can have off-centered origins, so adjust it.
			//Vector2 originAdj(t->transform->location.x - (t->texture->size.x * t->transform->scale.x) * (0.5f - t->texture->origin.x),
			//				  t->transform->location.y + (t->texture->size.y * t->transform->scale.y) * (0.5f - t->texture->origin.y));

			Vector2 vertpos[4]
			{
				//Bottom left
				Vector2(t->transform->location.x - texSizePos.x, 
						t->transform->location.y - texSizePos.y),
												 				
				//Bottom right					 				
				Vector2(t->transform->location.x + texSizeNeg.x,
						t->transform->location.y - texSizePos.y),
												 				
				//Top right										
				Vector2(t->transform->location.x + texSizeNeg.x,
						t->transform->location.y + texSizeNeg.y),
												 				
				//Top left						 				
				Vector2(t->transform->location.x - texSizePos.x,
						t->transform->location.y + texSizeNeg.y)
			};

			//Vector2 diff(0.5f / t->texture->atlas->Width(), -0.5f / t->texture->atlas->Height());

			Vector2 texpos[4]
			{
				//Bottom left
				t->texture->bottomLeft,

				//Bottom right
				t->texture->bottomRight,
												   
				//Top right						 
				t->texture->topRight,
												
				//Top left						 
				t->texture->topLeft
			};

			if (t->transform->rotation != 0.0f)
			{
				for (UInt v = 0; v < 4; v++)
				{
					vertpos[v].RotateAround(t->transform->location, t->transform->rotation);
				}
			}

			if (t->flags & VLK_SCREEN_RELATIVE_BIT)
			{
				for (UInt v = 0; v < 4; v++)
				{
					vertpos[v] *= CameraComponent2D::ACTIVE->zoom;
					vertpos[v] += (CameraComponent2D::ACTIVE->transform->location);
				}
			}

			for (UInt v = 0; v < 4; v++)
			{
				Vector2& vec = vertpos[v];
				Vector2& uv = texpos[v];
				
				//Pixels are measured at their center, so add 0.25f to each vertex to avoid pixel gaps
				//I've tried with other values and for whatever reason this is the only one that works
				//https://community.arm.com/developer/tools-software/graphics/f/discussions/7323/simple-opengl-texture-glitches-and-pixel-perfect-questions

				vertexBuffer.Put<Float>(vec.x);
				vertexBuffer.Put<Float>(vec.y);
				vertexBuffer.Put<Float>(t->depth);
				vertexBuffer.Put<Float>(t->color.r);
				vertexBuffer.Put<Float>(t->color.g);
				vertexBuffer.Put<Float>(t->color.b);
				vertexBuffer.Put<Float>(t->color.a);
				vertexBuffer.Put<Float>(uv.x);
				vertexBuffer.Put<Float>(uv.y);
			}

			elementBuffer.Put<UInt>((i * 4) + 0);
			elementBuffer.Put<UInt>((i * 4) + 1);
			elementBuffer.Put<UInt>((i * 4) + 2);
			elementBuffer.Put<UInt>((i * 4) + 0);
			elementBuffer.Put<UInt>((i * 4) + 3);
			elementBuffer.Put<UInt>((i * 4) + 2);
		}

		glBindVertexArray(texture2DVertexArray.id);
		glUseProgram(texture2DVertexArray.program.id);
		glBindBuffer(GL_ARRAY_BUFFER, texture2DVertexArray.vertexBuffer.id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texture2DVertexArray.elementBuffer.id);

		if (vertexBufferSize > texture2DVertexArray.vertexBuffer.size)
		{
			glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertexBuffer.Data(), GL_STREAM_DRAW);
			texture2DVertexArray.vertexBuffer.size = vertexBufferSize;
		}
		else
		{
			glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBufferSize, vertexBuffer.Data());
		}

		if (elementBufferSize > texture2DVertexArray.elementBuffer.size)
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementBufferSize, elementBuffer.Data(), GL_STREAM_DRAW);
			texture2DVertexArray.elementBuffer.size = elementBufferSize;
		}
		else
		{
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, elementBufferSize, elementBuffer.Data());
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Matrix4 viewport(CameraComponent2D::ACTIVE->GetView() * CameraComponent2D::ACTIVE->GetProjection());
		glUniformMatrix4fv(texture2DVertexArray.program.viewportBinding, 1, false, viewport.Data());

		glUniform1i(texture2DVertexArray.program.textureBinding, 0);

		ULong startPos = 0;
		ULong endPos = 0;

		while (true)
		{
			if (endPos < textures.size())
			{
				const TextureAtlas* atl = textures.at(startPos)->texture->atlas;

				if (atl != textures.at(endPos)->texture->atlas)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, textureMap[atl]);

					glDrawElements(GL_TRIANGLES, static_cast<UInt>((endPos - startPos) * 6), GL_UNSIGNED_INT, reinterpret_cast<void*>(startPos * 6 * elementSize));
					
					startPos = endPos;
					endPos++;
				}
				else
				{
					endPos++;
				}
			}
			else
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureMap[textures.at(startPos)->texture->atlas]);

				glDrawElements(GL_TRIANGLES, static_cast<UInt>((endPos - startPos) * 6), GL_UNSIGNED_INT, reinterpret_cast<void*>(startPos * 6 * elementSize));
				break;
			}
		}

		glBindVertexArray(0);
	}

	void DrawStrings()
	{
		std::vector<DrawStringComponent2D*> strings;

		strings.reserve(DrawStringComponent2D::GetCount());
		UInt charCount = 0;

		auto getTex = DrawStringComponent2D::ForEach([&strings, &charCount](DrawStringComponent2D* c)
		{
			strings.push_back(c);

			#ifdef _DEBUG
			if (!utf8::is_valid(c->text.begin(), c->text.end()))
			{
				LogError("GLRenderer", "UTF-8 text is not valid!");
				throw std::exception("Invalid UTF-8 text detected.");
			}
			#endif

			charCount += static_cast<UInt>(utf8::distance(c->text.begin(), c->text.end()));
		});

		if (strings.empty()) return;

		//Send translucent draws to end
		auto translucentPivot = std::stable_partition(strings.begin(), strings.end(), [](DrawStringComponent2D* a)
		{
			return !(a->flags & VLK_TRANSLUCENT_BIT);
		});

		struct
		{
			Boolean operator()(DrawStringComponent2D* a, DrawStringComponent2D* b)
			{
				return (a->font > b->font);
			}
		} sorter;

		std::sort(strings.begin(), translucentPivot, sorter);
		std::sort(translucentPivot, strings.end(), sorter);

		UInt vertexSize = (2 * 4) + (2 * 4) + (4 * 4) + (1 * 4);
		UInt elementSize = 4;

		UInt numVerticies = 4 * charCount;
		UInt numElements = 6 * charCount;

		UInt vertexBufferSize = numVerticies * vertexSize;
		UInt elementBufferSize = numElements * elementSize;

		ByteBuffer vertexBuffer;
		ByteBuffer elementBuffer;

		vertexBuffer.Allocate(vertexBufferSize);
		elementBuffer.Allocate(elementBufferSize);

		UInt charOffset = 0;

		for (auto it = strings.begin(); it != strings.end(); it++)
		{
			const DrawStringComponent2D* str = *it;
			Float stringOffset = 0.0f;
			
			auto s = str->text.begin();
			while (s != str->text.end())
			{
				#ifdef _DEBUG
				UInt codepoint = utf8::next(s, str->text.end());
				#else
				UInt codepoint = utf8::unchecked::next(s);
				#endif

				const Texture2D* c = str->font->GetChar(codepoint);

				//Y-Coord is flipped when going from image space to world space, so flip the orientation of the y-origin
				Vector2 texSizePos((c->origin.x)             * str->transform->scale.x * c->atlas->Width(),
					               (c->size.y - c->origin.y) * str->transform->scale.y * c->atlas->Height());

				Vector2 texSizeNeg((c->size.x - c->origin.x) * str->transform->scale.x * c->atlas->Width(),
					               (c->origin.y)             * str->transform->scale.y * c->atlas->Height());

				//Textures can have off-centered origins, so adjust it.
				//Vector2 originAdj(t->transform->location.x - (t->texture->size.x * t->transform->scale.x) * (0.5f - t->texture->origin.x),
				//				  t->transform->location.y + (t->texture->size.y * t->transform->scale.y) * (0.5f - t->texture->origin.y));

				Vector2 vertpos[4]
				{
					//Bottom left
					Vector2(str->transform->location.x - texSizePos.x + stringOffset, 
							str->transform->location.y - texSizePos.y),

					//Bottom right					 
					Vector2(str->transform->location.x + texSizeNeg.x + stringOffset,
							str->transform->location.y - texSizePos.y),

					//Top right						
					Vector2(str->transform->location.x + texSizeNeg.x + stringOffset,
							str->transform->location.y + texSizeNeg.y),

					//Top left						 
					Vector2(str->transform->location.x - texSizePos.x + stringOffset,
							str->transform->location.y + texSizeNeg.y)
				};

				Vector2 texpos[4]
				{
					//Bottom left
					c->bottomLeft,

					//Bottom right
					c->bottomRight,

					//Top right						 
					c->topRight,

					//Top left						 
					c->topLeft
				};

				if (str->transform->rotation != 0.0f)
				{
					for (UInt v = 0; v < 4; v++)
					{
						vertpos[v].RotateAround(str->transform->location, str->transform->rotation);
					}
				}


				if (str->flags & VLK_SCREEN_RELATIVE_BIT)
				{
					for (UInt v = 0; v < 4; v++)
					{
						vertpos[v] *= CameraComponent2D::ACTIVE->zoom;
						vertpos[v] += (CameraComponent2D::ACTIVE->transform->location);
					}
				}

				for (UInt v = 0; v < 4; v++)
				{
					Vector2& vec = vertpos[v];
					Vector2& uv = texpos[v];

					//Pixels are measured at their center, so add 0.25f to each vertex to avoid pixel gaps
					//I've tried with other values and for whatever reason this is the only one that works
					//https://community.arm.com/developer/tools-software/graphics/f/discussions/7323/simple-opengl-texture-glitches-and-pixel-perfect-questions

					vertexBuffer.Put<Float>(vec.x);
					vertexBuffer.Put<Float>(vec.y);
					vertexBuffer.Put<Float>(str->depth);
					vertexBuffer.Put<Float>(str->color.r);
					vertexBuffer.Put<Float>(str->color.g);
					vertexBuffer.Put<Float>(str->color.b);
					vertexBuffer.Put<Float>(str->color.a);
					vertexBuffer.Put<Float>(uv.x);
					vertexBuffer.Put<Float>(uv.y);
				}

				elementBuffer.Put<UInt>((4 * charOffset) + 0);
				elementBuffer.Put<UInt>((4 * charOffset) + 1);
				elementBuffer.Put<UInt>((4 * charOffset) + 2);
				elementBuffer.Put<UInt>((4 * charOffset) + 0);
				elementBuffer.Put<UInt>((4 * charOffset) + 3);
				elementBuffer.Put<UInt>((4 * charOffset) + 2);

				stringOffset += (c->size.x * c->atlas->Width() + 1.0f) * str->transform->scale.x;
				charOffset++;
			}
		}

		glBindVertexArray(texture2DVertexArray.id);
		glUseProgram(texture2DVertexArray.program.id);
		glBindBuffer(GL_ARRAY_BUFFER, texture2DVertexArray.vertexBuffer.id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, texture2DVertexArray.elementBuffer.id);

		if (vertexBufferSize > texture2DVertexArray.vertexBuffer.size)
		{
			glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertexBuffer.Data(), GL_STREAM_DRAW);
			texture2DVertexArray.vertexBuffer.size = vertexBufferSize;
		}
		else
		{
			glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBufferSize, vertexBuffer.Data());
		}

		if (elementBufferSize > texture2DVertexArray.elementBuffer.size)
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementBufferSize, elementBuffer.Data(), GL_STREAM_DRAW);
			texture2DVertexArray.elementBuffer.size = elementBufferSize;
		}
		else
		{
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, elementBufferSize, elementBuffer.Data());
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Matrix4 viewport(CameraComponent2D::ACTIVE->GetView() * CameraComponent2D::ACTIVE->GetProjection());
		glUniformMatrix4fv(texture2DVertexArray.program.viewportBinding, 1, false, viewport.Data());

		glUniform1i(texture2DVertexArray.program.textureBinding, 0);

		ULong startPos = 0;
		ULong endPos = 0;
		UInt charDraw = 0;

		while (true)
		{
			if (endPos < strings.size())
			{
				const DrawStringComponent2D* ds = strings.at(endPos);
				charDraw += static_cast<UInt>(utf8::distance(ds->text.begin(), ds->text.end()));

				if (ds->font != strings.at(endPos)->font)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, textureMap[dynamic_cast<const TextureAtlas*>(ds->font)]);

					glDrawElements(GL_TRIANGLES, static_cast<UInt>(6 * charDraw), GL_UNSIGNED_INT, reinterpret_cast<void*>(startPos * 6 * elementSize));

					startPos = endPos;
					endPos++;
					charDraw = 0;
				}
				else
				{
					endPos++;
				}
			}
			else
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureMap[strings.at(startPos)->font]);

				glDrawElements(GL_TRIANGLES, static_cast<UInt>(6 * charDraw), GL_UNSIGNED_INT, reinterpret_cast<void*>(startPos * 6 * elementSize));
				break;
			}
		}

		glBindVertexArray(0);
	}

	void DestroyTexture2DVertexArray()
	{
		UInt buffers[2] { texture2DVertexArray.vertexBuffer.id, texture2DVertexArray.elementBuffer.id };

		glDeleteBuffers(2, buffers);
		glDeleteVertexArrays(1, &texture2DVertexArray.id);
		glDeleteProgram(texture2DVertexArray.program.id);
	}
}

void GLRenderer::Init()
{
	glewInit();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	InitPrimitive2DVertexArray();
	InitTexture2DVertexArray();
	
	EventBus<vlk::WindowFramebufferEvent>::Get().AddEventListener(OnResize);
	EventBus<vlk::TextureAtlasLoadedEvent>::Get().AddEventListener(OnTextureAtlasLoad);
	EventBus<vlk::TextureAtlasUnloadedEvent>::Get().AddEventListener(OnTextureAtlasUnload);
}

void GLRenderer::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	DrawTextures2D();
	DrawStrings();

	DrawPrimitives2D();
}

void GLRenderer::Destroy()
{
	DestroyPrimitive2DVertexArray();
	DestroyTexture2DVertexArray();

	EventBus<vlk::WindowFramebufferEvent>::Get().RemoveEventListener(OnResize);
}*/