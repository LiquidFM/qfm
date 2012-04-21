#include "services.h"


static Services *selfInstance = 0;

Services::Services()
{
	selfInstance = this;
}

void Services::view(const FileSystemInfo &file)
{

}

void Services::edit(const FileSystemInfo &file)
{

}

void Services::open(const FileSystemInfo &file)
{

}

Services *Services::instance()
{
	Q_ASSERT(selfInstance != 0);
	return selfInstance;
}

void Services::registerService(View *service)
{
	Q_ASSERT(selfInstance != 0);
}

void Services::registerService(Edit *service)
{
	Q_ASSERT(selfInstance != 0);
}

void Services::registerService(Open *service)
{
	Q_ASSERT(selfInstance != 0);
}