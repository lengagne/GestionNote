#include "profs.hpp"

profs::profs()
{

}

profs::~profs()
{

}

void profs::print( )
{
    qDebug()<<"Enseignant : "<< first_name_ <<" "<<name_;
    for (int i=0;i<matieres_.size();i++)
        qDebug()<<"\t "<< matieres_[i]->alias_ <<" : "<<matieres_[i]->name_;
}
