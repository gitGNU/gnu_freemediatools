
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef WANT_PAIN_USE_WINDOWS

#include <openobjectlib/plugins/X3D/x3d_content_handler_msxml.hpp>

namespace fs = boost::filesystem;
namespace sg = olib::openobjectlib::sg;

namespace olib { namespace openobjectlib { namespace plugins { namespace X3D {

//
x3d_content_handler_msxml::x3d_content_handler_msxml( )
	: refcnt_( 0 )
{ }

//
x3d_content_handler_msxml::~x3d_content_handler_msxml( )
{ }

//
long __stdcall x3d_content_handler_msxml::QueryInterface( const struct _GUID& riid, void** ppvObject )
{
	*ppvObject = NULL;
	if( riid == IID_IUnknown || riid == __uuidof( MSXML2::ISAXContentHandler ) )
		*ppvObject = this;
					
	if( *ppvObject )
	{
		AddRef( );

		return S_OK;
	}

	return E_NOINTERFACE;
}
		
unsigned long __stdcall x3d_content_handler_msxml::AddRef( void )
{ return ++refcnt_; }

unsigned long __stdcall x3d_content_handler_msxml::Release( void )
{
	--refcnt_;
	if( refcnt_ == 0 )
	{
		delete this;

		return 0;
	}

	return refcnt_;
}

HRESULT STDMETHODCALLTYPE x3d_content_handler_msxml::putDocumentLocator( MSXML2::ISAXLocator __RPC_FAR* /*locator*/ )
{ return S_OK; }

HRESULT STDMETHODCALLTYPE x3d_content_handler_msxml::startDocument( void )
{ return S_OK; }	

HRESULT STDMETHODCALLTYPE x3d_content_handler_msxml::endDocument( void )
{ return S_OK; }
			
HRESULT STDMETHODCALLTYPE x3d_content_handler_msxml::startPrefixMapping( unsigned short __RPC_FAR* /*pwchPrefix*/,
																   		 int /*cchPrefix*/,
																   		 unsigned short __RPC_FAR* /*pwchUri*/,
																   		 int /*cchUri*/ )
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE x3d_content_handler_msxml::endPrefixMapping( unsigned short __RPC_FAR* /*pwchPrefix*/,
																 	   int /*cchPrefix*/ )
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE x3d_content_handler_msxml::startElement( unsigned short __RPC_FAR* /*pwchNamespaceUri*/,
															 	   int /*cchNamespaceUri*/,
															 	   unsigned short __RPC_FAR* pwchLocalName,
															 	   int cchLocalName,
															 	   unsigned short __RPC_FAR* /*pwchRawName*/,
															 	   int /*cchRawName*/,
															 	   MSXML2::ISAXAttributes __RPC_FAR* pAttributes )
{
	action_.set_attributes( pAttributes );
	action_.dispatch( openpluginlib::wstring( reinterpret_cast<wchar_t*>( pwchLocalName ), cchLocalName ) );
				
	return S_OK;
}

HRESULT STDMETHODCALLTYPE x3d_content_handler_msxml::endElement( unsigned short __RPC_FAR* /*pwchNamespaceUri*/,
														   		 int /*cchNamespaceUri*/,
														   		 unsigned short __RPC_FAR* /*pwchLocalName*/,
														   		 int /*cchLocalName*/,
														   		 unsigned short __RPC_FAR* /*pwchRawName*/,
														   		 int /*cchRawName*/ )
{
	// this pop is not totally symmetric with the dispatch above.
	// ideally we would like to do the pop in a dispatch type
	// method. however it seems totally unnecessary since it
	// only adds to a bit of ugly code.

	action_.pop( );
				
	return S_OK;
}
			
HRESULT STDMETHODCALLTYPE x3d_content_handler_msxml::characters( unsigned short __RPC_FAR* /*pwchChars*/, int /*cchChars*/ )
{
	return S_OK;
}
			
HRESULT STDMETHODCALLTYPE x3d_content_handler_msxml::ignorableWhitespace( unsigned short __RPC_FAR* /*pwchChars*/, int /*cchChars*/ )
{
	return S_OK;
}
			
HRESULT STDMETHODCALLTYPE x3d_content_handler_msxml::processingInstruction( unsigned short __RPC_FAR* /*pwchTarget*/,
																	  		int /*cchTarget*/,
																	  		unsigned short __RPC_FAR* /*pwchData*/,
																	  		int /*cchData*/ )
{
	return S_OK;
}
																		 
HRESULT STDMETHODCALLTYPE x3d_content_handler_msxml::skippedEntity( unsigned short __RPC_FAR* /*pwchName*/, int /*cchName*/ )
{
	return S_OK;
}

void x3d_content_handler_msxml::set_base_url( const fs::path& base_url )
{ action_.set_base_url( base_url ); }
			
sg::scene_ptr x3d_content_handler_msxml::root( )
{
	return action_.get_scene( );
}

} } } }
#endif
