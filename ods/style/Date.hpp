#ifndef ODS_STYLE_DATE_HPP_
#define ODS_STYLE_DATE_HPP_

#include "../decl.hxx"
#include "../err.hpp"
#include "../i18n.hxx"
#include "../ns.hxx"
#include "../ods.hxx"
#include "../tag.hxx"

namespace ods	{ // ods::
namespace style	{ // ods::style::

class ODS_API Date
{
public:
	Date(ods::Book*, const ods::StylePlace);
	virtual ~Date();

	void
	AddDay();

	void
	AddMonth();

	void
	AddSeparator(const qint8 id);

	void
	AddYear();

	const ods::DateInfo*
	info() const { return info_; }

	const QString&
	name() const { return name_; }

	void
	SetInfo(const ods::DateInfo &info);

	ods::Tag*
	tag() const { return tag_; }

private:
	NO_ASSIGN_COPY_MOVE(Date);

	ods::Tag*
	GetTag(ods::tag::func f, const qint8 id_num = -1);

	void
	Init();

	void
	SetUniqueName();

	ods::Book			*book_ = nullptr;
	ods::DateInfo		*info_ = nullptr;
	QString				name_;
	ods::StylePlace		place_;
	ods::Tag			*tag_ = nullptr;
};

} // ods::style::
} // ods::

#endif
