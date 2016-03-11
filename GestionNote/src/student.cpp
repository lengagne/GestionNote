#include "student.hpp"


student::student()
{

}

student::~student()
{

}

void student::print( )
{
    std::cout<<name_.toStdString()<<"\t"<< first_name_.toStdString()<<"\t"<< email_.toStdString();
    if (option_ != "")  std::cout<<"\t"<<option_.toStdString();
    else    std::cout<<"\tNO_OPTION"<<std::endl;
    std::cout<<std::endl;
}

bool student::get_dep(const QString& name)
{
    for (int i=0;i<notes_.size();i++)
        if (notes_[i].name == name)
            return true;
    return false;
}

ods::Sheet* student::get_dep_sheet(const QString& name)
{
    for (int i=0;i<notes_.size();i++)
        if (notes_[i].name == name)
            return notes_[i].sheet;
}
place student::get_dep_cell(const QString& name)
{
    for (int i=0;i<notes_.size();i++)
        if (notes_[i].name == name)
            return  notes_[i].cell;
    place out;
    out.row = -1;
    out.col = -1;
    return out;
}

void student::set_cell( const place& p ,
                        ods::Sheet * s,
                        QString name)
{
    //std::cout<<" l'etudiant "<< name_.toStdString()<<" recoit sa note de "<< name.toStdString()<< " sheet = "<< s <<" place = "<< p.col<<" "<<p.row<<std::endl;
    note tmp;
    tmp.name = name;
    tmp.sheet = s;
    tmp.cell = p;

    notes_.push_back(tmp);
}
