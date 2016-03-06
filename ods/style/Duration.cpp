#include "Duration.hpp"

#include "../Book.hpp"
#include "../DurationInfo.hpp"
#include "../Ns.hpp"
#include "Manager.hpp"
#include "style.hxx"
#include "tag.hh"
#include "../Tag.hpp"

namespace ods	{ // ods::
namespace style	{ // ods::style::

Duration::Duration(ods::Book *book,
	const ods::StylePlace place) :
	book_(book),
	place_(place)
{
	auto &ns = (place == ods::StylePlace::ContentFile) ?
		book_->content()->ns() : book_->style_manager()->ns();
	tag_ = ods::style::tag::NumberTimeStyle(ns, nullptr);
	Init();
}

Duration::~Duration()
{
	if (info_ != nullptr)
		delete info_;
}

void
Duration::AddHours()
{
	auto *day_tag = GetTag(ods::style::tag::NumberHours);
	day_tag->AttrSet(tag_->ns().number(), ods::ns::kStyle,
		info_->hours_number_style());
}

void
Duration::AddMinutes()
{
	auto *day_tag = GetTag(ods::style::tag::NumberMinutes);
	day_tag->AttrSet(tag_->ns().number(), ods::ns::kStyle,
		info_->minutes_number_style());
}

void
Duration::AddSeconds()
{
	auto *day_tag = GetTag(ods::style::tag::NumberSeconds);
	day_tag->AttrSet(tag_->ns().number(), ods::ns::kStyle,
		info_->seconds_number_style());
}

void
Duration::AddSeparator(const qint8 id)
{
	auto *sep_tag1 = GetTag(ods::tag::NumberText, id);
	const QString separator = info_->SeparatorAsString();
	sep_tag1->SubnodeAdd(new ods::Node(separator));
}

ods::Tag*
Duration::GetTag(ods::tag::func f, const qint8 id_num)
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
Duration::Init()
{
	SetUniqueName();
}


void
Duration::SetInfo(const ods::DurationInfo &info)
{
	if (info_ == nullptr)
		info_ = new ods::DurationInfo();
	info_->CopyFrom(info);

	/**
	<number:time-style style:name="N40">
	  <number:hours number:style="long"/>
	  <number:text>:</number:text>
	  <number:minutes number:style="long"/>
	</number:time-style>
	**/

	/**
	enum class Order : quint8 {
		HOURS_MINUTES,
		HOURS_MINUTES_SECONDS
	};
	**/
	const auto order = info_->order();
	if (order == ods::duration::Order::HOURS_MINUTES)
	{
		AddHours();
		AddSeparator(0);
		AddMinutes();
	} else if (order == ods::duration::Order::HOURS_MINUTES_SECONDS) {
		AddHours();
		AddSeparator(0);
		AddMinutes();
		AddSeparator(1);
		AddSeconds();
	}

	QString s = info_->truncate_on_overflow() ? "true" : "false";
	tag_->AttrSet(tag_->ns().number(), ods::style::kTruncateOnOverflow, s);
}

void
Duration::SetUniqueName()
{
	const QString base = (place_ == ods::StylePlace::StylesFile) ?
		QLatin1String("duration_st") : QLatin1String("duration_cn");

	int i = 0;
	while (true)
	{
		name_ = base + QString::number(i++);
		if (book_->GetDurationStyle(name_) == nullptr)
			break;
	}
	tag_->AttrSet(tag_->ns().style(), ods::ns::kName, name_);
}

} // ods::style::
} // ods::
