/*
 *     The MIT License (MIT)
 *
 *     Copyright (c) 2014 Ingo Rössner
 *                        Dan Häberlein
 *
 *     Permission is hereby granted, free of charge, to any person obtaining a copy
 *     of this software and associated documentation files (the "Software"), to deal
 *     in the Software without restriction, including without limitation the rights
 *     to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *     copies of the Software, and to permit persons to whom the Software is
 *     furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *     AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *     OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *     THE SOFTWARE.
 */


#ifndef TEXTTABLE_H_INCLUDED
#define TEXTTABLE_H_INCLUDED

#include <exception>
#include <map>
#include <list>
#include<string>

using std::list;
using std::map;
using std::string;
using std::initializer_list;
using std::runtime_error;

class TextTable{
    list<string> columnNames;
    map<string, list<string>> columns;
    map<string, unsigned int> columnWhide;
    unsigned int lineLength;
    unsigned int count;
    string seperator;

    void setColumnWhide(string column, string str){
        unsigned int whide = str.size();
        unsigned int old = columnWhide[column];

        if(whide > old){
            columnWhide[column] = whide;
        }
    }

    string getLine(char c){
        if(!lineLength){
            string result = "";
            unsigned int i = 0;
            for(auto column : columnNames){
                result += getStrWithSpace(column, "");
                if(++i < columnNames.size()){
                    result += seperator;
                }
            }
            lineLength = result.size();
        }
        string line = "";
        for(unsigned int i = 0; i<lineLength; i++){
                line += c;
            }
        return line;
    }

    string getStrWithSpace(string column, string str){
        string space = "";
        for(unsigned int i = str.size(); i<columnWhide[column]; i++){
            space += " ";
        }
        return str+space;
    }

    string getFild(string column, unsigned int index){
        auto it = columns[column].begin();
        for(unsigned int i=0; i<index; i++){
            it++;
        }
        return *it;
    }

public:
    TextTable(): lineLength(0), count(0), seperator(" | "){};

    void setColumns(initializer_list<string> columns){
        this->columns.clear();
        for(string column : columns){
            columnNames.push_back(column);
            this->columns[column] = list<string>();
            setColumnWhide(column, column);
        }
    }

    void addLine(initializer_list<string> line){
        count++;
        initializer_list<string>::iterator lineIt = line.begin();
        for(auto column : columnNames){
           if(lineIt == line.end()){
                throw runtime_error("expect line.size == columns.size");
           }
           columns[column].push_back(*lineIt);
           setColumnWhide(column, *lineIt);
           ++lineIt;
        }
    }

    string out(bool bodyline = false){
        return head()+body(bodyline);
    }

    string head(){
        string result = "";
        unsigned int i = 0;
        for(auto column : columnNames){
            result += getStrWithSpace(column, column);
            if(++i < columnNames.size()){
                result += seperator;
            }
        }
        result += "\n"+getLine('=')+"\n";
        return result;
    }

    string body(bool bodyline){
        string result = "";
        for(unsigned int i=0; i<count; i++){
            if(i!=0){
                result += "\n";
                if(bodyline){
                    result += getLine('-')+"\n";
                }
            }
            unsigned int j = 0;
            for(auto column : columnNames){
                result += getStrWithSpace(column, getFild(column, i));
                if(++j < columnNames.size()){
                    result += seperator;
                }
            }
        }
        return result;
    }
};

#endif // TEXTTABLE_H_INCLUDED
