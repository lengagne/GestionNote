#ifndef ODS_DATE_INFO_HPP_
#define ODS_DATE_INFO_HPP_

#include "err.hpp"
#include "global.hxx"

namespace ods	{ // ods::

namespace dateinfo { // ods::dateinfo::

enum class Order : quint8 {
	DAY_MONTH_YEAR,
	MONTH_DAY_YEAR,
	YEAR_DAY_MONTH,
	YEAR_MONTH_DAY
};

enum class Separator : quint8 {
	DOT,
	COMMA,
	FWD_SLASH
};

} // ods::dateinfo::

class ODS_API DateInfo
{
public:
	DateInfo();
	virtual ~DateInfo();

	void
	CopyFrom(const DateInfo&);

	const QString&
	data_style_name() const { return data_style_name_; }

	void
	data_style_name_set(const QString &s) { data_style_name_ = s; }

	const QString&
	day_number_style() const { return day_number_style_; }

	bool
	Equals(const ods::DateInfo &t) const;

	const QString&
	month_number_style() const { return month_number_style_; }

	const QString&
	name() const { return name_; }

	ods::dateinfo::Order
	order() const { return order_; }

	void
	order_set(const ods::dateinfo::Order &r) { order_ = r; }

	ods::dateinfo::Separator
	separator() const { return separator_; }

	void
	separator_set(const ods::dateinfo::Separator &r) { separator_ = r; }

	QString
	SeparatorAsString() const;

	const QString&
	year_number_style() const { return year_number_style_; }

private:
	NO_ASSIGN_COPY_MOVE(DateInfo);

	QString		data_style_name_;
	QString		day_number_style_ = "long";
	QString		month_number_style_ = "long";
	QString		year_number_style_ = "long";
	QString		name_;
	ods::dateinfo::Order	order_ = ods::dateinfo::Order::DAY_MONTH_YEAR;
	ods::dateinfo::Separator	separator_ = ods::dateinfo::Separator::DOT;
};

} // ods::

#endif
