#ifndef STUDENT_HPP_
#define STUDENT_HPP_

#include <iostream>
#include "util.hpp"
#include <QtXml>

typedef struct
{
    QString name;
    double value;
    bool defined_;
}note;

class student{
public:
    student();

    ~student();

    void print( );

    QString number_;
    QString date_of_brith;
    QString name_;
    QString first_name_;
    QString email_;
    std::vector<QString> options_;

    std::vector<note> notes_;
};


#endif
