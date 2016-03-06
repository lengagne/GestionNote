#ifndef APP_UTIL_HPP_
#define APP_UTIL_HPP_

#include <ods/ods>

QString
GetCellValue(ods::Cell *cell);

void
Save(ods::Book &book);

#endif
