// MyGLWidget.cpp
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

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_A: {
      rModels -= (float)M_PI/4;
    break;
	}
  case Qt::Key_D: {
      rModels += (float)M_PI/4;
    break;
	}
  case Qt::Key_E: {
      lEscena.on = not lEscena.on;
      apagaEncent();
    break;
	}
  case Qt::Key_B: {
      discoParty();
    break;
	}	
  case Qt::Key_R: {
      rBola += (float)M_PI/36;
      giraLlumetes();
    break;
	}
  case Qt::Key_S: {
      if (timer.isActive())
        timer.stop();
      else timer.start(100);
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::modelTransformMorty ()
{
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.f), glm::vec3(5,0,5));
  TG = glm::scale(TG, glm::vec3(escalaMorty, escalaMorty, escalaMorty));
  TG = glm::rotate(TG, rModels, glm::vec3(0, 1, 0));
  TG = glm::translate(TG, -centreBaseMorty);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformFantasma (float posX)
{
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.0f), glm::vec3(posX,0.5,5));
  TG = glm::scale(TG, glm::vec3(escalaFantasma, escalaFantasma, escalaFantasma));
  TG = glm::rotate(TG, rModels, glm::vec3(0, 1, 0));
  TG = glm::translate(TG, -centreBaseFantasma);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformDiscoBall ()
{
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.f), glm::vec3(5,5,5));
  TG = glm::scale(TG, glm::vec3(escalaDiscoBall, escalaDiscoBall, escalaDiscoBall));
  TG = glm::rotate(TG, rBola, glm::vec3(0, 1, 0));
  TG = glm::translate(TG, -centreBaseDiscoBall);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
  
}

void MyGLWidget::paintGL() 
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //iniLlumetes();

  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 30);
  
  // MORTY
  glBindVertexArray (VAO_Morty);
  modelTransformMorty ();
  glDrawArrays(GL_TRIANGLES, 0, morty.faces().size()*3);
  
  // FANTASMA
  glBindVertexArray (VAO_Fantasma);
  modelTransformFantasma (1.0f);
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);

  modelTransformFantasma (9.0f);
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);

  // DISCO BALL
  glBindVertexArray (VAO_DiscoBall);
  modelTransformDiscoBall ();
  glDrawArrays(GL_TRIANGLES, 0, discoBall.faces().size()*3);
  
  glBindVertexArray(0);
}

void MyGLWidget::initializeGL() {
  LL4GLWidget::initializeGL();
  carregaShaders();
  iniLlumetes();
  connect(&timer, SIGNAL(timeout()), this, SLOT(party()));
}

void MyGLWidget::carregaShaders() {

  TGModelLoc = glGetUniformLocation (program->programId(), "TGModel");

  posFELoc = glGetUniformLocation (program->programId(), "posFocusEscena");
  colorFELoc = glGetUniformLocation (program->programId(), "colorFocusEscena");
  
  Llums[0].posLoc = glGetUniformLocation (program->programId(), "posLlum0");
  Llums[0].colorLoc = glGetUniformLocation (program->programId(), "colorLlum0");

  Llums[1].posLoc = glGetUniformLocation (program->programId(), "posLlum1");
  Llums[1].colorLoc = glGetUniformLocation (program->programId(), "colorLlum1");

  Llums[2].posLoc = glGetUniformLocation (program->programId(), "posLlum2");
  Llums[2].colorLoc = glGetUniformLocation (program->programId(), "colorLlum2");

  Llums[3].posLoc = glGetUniformLocation (program->programId(), "posLlum3");
  Llums[3].colorLoc = glGetUniformLocation (program->programId(), "colorLlum3");
}

void MyGLWidget::apagaEncent() {
  glUniform3fv (posFELoc, 1, &lEscena.posicio[0]);
  if (lEscena.on) glUniform3fv (colorFELoc, 1, &lEscena.color[0]);
  else glUniform3fv (colorFELoc, 1, &apagada.color[0]);
}

void MyGLWidget::iniLlumetes() {

  lEscena = { glm::vec3(0.4, 0.4, 0.4), glm::vec3(5, 10, 5) };
  apagada = { glm::vec3(0, 0, 0) };
  
  Llums[0] = Llum{ glm::vec3(.4, 0, 0), glm::vec3(1.2, 0, 0) };
  Llums[1] = Llum{ glm::vec3(0, .4, 0), glm::vec3(-1.2, 0, 0) };
  Llums[2] = Llum{ glm::vec3(0, 0, .4), glm::vec3(0, 0, 1.2) };
  Llums[3] = Llum{ glm::vec3(.4, .4, 0), glm::vec3(0, 0, -1.2) };

  glUniform3fv (posFELoc, 1, &lEscena.posicio[0]);
  glUniform3fv (colorFELoc, 1, &lEscena.color[0]);

  enviaLlumetes();
  giraLlumetes();
}

void MyGLWidget::discoParty() {
  Llums[counter].on = not Llums[counter].on;
  ++counter;
  counter %= 4;
  enviaLlumetes();
}

void MyGLWidget::enviaLlumetes() {
  for (unsigned int i = 0; i < NUM_LLUMS; ++i) {
    if (Llums[i].on) {
      glUniform3fv(Llums[i].posLoc, 1, &Llums[i].posicio[0]);
      glUniform3fv(Llums[i].colorLoc, 1, &Llums[i].color[0]);
    }
    else {
      glUniform3fv(Llums[i].colorLoc, 1, &apagada.color[0]);
    }
  }
}

void MyGLWidget::giraLlumetes() {
  glm::mat4 TG = glm::translate(glm::mat4(1.f), glm::vec3(5,5,5));
  TG = glm::rotate(TG, rBola, glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseDiscoBall);

  glUniformMatrix4fv (TGModelLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::party() {
  makeCurrent();
  rBola += (float)M_PI/36;
  discoParty();
  update();
}