/*
 * Copyright 2017 ZTE - VNG
 */

/* 
 * File:   main.cpp
 * Author: minhnh3
 *
 * Created on January 12, 2018, 3:07 PM
 */

#include <cstdlib>
#include <fstream>
#include "../inc/prefix_search/prefix_search.h"
#include "../inc/prefix_search/smaz.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include "../inc/prefix_search/smaz_string.h"

using namespace std;

int test1(const char* fname);
int test2();
int test3();

/*
 * 
 */
int main(int argc, char** argv) {
	test1("small.txt");
	//test2();
	//test3();

	return 0;
}

int test1(const char* fname) {
	RaxTree<std::string> raxTree;

	ifstream myfile(fname);
	string line;
	int i = 0;
	{
		while (getline(myfile, line)) {
			i++;
			{
				raxTree.insert(line);
			}
		}

	}
	cout << i << endl;

	cout << "tree size: " << raxTree.size() << endl;

	raxTree.dumpSnapshot();

	cout << "---\n";
	raxTree.load();

	myfile.close();
}

int test2() {
	const char* file_name = "boost_example.txt";

	// Create some objects
	std::string obj1("object1");
	const std::string obj2("object2");
	char ch = 'a';

	// Save data
	{
		// Create an output archive
		std::ofstream ofs(file_name);
		boost::archive::text_oarchive ar(ofs);

		// Write data
		ar& obj1;
		ar& obj2;
		ar& ch;
	}

	// Restore data
//	std::string reObj1;
//	std::string reObj2;
//
//	{
//		// Create and input archive
//		std::ifstream ifs(file_name);
//		boost::archive::text_iarchive ar(ifs);
//
//		// Load data
//		ar& reObj1;
//		ar& reObj2;
//	}
//
//	// Make sure we restored the data correctly
//	assert(reObj1 == obj1);
//	assert(reObj2 == obj2);

	std::cout << "Press the 'Enter' key to exit..." << std::endl;
	std::cin.ignore();
	return 0;
}

int test3() {
	char in[512];
    char out[4096];
    char d[4096];
    int comprlen, decomprlen;
	
	char *str = "this is the text";
	comprlen = smaz_compress(str,strlen(str),out,sizeof(out));
	cout << "size org: " << strlen(str) << ", after: " << comprlen << endl;
	cout << "str after: " << out << endl;
}