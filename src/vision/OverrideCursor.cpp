#include "stdafx.h"
#include "OverrideCursor.h"


OverrideWaitCursor::OverrideWaitCursor()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
}

OverrideWaitCursor::~OverrideWaitCursor()
{
    QApplication::restoreOverrideCursor();
}







