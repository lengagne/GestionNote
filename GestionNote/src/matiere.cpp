#include "matiere.hpp"


matiere::matiere()
{

}

matiere::~matiere()
{

}

void matiere::print(const std::string & coeff,
                    const std::string & offset)
{
    std::cout<<offset<< coeff<< name_.toStdString()  <<" ("<<alias_.toStdString()<<")"<<std::endl;
    for (int i=0;i<dep_matiere_.size();i++)
        dep_matiere_[i]->print( "coeff (" + std::to_string(coeff_[i])+") ",offset+"\t");
}
