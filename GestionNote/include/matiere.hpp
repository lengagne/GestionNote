#ifndef MATIERE_HPP_
#define MATIERE_HPP_

#include <iostream>
#include "util.hpp"
#include <QtXml>

class matiere{
public:
    matiere();

    ~matiere();

    void print(const std::string & coeff ="",const std::string & offset = "");

    QString name_;
    QString alias_;
    QString apogee_;
    QString referent_;
    QString option_;
    std::vector<double> coeff_;
    std::vector<matiere*> dep_matiere_;
    std::vector<QString> dep_matiere_name_;
    unsigned int tree_level_;
    bool must_validate_;
};


#endif
