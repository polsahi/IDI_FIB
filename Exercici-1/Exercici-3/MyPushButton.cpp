#include "MyPushButton.h"

MyPushButton::MyPushButton(QWidget* parent = 0) : QPushButton(parent), started(true) {
    change();
}

void MyPushButton::change() {
    started = not started;
    if (started) {
        setStyleSheet("background-color: red");
        setText("&Stop");
    }
    else {
        setStyleSheet("background-color: green");
        setText("&Start");
    }
}