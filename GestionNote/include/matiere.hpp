#ifndef MATIERE_HPP_
#define MATIERE_HPP_

#include <iostream>
#include "util.hpp"
#include <QtXml>

class matiere{
public:
    matiere();

    ~matiere();

    int get_nb_dep();

    void print(const std::string & coeff ="",const std::string & offset = "");

    void update_dads();

    unsigned int update_col(unsigned int c=0);

    void update_level(unsigned int l = 0);

    QString name_;
    QString alias_;
    QString apogee_;
    QString referent_;
    QString option_;
    std::vector<double> coeff_;
    std::vector<matiere*> dads_;
    std::vector<matiere*> dep_matiere_;
    std::vector<QString> dep_matiere_name_;
    unsigned int tree_level_;
    bool must_validate_;

    unsigned int col_debut_, col_fin_;

    ods::Sheet* sheet_;
};


#endif
