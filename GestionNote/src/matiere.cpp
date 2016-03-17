#include "matiere.hpp"


matiere::matiere()
{
    col_debut_ = -1;
    col_fin_ = -1;
}

matiere::~matiere()
{

}

int matiere::get_nb_dep()
{
    int out = 1;
    for (int i=0;i<dep_matiere_.size();i++)
    {
        out += dep_matiere_[i]->get_nb_dep();
    }
    return out;
}

void matiere::print(const std::string & coeff,
                    const std::string & offset)
{
    //std::cout<<offset<< coeff<< name_.toStdString()  <<" ("<<alias_.toStdString()<<")"<<std::endl;
    for (int i=0;i<dep_matiere_.size();i++)
        dep_matiere_[i]->print( "coeff (" + std::to_string(coeff_[i])+") ",offset+"\t");
}

unsigned int matiere::update_col(unsigned int c)
{

    col_debut_ = c;
//    std::cout<<"\tdebut matiere = "<< alias_.toStdString()<<"\t deb = "<< col_debut_<<"\t fin = "<< col_fin_<<"\t\tc= "<< c<<"\t size = "<< dep_matiere_.size()<<std::endl;
    for (int i=0;i<dep_matiere_.size();i++)
    {
        // if(dep_matiere_[i]->dads_.size() > 0)qDebug()<<dep_matiere_[i]->dads_[0]->alias_  <<" == "<< alias_;

        if(dep_matiere_[i]->dads_.size() > 0 && dep_matiere_[i]->dads_[0]->alias_ == alias_)
        {
//            std::cout<<"Dans le if"<<std::endl;
            if (dep_matiere_[i]->col_debut_== -1)
                c = dep_matiere_[i]->update_col(c+1) ;
        }
    }
    col_fin_ = c;
//    std::cout<<"\tfin  matiere = "<< alias_.toStdString()<<"\t deb = "<< col_debut_<<"\t fin = "<< col_fin_<<"\t\tc= "<< c<<std::endl;
    return col_fin_;
}

void matiere::update_dads()
{
    for (int i=0;i<dep_matiere_.size();i++)
    {
        bool not_found = true;
        for (int j=0;j<dep_matiere_[i]->dads_.size();j++)
            if (dep_matiere_[i]->dads_[j]->alias_ == alias_)
                not_found = false;
        if(not_found)
            dep_matiere_[i]->dads_.push_back(this);

        dep_matiere_[i]->update_dads();
    }
}

void matiere::update_level(unsigned int l)
{
    tree_level_ = l;
    for (int i=0;i<dep_matiere_.size();i++)
        dep_matiere_[i]->update_level(l+1);
}
