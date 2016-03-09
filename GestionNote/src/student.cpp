#include "student.hpp"


student::student()
{

}

student::~student()
{

}

void student::print( )
{
    std::cout<<name_.toStdString()<<"\t"<< first_name_.toStdString()<<"\t"<< email_.toStdString()<<std::endl;
}

