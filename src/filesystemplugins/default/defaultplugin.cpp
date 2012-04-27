#include "defaultplugin.h"
#include "defaultfoldernode.h"


DEFAULT_PLUGIN_NS_BEGIN

Plugin::Plugin()
{}

void Plugin::registered()
{}

Node *Plugin::node(const IFileContainer *container, const IFileInfo *file, Node *parent) const
{
	IFileContainer::Holder folder(container->open(file->fileName(), false, m_error));

	if (folder)
		return new FolderNode(folder, parent);
	else
		return NULL;
}

DEFAULT_PLUGIN_NS_END