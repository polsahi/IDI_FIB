#include "LL2GLWidget.h"

#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();

  public slots:
    void animacio();

  protected:
    virtual void RickTransform();
    virtual void VideoCameraTransform();
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void projectTransform();
    virtual void viewTransform();
    virtual void paintGL();
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void MortyTransform();
    virtual void iniEscena();
    virtual void carregaShaders();
  private:
    
    void getCamParams();
    void viewTransform2();
    void projectTransform2();

      struct Camera {
        float FOVinicial;
        float FOV;
        glm::vec3 VRP;
        float psi;
        float theta;
        float distance;
        float zN;
        float zF;
        float ra;
        Camera() : FOV(float(M_PI)/3.0f), VRP(glm::vec3(0,0,0)), zN(.3f), zF(150.f), ra(1.f) {}
      };

    Camera camera;

    Camera video;

    QTimer timer;

    struct Mouse {
      bool pressed;
      int x;
      int y;
      Mouse() : pressed(false), x(0), y(0) { }
    };

    Mouse ratoli;

    struct Escena {
      glm::vec3 max;
      glm::vec3 min;
      glm::vec3 centre;
      float radi;
    };

    Escena escena;

    float angle_cam = float(M_PI/4);
    float angle_morty = 0;

    bool cam = false;
    bool started = true;
    int sentit = 1;
    
    GLuint rickLoc;
    GLuint filterLoc;

    bool inivisible = false;

    int printOglError(const char file[], int line, const char func[]);
    
};
