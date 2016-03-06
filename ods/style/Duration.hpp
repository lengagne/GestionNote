#ifndef ODS_STYLE_DURATION_HPP_
#define ODS_STYLE_DURATION_HPP_

#include "../decl.hxx"
#include "../err.hpp"
#include "../i18n.hxx"
#include "../ns.hxx"
#include "../ods.hxx"
#include "../tag.hxx"

namespace ods	{ // ods::
namespace style	{ // ods::style::

class ODS_API Duration
{
public:
	Duration(ods::Book*, const ods::StylePlace);
	virtual ~Duration();

	void
	AddHours();

	void
	AddMinutes();

	void
	AddSeconds();

	void
	AddSeparator(const qint8 id);

	const ods::DurationInfo*
	info() const { return info_; }

	const QString&
	name() const { return name_; }

	void
	SetInfo(const ods::DurationInfo &info);

	ods::Tag*
	tag() const { return tag_; }

private:
	NO_ASSIGN_COPY_MOVE(Duration);

	ods::Tag*
	GetTag(ods::tag::func f, const qint8 id_num = -1);

	void
	Init();

	void
	SetUniqueName();

	ods::Book			*book_ = nullptr;
	ods::DurationInfo	*info_ = nullptr;
	QString				name_;
	ods::StylePlace		place_;
	ods::Tag			*tag_ = nullptr;
};

} // ods::style::
} // ods::

#endif
