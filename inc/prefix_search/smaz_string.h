/*
 * Copyright 2017 ZTE - VNG
 */

/* 
 * File:   smaz_string.h
 * Author: minhnh3
 *
 * Created on January 12, 2018, 6:12 PM
 */

#ifndef SMAZ_STRING_H
#define SMAZ_STRING_H

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "smaz.h"

class SmazStr {
public:

    SmazStr(char* buf) {
        smaz_compress(buf, strlen(buf), _buf, sizeof (_buf));
    }

private:
    friend class boost::serialization::access;

    template<class Archive>
    void save(Archive& ar, const unsigned version) const {
        bool isNull = (_buf == 0);
        ar & isNull;
        if (!isNull) {
            std::string s(_buf);
            ar & s;
        }
    }

    template<class Archive>
    void load(Archive& ar, const unsigned version) {
        bool isNull;
        ar & isNull;
        if (!isNull) {
            std::string s;
            ar & s;
            //_buf = strdup(s.c_str());
            strncpy(_buf, s.c_str(), strlen(_buf));
        } else {
            _buf[0] = 0;
        }
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER();
private:
    char _buf[4096];
};

#endif /* SMAZ_STRING_H */

