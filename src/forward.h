#ifndef JMX_FORWARD_H_INCLUDED
#define JMX_FORWARD_H_INCLUDED

//==================================================
// @title        forward.h
// @author       Jonathan Hadida
// @contact      Jhadida87 [at] gmail
//==================================================

#include <deque>
#include <string>
#include <unordered_map>

// ------------------------------------------------------------------------

namespace jmx {

    class AbstractMapping
        : public Extractor<const char*>, public Creator<const char*>
    {
    public: 

        using fieldmap_type = std::unordered_map< std::string, mxArray* >;
        using fields_type   = std::deque< std::string >;

        virtual void clear();
        virtual bool valid() const =0;

        // Dimensions / validity
        inline bool      empty() const { return nfields() == 0; }
        inline index_t    size() const { return nfields(); }
        inline index_t nfields() const { return m_fields.size(); }
        inline operator   bool() const { return valid(); }

        // Check if field exists
        inline bool has_field( const std::string& name ) const { 
            return m_fmap.find(name) != m_fmap.end(); 
        }

        bool has_any    ( const inilst<const char*>& names ) const;
        bool has_fields ( const inilst<const char*>& names ) const;

        // Access by index
        inline const std::string& get_name  ( index_t n ) const { return m_fields.at(n); }
        inline mxArray*           get_value ( index_t n ) const { return get_value(get_name(n)); }

        // Access by name (overload necessary to avoid ambiguity)
        inline mxArray* operator[] ( const std::string& name ) const { return get_value(name); }
        inline mxArray* operator[] ( const char* name )        const { return get_value(name); }

        inline mxArray* get_value( const std::string& name ) const { 
            return has_field(name) ? m_fmap.find(name)->second : nullptr; 
        }

        virtual int set_value( const char *name, mxArray *value ) const =0;

        // extractor interface
        using extractor_type = Extractor<const char*>;
        using creator_type = Creator<const char*>;
        using key_t = extractor_type::key_t;
        using mxinptr_t = extractor_type::ptr_t;
        using mxoutptr_t = creator_type::ptr_t;

        virtual inline mxinptr_t _extractor_get( key_t name ) const 
            { return this->get_value(name); }

        virtual inline bool _extractor_valid_key( key_t name ) const 
            { return this->has_field(name); }

        virtual inline mxoutptr_t _creator_assign( key_t name, mxoutptr_t value )
            { set_value( name, value ); }

    protected:

        fieldmap_type  m_fmap;
        fields_type    m_fields;    
    };

}

#endif