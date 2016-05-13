#ifndef STRUCTURE_COURS_HPP_
#define STRUCTURE_COURS_HPP_

#include <iostream>
#include "util.hpp"
#include <QtXml>
#include "profs.hpp"
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

    void import_note(QString& alias_matiere);

    void print_cours();

    void print_profs();

    void print_students();

    void print_students_name();

	void print_tree();

	void read_ods();

    void read_notes(student &e , matiere & m, ods::Book& book_in);

    bool read_project();

	void read_student(QString ods);

	void read_xml( QString input);

	void send_mail_profs();

	void send_mail_student(QString & name);

	void send_mail_students(const std::string & alias = "");

    void set_notes(student &e , QString & m, ods::Book& book_in, double note_value);

    std::string get_output()const{ return output_.toStdString();}

private:

    void add_main_sheet_line( ods::Book& book,  matiere * tree,ods::Row * r, int level, ods::Cell* previous);

	void create_files();

    void create_main_sheet( ods::Book& book );

    void create_sub_sheet( ods::Book& book, matiere * m );

    QString get_matiere_from_col(unsigned int i);

    bool find_cell(int * COL, int * ROW, QString student, QString matiere,ods::Sheet * s);

	bool find_cell(int * COL, int * ROW, QString name,ods::Sheet * s, int count = 1);

	bool find_cell(int * col, int * row, QString name, QString ods_file, int count = 1);

	unsigned int get_biggest_level();

	bool get_cell_value( ods::Sheet * sheet, const place & p, double * out);

    // tries to read a project else recommend to create one

    // return the master and make the tree
    matiere* get_master_of_tree();



private:
	std::vector<matiere> liste_cours;
	std::vector<profs> liste_profs;
	std::vector<student> liste_etudiant;
	matiere* tree_matiere_;

	QString cours_xml_;
	QString student_ods_;
	QString output_;

	QString referent_;
	QString email_;

	unsigned int max_level_;

	ods::Sheet *main_sheet_;
};

#endif
