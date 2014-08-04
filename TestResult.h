#ifndef TESTRESULT_H_INCLUDED
#define TESTRESULT_H_INCLUDED

typedef string testname;

class TestResult
{
public:
    enum TestState{
        OK, FAILED, ABORTED, IGNORED
    };

private:

    TestState _state;
    double _time_taken;
    map<ostream*, string> _output_mapping;
    testname _tname;

public :

    TestResult(testname tname = "no_testname_given") : _state(TestState::IGNORED), _time_taken(0), _tname(tname) {}

    TestResult(TestState state, double time_taken, map<ostream*, string> output_mapping, testname tname)
        : _state(state), _time_taken(time_taken), _output_mapping(output_mapping), _tname(tname) {}

    TestState get_state() const
    {
        return _state;
    }

    double get_time_taken() const
    {
        return _time_taken;
    }

    map<ostream*, string> get_output_mapping() const
    {
        return _output_mapping;
    }

    string get_testname() const {
    	return _tname;
    }

};



#endif // TESTRESULT_H_INCLUDED
