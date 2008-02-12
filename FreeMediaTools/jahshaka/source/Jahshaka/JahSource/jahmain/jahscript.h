#ifndef JAHSCRIPT_H_INCLUDED
#define JAHSCRIPT_H_INCLUDED

#include <string>
class JahControl;

class JahScript
{
protected:
    std::string             m_script_file_name;

public:
    JahScript(std::string& script_file_name) {m_script_file_name = script_file_name; }
    void  executeScriptFileVector(JahControl* ) { }

};


#endif // JAHSCRIPT_H_INCLUDED
