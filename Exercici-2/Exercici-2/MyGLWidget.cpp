#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

void MyGLWidget::RickTransform() {
    glm::mat4 TG(1.0f);  
    TG = glm::translate(TG, glm::vec3(-2, 0, -2));
    TG = glm::scale(TG, glm::vec3(escalaModels[RICK]));
    TG = glm::rotate(TG, float(M_PI), glm::vec3(0,1,0));
    TG = glm::translate(TG, -centreBaseModels[RICK]);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::MortyTransform() {
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, posMorty);
    TG = glm::scale(TG, glm::vec3(escalaModels[MORTY]));
    TG = glm::rotate(TG, angle_morty, glm::vec3(0,1,0));
    TG = glm::translate(TG, -centreBaseModels[MORTY]);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::VideoCameraTransform() {
    glm::mat4 TG(1.0f);
    TG = glm::translate(TG, glm::vec3(-4, 3, -4));
    TG = glm::scale(TG, glm::vec3(escalaModels[VIDEOCAMERA]));
    TG = glm::rotate(TG, angle_cam, glm::vec3(0, 1, 0));
    TG = glm::rotate(TG, float(M_PI/4), glm::vec3(1, 0, 0));
    TG = glm::translate(TG, -centreBaseModels[VIDEOCAMERA]);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform() {
    glm::mat4 View(1.0f);
    View = glm::translate(View, glm::vec3(0,0, -camera.distance));
    View = glm::rotate(View, camera.theta, glm::vec3(1, 0, 0));
    View = glm::rotate(View, -camera.psi, glm::vec3(0, 1, 0));
    View = glm::translate(View, -camera.VRP);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: { 
        posMorty = posMorty + glm::vec3(1,0,1)*glm::vec3(0.1)*glm::vec3(float(sin(angle_morty)), 0,float(cos(angle_morty)));  	
        break;
    }
    case Qt::Key_Down: {
	    posMorty = posMorty - glm::vec3(1,0,1)*glm::vec3(0.1)*glm::vec3(float(sin(angle_morty)), 0,float(cos(angle_morty)));
        break;
    }
    case Qt::Key_Q: {
        if (angle_cam < float(M_PI/2))
            angle_cam += float(M_PI/180);
        break;
    }
    case Qt::Key_E: {
        if (angle_cam > 0)   
            angle_cam -= float(M_PI/180);
        break;
    }
    case Qt::Key_Left: {
        angle_morty += float(M_PI/4);
        if (angle_morty == float(2*M_PI + M_PI/4)) angle_morty = float(M_PI/4);
        break;
    }
    case Qt::Key_Right: {
        angle_morty -= float(M_PI/4);
        if (angle_morty == -float(M_PI/4)) angle_morty = float(2*M_PI - M_PI/4);
        break;
    }
    case Qt::Key_C: {
        cam = not cam;
        break;
    }
    case Qt::Key_T: {
        started = not started;
        if (not started)
            timer.start(1000);
        else timer.stop();
        break;
    }
    case Qt::Key_G: {
        inivisible = not inivisible;
        break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *event) {
    ratoli.pressed = false;
    event->ignore();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event) {
    
    makeCurrent();
    
    if (ratoli.pressed) {
        int deltax = event->x() - ratoli.x;
        int deltay = event->y() - ratoli.y;
        camera.theta += deltay*factorAngleY;
        camera.psi -= deltax*factorAngleX;
        ratoli.x = event->x();
        ratoli.y = event->y();
        update();
    }
}

void MyGLWidget::mousePressEvent(QMouseEvent *event) {
    
    makeCurrent();
    
    if (event->button() == Qt::LeftButton && not (event->modifiers() & (Qt::ShiftModifier | Qt::AltModifier | Qt::ControlModifier))) {
        ratoli.pressed = true; 
        ratoli.x = event->x();
        ratoli.y = event->y();
    }
}

void MyGLWidget::paintGL() {
  // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, ample, alt);
    viewTransform();
    projectTransform();

    glm::vec3 filter(1.f);
    glUniform3fv(filterLoc, 1, &filter[0]);
    GLint color;
    if (inivisible) color = 1;
    else color = 0;
   glUniform1i(rickLoc, color);

  // Rick
  glBindVertexArray (VAO_models[RICK]);
  RickTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[RICK].faces().size()*3);

    color = 0;
    glUniform1i(rickLoc, color);

  // Morty
  glBindVertexArray (VAO_models[MORTY]);
  MortyTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[MORTY].faces().size()*3);
  
  // VideoCamera
  glBindVertexArray (VAO_models[VIDEOCAMERA]);
  VideoCameraTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[VIDEOCAMERA].faces().size()*3);  
  
  // Terra
  glBindVertexArray (VAO_Terra);
  identTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  // Paret
  ParetTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    if (cam) {
        glViewport(ample-ample/4, alt-alt/4, ample/4, alt/4);
        glm::vec3 filter(0,1.f,0);
        glUniform3fv(filterLoc, 1, &filter[0]);
        viewTransform2();
        projectTransform2();
        glBindVertexArray (VAO_models[RICK]);
        if (inivisible) color = 1;
        else color = 0;
        glUniform1i(rickLoc, color);
        RickTransform();
        glDrawArrays(GL_TRIANGLES, 0, models[RICK].faces().size()*3);
        color = 0;
        glUniform1i(rickLoc, color);
        glBindVertexArray (VAO_models[MORTY]);
        MortyTransform();
        glDrawArrays(GL_TRIANGLES, 0, models[MORTY].faces().size()*3);
        glBindVertexArray (VAO_models[VIDEOCAMERA]);
        VideoCameraTransform();
        glDrawArrays(GL_TRIANGLES, 0, models[VIDEOCAMERA].faces().size()*3);
        glBindVertexArray (VAO_Terra);
        identTransform();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        ParetTransform();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    glBindVertexArray (0);
}

void MyGLWidget::projectTransform() {
  glm::mat4 Proj(1.0f);
  Proj = glm::perspective (camera.FOV, camera.ra, camera.zN, camera.zF);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::getCamParams() {
    camera.FOVinicial = 2*float(asin(1/2.));
    camera.FOV = camera.FOVinicial;
    camera.ra = 1.f;
    camera.zN = escena.radi;
    camera.zF = 3*escena.radi;
    camera.VRP = escena.centre;
    camera.psi = 0;
    camera.theta = float(M_PI/4);
    camera.distance = 2*escena.radi;
}

void MyGLWidget::initializeGL() {
    LL2GLWidget::initializeGL();
    getCamParams();
    connect(&timer, SIGNAL(timeout()), this, SLOT(animacio()));
}

void MyGLWidget::resizeGL(int w, int h) {
    ample = w;
    alt = h;
    LL2GLWidget::resizeGL(w,h);
    float raw = float(w)/float(h);

    camera.ra = raw;   
    if (raw < 1) 
        camera.FOV = 2*atan(tan(camera.FOVinicial/2)/raw);
    projectTransform();
}

void MyGLWidget::viewTransform2() {
   
    glm::mat4 View(1.0f);
    video.VRP = glm::vec3(float(sin(angle_cam)*sqrt(32)), -2, float(cos(angle_cam)*sqrt(32))) + glm::vec3(-4, 0, -4);
    View = glm::lookAt(glm::vec3(-4, 3, -4), video.VRP, glm::vec3(0, 1, 0));
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform2() {
    glm::mat4 Proj(1.0f);
    Proj = glm::perspective (video.FOV, video.ra, video.zN, video.zF);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::animacio() {
    
    makeCurrent();
    if (angle_cam >= float(M_PI/2))
        sentit = -1;
    else if (angle_cam <= 0)
        sentit = 1;
    angle_cam += sentit*float(M_PI/12);
    update();
}

void MyGLWidget::iniEscena() {
    float minx, miny, minz, maxx, maxy, maxz;

    minx = -4; miny = 0.0; minz = -4;
    maxx = 4;  maxy = 3.f;  maxz = 4;

    escena.min = glm::vec3(minx, miny, minz);     
    escena.max = glm::vec3(maxx, maxy, maxz);  
    escena.centre = glm::vec3((minx+maxx)/2.f, (miny+maxy)/2.f, (minz+maxz)/2.f);   // (min+max)/2.f
      

    escena.radi = glm::distance(escena.centre, escena.max);
}

void MyGLWidget::carregaShaders() {
    LL2GLWidget::carregaShaders();
    filterLoc  = glGetUniformLocation (program->programId(), "filtre");
    rickLoc  = glGetUniformLocation (program->programId(), "rick");
}

MyGLWidget::~MyGLWidget()
{

}



