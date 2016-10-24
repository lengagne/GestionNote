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

double student::get_note(const QString& name)
{
    for (int i=0;i<notes_.size();i++)
        if (notes_[i].name == name)
            return notes_[i].value;
    return -1.0;
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

void student::mail_notes(std::ofstream & outfile, QString & referent, QString & referent_email, matiere* tree, const std::string& alias)
{
    outfile << "echo \"Bonjour "<<first_name_.toStdString()<<" "<< name_.toStdString()<<", \n \n";
    if (alias !="")
    {
        outfile <<"\nJe viens de recevoir les notes de "<<alias<<". Vous avez obtenu : "<< get_note( QString::fromUtf8(alias.c_str()))<<" \n";
    }
    outfile <<"Je vous joins l'état actuel de vos notes \n";
    write_mail_notes(outfile, tree,"");

    outfile <<"\n \nBonne journée \n";
    outfile << referent.toStdString() <<"\n \n";
    outfile <<"---- Mail généré automatiquement par GestionNote :  https://github.com/lengagne/GestionNote ---\n ";
    outfile << " \" | mutt ";
    outfile<<" -s \"[GE4A] Fichier pour notes GE4A\" -c "<<referent_email.toStdString() <<" "<< email_.toStdString()<<" ";

    outfile<<"  "<<std::endl<<std::endl;
}

void student::write_mail_notes(std::ofstream & outfile, matiere * tree, std::string offset)
{
    for (int j=0;j<notes_.size();j++)   if (notes_[j].name == tree->alias_)
    {
        outfile << offset << notes_[j].name.toStdString()<<" : "<< notes_[j].value <<" \\n";
    }
    for (int i=0;i<tree->dep_matiere_.size();i++)
    {
        write_mail_notes(outfile,tree->dep_matiere_[i],offset+"\t");
    }
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
