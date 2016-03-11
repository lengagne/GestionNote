#ifndef STRUCTURE_COURS_HPP_
#define STRUCTURE_COURS_HPP_

#include <iostream>
#include "util.hpp"
#include <QtXml>
#include "Item.hpp"
#include "matiere.hpp"
#include "student.hpp"
#include <ods/ods>


class structure_cours
{
public:
	structure_cours();

	~structure_cours();

//	void add_matiere(QString prof, QString matiere);

    void create_project( const QString & cours_xml,
                         const QString& student_ods,
                         const QString& output = "GestionNoteOutput");

    void print_students();

	void print_tree();

    bool read_project();


	void read_student(QString ods);

	void read_xml( QString input);

private:

    void add_main_sheet_line( matiere * tree, ods::Row * r, int level, ods::Cell* previous);

	void create_files();

    void create_main_sheet(  );

    void create_sub_sheet( matiere * m );

	bool find_cell(int * col, int * row, QString name, QString ods_file);

	unsigned int get_biggest_level();



    // tries to read a project else recommend to create one



    // return the master and make the tree
    matiere* get_master_of_tree();



private:
	std::vector<matiere> liste_cours;
	std::vector<student> liste_etudiant;
	matiere* tree_matiere_;

	ods::Book book_master_;

	QString cours_xml_;
	QString student_ods_;
	QString output_;

	ods::Sheet *main_sheet_;
};


#endif
