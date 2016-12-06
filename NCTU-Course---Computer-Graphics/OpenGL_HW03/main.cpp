#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> /* offsetof */
#include "../GL/glew.h"
#include "../GL/glut.h""
#include "../shader_lib/shader.h"

#include "glm/glm.h"

extern "C"
{
	#include "glm_helper.h"
}

#define SkinTriNum 4040
#define StemTriNum 412

void init(void);
void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void idle(void);

void GetSkinData();         /* 將蘋果本體的Vertex Pos/Normal/Texture Coordinate座標資料依三角形順序取出  */
void GetStemData();         /* 將蘋果梗的Vertex Pos/Normal/Texture Coordinate座標資料依三角形順序取出  */
void GetSkinFacetNor();     /* 將蘋果本體各三角形的Normal座標資料依三角形順序取出  */
void GetStemFacetNor();     /* 將蘋果梗各三角形的Normal座標資料依三角形順序取出  */
void SettingVBO_VAO();      /* 設定一些要用到的VBO , 並建立VAO來管理 */

GLMmodel *XiaoPingGuo;
GLfloat light_pos[] = { 10.0, 10.0, 0.0 };
float eyex = 0.0;
float eyey = 0.0;

GLuint program;
GLuint SkinVbo , StemVbo;
GLuint SkinFacetNorVbo , StemFacetNorVbo;

float skinVertexData[SkinTriNum*24];   /* 存放本體各Vertex的 Pos/Normal/Texture Coordinate座標 (依序) */
float skinFacetNor[SkinTriNum*9];
float stemVertexData[StemTriNum*24];
float stemFacetNor[StemTriNum*9];

GLint CurrentShadeMode = 1 ; /* 1 : phong shading , -1 : Flat shading */

void SettingVBO_VAO()
{
    glGenBuffers(1, &SkinVbo);
    glBindBuffer(GL_ARRAY_BUFFER,SkinVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SkinTriNum * 24, skinVertexData, GL_STATIC_DRAW);
    
    glGenBuffers(1, &StemVbo);
    glBindBuffer(GL_ARRAY_BUFFER,StemVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * StemTriNum * 24, stemVertexData, GL_STATIC_DRAW);
    
    
    glGenBuffers(1, &SkinFacetNorVbo);
    glBindBuffer(GL_ARRAY_BUFFER,SkinFacetNorVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SkinTriNum * 9, skinFacetNor, GL_STATIC_DRAW);
    
    glGenBuffers(1, &StemFacetNorVbo);
    glBindBuffer(GL_ARRAY_BUFFER,StemFacetNorVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * StemTriNum * 9,stemFacetNor, GL_STATIC_DRAW);
    
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    glEnableVertexAttribArray(7);
    
    glBindBuffer(GL_ARRAY_BUFFER,SkinVbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(float)));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER,StemVbo);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), 0);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(float)));
    glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(float)));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER,SkinFacetNorVbo);
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER,StemFacetNorVbo);
    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0); 
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void GetSkinData()
{
	int tri_index = 0 ;
	for(int i = 0 ; i < SkinTriNum * 24 ; i ++)
	{	
		for(int j = 0 ; j < 3 ; j ++)
		{
			for(int k = 0 ; k < 3 ; k ++)
			{
				skinVertexData[i+8*j+k] = XiaoPingGuo->vertices[((XiaoPingGuo->triangles)[(((XiaoPingGuo->groups)->next)->triangles[tri_index])].vindices[j])*3+k];
				skinVertexData[i+8*j+k+3] = XiaoPingGuo->normals[((XiaoPingGuo->triangles)[(((XiaoPingGuo->groups)->next)->triangles[tri_index])].nindices[j])*3+k];
				if( k < 2 )
					skinVertexData[i+8*j+k+6] = XiaoPingGuo->texcoords[((XiaoPingGuo->triangles)[(((XiaoPingGuo->groups)->next)->triangles[tri_index])].tindices[j])*2+k];
				
			}
		}
		
		tri_index ++ ;
		i = i + 23 ;
		
	}
	
}

void GetStemData()
{
	int tri_index = 0 ;
	for(int i = 0 ; i < StemTriNum * 24 ; i ++)
	{
		for(int j = 0 ; j < 3 ; j ++)
		{
			for(int k = 0 ; k < 3 ; k ++)
			{
				stemVertexData[i+8*j+k] = XiaoPingGuo->vertices[((XiaoPingGuo->triangles)[((XiaoPingGuo->groups)[0].triangles[tri_index])].vindices[j])*3+k];
				stemVertexData[i+8*j+k+3] = XiaoPingGuo->normals[((XiaoPingGuo->triangles)[((XiaoPingGuo->groups)[0].triangles[tri_index])].nindices[j])*3+k];
				
				if( k < 2 )
					stemVertexData[i+8*j+k+6] = XiaoPingGuo->texcoords[((XiaoPingGuo->triangles)[((XiaoPingGuo->groups)[0].triangles[tri_index])].tindices[j])*2+k];
				
				
			}
		}
		tri_index ++ ;
		i = i + 23 ;
	}
}

void GetSkinFacetNor()
{
	int tri_index = 0 ;
	for(int i = 0 ; i < SkinTriNum * 9 ; i ++)
	{
		for(int j = 0 ; j < 3 ; j ++)
		{
			for(int k = 0 ; k < 3 ; k ++)
				skinFacetNor[i+j+3*k] = XiaoPingGuo->facetnorms[((XiaoPingGuo->triangles)[(((XiaoPingGuo->groups)->next)->triangles[tri_index])].findex)*3+j];
		}
		tri_index ++ ;
		i = i + 8;
	}
}

void GetStemFacetNor()
{
	int tri_index = 0 ;
	for(int i = 0 ; i < StemTriNum * 9 ; i ++)
	{
		for(int j = 0 ; j < 3 ; j ++)
		{
			for(int k = 0 ; k < 3 ; k ++)
				stemFacetNor[i+j+3*k] = XiaoPingGuo->facetnorms[((XiaoPingGuo->triangles)[((XiaoPingGuo->groups)[0].triangles[tri_index])].findex)*3+j];
		}
		tri_index ++ ;
		i = i + 8;
	}
}


int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("OpenGL HW3 - 0556087");
	glutReshapeWindow(512, 512);

	glewInit();

	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	glmDelete(XiaoPingGuo);
	return 0;
}

void init(void) {

	XiaoPingGuo = glmReadOBJ("Model/apple.obj");

	glmUnitize(XiaoPingGuo);
	glmFacetNormals(XiaoPingGuo);
	glmVertexNormals(XiaoPingGuo, 90.0, GL_FALSE);

	print_model_info(XiaoPingGuo);

    // Some Initilize .
	
    /* 讀入Shader Language file 並產生 Shader Program */
    GLuint vert = createShader("Shaders/sin.vert", "vertex");
	GLuint frag = createShader("Shaders/sin.frag", "fragment");
	program = createProgram(vert, frag);

	GetSkinData();
	GetStemData();
	GetSkinFacetNor();
	GetStemFacetNor();

    SettingVBO_VAO();
	

}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 1e-2, 1e2);
    
    /* 紀錄Projection Matrix , 並由Uniform傳給 GLSL program */
	GLfloat Pmtx[16];
	glGetFloatv (GL_PROJECTION_MATRIX, Pmtx);
	GLint Ploc = glGetUniformLocation(program, "Projection");

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyex, eyey, 3.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
    
    /* 紀錄Model View Matrix , 並由Uniform傳給 GLSL program */
	GLfloat Mmtx[16];
	glGetFloatv (GL_MODELVIEW_MATRIX, Mmtx);
	GLint Mloc = glGetUniformLocation(program, "ModelView");

	GLint currentTarget = 0 ;   /* 0 : skin , 1 : stem */
	
	GLint CLoc = glGetUniformLocation(program, "CurrentTarget");
	GLint SLoc = glGetUniformLocation(program, "CurrentShadeMode");

    GLint lightPosLoc    = glGetUniformLocation(program, "lightPos");
    GLint viewPosLoc     = glGetUniformLocation(program, "viewPos");

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
    
    
    /* 畫蘋果本體 */
	glUseProgram(program);
        glUniform3f(lightPosLoc,light_pos[0],light_pos[1],light_pos[2]);
        glUniform3f(viewPosLoc,  eyex  , eyey , 3.0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, XiaoPingGuo->textures[0].id);
        glUniform1i(glGetUniformLocation(program, "ourTexture1"), 0);

	
        glUniformMatrix4fv(Ploc, 1, GL_FALSE, Pmtx);
        glUniformMatrix4fv(Mloc, 1, GL_FALSE, Mmtx);
        glUniform1i(CLoc,currentTarget);
        glUniform1i(SLoc,CurrentShadeMode);
	
        glDrawArrays(GL_TRIANGLES, 0, SkinTriNum*3);
	glUseProgram(0);

	currentTarget = 1 ;
	
    /* 畫蘋果梗 */
	glUseProgram(program);

        glUniform3f(lightPosLoc,light_pos[0],light_pos[1],light_pos[2]);
        glUniform3f(viewPosLoc,  eyex  , eyey , 3.0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, XiaoPingGuo->textures[1].id);
        glUniform1i(glGetUniformLocation(program, "ourTexture2"), 1);
	
        glUniformMatrix4fv(Ploc, 1, GL_FALSE, Pmtx);
        glUniformMatrix4fv(Mloc, 1, GL_FALSE, Mmtx);
        glUniform1i(CLoc,currentTarget);
        glUniform1i(SLoc,CurrentShadeMode);
	
        glDrawArrays(GL_TRIANGLES, 0, StemTriNum*3);
	glUseProgram(0);

	glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
	{	//ESC
		exit(0);
		break;
	}
	case 'd':
	{
		eyex += 0.1;
		break;
	}
	case 'a':
	{
		eyex -= 0.1;
		break;
	}
	case 'w':
	{
		eyey += 0.1;
		break;
	}
	case 's':
	{
		eyey -= 0.1;
		break;
	}
	case 13:
	{
		CurrentShadeMode *= -1 ;
	}
	default:
	{
		break;
	}
	}
}

void idle(void)
{
	glutPostRedisplay();
}
