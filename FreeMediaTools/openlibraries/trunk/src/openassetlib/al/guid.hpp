#ifndef GUID_INC_
#define GUID_INC_

#include <iostream>
#include <boost/regex.hpp>
#include <openpluginlib/pl/openpluginlib.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>
#include <openassetlib/al/config.hpp>

#ifdef _MSC_VER
#	pragma warning ( push )
#	pragma warning ( disable: 4251 )
#endif

namespace opl = olib::openpluginlib;

namespace olib { namespace openassetlib { 
	
	struct AL_DECLSPEC guid_generator
	{
		virtual ~guid_generator() {};
		virtual const opl::string generate_guid() = 0;
	};

	typedef boost::shared_ptr<guid_generator> guid_generator_ptr;

	namespace al {

		typedef boost::shared_ptr<class guid> guid_ptr;

		class AL_DECLSPEC guid_factory
		{
		public:
			static void				initialize(guid_generator_ptr guid_gen_ptr);
			static void				uninitialize();
			static guid_factory&	instance();

			guid_ptr	create_guid();
			guid_ptr	create_guid(const opl::string& guid_string);
			guid_ptr	create_guid(long long upper, long long lower);

			void		housekeep();  // removes objects in container that no longer have any other references

		private:
			static guid_generator_ptr	guid_gen_ptr_;
			static guid_factory*		instance_;

			guid_factory(void);
			~guid_factory(void);

			guid_ptr	create_guid_object(const opl::string& guid_string);

			typedef std::vector<guid_ptr> guid_container_t;
			guid_container_t	guid_container_;
			unsigned short		count_;
		};

		class AL_DECLSPEC guid
		{
		public:
			~guid();

			long long	upper() const;
			long long	lower() const;
			opl::string	string() const;
			bool		operator==(const guid& rhs) const;
			bool		operator!=(const guid& rhs) const;

		private:
			friend class guid_factory;

			guid(long long upper, long long lower);
			guid(const opl::string& guid_string);

			long long	upper_;
			long long	lower_;

			opl::string guid_string_; 				// must be in this format:  {%08x-%04x-%04x-%04x-%012x}
		};
	}
}}

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif

#endif
