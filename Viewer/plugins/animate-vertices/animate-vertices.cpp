#include "animate-vertices.h"
#include <cstdlib>

// Documentation: http://www.cs.upc.edu/~virtual/Gdocs/interfaces/html/class_basic_plugin.html

void AnimateVertices::onPluginLoad()
{

    glwidget()->makeCurrent();
    program = new QOpenGLShaderProgram(this);


    //VERTEX SHADER:
    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile("plugins/animate-vertices/animate-vertices.vert");
    program->addShader(vs);

    //FRAMENT SHADER:
    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile("plugins/animate-vertices/animate-vertices.frag");
    program->addShader(fs);
    program->link();
    qDebug() << program->log() << endl;

    // Timers
    elapsedTimer.start();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), glwidget(), SLOT(update()));
    timer->start();
}


void AnimateVertices::preFrame() //Enllaç shaders + carregar uniforms
{
    program->bind();

    //UNIFORM FLOAT
    float f = 0.1;
    program->setUniformValue("amplitude", f);

    //UNIFORM FLOAT
    f = 1;
    program->setUniformValue("freq", f);

    //PROJECTION VIEW MATRIX
    QMatrix4x4 MVP = camera()->projectionMatrix() * camera()->viewMatrix();
    program->setUniformValue("modelViewProjectionMatrix", MVP);

    //NORMAL MATRIX
    QMatrix3x3 NM = camera()->viewMatrix().normalMatrix();
    program->setUniformValue("normalMatrix", NM);

    //TIME
    program->setUniformValue("time", float(elapsedTimer.elapsed()/1000.0f));

}

void AnimateVertices::postFrame()
{
    program->release();
    
}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(animate-vertices, AnimateVertices)   // plugin name, plugin class
#endif
