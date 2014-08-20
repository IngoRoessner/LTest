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
