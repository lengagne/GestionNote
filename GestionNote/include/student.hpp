#ifndef STUDENT_HPP_
#define STUDENT_HPP_

#include <iostream>
#include "util.hpp"
#include <QtXml>
#include <iostream>
#include <fstream>

#include "matiere.hpp"

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
    bool defined=false;
}note;

class student{
public:
    student();

    ~student();

    bool get_dep(const QString& name);

    double get_note(const QString& name);

    void set_note(const QString& name, double note);

    ods::Sheet* get_dep_sheet(const QString& name);

    place get_dep_cell(const QString& name);

    void mail_notes(std::ofstream & outfile, QString & referent, QString & email_referent, matiere * tree, const std::string& sentence="");

    void write_mail_notes(std::ofstream & outfile, matiere * tree, std::string offset="");


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
