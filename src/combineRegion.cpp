//
//  main.cpp
//  combineRegion
//
//  Created by Gang Peng on 9/26/16.
//  Copyright © 2016 Gang Peng. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
#include <sstream>

#include "normal.h"

using namespace std;

class bedInfo{
    int chr;
    int start;
    int end;
    bool withChr;
    
public:
    bedInfo();
    bedInfo(int c, int s, int e, bool w);
    
    int getChr() const { return chr;}
    int getStart() const { return start;}
    int getEnd() const { return end; }
    bool getWithChr() const { return withChr; }
    
    bool setChr(int c);
    bool setStart(int s);
    bool setEnd(int e);
    bool setWithChr(bool w);
    
    bedInfo & operator= (const bedInfo & other);
    bool operator < (const bedInfo & other) const;
    friend std::ostream & operator << (std::ostream & os, const bedInfo & pi);
};

bedInfo::bedInfo(){
    chr = 0;
    start = 0;
    end = 0;
    withChr = false;
}

bedInfo::bedInfo(int c, int s, int e, bool w){
    chr = c;
    start = s;
    end = e;
    withChr = w;
}

bool bedInfo::setChr(int c){
    chr = c;
    return true;
}

bool bedInfo::setStart(int s){
    start = s;
    return true;
}

bool bedInfo::setEnd(int e){
    end = e;
    return true;
}

bool bedInfo::setWithChr(bool w){
    withChr = w;
    return true;
}

bedInfo & bedInfo::operator= (const bedInfo & other){
    if(this == &other){
        return *this;
    }
    
    chr = other.chr;
    start = other.start;
    end = other.end;
    withChr = other.withChr;
    
    return *this;
    
}

bool bedInfo::operator< (const bedInfo & other) const{
    if(chr < other.chr){
        return true;
    } else if(chr == other.chr) {
        if(start < other.start){
            return true;
        } else if(start == other.start){
            return (end < other.end);
        } else {
            return false;
        }
    } else {
        return false;
    }
}

ostream & operator<< (ostream & os, const bedInfo & bi){
    if(bi.chr==23){
        if(bi.withChr){
            os << "chrX";
        } else{
            os << "X";
        }
    } else if (bi.chr == 24){
        if(bi.withChr){
            os << "chrY";
        } else{
            os << "Y";
        }
    } else if(bi.chr == 25){
        os << "MT";
    } else {
        if(bi.withChr){
            os << "chr" << bi.chr;
        } else {
            os << bi.chr;
        }
    }
    
    os << "\t" << bi.start << "\t" <<bi.end << endl;
    return os;
}

int main(int argc, const char * argv[]) {
    if(argc == 1){
        cout << "Combine the overlap regions in a bed file " <<endl;
        cout << "Usage :" <<endl;
        cout << "combineRegion inputbed outputbed" << endl;
        cout << "Both input and output file should be set" << endl;
    }
    
    if(argc == 2){
        cerr << "Both input and output file should be set" << endl;
        return -1;
    }
    
    bool combine = true;
    if(argc == 4){
        combine = false;
    }
    
    ifstream fin(argv[1]);
    if(!fin.is_open()){
        cerr << "Cannot open file " << argv[1] <<endl;
        return -1;
    }
    
    ofstream fout(argv[2]);
    
    set<bedInfo> allPos;
    
    while(!fin.eof()){
        string fline;
        getline(fin, fline);
        if(fline.size() < 2){
            break;
        }
        
        istringstream iss(fline);
        string chrTmp;
        int chr, start, end;
        bool withChr;
        iss >> chrTmp;
        iss >> start;
        iss >> end;
        
        transform(chrTmp.begin(), chrTmp.end(), chrTmp.begin(), ::toupper);
        if(chrTmp.substr(0, 3) == "CHR"){
            chrTmp  = chrTmp.substr(3);
            withChr = true;
        } else {
            withChr = false;
        }
        
        if(chrTmp == "X" || chrTmp == "x"){
            chr = 23;
        } else if(chrTmp == "Y" || chrTmp == "y"){
            chr = 24;
        } else if(chrTmp == "MT" || chrTmp == "mt"){
            chr = 25;
        } else {
            chr = atoi(chrTmp.c_str());
        }

        bedInfo tmp(chr, start, end, withChr);
        allPos.insert(tmp);
    }
    fin.close();
    fin.clear();
    
    if(!combine){
        for(set<bedInfo>::iterator it = allPos.begin(); it != allPos.end(); it++){
            fout << *it;
        }
        fout.close();
        fout.clear();
        return 0;
    }
    
    vector<bedInfo> rlt;
    bedInfo currentPos;
    for(set<bedInfo>::iterator it = allPos.begin(); it != allPos.end(); it++){
        if(it == allPos.begin()){
            currentPos = *it;
            continue;
        }
        
        if(currentPos.getChr() == it -> getChr()){
            if(currentPos.getEnd() >= (it -> getStart() - 1) ){
                currentPos.setEnd(max(currentPos.getEnd(), it -> getEnd()));
                continue;
            } else {
                rlt.push_back(currentPos);
                currentPos = *it;
            }
        } else {
            rlt.push_back(currentPos);
            currentPos = *it;
        }
    }
    rlt.push_back(currentPos);
    
    
    for(vector<bedInfo>::iterator it = rlt.begin(); it != rlt.end(); it++){
        fout << *it;
    }
    fout.close();
    fout.clear();
    
    return 0;
}



