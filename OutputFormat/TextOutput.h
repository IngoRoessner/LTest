#ifndef TEXTOUTPUT_H_INCLUDED
#define TEXTOUTPUT_H_INCLUDED

#include "OutputFormatBase.h"

template<typename ResultType>
class TextOutput: public OutputFormatBase<ResultType>{
private:
    string getOKStr(ResultType resultset){
        string result = "";
        for(auto elementPtr : resultset.getOK()){
            string time= util_toString(elementPtr->get_time_taken());
            result += elementPtr->get_testname()+": OK ("+time+" sec)\n";
        }
        return result;
    }

    string getIgnoreStr(ResultType resultset){
        string result = "";
        for(auto elementPtr : resultset.getIgnores()){
            result += elementPtr->get_testname()+": Ignored\n";
        }
        return result;
    }

    string getFailStr(ResultType resultset){
        string result = "";
        for(auto elementPtr : resultset.getFails()){
            auto ptr = ResultType::castToFailed(elementPtr);
            string time= util_toString(ptr->get_time_taken());
            result += elementPtr->get_testname()+": Fail ["+ptr->getMessage()+"] ("+time+" sec)\n";
        }
        return result;
    }

    string getAbordStr(ResultType resultset){
        string result = "";
        for(auto elementPtr : resultset.getAbords()){
            auto ptr = ResultType::castToAborted(elementPtr);
            string time= util_toString(ptr->get_time_taken());
            result += ptr->get_testname()+": Aborted ["+ptr->getMessage()+"] ("+time+" sec)\n";
        }
        return result;
    }

    string getCountStr(ResultType resultset){
        return "OK("+util_toString(resultset.getOK().size())+"), "+
        "Fail("+util_toString(resultset.getFails().size())+"), "+
        "Aborted("+util_toString(resultset.getAbords().size())+"), "+
        "Ignored("+util_toString(resultset.getIgnores().size())+")\n";
    }

    string getOut(ResultType resultset){
        string result = "";
        result+="============= OK =============\n";
        result+=getOKStr(resultset);
        result+="============= Fail =============\n";
        result+=getFailStr(resultset);
        result+="============= Aborted =============\n";
        result+=getAbordStr(resultset);
        result+="============= Ignored =============\n";
        result+=getIgnoreStr(resultset);
        result+="============= Count =============\n";
        result+=getCountStr(resultset);
        return result;
    }

public:
    string run(ResultType resultset){
        return getOut(resultset);
    }
};

#endif // TEXTOUTPUT_H_INCLUDED
