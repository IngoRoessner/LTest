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


#ifndef XMLOUTPUT_H_
#define XMLOUTPUT_H_

#include <string>
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include "OutputFormatBase.h"
#include "../toStringPatch.h"
#include <sstream>
#include "XmlOutput.h"

using rapidxml::xml_node;
using rapidxml::xml_attribute;
using rapidxml::node_type;
using rapidxml::xml_document;
using patch::to_string; //for mingw-32

template<typename ResultSetType>
class XmlOutput: public OutputFormatBase<ResultSetType>
{

    typedef xml_document<> xml_doc;
    xml_doc doc;

    const char * allocate_for_doc(const char * cstring)
    {
        return doc.allocate_string(cstring);
    }

    void append_sysstream_node_when_available(xml_node<> * current_node, const string& node_name, const string& stream_content){
    	if(!stream_content.empty()){
    		current_node->append_node(doc.allocate_node(node_type::node_element, allocate_for_doc(node_name.c_str()), allocate_for_doc(stream_content.c_str())));
    	}
    }

    template <typename ResultType>
	void create_sysout_and_syserr_nodes(const ResultType& result, xml_node<>* current_test_case_node) {
		append_sysstream_node_when_available(current_test_case_node, "system-out", result->get_system_out());
		append_sysstream_node_when_available(current_test_case_node, "system-err", result->get_system_err());
	}

    template<typename ResultType>
    xml_node<> * create_testcase_node(const shared_ptr<ResultType>& result){
        xml_node<> * current_test_case_node = doc.allocate_node(node_type::node_element, "testcase");
        const char * time_taken_cstr = to_string(result->get_time_taken()).c_str();
        current_test_case_node->append_attribute(doc.allocate_attribute("time", allocate_for_doc(time_taken_cstr)));
        current_test_case_node->append_attribute(doc.allocate_attribute("name", allocate_for_doc(result->get_testname().c_str())));
        return current_test_case_node;
    }

public :

    std::string run(ResultSetType resultset)
    {
        xml_node<> * node = doc.allocate_node(node_type::node_element, "testsuite");
        doc.append_node(node);
        node->append_attribute(doc.allocate_attribute("failures", allocate_for_doc(to_string(resultset.getFails().size()).c_str())));
        node->append_attribute(doc.allocate_attribute("time", allocate_for_doc(to_string(resultset.getTotalExecutionTimeInSeconds()).c_str())));
        node->append_attribute(doc.allocate_attribute("errors", allocate_for_doc(to_string(resultset.getAborts().size()).c_str())));
        node->append_attribute(doc.allocate_attribute("skipped", allocate_for_doc(to_string(resultset.getIgnores().size()).c_str())));
        node->append_attribute(doc.allocate_attribute("tests", allocate_for_doc(to_string(resultset.size()).c_str())));
        node->append_attribute(doc.allocate_attribute("name", "C++ LTest Testsuite")); // TODO request from user or infer testsuite name
        for(const auto& result : resultset.getIgnores()){
        	xml_node<> * current_test_case_node = create_testcase_node(result);
        	current_test_case_node->append_node(doc.allocate_node(node_type::node_element, "skipped"));
        	node->append_node(current_test_case_node);
        }
        for(const auto& result : resultset.getOK()){
            xml_node<> * current_test_case_node = create_testcase_node(result);
            node->append_node(current_test_case_node);
            create_sysout_and_syserr_nodes(result, current_test_case_node);
        }
        for(const auto& result : resultset.getFails()){
            xml_node<> * current_test_case_node = create_testcase_node(result);
            xml_node<> * failure_node = doc.allocate_node(node_type::node_element, "failure", allocate_for_doc(result->getMessage().c_str()));
            current_test_case_node->append_node(failure_node);
            node->append_node(current_test_case_node);
            create_sysout_and_syserr_nodes(result, current_test_case_node);
        }
        for(const auto& result : resultset.getAborts()){
            xml_node<> * current_test_case_node = create_testcase_node(result);
            xml_node<> * error_node = doc.allocate_node(node_type::node_element, "error", allocate_for_doc(result->getMessage().c_str()));
            current_test_case_node->append_node(error_node);
            node->append_node(current_test_case_node);
			create_sysout_and_syserr_nodes(result, current_test_case_node);
        }
        std::stringstream output_stream;
        output_stream << doc;
        return output_stream.str();
    }

};

#endif /* XMLOUTPUT_H_ */
