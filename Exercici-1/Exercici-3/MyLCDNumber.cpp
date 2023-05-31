#include "MyLCDNumber.h"

MyLCDNumber::MyLCDNumber(QWidget* parent = 0) : QLCDNumber(parent) {
    initialize_();
}

void MyLCDNumber::start() {
    if (not running) {
        setStyleSheet("color: default");
        timer.start(1);
        running = true;
    }
    else {
        timer.stop();
        running = false;
        if (("Rècord: " + current) < best) {
            setStyleSheet("color: green");
            current = "Rècord: " + current;
            emit(envia_best(current));
            best = current;
        }
        else setStyleSheet("color: red");
    } 
}

void MyLCDNumber::tick() {
    ++miliseconds;
    update_();
}

void MyLCDNumber::reset() {
    if (not running) {
        setStyleSheet("color: default");
        miliseconds = 0;
        seconds = 0;
        minutes = 0;
        hours = 0;
        update_();
    }
}

void MyLCDNumber::update_() {

    seconds += miliseconds/1000;
    miliseconds = miliseconds%1000;
    minutes += seconds/60;
    seconds = seconds%60;
    hours += minutes/60;
    minutes = minutes%60;
    hours = hours%24;

    current = QString::number(hours/10) + QString::number(hours%10) + ":" + QString::number(minutes/10) 
    + QString::number(minutes%10) + ":" + QString::number(seconds/10) + QString::number(seconds%10) + ":" 
    + QString::number(miliseconds/100) + QString::number((miliseconds/10)%10) + QString::number(miliseconds%10);
    QLCDNumber::display(current);
}

void MyLCDNumber::initialize_() {
    running = false;
    setDigitCount(12);
    best = "Rècord: 99:99:99:999";
    reset();
    connect(&timer, SIGNAL(timeout()), this, SLOT(tick()));
}