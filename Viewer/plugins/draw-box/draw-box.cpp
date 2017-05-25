#include "draw-box.h"
#include "glwidget.h"
#include <cstdlib>

// Documentation: http://www.cs.upc.edu/~virtual/Gdocs/interfaces/html/class_basic_plugin.html


void DrawBox::cleanUp()
{
  GLWidget &g = *glwidget();
  for(unsigned int i=0; i<coordBuffers.size(); i++) // for each buffer (that is, for each object)
    {
      g.glDeleteBuffers(coordBuffers.size(),  &coordBuffers[i]);
      g.glDeleteVertexArrays(VAOs.size(), &VAOs[i]);
    }
    coordBuffers.clear();
    VAOs.clear();

    numIndices.clear();
}

bool DrawBox::drawScene()
{
    Scene* scn = scene();
    // per cada objecte
    for (unsigned int i=0; i<scn->objects().size(); ++i)
    {
        const Object& obj = scn->objects()[i];
        // per cada cara
        for(unsigned int c=0; c<obj.faces().size(); c++)
        {
            const Face& face = obj.faces()[c];
            glBegin (GL_POLYGON);
            glNormal3f(face.normal().x(), face.normal().y(), face.normal().z());
            // per cada vertex
            for(int v=0; v<face.numVertices(); v++)
            {
                const Point& p = obj.vertices()[face.vertexIndex(v)].coord();
                glVertex3f(p.x(), p.y(), p.z());
            }
            glEnd();
        }
    }
    return true;
}

void DrawBox::drawBoxes(){
    GLWidget &g = *glwidget();
    g.makeCurrent();
    //static int counter = 0;
    cout << "drawScene " << scene()->objects().size() << endl;
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);
    QMatrix4x4 MVP = camera()->projectionMatrix()*camera()->viewMatrix();
    program->setUniformValue("modelViewProjectionMatrix", MVP);

    for (int i = 0; i < scene()->objects().size(); ++i) {
      g.glBindVertexArray(VAOs[i]);
      g.glDrawArrays(GL_TRIANGLE_STRIP, 0, 18);
      g.glBindVertexArray(0);
    }
}

void DrawBox::generarEscena(){

    //1. Crear variables: Generar VIA TRIANGLE_STRIP// (x,y,z)    Final size: 3*number of corners
    GLfloat vertices[]={
            1, 1, 1,
            0, 1, 1,
            1, 0, 1,
            0, 0, 1,
            1, 0, 0,
            0, 0, 0,
            1, 1, 0,
            0, 1, 0,
            1, 1, 1,
            0, 1, 1,
            0, 1, 1,
            0, 1, 0,
            0, 0, 1,
            0, 0, 0,
            1, 0, 1,
            1, 0, 0,
            1, 1, 1,
            1, 1, 0
        };
        GLfloat colors[]={
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0
        }; //SERA NUESTRO BUFFER DE INFORMACIÓN

    GLWidget &g = *glwidget();

    //2. Crear VAOs & buffers vacios-------------------------------------------------------------------------
    GLuint VAO;
    g.glGenVertexArrays(1, &VAO);
    VAOs.push_back(VAO);
    g.glBindVertexArray(VAO);

    GLuint coordBufferID;
    g.glGenBuffers(1, &coordBufferID);
    coordBuffers.push_back(coordBufferID);

    GLuint colorBufferID;
    g.glGenBuffers(1, &colorBufferID);
    colorBuffers.push_back(colorBufferID);

    numIndices.push_back(sizeof(vertices));
    //------------------------------------------------------------------------------------------------------

    //3. Define VBO-----------------------------------------------------------------------------------------

    g.glBindBuffer(GL_ARRAY_BUFFER, coordBuffers[0]);
    g.glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
    g.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);  // VAO
    g.glEnableVertexAttribArray(0);

    g.glBindBuffer(GL_ARRAY_BUFFER, colorBuffers[0]);
    g.glBufferData(GL_ARRAY_BUFFER, sizeof(colors), &colors[0], GL_STATIC_DRAW);
    g.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);  // VAO
    g.glEnableVertexAttribArray(1);


    //Desconect
    g.glBindVertexArray(0);
    g.glBindBuffer(GL_ARRAY_BUFFER,0);
    g.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    //------------------------------------------------------------------------------------------------------
}

//1. Generar VAO i buffers per a 6 cares
//2.Carregar VS+FS per pintar la capsa amb uniform
void DrawBox::onPluginLoad()
{

    generarEscena();

    //----------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------

    glwidget()->makeCurrent();
    program = new QOpenGLShaderProgram(this);


    //VERTEX SHADER:
    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile("plugins/draw-box/draw-box.vert");
    program->addShader(vs);

    //FRAMENT SHADER:
    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile("plugins/draw-box/draw-box.frag");
    program->addShader(fs);
    program->link();
    qDebug() << program->log() << endl;

    // Timers
    connect(&timer, SIGNAL(timeout()), glwidget(), SLOT(updateGL()));
    timer.start();
    elapsedTimer.start();
}


//1. Recorrer tots els objectes de l'escena (com draw-immediate
//2. Pintar la capsa amb el VAO i els shaders d'abans
void DrawBox::postFrame()
{
    program->bind();
    drawBoxes();
    program->release();
    
}


#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(draw-box, DrawBox)   // plugin name, plugin class
#endif
