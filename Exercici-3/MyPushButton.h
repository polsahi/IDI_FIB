#include <QPushButton>

class MyPushButton : public QPushButton {
    Q_OBJECT

public:
    MyPushButton(QWidget* parent);

public slots:

    void change();

private:

    bool started;

};