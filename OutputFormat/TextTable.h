#ifndef TEXTTABLE_H_INCLUDED
#define TEXTTABLE_H_INCLUDED

#include <exception>

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
            for(auto column : columns){
                result += getStrWithSpace(column.first, "") + seperator;
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

    string out(){
        return head()+body();
    }

    string head(){
        string result = "";
        for(auto column : columnNames){
            result += getStrWithSpace(column, column) + seperator;
        }
        result += "\n"+getLine('=')+"\n";
        return result;
    }

    string body(){
        string result = "";
        for(unsigned int i=0; i<count; i++){
            if(i!=0){
                result += "\n"+getLine('-')+"\n";
            }
            for(auto column : columnNames){
                result += getStrWithSpace(column, getFild(column, i)) + seperator;
            }
        }
        return result;
    }
};

#endif // TEXTTABLE_H_INCLUDED
