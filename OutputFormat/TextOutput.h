#ifndef TEXTOUTPUT_H_INCLUDED
#define TEXTOUTPUT_H_INCLUDED

#include "OutputFormatBase.h"
#include "TextTable.h"
#include "../toStringPatch.h"

template<typename ResultType>
class TextOutput: public OutputFormatBase<ResultType>{
private:
    TextTable table;

    void addOKToTable(ResultType resultset){
        for(auto elementPtr : resultset.getOK()){
            string time= patch::to_string(elementPtr->get_time_taken());
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
            string time= patch::to_string(ptr->get_time_taken());
            table.addLine({ptr->get_testname(), "Fail", ptr->getMessage(), time+" sec"});
        }
    }

    void addAbortToTable(ResultType resultset){
        for(auto ptr : resultset.getAborts()){
            string time= patch::to_string(ptr->get_time_taken());
            table.addLine({ptr->get_testname(), "Aborted", ptr->getMessage(), time+" sec"});
        }
    }

    string getTotal(ResultType resultset){
        TextTable table;
        table.setColumns({"Total", "OK", "Fail", "Aborted", "Ignored", "Execution time"});
        table.addLine({
                        patch::to_string(resultset.size()),
                        patch::to_string(resultset.getOK().size()),
                        patch::to_string(resultset.getFails().size()),
                        patch::to_string(resultset.getAborts().size()),
                        patch::to_string(resultset.getIgnores().size()),
                        patch::to_string(resultset.getTotalExecutionTimeInSeconds())
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
