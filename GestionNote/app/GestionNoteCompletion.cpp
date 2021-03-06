//      GestionNoteCompletion.cpp
//      Copyright (C) 2012 lengagne (lengagne@gmail.com)
//
//      This program is free software: you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation, either version 3 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//      This program was developped in the following labs:
//	from 2013 : Université Blaise Pascal / axis : ISPR / theme MACCS

#include <iostream>
#include <string>
#include <QtXml>
#include "structure_cours.hpp"

int main(int argc, char* argv[])
{
//    std::cout<<"argc="<<argc<<std::endl;
	if(QString(argv[argc-1]) != "args")
	{
		std::cerr<<"Error in the completion file"<<std::endl;
		exit(0);
	}
	if (argc == 2)
    {
        std::cout<<"help new show_tree_lesson show_students show_prof send_mail_all_profs send_mail_all_students send_mail_student import_note export_rattrapage "<<std::endl;
    }else if (argc ==3)
    {
        QString command(argv[1]);
        if(command == "show_tree_lesson" || command == "show_prof")
        {
            std::cout<<"file: xml"<<std::endl;
        }else if(command == "show_students")
        {
            std::cout<<"file: ods"<<std::endl;
        }else if(command == "new")
        {
            std::cout<<" file: xml"<<std::endl;
        }else if(command == "send_mail_student")
        {
            structure_cours cours;
            cours.read_project();
            cours.print_students_name();
        }else if(command == "import_note")
        {
            structure_cours cours;
            cours.read_project();
            cours.print_cours();
        }
    }else if (argc ==4)
    {
        QString command(argv[1]);
        if(command == "new")
        {
            std::cout<<" file: ods"<<std::endl;
        }
    }else if (argc ==5)
    {
        QString command(argv[1]);
        if(command == "new")
        {
            std::cout<<" file: xml "<<std::endl;
        }
    }else if (argc ==6)
    {
        QString command(argv[1]);
        if(command == "new")
        {
            std::cout<<" output_files "<<std::endl;
        }
    }
    else
    {
        std::cout<<" ERROR_IN_COMPLETION "<<std::endl;
    }
// 		std::cout<<"new robot environment problem copy export_all plugins create_result_file"<<std::endl;


/*	else
	{
		mogs_string command(argv[1]);
		if( command == "copy")
		{
			if (argc == 3)
				std::cout<<"file: xml"<<std::endl;
			if (argc == 4)
				std::cout<<" project.xml"<<std::endl;
		}else if (command == "open")
		{
			if (argc == 3 || argc == 7)
				std::cout<<"file: xml"<<std::endl;
			if (argc == 4)
				std::cout<<"add rm solve "<<std::endl;
			if (argc == 5)
			{
				mogs_string cmd = argv[3];
				if (cmd == "add" || cmd =="rm")
					std::cout<<"robot problem"<<std::endl;
				else if (cmd == "solve");
					std::cout<<"problem"<<std::endl;
			}
			if (argc == 6)
			{
				mogs_string cmd = argv[3];
				mogs_string type = argv[4];
// 				qDebug()<<cmd;
				if (cmd == "add")
					std::cout<<"name?"<<std::endl;
				else if (cmd == "rm" && type =="robot")
				{
					MogsProject project;
					project.read_project(argv[2]);
					project.print_robots();
				}else if (cmd == "rm" && type =="problem")
				{
					MogsProject project;
					project.read_project(argv[2]);
					project.print_problem();
				}else if (cmd == "solve" && type =="problem")
				{
					MogsProject project;
					project.read_project(argv[2]);
					project.print_problem();
				}
			}

		}else if (command == "plugins" && argc == 3)
		{
			std::cout<<"show"<<std::endl;
		}

	}
	if (argc == 2)
// 		std::cout<<"new robot environment problem copy export_all plugins create_result_file"<<std::endl;
		std::cout<<"new copy open help plugins "<<std::endl;
	else
	{
		mogs_string command(argv[1]);
		if( command == "copy")
		{
			if (argc == 3)
				std::cout<<"file: xml"<<std::endl;
			if (argc == 4)
				std::cout<<" project.xml"<<std::endl;
		}else if (command == "open")
		{
			if (argc == 3 || argc == 7)
				std::cout<<"file: xml"<<std::endl;
			if (argc == 4)
				std::cout<<"add rm solve "<<std::endl;
			if (argc == 5)
			{
				mogs_string cmd = argv[3];
				if (cmd == "add" || cmd =="rm")
					std::cout<<"robot problem"<<std::endl;
				else if (cmd == "solve");
					std::cout<<"problem"<<std::endl;
			}
			if (argc == 6)
			{
				mogs_string cmd = argv[3];
				mogs_string type = argv[4];
// 				qDebug()<<cmd;
				if (cmd == "add")
					std::cout<<"name?"<<std::endl;
				else if (cmd == "rm" && type =="robot")
				{
					MogsProject project;
					project.read_project(argv[2]);
					project.print_robots();
				}else if (cmd == "rm" && type =="problem")
				{
					MogsProject project;
					project.read_project(argv[2]);
					project.print_problem();
				}else if (cmd == "solve" && type =="problem")
				{
					MogsProject project;
					project.read_project(argv[2]);
					project.print_problem();
				}
			}

		}else if (command == "plugins" && argc == 3)
		{
			std::cout<<"show"<<std::endl;
		}

	}*/

	return 1;
}
