#ifndef PROFS_HPP_
#define PROFS_HPP_

#include <iostream>
#include "matiere.hpp"
#include <QtXml>


class profs{
public:
    profs();

    ~profs();

    void print( );

    QString name_;
    QString first_name_;
    QString email_;
    std::vector<matiere*> matieres_;

};


#endif
