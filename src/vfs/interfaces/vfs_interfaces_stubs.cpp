#include "vfs_inode.h"
#include "vfs_iplugin.h"
#include "vfs_inodeview.h"
#include "vfs_ifileinfo.h"
#include "vfs_ifiletype.h"
#include "vfs_iapplications.h"
#include "vfs_ifilecontainer.h"
#include "vfs_ifileoperations.h"
#include "vfs_ifilenavigation.h"
#include "vfs_ifilecontainerscanner.h"

VFS_NS_BEGIN

INode::~INode()
{}

IPlugin::~IPlugin()
{}

INodeView::~INodeView()
{}

IFileInfo::~IFileInfo()
{}

IFileType::~IFileType()
{}

IApplication::~IApplication()
{}

IApplications::~IApplications()
{}

IFileContainer::~IFileContainer()
{}

IFileOperations::~IFileOperations()
{}

IFileNavigation::~IFileNavigation()
{}

IFileContainerScanner::Filter::~Filter()
{}

IFileContainerScanner::~IFileContainerScanner()
{}

IFileContainerScanner::IEnumerator::~IEnumerator()
{}

VFS_NS_END