#ifndef FILESYSTEMITEMSCONTAINER_H_
#define FILESYSTEMITEMSCONTAINER_H_

#include <QtCore/QSet>
#include "../base/items/filesystemfolderbaseitem.h"
#include "../../model/filesystemnodemodelcontainer.h"
#include "../../../tools/containers/hashedlist.h"


FILE_SYSTEM_NS_BEGIN

class ItemsContainer : public NodeModelContainer
{
public:
	typedef FolderBaseItem * value_type;

public:
	ItemsContainer();
	virtual ~ItemsContainer();

	virtual size_type size() const;
	virtual Item *at(size_type index) const;
	virtual size_type indexOf(Item *item) const;

	const value_type &operator[](size_type index) const { return m_container.at(index); }

	const value_type &last() const { return m_container.last(); }
	value_type &last() { return m_container.last(); }

	bool isEmpty() const { return m_container.isEmpty(); }
	size_type lastIndex() const { return m_container.size() - 1; }
	size_type indexOf(Node *item) const
	{
		for (List::size_type i = 0, size = m_container.size(); i < size; ++i)
			if (m_container.at(i)->node() == item)
				return i;

		return InvalidIndex;
	}
	size_type indexOf(const QString &fileName) const { return m_container.indexOf(fileName); }

	void add(FolderBaseItem *item) { m_container.add(item->info().fileName(), item); }
	void remove(size_type index) { delete m_container.take(index); }
	value_type take(size_type index) { return m_container.take(index); }
	void replace(size_type index, const QString &oldHash, const QString &newHash) { m_container.replace(index, oldHash, newHash); }
	void clear()
	{
		List::ByIndex list(m_container);
		value_type root = list.at(0);

		for (List::size_type i = 1, size = list.size(); i < size; ++i)
			delete list.at(i);

		m_container.clear();
		m_container.add(root->info().fileName(), root);
	}

private:
	typedef ::Tools::Containers::HashedList<QString, value_type> List;

private:
	List m_container;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMITEMSCONTAINER_H_ */