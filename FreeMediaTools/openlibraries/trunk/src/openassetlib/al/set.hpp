// set.hpp
//
// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef SET_INC_
#define SET_INC_

#include <vector>
#include <boost/shared_ptr.hpp>
#include <openassetlib/al/asset.hpp>

#ifdef _MSC_VER
#	pragma warning ( push )
#	pragma warning ( disable: 4251 )
#endif

namespace olib { namespace openassetlib { namespace al {

typedef boost::shared_ptr<class set>	set_ptr;

class AL_DECLSPEC set
{
public:
	typedef std::vector<asset_ptr>					asset_vector;
	typedef asset_vector::value_type				value_type;
	typedef asset_vector::iterator					iterator;
	typedef asset_vector::const_iterator			const_iterator;
	typedef asset_vector::reverse_iterator			reverse_iterator;
	typedef asset_vector::const_reverse_iterator	const_reverse_iterator;
	typedef asset_vector::reference					reference;
	typedef asset_vector::const_reference			const_reference;
	typedef asset_vector::size_type					size_type;

	static set_ptr create(const opl::string& name = "");
	static set_ptr create(const asset_ptr& asset, const opl::string& name = "");
	static set_ptr create(const set_ptr& set, const opl::string& name = "");

	virtual ~set(void);

	opl::string		name() const;
	void			name(const opl::string& name);

	const_reference					at(size_type pos) const;
	reference						at(size_type pos);
	const_reference					back() const;
	reference						back();
	const_iterator					begin() const;
	iterator						begin();
	size_type						capacity() const;
	void							clear();
	bool							empty() const;
	const_iterator					end() const;
	iterator						end();
	iterator						erase(iterator where);
	iterator						erase(iterator first, iterator last);
	const_reference					front() const;
	reference						front();
	template<class iterator> void	insert(iterator where, iterator first, iterator last);
	iterator						insert(iterator where, const value_type& val);
	void							insert(iterator where, size_type count, const value_type& val);
	size_type						max_size() const;
	void							pop_back();
	void							push_back(const value_type& val);
	const_reverse_iterator			rbegin() const;
	reverse_iterator				rbegin();
	reverse_iterator				rend();
	const_reverse_iterator			rend() const;
	void							resize(size_type newsize);
	void							resize(size_type newsize, value_type val);
	void							reserve(size_type count);
	size_type						size() const;
	void							swap(set& right);

	// additional helper functions to be exported to python interface
	void remove(const asset_ptr& asset);
	bool insert(const asset_ptr& asset, int offset);
	bool insert(const set_ptr& set, int offset);
	asset_ptr find(const guid_ptr& guid);

private:
	set(const opl::string& name = "");
	set(const asset_ptr asset, const opl::string& name = "");
	set(const set_ptr set, bool deepcopy, const opl::string& name = "");

	opl::string		name_;
	asset_vector	vec_;
};

} } }

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif

#endif //SET_INC_
