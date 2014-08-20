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


#ifndef OUTPUTFORMAT_H_INCLUDED
#define OUTPUTFORMAT_H_INCLUDED

#include "OutputFormatBase.h"
#include "TextOutput.h"
#include "Format.h"
#include "XmlOutput.h"

using namespace std;

template<typename ResultType>
class GetOutputFormat{
    OutputFormatBase<ResultType>* output;
public:
    GetOutputFormat(Format format){
        switch (format){
            case Format::Text:
                output = new TextOutput<ResultType>();
                break;
            case Format::Xml:
                output = new XmlOutput<ResultType>();
                break;
            default:
                throw "their is no such format";
        }
    }

    ~GetOutputFormat(){
        delete output;
    }

    string run(ResultType resultset){
        return output->run(resultset);
    }
};

#endif // OUTPUTFORMAT_H_INCLUDED
