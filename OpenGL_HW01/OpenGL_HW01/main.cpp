/*
 * Computer Graphics HW01 : PPAP
 *  - Press 'Enter' to penetrate Apple , then press other key to recover .
 *  - Press 'Esc' to exit .
 */



#include <stdlib.h>
#include <GLUT/GLUT.h>

void init(void);
void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void idle(void);

GLfloat angle = 0.0f;
GLfloat delta = 0.5f; // rotate speed
GLfloat speed = 0.2f; // Pen & Apple move speed
GLfloat PenMovedPosY = 0.0f;
GLfloat AppleMovedPosX = 0.0f;
int Mode = 0 ; // 0 : displayMode , 1 : animationMode


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("OpenGL Assignment 1");
    glutReshapeWindow(512, 512);
    
    init();
    
    glutReshapeFunc(reshape); // 伸縮視窗時之call back func.
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard); // Keyboard事件之call back func.
    
    glutMainLoop();
    return 0;
}

void init(void) {
    
    GLfloat position[] = { 1.0, 1.0, 5.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(0.0, 0.0, 8.0, /* Eye pos XYZ */
              0.0, 0.0, 0.0, /* Target pos XYZ */
              0.0, 1.0, 0.0); /* Up vector */
    
    // do something awesome here
    
    glEnable(GL_BLEND); // 開啟混成
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_COLOR);//混成的規則
    glDisable(GL_DEPTH_TEST); // 關閉深度測試 (使用混成時)
    glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION); // 使用GL_EMISSION
    
    // Draw cube (with 6 quards)
    glPushMatrix();
    glTranslated(0.0, 1.8, -5.0);
    glRotatef(angle, 1 , 1, 0);
    glRotatef(angle, 0 , 1, 1);
    glRotatef(angle, 1 , 0, 1);
    
    glBegin(GL_QUADS);
    // top
    glColor4f(1.0f, 0.0f, 0.0f , 0.4f);
    glNormal3f(0.0f, 1.0f, 0.0f);   // 指定法相量 (讓light照在上面時正常)
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();
    
    glBegin(GL_QUADS);
    // front
    glColor4f(0.0f, 1.0f, 0.0f , 0.4f);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();
    
    glBegin(GL_QUADS);
    // right
    glColor4f(0.0f, 0.0f, 1.0f,0.4f);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glEnd();
    
    glBegin(GL_QUADS);
    // left
    glColor4f(0.5f, 0.5f, 0.5f ,0.4f);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glEnd();
    
    glBegin(GL_QUADS);
    // bottom
    glColor4f(0.0f, 0.5f, 0.5f ,0.4f);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glEnd();
    
    glBegin(GL_QUADS);
    // back
    glColor4f(5.0f, 0.5f, 0.0f ,0.4f);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();
    
    glPopMatrix();
    
    //------------------------------------------------------------
    

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE );  // 當有燈時,需設定,才可讓color3f/4f顯示出顏色
    
    // Draw Apple ------------------------------------
    glColor3f(0.75 , 0.75, 0.75 );
    glPushMatrix();
        glTranslated(1.5, 0, 0);
        if(Mode == 0){                      // Mode : 0 一般模式
            AppleMovedPosX = 0;
            glRotatef(angle, 0.0f, 1.0f, 0.0f);
        }
        else{                               // Mode : 1 穿刺模式
            glTranslated(AppleMovedPosX, 0, 0);
            if (AppleMovedPosX > -1.5 ) {
                AppleMovedPosX -= speed;
            }
        }
    
        // 圓錐 1
        glPushMatrix();
            glRotatef(270, 1, 0, 0);  // 延x順轉270,local x,y,z也跟著轉 此時y向螢幕外,z向上
            glTranslated(0, 0, 0.8);    // 依local x,y,z 移動
            glutSolidCone(0.05, 0.3, 40, 40);
        glPopMatrix();
    
        // 圓錐 2
        glPushMatrix();
            glRotatef(90, 1, 0, 0);
            glRotatef(60, 0, 1, 0);
            glTranslated(0.7, 0, -0.7);
            glutSolidCone(0.05, 0.3, 40, 40);
        glPopMatrix();
    
        glutSolidSphere(0.8, 100,100);
    glPopMatrix();
    // -----------------------------------------------------
    
    // Draw Pen -------------------------------------------------------------
    glPushMatrix();
        glTranslated(-1.5, 0, 0);    // 先在原點繪製好筆(即下面code),再一併往-1.5 x移動,在轉
        if(Mode == 0){
            PenMovedPosY = 0;
            glRotatef(15, 0, 0, 1);      // 以local Z軸為中心轉15度
            glRotatef(angle, 0.3, 1, 0); // 以loacl (0.3,1,0)轉 (此時local和上ㄧ行local已不同)
        }
        else{
            glRotatef(90, 0.0f, 0.0f, -1.0f);
            glTranslated(0, PenMovedPosY, 0);
            if (PenMovedPosY < 1.3 ) {
                PenMovedPosY += speed;
            }
        }
        // 圓柱
        glPushMatrix();
            GLUquadricObj *quadratic;
            quadratic = gluNewQuadric();
            glTranslated(0, 0.8, 0);
            glRotatef(90, 1, 0, 0);
            gluCylinder(quadratic,0.05f,0.05f,1.8f,32,32);
        glPopMatrix();
        // 圓錐
        glPushMatrix();
            glRotatef(270, 1, 0, 0);  // 延x順轉270,local x,y,z也跟著轉 此時y向螢幕外,z向上
            glTranslated(0, 0, 0.8);    // 依local x,y,z 移動
            glutSolidCone(0.05, 0.3, 40, 40);
        glPopMatrix();
    glPopMatrix();
    
    // ----------------------------------------------------------
    
    
    
    
    angle += delta;
    glutPostRedisplay(); // 再call一次glutDisplayFunc指定的call back func.
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.1, 100.0);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 13:
        {   //enter
            Mode = 1 ;
            break;
        }
        case 27:
        {   //ESC
            exit(0);
            break;
        }
        default:
        {
            Mode = 0 ;
            break;
        }
    }
    
    glutPostRedisplay(); // 按下按鍵後 , Redraw畫面
}

void idle(void)
{
    glutPostRedisplay();
}
