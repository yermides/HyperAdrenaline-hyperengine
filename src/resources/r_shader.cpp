#include "r_shader.hpp"

namespace hyper {

RShader::RShader() 
: Resource{}
{
}

RShader::RShader(std::string const& path)
: Resource{}
{
	this->loadFromFile(path);
}

RShader::~RShader()
{
	glDeleteProgram(m_programID);
}

ProgramIdentifier 
RShader::loadShaders( Cstring path_vertex, Cstring path_fragment )
{
	// Guardar los valores de las rutas en nuestra clase, por si acaso
	m_vertexPath = path_vertex;
	m_fragmentPath = path_fragment;

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(path_vertex, std::ios::in);

	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	} else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", path_vertex);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(path_fragment, std::ios::in);

	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	LOG("Compiling shader: " << path_vertex);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
		VertexShaderErrorMessage.clear();
	}

	// Compile Fragment Shader
	LOG("Compiling shader: " << path_fragment);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
		FragmentShaderErrorMessage.clear();
	}

	// Link the program
	LOG("Linking program");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
		ProgramErrorMessage.clear();
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	// Store program ID and return it
	this->m_programID = ProgramID;
	return ProgramID;
}

void 
RShader::loadFromFile( std::string const& path )
{
	constexpr static Cstring extensionVertex 	{ ".vs" };
	constexpr static Cstring extensionFragment 	{ ".fs" };
	// constexpr static Cstring extensionGeometry 	{ ".gs" };

	// Como necesita dos paths, el método asume mismo nombre
    this->setName(path);
	m_vertexPath 	= path + extensionVertex; 
	m_fragmentPath 	= path + extensionFragment; 
	// TODO:: incluir el geometry opcional

	this->loadShaders(m_vertexPath.c_str(), m_fragmentPath.c_str());
}

}