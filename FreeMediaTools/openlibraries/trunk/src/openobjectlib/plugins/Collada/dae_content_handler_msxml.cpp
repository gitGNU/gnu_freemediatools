
// sg - A scene graph representation.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef WANT_PAIN_USE_WINDOWS

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#include <openobjectlib/plugins/Collada/dae_content_handler_msxml.hpp>

namespace olib { namespace openobjectlib { namespace plugins { namespace dae {

//
dae_content_handler_msxml::dae_content_handler_msxml( )
    : refcnt_( 0 )
{ }

//
dae_content_handler_msxml::~dae_content_handler_msxml( )
{ }

//
long __stdcall dae_content_handler_msxml::QueryInterface( const struct _GUID& riid, void** ppvObject )
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

unsigned long __stdcall dae_content_handler_msxml::AddRef( void )
{ return ++refcnt_; }

unsigned long __stdcall dae_content_handler_msxml::Release( void )
{
    --refcnt_;
    if( refcnt_ == 0 )
    {
        delete this;

        return 0;
    }

    return refcnt_;
}

HRESULT STDMETHODCALLTYPE dae_content_handler_msxml::putDocumentLocator( MSXML2::ISAXLocator __RPC_FAR* /*locator*/ )
{ return S_OK; }

HRESULT STDMETHODCALLTYPE dae_content_handler_msxml::startDocument( void )
{ return S_OK; }

HRESULT STDMETHODCALLTYPE dae_content_handler_msxml::endDocument( void )
{ return S_OK; }

HRESULT STDMETHODCALLTYPE dae_content_handler_msxml::startPrefixMapping( unsigned short __RPC_FAR* /*pwchPrefix*/,
                                                                         int /*cchPrefix*/,
                                                                         unsigned short __RPC_FAR* /*pwchUri*/,
                                                                         int /*cchUri*/ )
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE dae_content_handler_msxml::endPrefixMapping( unsigned short __RPC_FAR* /*pwchPrefix*/,
                                                                       int /*cchPrefix*/ )
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE dae_content_handler_msxml::startElement( unsigned short __RPC_FAR* /*pwchNamespaceUri*/,
                                                                   int /*cchNamespaceUri*/,
                                                                   unsigned short __RPC_FAR* pwchLocalName,
                                                                   int cchLocalName,
                                                                   unsigned short __RPC_FAR* /*pwchRawName*/,
                                                                   int /*cchRawName*/,
                                                                   MSXML2::ISAXAttributes __RPC_FAR* pAttributes )
{
    characters_.clear();
    action_.set_attributes( pAttributes );
    action_.dispatch( openpluginlib::wstring( reinterpret_cast<wchar_t*>( pwchLocalName ), cchLocalName ) );

    return S_OK;
}

HRESULT STDMETHODCALLTYPE dae_content_handler_msxml::endElement( unsigned short __RPC_FAR* /*pwchNamespaceUri*/,
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

    action_.characters( characters_ );
    action_.pop( );

    return S_OK;
}

HRESULT STDMETHODCALLTYPE dae_content_handler_msxml::characters( unsigned short __RPC_FAR* pwchChars, int cchChars )
{
    characters_ += openpluginlib::wstring( reinterpret_cast<wchar_t*>( pwchChars ), cchChars );
    return S_OK;
}

HRESULT STDMETHODCALLTYPE dae_content_handler_msxml::ignorableWhitespace( unsigned short __RPC_FAR* /*pwchChars*/, int /*cchChars*/ )
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE dae_content_handler_msxml::processingInstruction( unsigned short __RPC_FAR* /*pwchTarget*/,
                                                                            int /*cchTarget*/,
                                                                            unsigned short __RPC_FAR* /*pwchData*/,
                                                                            int /*cchData*/ )
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE dae_content_handler_msxml::skippedEntity( unsigned short __RPC_FAR* /*pwchName*/, int /*cchName*/ )
{
    return S_OK;
}

sg::scene_ptr dae_content_handler_msxml::root( )
{
    return action_.get_scene( );
}

} } } }
#endif
