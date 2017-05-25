#include "reflection.h"
#include <QCoreApplication>

const int IMAGE_WIDTH = 1024;
const int IMAGE_HEIGHT = IMAGE_WIDTH;

void Reflection::onPluginLoad() {
    trianglePlane = vector<Point>(3);
    trianglePlane[0] = Point(-3,-2,-1);
    trianglePlane[1] = Point(-2.5,0,5);
    trianglePlane[2] = Point(-2.5,4,-5);
    GLWidget & g = *glwidget();
    g.makeCurrent();
    // Carregar shader, compile & link 
    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile(QCoreApplication::applicationDirPath()+
              "/../../plugins/reflection/reflection.vert");

    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile(QCoreApplication::applicationDirPath()+
              "/../../plugins/reflection/reflection.frag");

    program = new QOpenGLShaderProgram(this);
    program->addShader(vs);
    program->addShader(fs);
    program->link();

    vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vs->compileSourceFile(QCoreApplication::applicationDirPath()+
              "/../../plugins/reflection/template.vert");

    fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fs->compileSourceFile(QCoreApplication::applicationDirPath()+
              "/../../plugins/reflection/template.frag");

    mainProgram = new QOpenGLShaderProgram(this);
    mainProgram->addShader(vs);
    mainProgram->addShader(fs);
    mainProgram->link();

    // Setup texture
    g.glActiveTexture(GL_TEXTURE0);
    g.glGenTextures( 1, &textureId);
    g.glBindTexture(GL_TEXTURE_2D, textureId);
    g.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    g.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    g.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		      GL_LINEAR_MIPMAP_LINEAR );
    g.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    g.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, IMAGE_WIDTH, IMAGE_HEIGHT,
		   0, GL_RGB, GL_FLOAT, NULL);
    g.glBindTexture(GL_TEXTURE_2D, 0);
    // Resize to power-of-two viewport
    g.resize(IMAGE_WIDTH,IMAGE_HEIGHT);
}

void drawRect(GLWidget &g, vector<Point>& plane) {
    static bool created = false;
    static GLuint VAO_rect;

    // 1. Create VBO Buffers
    if (not created) {
        created = true;
        
        // Create VBO with (x,y,z) coordinates
        Point point = plane[0] + (plane[1]-plane[0] + plane[2]-plane[0]);
        float coords[] = { plane[0][0], plane[0][1], plane[0][2],
                           plane[1][0], plane[1][1], plane[1][2],
                           plane[2][0], plane[2][1], plane[2][2],
                           point[0], point[1], point[2]};


        // Create & bind empty VAO
        g.glGenVertexArrays(1, &VAO_rect);
        g.glBindVertexArray(VAO_rect);

        GLuint VBO_coords;
        g.glGenBuffers(1, &VBO_coords);
        g.glBindBuffer(GL_ARRAY_BUFFER, VBO_coords);
        g.glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
        g.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        g.glEnableVertexAttribArray(0);
        g.glBindVertexArray(0);
    }

    // 2. Draw
    g.glBindVertexArray (VAO_rect);
    g.glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    g.glBindVertexArray(0);
}

bool Reflection::paintGL() {
    /*Object& object = scene()->objects()[0];
    Point size = object.boundingBox().max()-object.boundingBox().min();
    trianglePlane[0] = object.boundingBox().min();
    trianglePlane[1] = Point(object.boundingBox().min().x() + size.x(), object.boundingBox().min().y(), object.boundingBox().min().z());
    trianglePlane[2] = Point(object.boundingBox().min().x(), object.boundingBox().min().y() + size.y(), object.boundingBox().min().z());*/
    GLWidget & g = *glwidget();
    // Pass 1. Draw scene
    g.glClearColor(1,1,1,0);
    g.glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    QVector3D vec1 = trianglePlane[1]-trianglePlane[0];
    QVector3D vec2 = trianglePlane[2]-trianglePlane[0];
    QVector3D crossProd = QVector3D::crossProduct(vec1, vec2).normalized();
    float a = crossProd.x();
    float b = crossProd.y();
    float c = crossProd.z();
    float d = -(a*trianglePlane[1][0] + b*trianglePlane[1][1] + c*trianglePlane[1][2]);
    cout << "Plane: ";
    if (a != 0) {
        if (a != 1) cout << a;
        cout << "x ";
    }
    if (b != 0) {
        if (a != 0) cout << "+ ";
        if (b != 1) cout << b;
        cout << "y ";
    }
    if (c != 0) {
        if (a != 0 or b != 0) cout << "+ ";
        if (c != 1) cout << c;
        cout << "z ";
    }
    if (d != 0) {
        if (a != 0 or b != 0 or c != 0) {
            if (d < 0) cout << d;
            else cout << "+ " << d;
        }
        else cout << d;
        cout << " ";
    }
    cout << "= 0" << endl;
    QMatrix4x4 reflection(
                1-2*a*a, -2*a*b, -2*a*c, -2*d*a,
                -2*a*b,1-2*b*b, -2*b*c, -2*d*b,
                -2*a*c, -2*b*c, 1-2*c*c, -2*d*c,
                0, 0, 0, 1
                );
    for (int i = 0; i < 4; ++i) {
        QVector4D row = reflection.row(i);
        for (int j = 0; j < 4; ++j) {
            cout << row[j] << " ";
        }
        cout << endl;
    }
    mainProgram->bind();
    mainProgram->setUniformValue("modelViewProjectionMatrix", camera()->projectionMatrix()*camera()->viewMatrix()*reflection);
    mainProgram->setUniformValue("normalMatrix", camera()->viewMatrix().normalMatrix());
    if (drawPlugin()) drawPlugin()->drawScene();

    // Get texture
    g.glBindTexture(GL_TEXTURE_2D, textureId);
    g.glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0,
			  IMAGE_WIDTH, IMAGE_HEIGHT);
    g.glGenerateMipmap(GL_TEXTURE_2D);

    // Pass 2. Draw quad using texture
    g.glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    program->bind();
    program->setUniformValue("colorMap", 0);

    QMatrix4x4 transform;

    //transform.translate(-3, 0, -3);
    //transform.scale(6);

// quad covering viewport 
    program->setUniformValue("modelViewProjectionMatrix",  camera()->projectionMatrix()*camera()->viewMatrix()*transform);

    drawRect(g, trianglePlane);
    program->release();
    g.glBindTexture(GL_TEXTURE_2D, 0);

    mainProgram->bind();
    mainProgram->setUniformValue("modelViewProjectionMatrix", camera()->projectionMatrix()*camera()->viewMatrix());
    mainProgram->setUniformValue("normalMatrix", camera()->viewMatrix().normalMatrix());
    if (drawPlugin()) drawPlugin()->drawScene();

    mainProgram->release();
    return true;
}
