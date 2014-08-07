#ifndef TEXTOUTPUT_H_INCLUDED
#define TEXTOUTPUT_H_INCLUDED

#include "OutputFormatBase.h"
#include "TextTable.h"

template<typename ResultType>
class TextOutput: public OutputFormatBase<ResultType>{
private:
    TextTable table;

    void addOKToTable(ResultType resultset){
        for(auto elementPtr : resultset.getOK()){
            string time= util_toString(elementPtr->get_time_taken());
            table.addLine({elementPtr->get_testname(), "OK", "", time+" sec"});
        }
    }

    void addIgnoreToTable(ResultType resultset){
        for(auto elementPtr : resultset.getIgnores()){
            table.addLine({elementPtr->get_testname(), "Ignored", "", ""});
        }
    }

    void addFailToTable(ResultType resultset){
        for(auto ptr : resultset.getFails()){
            string time= util_toString(ptr->get_time_taken());
            table.addLine({ptr->get_testname(), "Fail", ptr->getMessage(), time+" sec"});
        }
    }

    void addAbortToTable(ResultType resultset){
        for(auto ptr : resultset.getAborts()){
            string time= util_toString(ptr->get_time_taken());
            table.addLine({ptr->get_testname(), "Aborted", ptr->getMessage(), time+" sec"});
        }
    }

    string getTotal(ResultType resultset){
        TextTable table;
        table.setColumns({"Total", "OK", "Fail", "Aborted", "Ignored", "Execution time"});
        table.addLine({
                        util_toString(resultset.size()),
                        util_toString(resultset.getOK().size()),
                        util_toString(resultset.getFails().size()),
                        util_toString(resultset.getAborts().size()),
                        util_toString(resultset.getIgnores().size()),
                        util_toString(resultset.getTotalExecutionTimeInSeconds())
                      });
       return table.out();
    }


    string getOut(ResultType resultset){
        table.setColumns({"TestName", "Status", "Message", "Time"});
        addOKToTable(resultset);
        addFailToTable(resultset);
        addAbortToTable(resultset);
        addIgnoreToTable(resultset);

        return table.out()+"\n\n"+getTotal(resultset)+"\n\n";
    }

public:
    string run(ResultType resultset){
        return getOut(resultset);
    }
};

#endif // TEXTOUTPUT_H_INCLUDED
