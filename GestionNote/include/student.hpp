#ifndef STUDENT_HPP_
#define STUDENT_HPP_

#include <iostream>
#include "util.hpp"
#include <QtXml>

typedef struct{
    int row;
    int col;
}place;

typedef struct
{
    QString name;
    ods::Sheet *sheet;
    place cell ;
    double value;
    bool defined_;
}note;


class student{
public:
    student();

    ~student();

    bool get_dep(const QString& name);

    ods::Sheet* get_dep_sheet(const QString& name);

    place get_dep_cell(const QString& name);

    void set_cell(const place& p ,
                  ods::Sheet * s,
                  QString name);

    void print( );

    QString number_;
    QString date_of_brith;
    QString name_;
    QString first_name_;
    QString email_;
    QString option_;

    std::vector<note> notes_;

};


#endif