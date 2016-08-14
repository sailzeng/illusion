#include "biko_predefine.h"


void illusion::process_tips(QString &tip_info, QStringList &tips_ary)
{
	tips_ary.append(tip_info);
	fprintf(stdout, "%s\n",tip_info.toLocal8Bit().toStdString().c_str());
}