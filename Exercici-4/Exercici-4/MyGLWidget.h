// MyGLWidget.h
#include "LL4GLWidget.h"
#include <vector>
#include <QTimer>

const unsigned int NUM_LLUMS = 4;

class MyGLWidget : public LL4GLWidget
{
  Q_OBJECT
public:
  MyGLWidget(QWidget *parent = 0) : LL4GLWidget(parent) {}
  ~MyGLWidget();

public slots:

  void party();

protected:
  virtual void mouseMoveEvent(QMouseEvent *e);
  virtual void keyPressEvent(QKeyEvent *event);
  virtual void modelTransformDiscoBall();
  virtual void modelTransformFantasma(float posX);
  virtual void modelTransformMorty();
  virtual void paintGL();
  virtual void initializeGL();
  virtual void carregaShaders();

private:

  struct Llum
  {
    Llum() = default;
    Llum(glm::vec3 color, glm::vec3 posicio, GLuint colorLoc, GLuint posLoc) : 
      color(color), posicio(posicio), colorLoc(colorLoc), posLoc(posLoc), on(true) {}
    Llum(glm::vec3 color, glm::vec3 posicio) : color(color), posicio(posicio), on(true) {}
    Llum(glm::vec3 color) : color(color) {}
 
    glm::vec3 color;
    glm::vec3 posicio;

    GLuint colorLoc;
    GLuint posLoc;

    bool on;
  };

  Llum lEscena;

  Llum apagada;

  std::vector<Llum> Llums = std::vector<Llum>(NUM_LLUMS);

  float rBola = 0;
  float rModels = 0;
  int counter = 0;
  
  QTimer timer;

  GLuint TGModelLoc;

  GLuint posFELoc;
  GLuint colorFELoc;

  void iniLlumetes();
  void apagaEncent();
  void giraLlumetes();
  void discoParty();
  void enviaLlumetes();
  int printOglError(const char file[], int line, const char func[]);
};
