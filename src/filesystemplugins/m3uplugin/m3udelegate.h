#ifndef M3UDELEGATE_H_
#define M3UDELEGATE_H_

#include "m3uplugin_ns.h"
#include "../../filesystem/filesystemdelegate.h"


M3U_PLUGIN_NS_BEGIN

class M3uDelegate : public Delegate
{
	Q_DISABLE_COPY(M3uDelegate)

public:
	M3uDelegate(QObject *parent = 0)
	{}
};

M3U_PLUGIN_NS_END

#endif /* M3UDELEGATE_H_ */