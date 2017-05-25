#VA-VBO

##Vertex Arrays

1. **glDrawElements( 1 , 2 , 3 , 4 )**

- 1.Primitiva: GL_TRIANGLES,GL_QUADS,...
- 2. Num. index array: 12 triangles, 12*3 = 36 vertexs
- 3. Tipus d'index:  GL_UNSIGNED_INT
- 4. Apuntados array: Variable del vector de vertexs.

Exemple:

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices)

2. **glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)verts);**

Es crida per a cada objecte diferent.

- GLuint **index**, // VS: layout (location = 0) in vec3 vertex;
- GLint **size**, // Num de coordenades (1,2,3,4)
- GLenum **type**, // Tipus de cada coordenada: GL_FLOAT o GL_DOUBLE
- GLboolean **normalized**, // Per convertir a valors en [0,1]
- GLsizei **stride**, // Normalment 0 (un array per cada atribut)
- const GLvoid * **pointer**; // Apuntador a les dades

3. **glEnableVertexAttribArray(0);**

##Vertex Buffer Object

1. **SET UP**

	// Step 1: Create and fill STL arrays (coords, normals, idx…)
	vector<float> coords; // x,y,z,x,y,z…
	vector<float> normals; // nx,ny,nz,nx,ny,nz…
	vector<unsigned int> indices; // i0,i1,i2 i3,i4,i5…
	
	for (…)
	 vertices.push_back(x);
	 vertices.push_back(y);
	 vertices.push_back(z);
	
	// Step 2: Create VAO & empty buffers (coords, normals, indices)
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	
	GLuint coordBufferID;
	glGenBuffers(1, &coordBufferID);
	
	GLuint normalBufferID;
	glGenBuffers(1, &normalBufferID);
	
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID); 
	
	for (…)
	indices.push_back(index);
	
	// Step 3: Define VBO data (coords, normals, indices)

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, coordBufferID); // VAO

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*coords.size(), &coords[0], GL_STATIC_DRAW); // ONCE
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // VA
	
	glEnableVertexAttribArray(0); // VAO
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferID); // VAO
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normals.size(), &normals[0], GL_STATIC_DRAW); // ONCE
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); // VAO
	
	glEnableVertexAttribArray(1); // VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffersID); // VAO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indices.size(),&indices[0], GL_STATIC_DRAW); // ONCE
	glBindVertexArray(0);
	
2. **DRAW**

	// Draw
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT,
	(GLvoid*) 0); //numIndices=indices.size()
	glBindVertexArray(0);
	
	// Draw multiples instances
	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, numIndices,
	GL_UNSIGNED_INT, (GLvoid*) 0, numInstances);
	glBindVertexArray(0);
	
	// VS: int gl_InstanceID -> instance number (0…numInstances-1)

3. **CLEAN UP**

	// Clean up
	glDeleteBuffers(1, &coordBufferID);
	glDeleteBuffers(1, &normalBufferID);
	glDeleteBuffers(1, &indexBufferID);
	glDeleteVertexArrays(1, &VAO);
