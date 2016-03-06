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

#include "sample.hpp"
#include "structure_cours.hpp"

#include <ods/ods>
#include <QtCore>
#include <iostream>

void print_help()
{
	std::cout<<"aide"<<std::endl;
	std::cout<<"générer fichier note :    executable create_files liste_etudiants.ods liste_enseignements.xml"<<std::endl;
	std::cout<<"générer fichier note :    executable import liste_enseignements.xml main.ods responsable.ods"<<std::endl;
	
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	
	qDebug().nospace() << "QOds version: " << ods::version_major() << "." << ods::version_minor() << "." << ods::version_micro();
	
	std::cout<<"il y a "<< argc <<" paramètres" <<std::endl;
		
	if (argc==4)
	{
		QString command(argv[1]);
		if (command == "create_files")
		{
			
			QString ods(argv[2]);
			QString xml(argv[3]);
			app::sample toto;
			
			
			structure_cours cours;
			cours.read_xml(xml);
			cours.read_student(ods);
			cours.create_file();

			return 1;
		}
	}
	print_help();
	
	
	return 0;
}


