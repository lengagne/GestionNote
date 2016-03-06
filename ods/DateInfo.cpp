#include "DateInfo.hpp"

namespace ods	{ // ods::

DateInfo::DateInfo()
{}

DateInfo::~DateInfo()
{}

void
DateInfo::CopyFrom(const DateInfo &r)
{
	data_style_name_ = r.data_style_name();
	order_ = r.order();
	separator_ = r.separator();
}

bool
DateInfo::Equals(const ods::DateInfo &r) const
{
	if (data_style_name_ != r.data_style_name())
		return false;
	if (day_number_style_ != r.day_number_style())
		return false;
	if (month_number_style_ != r.month_number_style())
		return false;
	if (year_number_style_ != r.year_number_style())
		return false;
	if (separator_ != r.separator())
		return false;
	return true;
}

QString
DateInfo::SeparatorAsString() const
{
	if (separator_ == ods::dateinfo::Separator::DOT)
		return QLatin1String(".");
	else if (separator_ == ods::dateinfo::Separator::FWD_SLASH)
		return QLatin1String("/");
	return QLatin1String(",");
}

} // ods::
