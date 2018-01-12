/*
 * Copyright 2017 ZTE - VNG
 */

/* 
 * File:   raxTree.h
 * Author: minhnh3
 *
 * Created on January 12, 2018, 9:08 AM
 */

#ifndef RAXTREE_H
#define RAXTREE_H

#include <set>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/impl/text_iarchive_impl.ipp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/impl/text_oarchive_impl.ipp>

#include <boost/serialization/string.hpp>
#include "rax.h"
#include "smaz_string.h"

using namespace boost::archive;

template <typename T>
class RaxTree {
public:

    //-----------------------------------------------------
    //---   Constructors & Destructor
    //-----------------------------------------------------

    RaxTree() {
        //_cwd = prefix_search_util::getExePath();
        rt = raxNew();
        _dbFileName = _cwd + "db.db";
    }

    ~RaxTree() {
        if (_ofsDb.is_open()) {
            _ofsDb.close();
        }
    }

    RaxTree& setName(const std::string& name) {
        _name = name;
        _dbFileName = _cwd + _name + ".db";
        if (_ofsDb.is_open()) {
            _ofsDb.close();
            _ofsDb.open(_dbFileName.c_str(), std::ios_base::binary | std::ios_base::app);
        }
        /* TODO: check if name is empty */
        return *this;
    }

    //-----------------------------------------------------
    //---   Element access
    //-----------------------------------------------------

    void prefixSearch(T prefix, int num, int offset = 0) {
        //PROFILER_TASK(prefixSearch);
        raxIterator iter;
        raxStart(&iter, rt);

        raxSeek(&iter, ">=", (unsigned char*) prefix.template c_str(), prefix.template size());

        //--------------  seek offset  ---------------
        while (raxNext(&iter) && (offset-- >= 0)) {
        }

        //--------------  get num result  ---------------
        while (raxNext(&iter) && (num-- >= 0)) {
            printf("Key: %.*s\n", (int) iter.key_len, (char*) iter.key);
        }
        raxStop(&iter);
    }

    void prefixSearch(T prefix, std::vector<T>& vec, int num, int offset = 0) {
        //PROFILER_TASK(prefixSearch);
        raxIterator iter;
        raxStart(&iter, rt);

        raxSeek(&iter, ">=", (unsigned char*) prefix.template c_str(), prefix.template size());

        //--------------  seek offset  ---------------
        while (raxNext(&iter) && (offset-- >= 0)) {
        }
        //--------------  get num result  ---------------
        while (raxNext(&iter) && (num-- >= 0)) {
            vec.push_back(iter.key);
        }

        raxStop(&iter);
    }

    //-----------------------------------------------------
    //---   Capacity
    //-----------------------------------------------------

    std::string stat() const {
        return "";
    }

    uint64_t size() const {
        return raxSize(rt);
    }

    //-----------------------------------------------------
    //---   Modifiers
    //-----------------------------------------------------

    void insert(T key) {
        //        PROFILER_TASK(insert);
        char buf[512];
        sprintf(buf, "%s", key.template c_str());
        raxInsert(rt, (unsigned char*) buf, strlen(buf), NULL, NULL);
    }

    //=====================================================
    //===   Dump
    //=====================================================

    void dumpSnapshot() {

        std::ofstream file(_dbFileName.c_str());
        boost::archive::text_oarchive ar(file);

        raxIterator iter;
        raxStart(&iter, rt);
        raxSeek(&iter, "^", NULL, 0);

        while (raxNext(&iter)) {
            printf("Key: %.*s\n", (int) iter.key_len, (char*) iter.key);
            SmazStr sstr((char *&) iter.key);
            ar & sstr;
        }
        file.close();
        raxStop(&iter);
    }

    void load() {
        printf("1\n");
        std::ifstream file(_dbFileName.c_str());
        boost::archive::text_iarchive ar(file);

        printf("2\n");
        char* res_str;
        SmazStr sstr(res_str);
        //        printf("3");
        //        ar & sstr;
        //        
        //        printf("%s\n", res_str);
    }

private:
    rax *rt;

    std::string _name;
    std::string _dbFileName;
    std::string _cwd;

    std::ofstream _ofsDb;
};

#endif /* RAXTREE_H */

