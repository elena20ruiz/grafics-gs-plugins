#include "glowing.h"

const int IMAGE_WIDTH = 1024;
const int IMAGE_HEIGHT = IMAGE_WIDTH;

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << endl;

int printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
			     file, line, gluErrorString(glErr), func);
        retCode = 1;
    }
    return retCode;
}

void Glowing::onPluginLoad()
{
    // Resize to power-of-two viewport
    glwidget()->resize(IMAGE_WIDTH,IMAGE_HEIGHT);

    // Carregar shader, compile & link 
    vs = new QGLShader(QGLShader::Vertex, this);
    vs->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/glowing/glowing.vert");

    fs = new QGLShader(QGLShader::Fragment, this);
    fs->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/glowing/glowing.frag");

    program = new QGLShaderProgram(this);
    program->addShader(vs);
    program->addShader(fs);
    program->link();

    // Setup texture
	glActiveTexture(GL_TEXTURE0);
	glGenTextures( 1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, IMAGE_WIDTH, IMAGE_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void drawRect()
{
    static bool created = false;
    static GLuint VAO_rect;

    // 1. Create VBO Buffers
    if (!created)
    {
        created = true;
        

        // Create & bind empty VAO
        glGenVertexArrays(1, &VAO_rect);
        glBindVertexArray(VAO_rect);

        // Create VBO with (x,y,z) coordinates
        float coords[] = { -1, -1, 0, 
                            1, -1, 0, 
                           -1,  1, 0, 
                            1,  1, 0};

        GLuint VBO_coords;
        glGenBuffers(1, &VBO_coords);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_coords);
        glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    // 2. Draw
    glBindVertexArray (VAO_rect);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}



bool Glowing::paintGL()
{
    // Pass 1. Draw scene
    glClearColor(0,0,0,0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    drawPlugin()->drawScene();

    // Get texture
    glBindTexture(GL_TEXTURE_2D, textureId);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Pass 2. Draw quad using texture
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    program->bind();
    program->setUniformValue("colorMap", 0);
    program->setUniformValue("SIZE", float(IMAGE_WIDTH));  
 
// quad covering viewport 
    program->setUniformValue("modelViewProjectionMatrix", QMatrix4x4() );  

    drawRect();
    program->release();
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

Q_EXPORT_PLUGIN2(glowing, Glowing)   // plugin name, plugin class
