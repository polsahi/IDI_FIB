#include <QLCDNumber>
#include <QTimer>

class MyLCDNumber : public QLCDNumber {
    Q_OBJECT

public:
    MyLCDNumber(QWidget* parent);

public slots:

    void reset();

    void start();

    void tick();

signals:

    void envia_best(const QString& best);

private:
    unsigned int miliseconds;
    unsigned int seconds;
    unsigned int minutes;
    unsigned int hours;

    QTimer timer;

    QString best;
    QString current;

    bool running;

    void initialize_();

    void update_();
    
};