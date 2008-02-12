
// OBJ - An OBJ plugin to sg.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include <boost/filesystem/fstream.hpp>
#include <boost/tokenizer.hpp>

#include <openpluginlib/pl/geometry.hpp>
#include <openpluginlib/pl/property.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>

#include <openobjectlib/sg/appearance.hpp>
#include <openobjectlib/sg/spatial.hpp>
#include <openobjectlib/sg/meshes.hpp>

#include <openobjectlib/plugins/wavefront/obj_plugin.hpp>
#include <openobjectlib/plugins/wavefront/spirit.hpp>

namespace fs		= boost::filesystem;
namespace spirit	= boost::spirit;
namespace opl		= olib::openpluginlib;
namespace sg		= olib::openobjectlib::sg;

namespace olib { namespace openobjectlib { namespace plugins { namespace OBJ {

namespace
{
	opl::string get_base_url( const opl::string& url )
	{
#ifdef WIN32
		size_t pos = url.find_last_of( '\\' );
		if( pos != opl::string::npos )
			return opl::string( url.begin( ), url.begin( ) + pos + 1 );
#else
		return opl::string( url.begin( ), url.begin( ) + url.find_last_of( '/' ) + 1 );
#endif
		return opl::string( );
	}

	///////////////////////////////////////////////////////////////
	// grammar state
	struct material_state
	{
		explicit material_state( const opl::string& name_ )
			: ka( 0.2f, 0.2f, 0.2f )
			, kd( 0.8f, 0.8f, 0.8f )
			, ks( 0.2f, 0.2f, 0.2f )
			, tf( 1.0f, 1.0f, 1.0f )
			, ns( 65.0f )
			, name( name_ )
		{ }
		
		opl::vec3f	ka, kd, ks, tf;
		float		ns;
		float		sharpness;
		opl::string	name;
		opl::string	map_kd;
	};

	struct group_state
	{
		typedef std::vector<int> vec_ints;
	
		explicit group_state( const opl::string& mtl_ )
			: mtl( mtl_ )
		{ }
	
		vec_ints	vi, vni, vti;
		opl::string mtl;
	};
	
	struct grammar_state
	{
		typedef std::vector<float>						vec_floats;
		typedef std::vector<int>						vec_ints;
		typedef std::map<opl::string, group_state>		groups;
		typedef std::map<opl::string, material_state>	materials;

		explicit grammar_state( )
			: grp( 0 )
			, mtl( 0 )
		{ set_group( "default" ); }
		
		
		// XXX merge these two members by means of templates.
		// XXX this totally sucks. needs a revamp ASAP.
		void set_group( const opl::string& name )
		{
			typedef groups::iterator iterator;
			
			iterator I = group.find( name );
			if( I != group.end( ) )
			{
				grp = &( I->second );
			}
			else
			{
				opl::string mtl_name( "default" );
				
				if( mtl ) mtl_name = mtl->name;
				
				grp = &( group.insert( groups::value_type( name, group_state( mtl_name ) ) ).first->second );
			}
		}
		
		void set_material( const opl::string& name )
		{
			typedef materials::iterator iterator;

			iterator I = material.find( name );
			if( I != material.end( ) )
			{
				mtl = &( I->second );
			}
			else
			{
				mtl = &( material.insert( materials::value_type( name, material_state( name ) ) ).first->second );
			}
			
			grp->mtl = name;
		}
		
		groups		group;
		materials	material;
		vec_floats	v, vn, vt;
		
		group_state*	grp;
		material_state* mtl;
	};

	///////////////////////////////////
	/// functors
	class geometry_action
	{
	public:
		explicit geometry_action( grammar_state::vec_floats& v )
			: v_( v )
		{ }
			
		void operator( )( const char* first, const char* last ) const
		{ v_.push_back( static_cast<float>( atof( opl::string( first, last ).c_str( ) ) ) ); }
			
	private:
		grammar_state::vec_floats& v_;
	};

	//
	class topology_action
	{
	public:
		explicit topology_action( grammar_state& gs )
			: gs_( gs )
		{ }
			
		void operator( )( const char* first, const char* last ) const
		{
			const char* iter = first;
					
			for( ; iter != last && *iter != '/'; ++iter )
				;
					
			gs_.grp->vi.push_back( atoi( opl::string( first, iter ).c_str( ) ) - 1 );
					
			// texture
			if( iter != last && *++iter != '/' )
			{
				for( ; iter != last && *iter != '/'; ++iter )
					;

				gs_.grp->vti.push_back( atoi( opl::string( first, iter ).c_str( ) ) - 1 );
			}
						
			// normal
			if( iter != last )
			{
				first = ++iter;

				gs_.grp->vni.push_back( atoi( opl::string( first, last ).c_str( ) ) - 1 );
			}
		}
				
	private:
		grammar_state& gs_;
	};

	//
	class topology_wrap_up_action
	{
	public:
		explicit topology_wrap_up_action( grammar_state& gs )
			: gs_( gs )
		{ }
				
		void operator( )( const char* /*first*/, const char* /*last*/ ) const
		{
			gs_.grp->vi.push_back(  -1 );
			gs_.grp->vni.push_back( -1 );
			gs_.grp->vti.push_back( -1 );
		}
			
	private:
		grammar_state& gs_;
	};
			
	//
	struct group_action
	{
		explicit group_action( grammar_state& gs )
			: gs_( gs )
		{ }
				
		void operator( )( const char* first, const char* last ) const
		{
			opl::string s;
					
			if( ++first == last )
				s.assign( "default" );
			else
				s.assign( ++first, last ); // XXX needs a proper trim left and trim right.
				
			gs_.set_group( s );
		}

		grammar_state& gs_;
	};
	
	//
	struct use_material_action
	{
		explicit use_material_action( grammar_state& gs )
			: gs_( gs )
		{ }
		
		void operator( )( const char* first, const char* last ) const
		{ gs_.set_material( opl::string( first, last ) ); }
		
		grammar_state& gs_;
	};
	
	//
	struct material_action
	{
		explicit material_action( grammar_state& gs )
			: gs_( gs )
		{ }
		
		void operator( )( const char* first, const char* last ) const
		{ gs_.set_material( opl::string( first, last ) ); }

		grammar_state& gs_;
	};

	//
	opl::vec3f extract_vec3f_info( const opl::string& str )
	{
		typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
		
		opl::vec3f vec;
		
		std::string st( str.c_str( ) );
		
		boost::char_separator<char> sep( " \t" );
		tokenizer tok( st.begin( ), st.end( ), sep );
			
		tokenizer::const_iterator I = tok.begin( );
		if( I != tok.end( ) )
			vec[ 0 ] = static_cast<float>( strtod( I->c_str( ), 0 ) );
		++I;	

		if( I != tok.end( ) )
			vec[ 1 ] = static_cast<float>( strtod( I->c_str( ), 0 ) );
		++I;	

		if( I != tok.end( ) )
			vec[ 2 ] = static_cast<float>( strtod( I->c_str( ), 0 ) );
		++I;	
		
		return vec;
	}

	struct ka_action
	{
		explicit ka_action( grammar_state& gs )
			: gs_( gs )
		{ }
		
		void operator( )( const char* first, const char* last ) const
		{
			opl::string tmp( first, last );
			opl::vec3f ka = extract_vec3f_info( tmp );
			
			gs_.mtl->ka = ka;
		}
		
		grammar_state& gs_;
	};

	struct kd_action
	{
		explicit kd_action( grammar_state& gs )
			: gs_( gs )
		{ }
		
		void operator( )( const char* first, const char* last ) const
		{
			opl::string tmp( first, last );
			opl::vec3f kd = extract_vec3f_info( tmp );
			
			gs_.mtl->kd = kd;
		}
		
		grammar_state& gs_;
	};

	struct ks_action
	{
		explicit ks_action( grammar_state& gs )
			: gs_( gs )
		{ }
		
		void operator( )( const char* first, const char* last ) const
		{
			opl::string tmp( first, last );
			opl::vec3f ks = extract_vec3f_info( tmp );
			
			gs_.mtl->ks = ks;
		}
		
		grammar_state& gs_;
	};
	
	struct map_kd_action
	{
		explicit map_kd_action( grammar_state& gs )
			: gs_( gs )
		{ }
		
		void operator( )( const char* first, const char* last ) const
		{
			opl::string tmp( first, last );
			
			gs_.mtl->map_kd = tmp;
		}
		
		grammar_state& gs_;
	};
	
	///////////////////////////////////////////////////////////////
	// skip grammar (whitespace and comments)
	struct skip_grammar : public spirit::grammar<skip_grammar>
	{
		template<class ScannerT>
		struct definition
		{
			definition( const skip_grammar& /*self*/)
			{
				skip
					=	spirit::space_p
					|	spirit::comment_p( "#" )
					;
					
#		ifndef NDEBUG
				BOOST_SPIRIT_DEBUG_NODE( skip );
#		endif
			}

			spirit::rule<ScannerT> skip;
		
			const spirit::rule<ScannerT>& start( ) const
			{ return skip; }
		};
	};
	
	//////////////////////////////////////////////////////////////
	// obj material grammar.
	struct mtl_grammar : public spirit::grammar<mtl_grammar>
	{
		mtl_grammar( grammar_state& eval_ )
			: eval( eval_ )
		{ }

		template<class ScannerT>
		struct definition
		{
			definition( const mtl_grammar& self )
			{
				using namespace boost::spirit;

				keywords
					=	"newmtl", "Ka", "Kd", "Ks", "illum", "Ns", "d", "map_Kd", "Tf"
					;

				NEWMTL		= strlit<>( "newmtl" );
				KA			= strlit<>( "Ka" );
				KD			= strlit<>( "Kd" );
				KS			= strlit<>( "Ks" );
				ILLUM		= strlit<>( "illum" );
				NS			= strlit<>( "Ns" );
				NI			= strlit<>( "Ni" );
				D			= strlit<>( "d" );
				MAP_KD		= strlit<>( "map_Kd" );
				TF			= strlit<>( "Tf" );
				REFL		= strlit<>( "refl" );
				REFLTYPE	= strlit<>( "-type" );
				REFLMM		= strlit<>( "-mm" );
				REFLCLAMP	= strlit<>( "-clamp" );
				SHARPNESS	= strlit<>( "sharpness" );
				ON			= strlit<>( "on" );
				OFF			= strlit<>( "off" );

				INTEGER_DECIMAL_CONSTANT
					=	lexeme_d[ +digit_p ]
					;
					
				INTEGER_CONSTANT
					=	INTEGER_DECIMAL_CONSTANT
					;

				FLOAT_CONSTANT_1    // 12345[eE][+-]123[lLfF]?
					=	lexeme_d
						[	
							+digit_p
						>>	( chlit<>( 'e' )  | chlit<>( 'E' ) )
						>>	!sign_p
						>>	+digit_p
						>>	!as_lower_d[ chlit<>( 'l' ) | chlit<>( 'f' ) ]
						]
					;

				FLOAT_CONSTANT_2    // .123([[eE][+-]123)?[lLfF]?
					=	lexeme_d
						[ 
							
							*digit_p
						>>	chlit<>('.')
						>>	+digit_p
						>>	!(  ( chlit<>( 'e' ) | chlit<>( 'E' ) )
							 >> !sign_p
							 >> +digit_p
							 )
						>> !as_lower_d[ chlit<>( 'l' ) | chlit<>( 'f' ) ]
						]
					;

				FLOAT_CONSTANT_3    // 12345.([[eE][+-]123)?[lLfF]?
					=	lexeme_d
						[ 
								+digit_p
							>>	chlit<>( '.' )
							>>	*digit_p
							>>	!(  ( chlit<>( 'e' ) | chlit<>( 'E' ) )
								 >> !sign_p
								 >> +digit_p
								 )
							>>	!as_lower_d[ chlit<>( 'l' ) | chlit<>( 'f' ) ]
						]
					;

				FLOAT_CONSTANT
					=	FLOAT_CONSTANT_1
					|	FLOAT_CONSTANT_2
					|	FLOAT_CONSTANT_3
					;
					
				CONSTANT
					=	!sign_p >> longest_d[ FLOAT_CONSTANT | INTEGER_CONSTANT ]
					;

				
				IDENTIFIER
					=	lexeme_d
						[ 
							( ( alpha_p | '_' | '$' ) >> *( alnum_p | '_' | '$' | '-' | '.' ) ) - ( keywords >> anychar_p - ( alnum_p | '_' | '$' ) )
						]
					;
				
				newmtl
					=	NEWMTL >> IDENTIFIER[ material_action( ( const_cast<mtl_grammar&>( self ) ).eval ) ]
					>>	*( ka | kd | ks | ns | ni | illum | d | sharpness | tf | refl | map_Kd )
					;
					
				ka
					=	KA >> ( CONSTANT >> CONSTANT >> CONSTANT )[ ka_action( ( const_cast<mtl_grammar&>( self ) ).eval ) ]
					;
					
				kd	=	KD >> ( CONSTANT >> CONSTANT >> CONSTANT )[ kd_action( ( const_cast<mtl_grammar&>( self ) ).eval ) ]
					;

				ks	=	KS >> ( CONSTANT >> CONSTANT >> CONSTANT )[ ks_action( ( const_cast<mtl_grammar&>( self ) ).eval ) ]
					;
					
				ns	=	NS >> CONSTANT
					;
					
				ni	=	NI >> CONSTANT
					;
					
				sharpness
					=	SHARPNESS >> CONSTANT
					;
					
				illum
					=	ILLUM >> CONSTANT
					;
				
				d
					=	D >> CONSTANT
					;
					
				map_Kd
					=	MAP_KD >> IDENTIFIER[ map_kd_action( ( const_cast<mtl_grammar&>( self ) ).eval ) ]
					;
					
				tf
					=	TF >> ( CONSTANT >> CONSTANT >> CONSTANT )
					;
					
				reflmm
					=	REFLMM >> CONSTANT >> CONSTANT
					;
					
				reflclamp
					=	REFLCLAMP >> ( ON | OFF )
					;
					
				refl
					=	REFL >> REFLTYPE >> IDENTIFIER >> !( reflmm | reflclamp ) >> IDENTIFIER
					;
				
				unit
					=	*newmtl
					;
			
#		ifndef NDEBUG
				BOOST_SPIRIT_DEBUG_NODE( INTEGER_DECIMAL_CONSTANT );
				BOOST_SPIRIT_DEBUG_NODE( INTEGER_CONSTANT );
				BOOST_SPIRIT_DEBUG_NODE( FLOAT_CONSTANT );
				BOOST_SPIRIT_DEBUG_NODE( FLOAT_CONSTANT_1 );
				BOOST_SPIRIT_DEBUG_NODE( FLOAT_CONSTANT_2 );
				BOOST_SPIRIT_DEBUG_NODE( FLOAT_CONSTANT_3 );
				BOOST_SPIRIT_DEBUG_NODE( CONSTANT );
				BOOST_SPIRIT_DEBUG_NODE( ILLUM );
				BOOST_SPIRIT_DEBUG_NODE( NS );
				BOOST_SPIRIT_DEBUG_NODE( NI );
				BOOST_SPIRIT_DEBUG_NODE( KS );
				BOOST_SPIRIT_DEBUG_NODE( KD );
				BOOST_SPIRIT_DEBUG_NODE( KA );
				BOOST_SPIRIT_DEBUG_NODE( D );
				BOOST_SPIRIT_DEBUG_NODE( NEWMTL );
				BOOST_SPIRIT_DEBUG_NODE( IDENTIFIER );
				BOOST_SPIRIT_DEBUG_NODE( MAP_KD );
				BOOST_SPIRIT_DEBUG_NODE( TF );
				BOOST_SPIRIT_DEBUG_NODE( SHARPNESS );
				BOOST_SPIRIT_DEBUG_NODE( REFL );
				BOOST_SPIRIT_DEBUG_NODE( REFLTYPE );
				BOOST_SPIRIT_DEBUG_NODE( REFLMM );
				BOOST_SPIRIT_DEBUG_NODE( REFLCLAMP );
				BOOST_SPIRIT_DEBUG_NODE( ON );
				BOOST_SPIRIT_DEBUG_NODE( OFF );
				BOOST_SPIRIT_DEBUG_NODE( unit );
				BOOST_SPIRIT_DEBUG_NODE( newmtl );
				BOOST_SPIRIT_DEBUG_NODE( ka );
				BOOST_SPIRIT_DEBUG_NODE( kd );
				BOOST_SPIRIT_DEBUG_NODE( ks );
				BOOST_SPIRIT_DEBUG_NODE( ns );
				BOOST_SPIRIT_DEBUG_NODE( ni );
				BOOST_SPIRIT_DEBUG_NODE( illum );
				BOOST_SPIRIT_DEBUG_NODE( d );
				BOOST_SPIRIT_DEBUG_NODE( map_Kd );
				BOOST_SPIRIT_DEBUG_NODE( tf );
				BOOST_SPIRIT_DEBUG_NODE( sharpness );
				BOOST_SPIRIT_DEBUG_NODE( refl );
				BOOST_SPIRIT_DEBUG_NODE( reflmm );
				BOOST_SPIRIT_DEBUG_NODE( reflclamp );
#		endif
			}
			
			spirit::symbols<> keywords;
			
			spirit::rule<ScannerT> unit, newmtl, ka, kd, ks, ns, ni, illum, d, map_Kd, tf, sharpness, refl, reflmm, reflclamp;
			spirit::rule<ScannerT> IDENTIFIER, NEWMTL, KA, KD, KS, NS, NI, ILLUM, SHARPNESS;
			spirit::rule<ScannerT> INTEGER_DECIMAL_CONSTANT, INTEGER_CONSTANT;
			spirit::rule<ScannerT> FLOAT_CONSTANT, FLOAT_CONSTANT_1, FLOAT_CONSTANT_2;
			spirit::rule<ScannerT> FLOAT_CONSTANT_3, CONSTANT, D, MAP_KD, TF;
			spirit::rule<ScannerT> REFL, REFLTYPE, REFLMM, REFLCLAMP, ON, OFF;

			const spirit::rule<ScannerT>& start( ) const
			{ return unit; }
		};
		
		grammar_state& eval;
	};
	
	//////////////////////////////////////////////////////////////
	// obj grammar. some may think is a bit overkill to use a
	// spirit grammar for the obj format. however, using spirit
	// here gives us good insight on how spirit may be used for
	// more interesting grammars like the sg fx one.
	struct OBJ_grammar : public spirit::grammar<OBJ_grammar>
	{
		OBJ_grammar( grammar_state& eval_ )
			: eval( eval_ )
		{ }
		
		template<class ScannerT>
		struct definition
		{
			struct load_material_action
			{
				explicit load_material_action( const fs::path& base_url, grammar_state& gs )
					: gs_( gs )
					, base_url_( base_url )
				{ }
		
				void operator( )( const char* first, const char* last ) const
				{
					fs::path orig_path( std::string( first, last ), fs::native );

					fs::ifstream file;
					file.open( orig_path, std::ios::in );
					if( !file.is_open( ) )
					{
						file.open( base_url_ / orig_path, std::ios::in );
						if( !file.is_open( ) )
							return;
					}

					file.unsetf( std::ios::skipws );

					std::string mtl( ( std::istreambuf_iterator<char>( file ) ), std::istreambuf_iterator<char>( ) );
				
					mtl_grammar	 mtl_g( gs_ );
					skip_grammar skip_g;
		
#			ifndef NDEBUG
					BOOST_SPIRIT_DEBUG_NODE( mtl_g );
					BOOST_SPIRIT_DEBUG_NODE( skip_g );
#			endif

					spirit::parse_info<> result = spirit::parse( mtl.c_str( ), mtl_g, skip_g );
				}
		
				grammar_state& 	gs_;
				fs::path		base_url_;
			};

			static const int RESERVE_SIZE = 1000;
			
			//////////////////////////////////////////////////
			// grammar definition
			definition( const OBJ_grammar& self )
				: V( 'v' )
				, N( 'n' )
				, T( 't' )
				, F( 'f' )
				, U( 'u' )
				, G( 'g' )
				, S( 's' )
				, O( 'o' )
				, SLASH( '/' )
			{
				using namespace boost::spirit;

				keywords
					=	"v", "vn", "vt", "f", "g", "s", "u", "mtllib", "usemtl", "on", "off"
					;

				MTLLIB	= strlit<>( "mtllib" );
				USEMTL	= strlit<>( "usemtl" );
				ON		= strlit<>( "on" );
				OFF		= strlit<>( "off" );

				IDENTIFIER
					=	lexeme_d
						[ 
							( ( alpha_p | '_' | '$' ) >> *( alnum_p | '_' | '$' | '-' | '.' ) ) - ( keywords >> anychar_p - ( alnum_p | '_' | '$' ) )
						]
					;

				INTEGER_DECIMAL_CONSTANT
					=	lexeme_d[ +digit_p ]
					;
					
				INTEGER_CONSTANT
					=	INTEGER_DECIMAL_CONSTANT
					;

				FLOAT_CONSTANT_1    // 12345[eE][+-]123[lLfF]?
					=	lexeme_d
						[	
							+digit_p
						>>	( chlit<>( 'e' )  | chlit<>( 'E' ) )
						>>	!sign_p
						>>	+digit_p
						>>	!as_lower_d[ chlit<>( 'l' ) | chlit<>( 'f' ) ]
						]
					;

				FLOAT_CONSTANT_2    // .123([[eE][+-]123)?[lLfF]?
					=	lexeme_d
						[ 
							
							*digit_p
						>>	chlit<>('.')
						>>	+digit_p
						>>	!(  ( chlit<>( 'e' ) | chlit<>( 'E' ) )
							 >> !sign_p
							 >> +digit_p
							 )
						>> !as_lower_d[ chlit<>( 'l' ) | chlit<>( 'f' ) ]
						]
					;

				FLOAT_CONSTANT_3    // 12345.([[eE][+-]123)?[lLfF]?
					=	lexeme_d
						[ 
								+digit_p
							>>	chlit<>( '.' )
							>>	*digit_p
							>>	!(  ( chlit<>( 'e' ) | chlit<>( 'E' ) )
								 >> !sign_p
								 >> +digit_p
								 )
							>>	!as_lower_d[ chlit<>( 'l' ) | chlit<>( 'f' ) ]
						]
					;

				FLOAT_CONSTANT
					=	FLOAT_CONSTANT_1
					|	FLOAT_CONSTANT_2
					|	FLOAT_CONSTANT_3
					;
					
				CONSTANT
					=	!sign_p >> longest_d[ FLOAT_CONSTANT | INTEGER_CONSTANT ]
					;

				vertex_element
					=	V 
					>>	CONSTANT[ geometry_action( ( const_cast<OBJ_grammar&>( self ) ).eval.v ) ] 
					>>	CONSTANT[ geometry_action( ( const_cast<OBJ_grammar&>( self ) ).eval.v ) ] 
					>>	CONSTANT[ geometry_action( ( const_cast<OBJ_grammar&>( self ) ).eval.v ) ]
					;
					
				normal_element
					=	V >> N 
					>>	CONSTANT[ geometry_action( ( const_cast<OBJ_grammar&>( self ) ).eval.vn ) ] 
					>>	CONSTANT[ geometry_action( ( const_cast<OBJ_grammar&>( self ) ).eval.vn ) ] 
					>>	CONSTANT[ geometry_action( ( const_cast<OBJ_grammar&>( self ) ).eval.vn ) ]
					;
					
				texcoord_element
					=	V >> T 
					>>	CONSTANT[ geometry_action( ( const_cast<OBJ_grammar&>( self ) ).eval.vt ) ] 
					>>	CONSTANT[ geometry_action( ( const_cast<OBJ_grammar&>( self ) ).eval.vt ) ]
					>>	( *CONSTANT )
					;
				
				face			// can be one of %d, %d//%d, %d/%d, %d/%d/%d
					=	longest_d
						[	
							INTEGER_CONSTANT
						|	INTEGER_CONSTANT >> SLASH >> SLASH >> INTEGER_CONSTANT
						|	INTEGER_CONSTANT >> SLASH >> INTEGER_CONSTANT
						|	INTEGER_CONSTANT >> SLASH >> INTEGER_CONSTANT >> SLASH >> INTEGER_CONSTANT
						]
						[ topology_action( ( const_cast<OBJ_grammar&>( self ) ).eval ) ]
					;
				
				face_element
					=	( F >> face >> face >> face >> *face )
						[ topology_wrap_up_action( ( const_cast<OBJ_grammar&>( self ) ).eval ) ]
					;
					
				material_element
					=	MTLLIB >> IDENTIFIER[ load_material_action( self.base_url_, ( const_cast<OBJ_grammar&>( self ) ).eval ) ]
					;
					
				usemtl_element
					=	USEMTL >> IDENTIFIER[ use_material_action( ( const_cast<OBJ_grammar&>( self ) ).eval ) ]
					;
					
				group_element
					=	( G >> *IDENTIFIER )[ group_action( ( const_cast<OBJ_grammar&>( self ) ).eval ) ]
					;

				smoothing_group_element
					=	S >> ( INTEGER_CONSTANT | ON | OFF )
					;
					
				theo_element
					=	( O >> *IDENTIFIER )
					;
					
				elements
					=	vertex_element
					|	normal_element
					|	texcoord_element
					|	face_element
					|	material_element
					|	group_element
					|	smoothing_group_element
					|	theo_element
					|	usemtl_element
					;
				
				unit
					=	*elements
					;
				
		#ifndef NDEBUG
				BOOST_SPIRIT_DEBUG_NODE( INTEGER_DECIMAL_CONSTANT );
				BOOST_SPIRIT_DEBUG_NODE( INTEGER_CONSTANT );
				BOOST_SPIRIT_DEBUG_NODE( FLOAT_CONSTANT );
				BOOST_SPIRIT_DEBUG_NODE( FLOAT_CONSTANT_1 );
				BOOST_SPIRIT_DEBUG_NODE( FLOAT_CONSTANT_2 );
				BOOST_SPIRIT_DEBUG_NODE( FLOAT_CONSTANT_3 );
				BOOST_SPIRIT_DEBUG_NODE( CONSTANT );
				BOOST_SPIRIT_DEBUG_NODE( IDENTIFIER );
				BOOST_SPIRIT_DEBUG_NODE( MTLLIB );
				BOOST_SPIRIT_DEBUG_NODE( USEMTL );
				BOOST_SPIRIT_DEBUG_NODE( ON );
				BOOST_SPIRIT_DEBUG_NODE( OFF );

				BOOST_SPIRIT_DEBUG_NODE( unit );
				BOOST_SPIRIT_DEBUG_NODE( elements );
				BOOST_SPIRIT_DEBUG_NODE( vertex_element );
				BOOST_SPIRIT_DEBUG_NODE( normal_element );
				BOOST_SPIRIT_DEBUG_NODE( texcoord_element );
				BOOST_SPIRIT_DEBUG_NODE( face_element );
				BOOST_SPIRIT_DEBUG_NODE( material_element );
				BOOST_SPIRIT_DEBUG_NODE( param_element );
				BOOST_SPIRIT_DEBUG_NODE( group_element );
				BOOST_SPIRIT_DEBUG_NODE( smoothing_group_element );
				BOOST_SPIRIT_DEBUG_NODE( face );
				BOOST_SPIRIT_DEBUG_NODE( theo_element );
				BOOST_SPIRIT_DEBUG_NODE( usemtl_element );
		#endif
			}
			
			////////////////////////////////////////////////////
			spirit::symbols<> keywords;
			
			spirit::chlit<> V, N, T, F, U, G, S, O, SLASH;
			
			spirit::rule<ScannerT> INTEGER_DECIMAL_CONSTANT, INTEGER_CONSTANT;
			spirit::rule<ScannerT> FLOAT_CONSTANT, FLOAT_CONSTANT_1, FLOAT_CONSTANT_2;
			spirit::rule<ScannerT> FLOAT_CONSTANT_3, CONSTANT, IDENTIFIER, MTLLIB, USEMTL;
			spirit::rule<ScannerT> ON, OFF;
			
			spirit::rule<ScannerT> unit, elements, vertex_element, normal_element;
			spirit::rule<ScannerT> texcoord_element, face_element, material_element;
			spirit::rule<ScannerT> param_element, group_element, face, smoothing_group_element;
			spirit::rule<ScannerT> theo_element, usemtl_element;
			
			const spirit::rule<ScannerT>& start( ) const
			{ return unit; }
		};

		void set_base_url( const fs::path& base_url )
		{ base_url_ = base_url; }

		grammar_state&	eval;
		fs::path 		base_url_;
	};

	//
	template<typename C, typename V>
	void copy_( const C& c, V& v )
	{
#if _MSC_VER >= 1400
		std::copy( c.begin( ), c.end( ), stdext::checked_array_iterator<typename V::iterator>( v.begin( ), v.size( ) ) );
#else
		std::copy( c.begin( ), c.end( ), v.begin( ) );
#endif
	}
	
	void unify_indices( OBJ_grammar& g )
	{
		std::vector<float> vn, vt;
		vn.resize( g.eval.v.size( ) );
		vt.resize( g.eval.v.size( ) );

		if( !g.eval.vn.empty( ) )
		{
			typedef std::map<opl::string, group_state>::const_iterator const_iterator;
			for( const_iterator I = g.eval.group.begin( ); I != g.eval.group.end( ); ++I )
			{
				for( int j = 0; j < I->second.vni.size( ); ++j )
				{
					int i0 = I->second.vi[ j ];
					int i1 = I->second.vni[ j ];
					if( i0 == -1 )
						continue;

					i0 *= 3;
					i1 *= 3;

					vn[ i0 + 0 ] = g.eval.vn[ i1 + 0 ];
					vn[ i0 + 1 ] = g.eval.vn[ i1 + 1 ];
					vn[ i0 + 2 ] = g.eval.vn[ i1 + 2 ];
				}
			}
			g.eval.vn = vn;
		}
		
		if( !g.eval.vt.empty( ) )
		{
			typedef std::map<opl::string, group_state>::const_iterator const_iterator;
			for( const_iterator I = g.eval.group.begin( ); I != g.eval.group.end( ); ++I )
			{
				for( int j = 0; j < I->second.vti.size( ); ++j )
				{
					int i0 = I->second.vi[ j ];
					int i1 = I->second.vti[ j ];
					if( i0 == -1 )
						continue;

					i0 *= 3;
					i1 *= 3;

					vt[ i0 + 0 ] = g.eval.vt[ i1 + 0 ];
					vt[ i0 + 1 ] = g.eval.vt[ i1 + 1 ];
					vt[ i0 + 2 ] = g.eval.vt[ i1 + 2 ];
				}
			}
			g.eval.vt = vt;
		}
	}
	
	void build_topology( const group_state& state, const sg::mesh_ptr& geometry )
	{
		opl::m_int32* index = geometry->pointer<opl::m_int32>( L"index" );
		opl::m_int32* count = geometry->pointer<opl::m_int32>( L"count" );

		index->reserve( state.vi.size( ) );
		count->reserve( state.vi.size( ) / 3 );

		for( int i = 0; i < state.vi.size( ); ++i  )
		{
			int prim_count = 0;
			for( int j = i; j < state.vi.size( ); ++j, ++i )
			{
				if( state.vi[ j ] == -1 )
				{
					count->push_back( prim_count );
					break;
				}
				
				index->push_back( state.vi[ j ] );
				++prim_count;
			}
		}
	}

	//
	void add_attribute( const sg::mesh_ptr& ifs, const sg::attribute_array_ptr& attr, const opl::wstring& name )
	{
		sg::v_attribute_array_ptr* attr_ = ifs->pointer<sg::v_attribute_array_ptr>( name );
		attr_->get( ) = attr;
	}
	
	sg::mesh_ptr create_geometry_node( 
		const OBJ_grammar& g, 
		const sg::attribute_array_ptr& coordinate, 
		const sg::attribute_array_ptr& normal, 
		const sg::attribute_array_ptr& texcoord )
	{
		sg::mesh_ptr ifs( new sg::mesh( ) );
		ifs->prim_type( ) = sg::mesh::POLYGONS;
		
		if( !g.eval.v.empty( ) )
			add_attribute( ifs, coordinate, L"coord" );
		
		if( !g.eval.vn.empty( ) )
			add_attribute( ifs, normal, L"normal" );
		
		if( !g.eval.vt.empty( ) )
		{
			sg::mv_uv_set_ptr* uv_ = ifs->pointer<sg::mv_uv_set_ptr>( L"uv" );
			
			sg::uv_set_ptr uv_set( new sg::uv_set( ) );
			uv_->push_back( sg::v_uv_set_ptr( uv_set ) );

			sg::v_attribute_array_ptr* uv = uv_set->pointer<sg::v_attribute_array_ptr>( L"uv" );
			uv->get( ) = texcoord;
		}

		return ifs;
	}
	
	// XXX TODO: create could try to share common materials between groups...
	sg::appearance_ptr create_appearance_node( const OBJ_grammar& g, const group_state& grp_state )
	{
		typedef grammar_state::materials::const_iterator const_iterator;

		sg::appearance_ptr appearance( new sg::appearance( ) );

		sg::v_material_ptr* material = appearance->pointer<sg::v_material_ptr>( L"material" );
		
		sg::material_ptr mtl( new sg::material( ) );
		material->get( ) = mtl;

		const_iterator I = g.eval.material.find( grp_state.mtl );
		if( I == g.eval.material.end( ) )
			return appearance;
			
		opl::v_color* diffuse = mtl->pointer<opl::v_color>( L"diffuseColor" );
			
		( *diffuse ).get( )[ 0 ] = I->second.kd[ 0 ];
		( *diffuse ).get( )[ 1 ] = I->second.kd[ 1 ];
		( *diffuse ).get( )[ 2 ] = I->second.kd[ 2 ];
		
		if( !I->second.map_kd.empty( ) )
		{
			sg::m_texture_ptr* texture = appearance->pointer<sg::m_texture_ptr>( L"texture" );

			sg::texture_ptr tex( new sg::texture( ) );
			texture->push_back( tex );

			opl::m_url* url = tex->pointer<opl::m_url>( L"url" );
			url->push_back( fs::path( I->second.map_kd.c_str( ), fs::native ) );
		}
		
		return appearance;
	}
	
	void center_object( std::vector<float>& v )
	{
		float min[ 3 ];
		float max[ 3 ];
		
		min[ 0 ] = max[ 0 ] = v[ 0 ];
		min[ 1 ] = max[ 1 ] = v[ 1 ];
		min[ 2 ] = max[ 2 ] = v[ 2 ];
		
		for( size_t i = 3; i < v.size( ); i += 3 )
		{
			if( min[ 0 ] >= v[ i + 0 ] )
				min[ 0 ] = v[ i + 0 ];
			else if( max[ 0 ] < v[ i + 0 ] )
				max[ 0 ] = v[ i + 0 ];
				
			if( min[ 1 ] >= v[ i + 1 ] )
				min[ 1 ] = v[ i + 1 ];
			else if( max[ 1 ] < v[ i + 1 ] )
				max[ 1 ] = v[ i + 1 ];

			if( min[ 2 ] >= v[ i + 2 ] )
				min[ 2 ] = v[ i + 2 ];
			else if( max[ 2 ] < v[ i + 2 ] )
				max[ 2 ] = v[ i + 2 ];
		}
		
		float centre[ 3 ];
		centre[ 0 ] = ( max[ 0 ] + min[ 0 ] ) * 0.5f;
		centre[ 1 ] = ( max[ 1 ] + min[ 1 ] ) * 0.5f;
		centre[ 2 ] = ( max[ 2 ] + min[ 2 ] ) * 0.5f;
		
		for( size_t i = 0; i < v.size( ); i += 3 )
		{
			v[ i + 0 ] -= centre[ 0 ];
			v[ i + 1 ] -= centre[ 1 ];
			v[ i + 2 ] -= centre[ 2 ];
		}
	}

	void create_geometry( const std::vector<float>& v, sg::attribute_array_ptr& arr, int comp )
	{
		opl::m_float* val = arr->pointer<opl::m_float>( L"value" );
		opl::v_int32* nc  = arr->pointer<opl::v_int32>( L"components" );
		
		val->resize( v.size( ) );
		nc->get( ) = comp;
		
		copy_( v, *val );
	}
	
	sg::spatial_ptr build_obj_graph( OBJ_grammar& g )
	{
		sg::spatial_ptr grp( new sg::group( ) );
		sg::m_spatial_ptr* children = grp->pointer<sg::m_spatial_ptr>( L"children" );
		
		sg::attribute_array_ptr varr( new sg::attribute_array( ) );
		sg::attribute_array_ptr narr( new sg::attribute_array( ) );
		sg::attribute_array_ptr tarr( new sg::attribute_array( ) );
		
		unify_indices( g );
		
		center_object( g.eval.v );
		create_geometry( g.eval.v,  varr, 3 );
		create_geometry( g.eval.vn, narr, 3 );
		create_geometry( g.eval.vt, tarr, 2 );
		
		typedef std::map<opl::string, group_state>::const_iterator const_iterator;
		for( const_iterator I = g.eval.group.begin( ); I != g.eval.group.end( ); ++I )
		{
			sg::spatial_ptr shape( new sg::shape( ) );

			sg::v_mesh_ptr*			geometry   = shape->pointer<sg::v_mesh_ptr>( L"geometry" );
			sg::v_appearance_ptr*	appearance = shape->pointer<sg::v_appearance_ptr>( L"appearance" );

			appearance->get( ) = create_appearance_node( g, I->second );

			sg::mesh_ptr ifs = create_geometry_node( g, varr, narr, tarr );
			geometry->get( ) = ifs;

			build_topology( I->second, ifs );

			children->push_back( shape );
		}
		
		return grp;
	}
	
	//
	bool parse_and_build_obj_graph( const fs::path& base_url, const std::string& obj, sg::spatial_ptr& root )
	{
		grammar_state	eval;
		
		OBJ_grammar		obj_g( eval );
		skip_grammar	skip_g;
		
#ifndef NDEBUG
		BOOST_SPIRIT_DEBUG_NODE( obj_g );
		BOOST_SPIRIT_DEBUG_NODE( skip_g );
#endif

		obj_g.set_base_url( base_url );
		spirit::parse_info<> result = spirit::parse( obj.c_str( ), obj_g, skip_g );
		
		return result.full && ( root = build_obj_graph( obj_g ) );
	}
	
	//
	sg::scene_ptr load_obj( const fs::path& path )
	{
		sg::scene_ptr sc( new sg::scene( ) );
		
		fs::ifstream file( path, std::ios::in );
		if( !file.is_open( ) )
			return sc;

		file.unsetf( std::ios::skipws );

		std::string obj( ( std::istreambuf_iterator<char>( file ) ), std::istreambuf_iterator<char>( ) );
		
		sg::spatial_ptr root;
		if( !parse_and_build_obj_graph( path.branch_path( ), obj, root ) )
			return sc;

		sc->set_root( root );
		
		return sc;
	}
}
			
sg::scene_ptr OBJ_plugin::load( const fs::path& path, const opl::string& /*options*/ )
{ return load_obj( path ); }

bool OBJ_plugin::store( const fs::path& /*path*/, const sg::scene_ptr& /*root*/ )
{ return false; }

} } } }
