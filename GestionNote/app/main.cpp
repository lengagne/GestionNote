/*
 * Copyright © 2014 f35f22fan@gmail.com
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of the copyright holders not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no representations
 * about the suitability of this software for any purpose.  It is provided "as
 * is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

// #include "examples1.hpp"
// #include "examples2.hpp"

#include "structure_cours.hpp"

#include <ods/ods>
#include <QtCore>
#include <iostream>

// TODO : envoie par email des fichiers vides aux profs concernés
// TODO : import des notes depuis les fichiers dans le ods général
// TODO : envoie des notes aux étudiants (par matière et/ou global)
// TODO : gérer les couleurs en cas de choses non validées
// TODO : ajouter case pour points jury

void print_help()
{
	std::cout<<"aide"<<std::endl;
	std::cout<<"afficher arborescence cours :    GestionNote show_tree_lesson *.xml "<<std::endl;
	std::cout<<"afficher etudiant :    GestionNote show_students *.ods "<<std::endl;
	std::cout<<"afficher professeur :    GestionNote show_prof *.xml "<<std::endl;
	std::cout<<"Préparer mail aux professeurs :    GestionNote send_mail_all_profs "<<std::endl;
	std::cout<<"Préparer mail aux étudiants : GestionNote send_mail_all_students"<<std::endl;
	std::cout<<"Préparer mail à un étudiant : GestionNote send_mail_student student_name"<<std::endl;
	std::cout<<"Importer notes :  GestionNote import_note note_alias"<<std::endl;
	std::cout<<"Créer nouveau projet :  GestionNote new lessons.xml student.ods redoublement.xml output"<<std::endl;

}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

    if (argc == 2)
    {
        QString command(argv[1]);
        if (command == "help")
        {
            print_help();
            return 1;
        }else if ( command =="send_mail_all_profs")
        {
            structure_cours cours;
            cours.read_project();
            cours.send_mail_profs();
            return 1;
        }else if (command =="show_students")
        {
            QString file(argv[2]);
            structure_cours cours;
            cours.read_project();
            cours.print_students_name();
            return 1;
        }else if (command =="send_mail_all_students")
        {
            structure_cours cours;
            cours.read_project();
            cours.send_mail_students( );
            return 1;
        }else if (command =="export_rattrapage")
        {
            std::cout<<" export_rattrapage "<<std::endl;
            structure_cours *cours = new structure_cours;
            cours->read_project();
            cours->show_rattrapage();

//            cours->import_note(matiere);
//            std::string command =  "libreoffice "+cours->get_output()+".ods";
            delete cours;

            return 1;
        }
    }else if (argc == 3)
    {
        QString command(argv[1]);
        qDebug()<<" command = "<< command;
        if (command =="show_tree_lesson")
        {
            QString file(argv[2]);
            structure_cours cours;
			qDebug()<<"Read xml";
            cours.read_xml(file);
			qDebug()<<"Print tree";
            cours.print_tree();

            return 1;
        }else if (command =="show_students")
        {
            QString file(argv[2]);
            structure_cours cours;
            cours.read_student(file);
            cours.print_students();
            return 1;
        }else if (command =="show_prof")
        {
            QString file(argv[2]);
            structure_cours cours;
            cours.read_xml(file);
            cours.print_profs();
            return 1;
        }else if (command =="send_mail_student")
        {
            QString student(argv[2]);
            structure_cours cours;
            cours.read_project();
            cours.send_mail_student(student);
            return 1;
        }else if (command =="import_note")
        {

            QString matiere(argv[2]);
			std::cout<<"Importing note of "<< matiere.toStdString() <<std::endl;
            structure_cours *cours = new structure_cours;
            cours->read_project();
            cours->import_note(matiere);
            std::string command =  "libreoffice "+cours->get_output()+".ods";
            delete cours;
            std::cout<<"Pour que les notes soient à jour il faut ré enregistrer (avec une modif qui sert à rien)"<<std::endl;
            std::cout<<"Etes vous prêt ?"<< std::endl;
            getchar();
            int dummy = system(command.c_str());

            structure_cours cours2;
            cours2.read_project();
            cours2.send_mail_students( matiere.toStdString() );
            return 1;
        }
/*    }
    else if (argc==4)
	{
		QString command(argv[1]);

        if (command == "new")
		{
			QString xml(argv[2]);
			QString ods(argv[3]);

			structure_cours cours;
			cours.create_project(xml,ods);

            std::string cmd =  "libreoffice "+ods.toStdString()+".ods";
            std::cout<<"Pour que le fichier soit opérationnel il faut l'ouvrir et le ré enregistrer (avec une modif qui sert à rien)"<<std::endl;
            std::cout<<"Etes vous prêt ?"<< std::endl;
            getchar();
            int dummy = system(cmd.c_str());

			return 1;
		}*/
    }else if (argc==6)
	{
		QString command(argv[1]);

        if (command == "new")
		{
			QString xml(argv[2]);
			QString ods(argv[3]);
			QString xml_redoublement(argv[4]);
			QString out(argv[5]);

			structure_cours cours;
			cours.create_project(xml,xml_redoublement,ods,out);

			return 1;
		}
	}
	print_help();
	return 0;
}


