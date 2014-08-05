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
    static constexpr unsigned int max = 35;

    void setColumnWhide(string column, string str){
        unsigned int whide = str.size();
        unsigned int old = columnWhide[column];
        if(whide > max){
            whide = max;
        }
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
