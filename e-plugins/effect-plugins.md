#Effect plugins

## Métodes:

### Per implementar:

- virtual void preFrame();
- virtual void postFrame();
- virtual void onPluginLoad();
- virtual void onObjectAdd();

### Per utilitzar:

- GLWidget* pglwidget;
- Scene* scene();
- Camera* camera();


## Exemples important:

### Accessos:

- scene()->objects().size(); // num objectes
- camera()->getObs(); // pos de l’observador
- glwidget()->defaultProgram();
- glwidget()->makeCurrent();

### onPluginLoad(): Per carregar shaders

	glwidget()->makeCurrent();
	program = new QOpenGLShaderProgram(this);


	//VERTEX SHADER:
	vs = new QOpenGLShader(QOpenGLShader::Vertex, this);
    	vs->compileSourceFile("plugins/show-help/show.vert");
    	program->addShader(vs);
    
	//FRAMENT SHADER:
    	fs = new QOpenGLShader(QOpenGLShader::Fragment, this);
    	fs->compileSourceFile("plugins/show-help/show.frag");
    	program->addShader(fs);
    	program->link();
    
### preFrame(): Enllaçar shaders i carregar uniforms

    program->bind(); //!!!

    //UNIFORM FLOAT
    float f = 1;
    program->setUniformValue("speed", f);

    //PROJECTION VIEW MATRIX
    QMatrix4x4 MVP = camera()->projectionMatrix() * camera()->viewMatrix();
    program->setUniformValue("modelViewProjectionMatrix", MVP);

    //NORMAL MATRIX
    QMatrix3x3 NM = camera()->viewMatrix().normalMatrix();
    program->setUniformValue("normalMatrix", NM);

    //TIME
    program->setUniformValue("time", float(elapsedTimer.elapsed()/1000.0f));
    
### postFrame(): Desnllaçar shaders
	
	program->release();
	
### postFrame(): Dibuixar text per pantalla
	
	GLWidget &g=*glwidget(); // !!!
	g.makeCurrent();
	const int SIZE = 1024;
		
	QImage image(SIZE,SIZE,QImage::Format_RGB32);
	mage.fill(Qt::white);
	
	QPainter painter;
	painter.begin(&image);
	
	QFont font;
	font.setPixelSize(32);
	
	painter.setFont(font);
	painter.setPen(QColor(50,50,50));
	int x = 15;
	int y = 50;
	
	painter.drawText(x, y, QString("L - Load object A - Add plugin"));
	painter.end();
	
### postFrame(): Crear textura

	const int textureUnit = 5;
	g.glActiveTexture(GL_TEXTURE0+textureUnit);
	
	QImage im0 = image.mirrored(false, true).convertToFormat(QImage::Format_RGBA8888, Qt::ColorOnly);
	
	g.glGenTextures( 1, &textureID);
	g.glBindTexture(GL_TEXTURE_2D, textureID);
	g.glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, im0.width(), im0.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, im0.bits());
	
	g.glGenerateMipmap(GL_TEXTURE_2D);
	
	g.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	
	g.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	g.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	g.glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	
### postFrame(): Dibuixar quadrat

	program->bind();
	program->setUniformValue("colorMap", textureUnit);
	program->setUniformValue("WIDTH", float(glwidget()->width()));
	program->setUniformValue("HEIGHT", float(glwidget()->height()));
	 
	// quad covering viewport 
	drawRect(g);
	program->release();
	g.glBindTexture(GL_TEXTURE_2D, 0);
	   
	g.glDeleteTextures(1, &textureID); //FINISH WITH THAT
    
## Utils

##drawRect

	void drawRect(GLWidget &g)
	{
		static bool created = false;
		static GLuint VAO_rect;

		// 1. Create VBO Buffers
		if (!created)
		{
			created = true;
			
			// Create & bind empty VAO
			g.glGenVertexArrays(1, &VAO_rect);
			g.glBindVertexArray(VAO_rect);

			float z = -0.99999;
			// Create VBO with (x,y,z) coordinates
			float coords[] = { -1, -1, z, 
								1, -1, z, 
							   -1,  1, z, 
								1,  1, z};

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






    
    
    
