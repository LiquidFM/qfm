#ifndef SERVICES_H_
#define SERVICES_H_

#include <QtCore/QString>
#include "../filesysteminfo/filesysteminfo.h"


class Services
{
public:
	class Service
	{
	public:
		typedef void *  File;
		typedef quint64 size_type;

	public:
		virtual ~Service() {}
	};

	class View : public Service
	{
	public:
		virtual void view(const File file, size_type size) = 0;
	};

	class Edit : public Service
	{
	public:
		virtual void edit(const QString &filePath) = 0;
	};

	class Open : public Service
	{
	public:
		virtual void open(const File file, size_type size) = 0;
	};


public:
	Services();

	void view(const FileSystemInfo &file);
	void edit(const FileSystemInfo &file);
	void open(const FileSystemInfo &file);

public:
	static Services *instance();
	static void registerService(View *service);
	static void registerService(Edit *service);
	static void registerService(Open *service);
};

#endif /* SERVICES_H_ */