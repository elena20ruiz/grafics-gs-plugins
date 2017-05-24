#include "shadowmap.h"

const int SHADOW_MAP_WIDTH = 1024;
const int SHADOW_MAP_HEIGHT = 1024;

void ShadowMap::onPluginLoad()
{
    // Resize to power-of-two viewport
    glwidget()->resize(SHADOW_MAP_WIDTH,SHADOW_MAP_HEIGHT);

    // Carregar shader, compile & link 
    vs = new QGLShader(QGLShader::Vertex, this);
    vs->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/shadowmap/shadowmap.vert");

    fs = new QGLShader(QGLShader::Fragment, this);
    fs->compileSourceFile("/home/llop/Llop/FIB/2015-2016QT/G/Lab/NewViewer/plugins/shadowmap/shadowmap.frag");

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
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    // Camera
    lightCamera = *camera();
}

void ShadowMap::keyPressEvent(QKeyEvent*e)
{
    if (e->key() == Qt::Key_C) lightCamera = *camera();

    glwidget()->updateGL();
}

// TODO: update VBO coords after creation...
void drawQuad(const Point& p0, const Point& p1, const Point& p2, const Point& p3)
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
        float coords[] = { p0.x(), p0.y(), p0.z(), p1.x(), p1.y(), p1.z(), p3.x(), p3.y(), p3.z(), p2.x(), p2.y(), p2.z() };

        GLuint VBO_coords;
        glGenBuffers(1, &VBO_coords);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_coords);
        glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        //glBindVertexArray(0);

        // Create VBO with (x,y,z) coordinates
        float normals[] = { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0};

        GLuint VBO_normals;
        glGenBuffers(1, &VBO_normals);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
        glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
    }

    // 2. Draw
    glBindVertexArray (VAO_rect);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}



bool ShadowMap::paintGL()
{
    // Pass 1. Build depth map
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    QMatrix4x4 CMVP = lightCamera.projectionMatrix() * lightCamera.modelviewMatrix();
    glwidget()->defaultProgram()->setUniformValue("modelViewProjectionMatrix", CMVP); 
    
    // Draw scene (with z offset)
    glPolygonOffset(1,1);
    glEnable(GL_POLYGON_OFFSET_FILL);
    drawPlugin()->drawScene();
    glDisable(GL_POLYGON_OFFSET_FILL);

    //return true; // show shadow map

    // Get texture
    glBindTexture(GL_TEXTURE_2D, textureId);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);

    // Pass 2. Draw scene using shadowmap
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    program->bind();
    program->setUniformValue("lightViewMatrix", lightCamera.modelviewMatrix());
    program->setUniformValue("lightProjectionMatrix", lightCamera.projectionMatrix());
    QMatrix4x4 biasMatrix;
    biasMatrix.translate(0.5, 0.5, 0.5);
    biasMatrix.scale(0.5, 0.5, 0.5);
    program->setUniformValue("biasMatrix", biasMatrix);
    program->setUniformValue("shadowMap", 0); 
    program->setUniformValue("lightPos", lightCamera.getObs());
    QMatrix4x4 MVP = camera()->projectionMatrix() * camera()->modelviewMatrix();
    program->setUniformValue("modelViewProjectionMatrix", MVP); 
    program->setUniformValue("modelViewMatrix", camera()->modelviewMatrix());
    program->setUniformValue("normalMatrix", camera()->modelviewMatrix().normalMatrix());

    // draw scene + floor 
    drawPlugin()->drawScene();


    Point min = scene()->boundingBox().min();
    float r = scene()->boundingBox().radius();
    drawQuad(Point(-r,min.y(), -r), Point( r,min.y(), -r), Point( r,min.y(),  r), Point(-r,min.y(),  r));

    program->release();
    glBindTexture(GL_TEXTURE_2D, 0);

    // Help
    //glColor3f(0.0, 0.0, 0.0);
	//int x = 5;
	//int y = 15;
	//glwidget()->renderText(x,y, QString("[Shadowmap plugin] C - Copy camera position to light position"));
    cout << "[Shadowmap plugin] C - Copy camera position to light position" << endl;


    return true;
}

Q_EXPORT_PLUGIN2(shadowmap, ShadowMap)   // plugin name, plugin class
