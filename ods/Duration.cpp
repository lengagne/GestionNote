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

#include "Duration.hpp"
#include "err.hpp"

#include <QtCore/QDebug>
#include <QtCore/QStringRef>

namespace ods	{

Duration::Duration()
{}

Duration::Duration(const qint32 h, const qint32 m, const qint32 s)
	: hours_(h), minutes_(m), seconds_(s)
{}

Duration::~Duration()
{}

void
Duration::Decode(const QString &str)
{
	int last_offset = 1;
	int i = last_offset+1;
	bool is_month = true;
	bool ok;

	for (; i <= str.size(); i++) {

		QStringRef s(&str, last_offset, i - last_offset);
		int n = s.toInt(&ok);
		if (ok)
			continue;
		s = QStringRef(&str, last_offset, i - last_offset - 1);
		n = s.toInt(&ok);
		last_offset = i;
		const QChar c = str.at(i - 1);

		if (c == 'Y') {
			years_ = n;
		} else if (c == 'M') {
			if (is_month) {
				months_ = n;
			} else {
				minutes_ = n;
			}
		} else if (c == 'D') {
			days_ = n;
		} else if (c == 'H') {
			hours_ = n;
		} else if (c == 'S') {
			seconds_ = n;
		} else if (c == 'T') {
			is_month = false;
		} else {
			mtl_warn("unknown char");
			return;
		}
	}
	valid_ = true;
}

Duration::Duration(const ods::Duration &rhs)
{
	years_set(rhs.years());
	months_set(rhs.months());
	days_set(rhs.days());
	hours_set(rhs.hours());
	minutes_set(rhs.minutes());
	seconds_set(rhs.seconds());
	valid_set(rhs.valid());
}

Duration::operator QString()
{
	QString s = "years: ";
	s += QString::number(years_);
	s += ", months: ";
	s += QString::number(months_);
	s += ", days: ";
	s += QString::number(days_);
	s += ", hours: ";
	s += QString::number(hours_),
	s += ", minutes: ";
	s += QString::number(minutes_);
	s += ", seconds: ";
	s += QString::number(seconds_);
	s += ", valid: ";
	s += (valid_ ? "true" : "false");
	return s;
}

QString
Duration::ToString() const
{
	QString str = "PT";
	str += QString::number(hours_);
	str += 'H';
	str += QString::number(minutes_);
	str += 'M';
	str += QString::number(seconds_);
	str += 'S';
	return str;
}

} // ods::
