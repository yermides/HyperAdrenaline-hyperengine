#include "r_shader.hpp"

namespace hyper {

RShader::RShader() 
: Resource{}
{
}

RShader::RShader(std::string const& path)
: Resource{}
{
	loadFromFile(path);
}

RShader::RShader(Cstring path_vertex, Cstring path_fragment, Cstring path_geometry)
: Resource{}
{
	loadShadersFromMemory(path_vertex, path_fragment, path_geometry);
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
	// INFOLOG("Compiling shader: " << path_vertex);
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
	// INFOLOG("Compiling shader: " << path_fragment);
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
	// INFOLOG("Linking program");
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

ProgramIdentifier 
RShader::loadShadersFromMemory( Cstring path_vertex, Cstring path_fragment, Cstring path_geometry )
{
	int success;
	char info_log[512];
	uint32_t
		program = glCreateProgram()
	,	vShader {0} 
	,	fShader {0} 
	,	gShader {0}
	;

	if(path_vertex)
		vShader = glCreateShader(GL_VERTEX_SHADER);

	if(path_fragment)
		fShader = glCreateShader(GL_FRAGMENT_SHADER);

	if(path_geometry)
		gShader = glCreateShader(GL_GEOMETRY_SHADER);

	// Compilar vertex shader

	if(vShader)
	{
		glShaderSource(vShader, 1, &path_vertex, 0);
		glCompileShader(vShader);
		glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vShader, 512, 0, info_log);
			ERRLOG( "vertex shader compilation failed!\n" << info_log );
		}
	}

	// Compilar fragment shader

	if(fShader)
	{
		glShaderSource(fShader, 1, &path_fragment, 0);
		glCompileShader(fShader);
		glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fShader, 512, 0, info_log);
			ERRLOG( "fragment shader compilation failed!\n" << info_log );
		}
	}

	// Compilar geometry shader

	if(gShader)
	{
		glShaderSource(gShader, 1, &path_geometry, 0);
		glCompileShader(gShader);
		glGetShaderiv(gShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(gShader, 512, 0, info_log);
			ERRLOG( "fragment shader compilation failed!\n" << info_log );
		}
	}

	// Creando el programa a partir de los dos o tres programas de shader
	if(vShader)
		glAttachShader(program, vShader);

	if(fShader)
		glAttachShader(program, fShader);

	if(gShader) 
		glAttachShader(program, gShader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, 0, info_log);
		ERRLOG( "program linking failed!\n" << info_log );
	}

	// Cleanup de los programas del shader
	if(vShader)
	{
		glDetachShader(program, vShader);
		glDeleteShader(vShader);
	}
	if(fShader)
	{
		glDetachShader(program, fShader);
		glDeleteShader(fShader);
	}
	if(gShader)
	{
		glDetachShader(program, gShader);
		glDeleteShader(gShader);
	}

	//  Recoger el id del programa
	m_programID = program;

	return program;
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