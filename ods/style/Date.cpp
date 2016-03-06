#include "Date.hpp"

#include "../Book.hpp"
#include "../DateInfo.hpp"
#include "../Ns.hpp"
#include "Manager.hpp"
#include "style.hxx"
#include "tag.hh"
#include "../Tag.hpp"

namespace ods	{ // ods::
namespace style	{ // ods::style::

Date::Date(ods::Book *book,
	const ods::StylePlace place) :
	book_(book),
	place_(place)
{
	auto &ns = (place == ods::StylePlace::ContentFile) ?
		book_->content()->ns() : book_->style_manager()->ns();
	tag_ = ods::style::tag::NumberDateStyle(ns, nullptr);
	Init();
}

Date::~Date()
{
	if (info_ != nullptr)
		delete info_;
}

void
Date::AddDay()
{
	auto *day_tag = GetTag(ods::style::tag::NumberDay);
	day_tag->AttrSet(tag_->ns().number(), ods::ns::kStyle, info_->day_number_style());
}

void
Date::AddMonth()
{
	auto *day_tag = GetTag(ods::style::tag::NumberMonth);
	day_tag->AttrSet(tag_->ns().number(), ods::ns::kStyle, info_->month_number_style());
}

void
Date::AddSeparator(const qint8 id)
{
	auto *sep_tag1 = GetTag(ods::tag::NumberText, id);
	const QString separator = info_->SeparatorAsString();
	sep_tag1->SubnodeAdd(new ods::Node(separator));
}

void
Date::AddYear()
{
	auto *day_tag = GetTag(ods::style::tag::NumberYear);
	day_tag->AttrSet(tag_->ns().number(), ods::ns::kStyle, info_->year_number_style());
}

ods::Tag*
Date::GetTag(ods::tag::func f, const qint8 id_num)
{
	auto *tag = tag_->GetSubtag(f, id_num);
	if (tag != nullptr)
		return tag;
	tag = f(tag_->ns(), nullptr);
	tag->id_num_set(id_num);
	tag_->SubtagAdd(tag);
	return tag;
}

void
Date::Init()
{
	SetUniqueName();
}

void
Date::SetInfo(const ods::DateInfo &info)
{
	if (info_ == nullptr)
		info_ = new ods::DateInfo();
	info_->CopyFrom(info);

	/**
		<number:date-style style:name="N37">
			<number:day number:style="long"/>
			<number:text>.</number:text>
			<number:month number:style="long"/>
			<number:text>.</number:text>
			<number:year number:style="long"/>
		</number:date-style>
	**/

	/**
		enum class Order : quint8 {
			DAY_MONTH_YEAR,
			MONTH_DAY_YEAR,
			YEAR_DAY_MONTH,
			YEAR_MONTH_DAY
		};
	**/
	auto order = info_->order();
	if (order == ods::dateinfo::Order::DAY_MONTH_YEAR)
		AddDay();
	else if (order == ods::dateinfo::Order::MONTH_DAY_YEAR)
		AddMonth();
	else
		AddYear();

	AddSeparator(0);

	if (order == ods::dateinfo::Order::DAY_MONTH_YEAR ||
		order == ods::dateinfo::Order::YEAR_MONTH_DAY)
		AddMonth();
	else
		AddDay();

	AddSeparator(1);

	if (order == ods::dateinfo::Order::DAY_MONTH_YEAR ||
		order == ods::dateinfo::Order::MONTH_DAY_YEAR)
		AddYear();
	else if (order == ods::dateinfo::Order::YEAR_DAY_MONTH)
		AddMonth();
	else
		AddDay();
}

void
Date::SetUniqueName()
{
	const QString base = (place_ == ods::StylePlace::StylesFile) ?
		QStringLiteral("date_st") : QStringLiteral("date_cn");

	int i = 0;
	while (true)
	{
		name_ = base + QString::number(i++);
		if (book_->GetDateStyle(name_) == nullptr)
			break;
	}
	tag_->AttrSet(tag_->ns().style(), ods::ns::kName, name_);
}

} // ods::style::
} // ods::
