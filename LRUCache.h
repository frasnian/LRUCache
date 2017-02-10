// LRUCache.h:
// A simple LRU cache implementation in C++
//
// <copyright>
// This is a part of Euler Zero - see https://github.com/frasnian/euler-zero
// Copyright (c) 2017 David Starr
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
// </copyright>
//
#include <cstddef>
#include <unordered_map>
#include <list>
#include <utility>

template<class TKey, class TValue, class TContainer = std::list< std::pair<TKey, TValue> > >
class LRUCache {
public:
    typedef TContainer                                     container_type;
    typedef typename container_type::iterator              iterator;
    typedef typename container_type::const_iterator        const_iterator;
    typedef typename container_type::const_reference       const_reference;
    typedef std::size_t                                    size_type;

    LRUCache(size_type size) : max_size_(size)
    {}

    const_iterator get(TKey key){
        auto itrLookup = lookupMap.find(key);
        if (lookupMap.end() == itrLookup){
            return end();
        }
        
        auto containerPos = itrLookup->second;
        auto existing = *containerPos;          // save the existing key/value pair
        
        container.erase(containerPos);          // get rid of the old one from the existing position list
        container.push_front(existing);         // and stick it at the front
        lookupMap[key] = container.begin();     // fix the reference to it in the lookup map
        return container.begin();
    }
    
    const_iterator peek(TKey key) const {
        auto itrLookup = lookupMap.find(key);
        if (lookupMap.end() == itrLookup){
            return end();
        }
        else{
            return itrLookup->second;
        }
    }

    void put(TKey key, TValue value){   // consider: return bool (new vs. replaced existing)?
        auto itrLookup = lookupMap.find(key);
        if (lookupMap.end() == itrLookup){ // it's not in there, need to add a new value
            if (container.size() == max_size()){ 
                // if already full, get rid of the oldest one:                
                lookupMap.erase(container.back().first); // erase the oldest key from the lookup map
                container.pop_back();                    // ...and the LRU container
            }            
            container.push_front(std::make_pair(key, value));
            lookupMap.insert(std::make_pair(key, container.begin()));
        }
        else{
            // it exists, replace existing value
            container.erase(itrLookup->second);
            container.push_front(std::make_pair(key, value));
            lookupMap[key] = container.begin();
        }
    }

    inline const_iterator  begin() const { return container.begin(); }
    inline const_iterator  end() const   { return container.end();   }   
    inline const_reference front() const { return container.front(); }
    inline const_reference back() const  { return container.back();  }

    inline bool empty() const            { return container.empty(); }
    inline size_type size() const        { return container.size();  }
    inline size_type max_size() const    { return max_size_;            }
    
    inline void clear() {
        container.clear();
        lookupMap.clear();
    }
    
    inline bool is_cached(TKey key) const {
        // this is faster than (peek(k) != end()), but probably just as useless.
        return (lookupMap.find(key) != lookupMap.end());
    }

    // TODO: Consider adding the following members to the public interface:
    //      resize           ???
    
private:    
    size_type                          max_size_;
    container_type                     container;
    std::unordered_map<TKey, iterator> lookupMap;
};
