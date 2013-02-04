//
//  lru.hxx
//  box_cache
//
//  Created by Igor Okulist on 2/3/13.
//  Copyright (c) 2013 okigan software. All rights reserved.
//

#ifndef okigan_lru_hxx
#define okigan_lru_hxx

#include <stdio.h>
#include <string.h>

#include <string>
#include <deque>
#include <algorithm>
#include <map>
#include <vector>
#include <unordered_map>
#include <list>
#include <sstream>
#include <cmath>
#include <fstream>



/** Least Recently Used (LRU/1) single threaded template based implementation
 */
template < typename KEY = std::string, typename VALUE = std::string, typename HASHER = std::hash<KEY> >
class lrc
{
private:
    typedef KEY key_type;
    typedef VALUE value_type;
    
    typedef std::list< key_type > key_list;
    
    struct cache_value_type {
        value_type                  _value;
        typename key_list::iterator _key_list_it;
        
        cache_value_type()
        {
        }
        
        cache_value_type(value_type value, typename key_list::iterator it)
        {
            _value = value;
            _key_list_it = it;
        }
    };
    
    //typedef std::map<key_type, typename cache_value_container::iterator> cache_lookup_container;
    typedef std::unordered_map<key_type, cache_value_type, HASHER > cache_container;
    
public:
    lrc()
    {
        _bound = 0;
    }
    
    ~lrc()
    {
    }
    
    //! set number of entries cached (shall be the first call)
    void bound_set(int bound)
    {
        _bound = bound;
        
        _shrink();
    }
    
    //! set null value (returned in case value is not found in cache)
    void set_null_value( const value_type & null_value)
    {
        _null_value = null_value;
    }
    
    //! set a value
    void set(const key_type & key, const value_type & value)
    {
        typename cache_container::iterator it = _cache.find(key);
        
        if( it != _cache.end() ) {
            //found
            
            _key_list.erase(it->second._key_list_it);
            it->second._key_list_it = _key_list.insert(_key_list.end(), key);
            it->second._value       = value;
        } else {
            auto key_list_it = _key_list.insert(_key_list.end(), key);
            _cache[key]  = cache_value_type(value, key_list_it);
        }
        
        _shrink();
    }
    
    const value_type & get(const key_type & key, bool peek)
    {
        typename cache_container::iterator it = _cache.find(key);
        
        if( it != _cache.end() ) {
            
            if( !peek ) {
                _key_list.erase(it->second._key_list_it);
                it->second._key_list_it = _key_list.insert(_key_list.end(), key);
            }
            
            return it->second._value;
        } else {
            return _null_value;
        }
    }
    
    //! dump current contens of the cache to FILE* stream
    void dump(FILE * os)
    {
        std::map<key_type, value_type> map;
        
        std::for_each(_cache.begin(),
                      _cache.end(),
                      [&map](typename cache_container::value_type & p) {
                          map[p.first] = p.second._value;
                      }
                      );
        
        std::for_each(map.begin(),
                      map.end(),
                      [&os](const std::pair<key_type, value_type> & p) {
                          fprintf(os, "%s %s\n", p.first.c_str(), p.second.c_str());
                      }
                      );
    }
    
private:
    void _shrink()
    {
        while( _key_list.size() > _bound ) {
            key_type & k = _key_list.front();
            
            auto it = _cache.find(k);
            _cache.erase(it);
            
            _key_list.pop_front();
        }
        
    }
    
private:
    
    key_list _key_list;
    cache_container _cache;
    unsigned int _bound;
    
    value_type _null_value;
};


#endif
