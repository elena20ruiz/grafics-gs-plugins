#include "light.h"
#include <cstdlib>

void Light::onPluginLoad() {
    // Carregar shader, compile & link
    QOpenGLShader* vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile("plugins/light/light.vert");
    cout << "VS log:" << vs->log().toStdString() << endl;

    QOpenGLShader* fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile("plugins/light/light.frag");
    cout << "FS log:" << fs->log().toStdString() << endl;

    program = new QOpenGLShaderProgram(this);
    program->addShader(vs);
    program->addShader(fs);
    program->link();
    cout << "Link log:" << program->log().toStdString() << endl;

    lightAmbient = Vector(0.1,0.1,0.1);
    lightDiffuse = Vector(1,1,1);
    lightSpecular = Vector(1,1,1);

    lightPosition = QVector4D(0,0,0,1);
    lightEncodedInEyeSpace = true;

    materialAmbient = Vector(0.8, 0.8, 0.6);
    materialDiffuse = Vector(0.8, 0.8, 0.6);
    materialSpecular = Vector(1.0, 1.0, 1.0);
    materialShininess = 64.0;
}

void Light::preFrame() {
    // bind shader and define uniforms
    program->bind();
    program->setUniformValue("modelViewProjectionMatrix",camera()->projectionMatrix()*camera()->viewMatrix());
    program->setUniformValue("normalMatrix", camera()->viewMatrix().normalMatrix());
    program->setUniformValue("viewMatrix", camera()->viewMatrix());

    program->setUniformValue("matAmbient", QVector4D(materialAmbient,1));
    program->setUniformValue("matDiffuse", QVector4D(materialDiffuse,1));
    program->setUniformValue("matSpecular", QVector4D(materialSpecular,1));
    program->setUniformValue("matShininess", materialShininess);

    program->setUniformValue("lightAmbient", QVector4D(lightAmbient,1));
    program->setUniformValue("lightDiffuse", QVector4D(lightDiffuse,1));
    program->setUniformValue("lightSpecular", QVector4D(lightSpecular,1));

    if (lightEncodedInEyeSpace)
        program->setUniformValue("lightPosition", lightPosition);
    else
        program->setUniformValue("lightPosition", camera()->viewMatrix().map(lightPosition));

}

void Light::postFrame() {
    program->release();
}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(light, Light)   // plugin name, plugin class
#endif

